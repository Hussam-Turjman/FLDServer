// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TASK_THREAD_POOL_POOLED_SEQUENCED_TASK_RUNNER_H_
#define BASE_TASK_THREAD_POOL_POOLED_SEQUENCED_TASK_RUNNER_H_

#include "fldserver/base/callback_forward.h"
#include "fldserver/base/location.h"
#include "fldserver/base/task/task_traits.h"
#include "fldserver/base/task/thread_pool/pooled_task_runner_delegate.h"
#include "fldserver/base/task/thread_pool/sequence.h"
#include "fldserver/base/time/time.h"
#include "fldserver/base/updateable_sequenced_task_runner.h"
#include "fldserver/fldserver_config.h"

namespace base
{
namespace internal
{
// A task runner that runs tasks in sequence.
class CORE_EXPORT PooledSequencedTaskRunner : public UpdateableSequencedTaskRunner
{
public:
    // Constructs a PooledSequencedTaskRunner which can be used to post tasks.
    PooledSequencedTaskRunner(const TaskTraits& traits,
                              PooledTaskRunnerDelegate* pooled_task_runner_delegate);
    PooledSequencedTaskRunner(const PooledSequencedTaskRunner&) = delete;
    PooledSequencedTaskRunner&
    operator=(const PooledSequencedTaskRunner&) = delete;

    // UpdateableSequencedTaskRunner:
    bool
    PostDelayedTask(const Location& from_here, OnceClosure closure, TimeDelta delay) override;

    bool
    PostNonNestableDelayedTask(const Location& from_here,
                               OnceClosure closure,
                               TimeDelta delay) override;

    bool
    RunsTasksInCurrentSequence() const override;

    void
    UpdatePriority(TaskPriority priority) override;

private:
    ~PooledSequencedTaskRunner() override;

    PooledTaskRunnerDelegate* const pooled_task_runner_delegate_;

    // Sequence for all Tasks posted through this TaskRunner.
    const scoped_refptr<Sequence> sequence_;
};

}  // namespace internal
}  // namespace base

#endif  // BASE_TASK_THREAD_POOL_POOLED_SEQUENCED_TASK_RUNNER_H_
