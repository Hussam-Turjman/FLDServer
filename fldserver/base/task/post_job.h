// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TASK_POST_JOB_H_
#define BASE_TASK_POST_JOB_H_

#include "fldserver/base/callback.h"
#include "fldserver/base/check_op.h"
#include "fldserver/base/location.h"
#include "fldserver/base/macros.h"
#include "fldserver/base/memory/ref_counted.h"
#include "fldserver/base/task/task_traits.h"
#include "fldserver/base/time/time.h"
#include "fldserver/fldserver_config.h"

#include <limits>

namespace base
{
namespace internal
{
class JobTaskSource;
class PooledTaskRunnerDelegate;
}  // namespace internal

// Delegate that's passed to Job's worker task, providing an entry point to
// communicate with the scheduler. To prevent deadlocks, JobDelegate methods
// should never be called while holding a user lock.
class CORE_EXPORT JobDelegate
{
public:
    // A JobDelegate is instantiated for each worker task that is run.
    // |task_source| is the task source whose worker task is running with this
    // delegate and |pooled_task_runner_delegate| is used by ShouldYield() to
    // check whether the pool wants this worker task to yield (null if this worker
    // should never yield -- e.g. when the main thread is a worker).
    JobDelegate(internal::JobTaskSource* task_source,
                internal::PooledTaskRunnerDelegate* pooled_task_runner_delegate);
    ~JobDelegate();

    // Returns true if this thread *must* return from the worker task on the
    // current thread ASAP. Workers should periodically invoke ShouldYield (or
    // YieldIfNeeded()) as often as is reasonable.
    bool
    ShouldYield();

    // If ShouldYield(), this will pause the current thread (allowing it to be
    // replaced in the pool); no-ops otherwise. If it pauses, it will resume and
    // return from this call whenever higher priority work completes.
    // Prefer ShouldYield() over this (only use YieldIfNeeded() when unwinding
    // the stack is not possible).
    void
    YieldIfNeeded();

    // Notifies the scheduler that max concurrency was increased, and the number
    // of worker should be adjusted accordingly. See PostJob() for more details.
    void
    NotifyConcurrencyIncrease();

    // Returns a task_id unique among threads currently running this job, such
    // that GetTaskId() < worker count. To achieve this, the same task_id may be
    // reused by a different thread after a worker_task returns.
    uint8_t
    GetTaskId();

    // Returns true if the current task is called from the thread currently
    // running JobHandle::Join().
    bool
    IsJoiningThread() const
    {
        return pooled_task_runner_delegate_ == nullptr;
    }

private:
    static constexpr uint8_t kInvalidTaskId = std::numeric_limits<uint8_t>::max();

    internal::JobTaskSource* const task_source_;
    internal::PooledTaskRunnerDelegate* const pooled_task_runner_delegate_;
    uint8_t task_id_ = kInvalidTaskId;

#if DCHECK_IS_ON()
    // Value returned by the last call to ShouldYield().
    bool last_should_yield_ = false;
#endif

    DISALLOW_COPY_AND_ASSIGN(JobDelegate);
};

// Handle returned when posting a Job. Provides methods to control execution of
// the posted Job. To prevent deadlocks, JobHandle methods should never be
// called while holding a user lock.
class CORE_EXPORT JobHandle
{
public:
    JobHandle();
    // A job must either be joined, canceled or detached before the JobHandle is
    // destroyed.
    ~JobHandle();

    JobHandle(JobHandle&&);
    JobHandle&
    operator=(JobHandle&&);

    // Returns true if associated with a Job.
    explicit operator bool() const
    {
        return task_source_ != nullptr;
    }

    // Returns true if there's any work pending or any worker running.
    bool
    IsActive() const;

    // Update this Job's priority.
    void
    UpdatePriority(TaskPriority new_priority);

    // Notifies the scheduler that max concurrency was increased, and the number
    // of workers should be adjusted accordingly. See PostJob() for more details.
    void
    NotifyConcurrencyIncrease();

    // Contributes to the job on this thread. Doesn't return until all tasks have
    // completed and max concurrency becomes 0. This also promotes this Job's
    // priority to be at least as high as the calling thread's priority.
    void
    Join();

    // Forces all existing workers to yield ASAP. Waits until they have all
    // returned from the Job's callback before returning.
    void
    Cancel();

    // Forces all existing workers to yield ASAP but doesn’t wait for them.
    // Warning, this is dangerous if the Job's callback is bound to or has access
    // to state which may be deleted after this call.
    void
    CancelAndDetach();

    // Can be invoked before ~JobHandle() to avoid waiting on the job completing.
    void
    Detach();

private:
    friend class internal::JobTaskSource;

    explicit JobHandle(scoped_refptr<internal::JobTaskSource> task_source);

    scoped_refptr<internal::JobTaskSource> task_source_;

    DISALLOW_COPY_AND_ASSIGN(JobHandle);
};

// Callback used in PostJob() to control the maximum number of threads calling
// the worker task concurrently.

// Returns the maximum number of threads which may call a job's worker task
// concurrently. |worker_count| is the number of threads currently assigned to
// this job which some callers may need to determine their return value.
using MaxConcurrencyCallback = RepeatingCallback<size_t(size_t /*worker_count*/)>;

// Posts a repeating |worker_task| with specific |traits| to run in parallel on
// base::ThreadPool.
// Returns a JobHandle associated with the Job, which can be joined, canceled or
// detached.
// ThreadPool APIs, including PostJob() and methods of the returned JobHandle,
// must never be called while holding a lock that could be acquired by
// |worker_task| or |max_concurrency_callback| -- that could result in a
// deadlock. This is because [1] |max_concurrency_callback| may be invoked while
// holding internal ThreadPool lock (A), hence |max_concurrency_callback| can
// only use a lock (B) if that lock is *never* held while calling back into a
// ThreadPool entry point from any thread (A=>B/B=>A deadlock) and [2]
// |worker_task| or |max_concurrency_callback| is invoked synchronously from
// JobHandle::Join() (A=>JobHandle::Join()=>A deadlock).
// To avoid scheduling overhead, |worker_task| should do as much work as
// possible in a loop when invoked, and JobDelegate::ShouldYield() should be
// periodically invoked to conditionally exit and let the scheduler prioritize
// work.
//
// A canonical implementation of |worker_task| looks like:
//   void WorkerTask(JobDelegate* job_delegate) {
//     while (!job_delegate->ShouldYield()) {
//       auto work_item = worker_queue.TakeWorkItem(); // Smallest unit of work.
//       if (!work_item)
//         return:
//       ProcessWork(work_item);
//     }
//   }
//
// |max_concurrency_callback| controls the maximum number of threads calling
// |worker_task| concurrently. |worker_task| is only invoked if the number of
// threads previously running |worker_task| was less than the value returned by
// |max_concurrency_callback|. In general, |max_concurrency_callback| should
// return the latest number of incomplete work items (smallest unit of work)
// left to processed. JobHandle/JobDelegate::NotifyConcurrencyIncrease() *must*
// be invoked shortly after |max_concurrency_callback| starts returning a value
// larger than previously returned values. This usually happens when new work
// items are added and the API user wants additional threads to invoke
// |worker_task| concurrently. The callbacks may be called concurrently on any
// thread until the job is complete. If the job handle is detached, the
// callbacks may still be called, so they must not access global state that
// could be destroyed.
//
// |traits| requirements:
// - base::ThreadPolicy must be specified if the priority of the task runner
//   will ever be increased from BEST_EFFORT.
JobHandle CORE_EXPORT
PostJob(const Location& from_here,
        const TaskTraits& traits,
        RepeatingCallback<void(JobDelegate*)> worker_task,
        MaxConcurrencyCallback max_concurrency_callback);

}  // namespace base

#endif  // BASE_TASK_POST_JOB_H_
