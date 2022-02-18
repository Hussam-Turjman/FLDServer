// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TASK_THREAD_POOL_POOLED_PARALLEL_TASK_RUNNER_H_
#define BASE_TASK_THREAD_POOL_POOLED_PARALLEL_TASK_RUNNER_H_

#include "fldserver/base/callback_forward.h"
#include "fldserver/base/containers/flat_set.h"
#include "fldserver/base/location.h"
#include "fldserver/base/task/common/checked_lock.h"
#include "fldserver/base/task/task_traits.h"
#include "fldserver/base/task_runner.h"
#include "fldserver/base/thread_annotations.h"
#include "fldserver/base/time/time.h"
#include "fldserver/fldserver_config.h"

namespace base
{
namespace internal
{
class Sequence;
class PooledTaskRunnerDelegate;

// A task runner that runs tasks in parallel.
class CORE_EXPORT PooledParallelTaskRunner : public TaskRunner
{
public:
    // Constructs a PooledParallelTaskRunner which can be used to post tasks.
    PooledParallelTaskRunner(const TaskTraits& traits,
                             PooledTaskRunnerDelegate* pooled_task_runner_delegate);
    PooledParallelTaskRunner(const PooledParallelTaskRunner&) = delete;
    PooledParallelTaskRunner&
    operator=(const PooledParallelTaskRunner&) = delete;

    // TaskRunner:
    bool
    PostDelayedTask(const Location& from_here, OnceClosure closure, TimeDelta delay) override;

    // Removes |sequence| from |sequences_|.
    void
    UnregisterSequence(Sequence* sequence);

private:
    ~PooledParallelTaskRunner() override;

    const TaskTraits traits_;
    PooledTaskRunnerDelegate* const pooled_task_runner_delegate_;

    CheckedLock lock_;

    // List of alive Sequences instantiated by this PooledParallelTaskRunner.
    // Sequences are added when they are instantiated, and removed when they are
    // destroyed.
    base::flat_set<Sequence*> sequences_ GUARDED_BY(lock_);
};

}  // namespace internal
}  // namespace base

#endif  // BASE_TASK_THREAD_POOL_POOLED_PARALLEL_TASK_RUNNER_H_
