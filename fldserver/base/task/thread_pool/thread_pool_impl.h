// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TASK_THREAD_POOL_THREAD_POOL_IMPL_H_
#define BASE_TASK_THREAD_POOL_THREAD_POOL_IMPL_H_

#include "fldserver/base/callback.h"
#include "fldserver/base/check_op.h"
#include "fldserver/base/memory/ptr_util.h"
#include "fldserver/base/memory/ref_counted.h"
#include "fldserver/base/optional.h"
#include "fldserver/base/sequence_checker.h"
#include "fldserver/base/strings/string_piece.h"
#include "fldserver/base/synchronization/atomic_flag.h"
#include "fldserver/base/task/single_thread_task_runner_thread_mode.h"
#include "fldserver/base/task/task_executor.h"
#include "fldserver/base/task/task_traits.h"
#include "fldserver/base/task/thread_pool/delayed_task_manager.h"
#include "fldserver/base/task/thread_pool/environment_config.h"
#include "fldserver/base/task/thread_pool/pooled_single_thread_task_runner_manager.h"
#include "fldserver/base/task/thread_pool/pooled_task_runner_delegate.h"
#include "fldserver/base/task/thread_pool/service_thread.h"
#include "fldserver/base/task/thread_pool/task_source.h"
#include "fldserver/base/task/thread_pool/task_tracker.h"
#include "fldserver/base/task/thread_pool/thread_group.h"
#include "fldserver/base/task/thread_pool/thread_group_impl.h"
#include "fldserver/base/task/thread_pool/thread_pool_instance.h"
#include "fldserver/base/updateable_sequenced_task_runner.h"
#include "fldserver/fldserver_config.h"

#include <memory>
#include <vector>

#if IS_POSIX && !defined(OS_NACL_SFI)
#include "fldserver/base/task/thread_pool/task_tracker_posix.h"
#endif

#if IS_WIN
#include "fldserver/base/win/com_init_check_hook.h"
#endif

namespace base
{
namespace internal
{
// Default ThreadPoolInstance implementation. This class is thread-safe.
class CORE_EXPORT ThreadPoolImpl : public ThreadPoolInstance,
                                   public TaskExecutor,
                                   public ThreadGroup::Delegate,
                                   public PooledTaskRunnerDelegate
{
public:
    using TaskTrackerImpl =
#if IS_POSIX && !defined(OS_NACL_SFI)
            TaskTrackerPosix;
#else
            TaskTracker;
#endif

    // Creates a ThreadPoolImpl with a production TaskTracker. |histogram_label|
    // is used to label histograms. No histograms are recorded if it is empty.
    explicit ThreadPoolImpl(StringPiece histogram_label);

    // For testing only. Creates a ThreadPoolImpl with a custom TaskTracker.
    ThreadPoolImpl(StringPiece histogram_label, std::unique_ptr<TaskTrackerImpl> task_tracker);

    ThreadPoolImpl(const ThreadPoolImpl&) = delete;
    ThreadPoolImpl&
    operator=(const ThreadPoolImpl&) = delete;
    ~ThreadPoolImpl() override;

    // ThreadPoolInstance:
    void
    Start(const ThreadPoolInstance::InitParams& init_params,
          WorkerThreadObserver* worker_thread_observer) override;
    int
    GetMaxConcurrentNonBlockedTasksWithTraitsDeprecated(const TaskTraits& traits) const override;
    void
    Shutdown() override;
    void
    FlushForTesting() override;
    void
    FlushAsyncForTesting(OnceClosure flush_callback) override;
    void
    JoinForTesting() override;
    void
    BeginFence() override;
    void
    EndFence() override;
    void
    BeginBestEffortFence() override;
    void
    EndBestEffortFence() override;

    // TaskExecutor:
    bool
    PostDelayedTask(const Location& from_here,
                    const TaskTraits& traits,
                    OnceClosure task,
                    TimeDelta delay) override;
    scoped_refptr<TaskRunner>
    CreateTaskRunner(const TaskTraits& traits) override;
    scoped_refptr<SequencedTaskRunner>
    CreateSequencedTaskRunner(const TaskTraits& traits) override;
    scoped_refptr<SingleThreadTaskRunner>
    CreateSingleThreadTaskRunner(const TaskTraits& traits,
                                 SingleThreadTaskRunnerThreadMode thread_mode) override;
#if IS_WIN
    scoped_refptr<SingleThreadTaskRunner>
    CreateCOMSTATaskRunner(const TaskTraits& traits,
                           SingleThreadTaskRunnerThreadMode thread_mode) override;
#endif  // IS_WIN
    scoped_refptr<UpdateableSequencedTaskRunner>
    CreateUpdateableSequencedTaskRunner(const TaskTraits& traits);

    // PooledTaskRunnerDelegate:
    bool
    EnqueueJobTaskSource(scoped_refptr<JobTaskSource> task_source) override;
    void
    RemoveJobTaskSource(scoped_refptr<JobTaskSource> task_source) override;
    void
    UpdatePriority(scoped_refptr<TaskSource> task_source, TaskPriority priority) override;
    void
    UpdateJobPriority(scoped_refptr<TaskSource> task_source, TaskPriority priority) override;

    // Returns the TimeTicks of the next task scheduled on ThreadPool (Now() if
    // immediate, nullopt if none). This is thread-safe, i.e., it's safe if tasks
    // are being posted in parallel with this call but such a situation obviously
    // results in a race as to whether this call will see the new tasks in time.
    Optional<TimeTicks>
    NextScheduledRunTimeForTesting() const;

    // Forces ripe delayed tasks to be posted (e.g. when time is mocked and
    // advances faster than the real-time delay on ServiceThread).
    void
    ProcessRipeDelayedTasksForTesting();

    // Requests that all threads started by future ThreadPoolImpls in this process
    // have a synchronous start (if |enabled|; cancels this behavior otherwise).
    // Must be called while no ThreadPoolImpls are alive in this process. This is
    // exposed here on this internal API rather than as a ThreadPoolInstance
    // configuration param because only one internal test truly needs this.
    static void
    SetSynchronousThreadStartForTesting(bool enabled);

private:
    // Invoked after |num_fences_| or |num_best_effort_fences_| is updated. Sets
    // the CanRunPolicy in TaskTracker and wakes up workers as appropriate.
    void
    UpdateCanRunPolicy();

    const ThreadGroup*
    GetThreadGroupForTraits(const TaskTraits& traits) const;

    // ThreadGroup::Delegate:
    ThreadGroup*
    GetThreadGroupForTraits(const TaskTraits& traits) override;

    // Posts |task| to be executed by the appropriate thread group as part of
    // |sequence|. This must only be called after |task| has gone through
    // TaskTracker::WillPostTask() and after |task|'s delayed run time.
    bool
    PostTaskWithSequenceNow(Task task, scoped_refptr<Sequence> sequence);

    // PooledTaskRunnerDelegate:
    bool
    PostTaskWithSequence(Task task, scoped_refptr<Sequence> sequence) override;
    bool
    ShouldYield(const TaskSource* task_source) override;

    const std::unique_ptr<TaskTrackerImpl> task_tracker_;
    ServiceThread service_thread_;
    DelayedTaskManager delayed_task_manager_;
    PooledSingleThreadTaskRunnerManager single_thread_task_runner_manager_;

    std::unique_ptr<ThreadGroup> foreground_thread_group_;
    std::unique_ptr<ThreadGroup> background_thread_group_;

    bool disable_job_yield_ = false;
    bool disable_fair_scheduling_ = false;
    std::atomic<bool> disable_job_update_priority_{false};

    // Whether this TaskScheduler was started. Access controlled by
    // |sequence_checker_|.
    bool started_ = false;

    // Whether the --disable-best-effort-tasks switch is preventing execution of
    // BEST_EFFORT tasks until shutdown.
    const bool has_disable_best_effort_switch_;

    // Number of fences preventing execution of tasks of any/BEST_EFFORT priority.
    // Access controlled by |sequence_checker_|.
    int num_fences_ = 0;
    int num_best_effort_fences_ = 0;

#if DCHECK_IS_ON()
    // Set once JoinForTesting() has returned.
    AtomicFlag join_for_testing_returned_;
#endif

#if IS_WIN && defined(COM_INIT_CHECK_HOOK_ENABLED)
    // Provides COM initialization verification for supported builds.
    base::win::ComInitCheckHook com_init_check_hook_;
#endif

    // Asserts that operations occur in sequence with Start().
    SEQUENCE_CHECKER(sequence_checker_);

    TrackedRefFactory<ThreadGroup::Delegate> tracked_ref_factory_;
};

}  // namespace internal
}  // namespace base

#endif  // BASE_TASK_THREAD_POOL_THREAD_POOL_IMPL_H_