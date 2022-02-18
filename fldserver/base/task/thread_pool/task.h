// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TASK_THREAD_POOL_TASK_H_
#define BASE_TASK_THREAD_POOL_TASK_H_

#include "fldserver/base/callback.h"
#include "fldserver/base/location.h"
#include "fldserver/base/memory/ref_counted.h"
#include "fldserver/base/pending_task.h"
#include "fldserver/base/sequenced_task_runner.h"
#include "fldserver/base/single_thread_task_runner.h"
#include "fldserver/base/time/time.h"
#include "fldserver/fldserver_config.h"

namespace base
{
namespace internal
{
// A task is a unit of work inside the thread pool. Support for tracing and
// profiling inherited from PendingTask.
// TODO(etiennep): This class is now equivalent to PendingTask, remove it.
struct CORE_EXPORT Task : public PendingTask
{
    Task() = default;

    // |posted_from| is the site the task was posted from. |task| is the closure
    // to run. |delay| is a delay that must expire before the Task runs.
    Task(const Location& posted_from, OnceClosure task, TimeTicks queue_time, TimeDelta delay);

    // Task is move-only to avoid mistakes that cause reference counts to be
    // accidentally bumped.
    Task(Task&& other) noexcept;

    Task(const Task&) = delete;
    Task&
    operator=(const Task&) = delete;

    ~Task() = default;

    Task&
    operator=(Task&& other);
};

}  // namespace internal
}  // namespace base

#endif  // BASE_TASK_THREAD_POOL_TASK_H_
