// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/task/thread_pool/task_tracker_posix.h"

#include "fldserver/base/files/file_descriptor_watcher_posix.h"

#include <utility>

namespace base
{
namespace internal
{
TaskTrackerPosix::TaskTrackerPosix() = default;
TaskTrackerPosix::~TaskTrackerPosix() = default;

void
TaskTrackerPosix::RunTask(Task task, TaskSource* task_source, const TaskTraits& traits)
{
    DCHECK(io_thread_task_runner_);
    FileDescriptorWatcher file_descriptor_watcher(io_thread_task_runner_);
    TaskTracker::RunTask(std::move(task), task_source, traits);
}

}  // namespace internal
}  // namespace base
