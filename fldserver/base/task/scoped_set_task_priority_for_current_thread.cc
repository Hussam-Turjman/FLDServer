// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/task/scoped_set_task_priority_for_current_thread.h"

#include "fldserver/base/check_op.h"
#include "fldserver/base/lazy_instance.h"
#include "fldserver/base/threading/thread_local.h"

namespace base
{
namespace internal
{
namespace
{
LazyInstance<ThreadLocalPointer<const TaskPriority>>::Leaky tls_task_priority_for_current_thread =
        LAZY_INSTANCE_INITIALIZER;

}  // namespace

ScopedSetTaskPriorityForCurrentThread::ScopedSetTaskPriorityForCurrentThread(
        TaskPriority priority) :
    priority_(priority)
{
    DCHECK(!tls_task_priority_for_current_thread.Get().Get());
    tls_task_priority_for_current_thread.Get().Set(&priority_);
}

ScopedSetTaskPriorityForCurrentThread::~ScopedSetTaskPriorityForCurrentThread()
{
    DCHECK_EQ(&priority_, tls_task_priority_for_current_thread.Get().Get());
    tls_task_priority_for_current_thread.Get().Set(nullptr);
}

TaskPriority
GetTaskPriorityForCurrentThread()
{
    const TaskPriority* priority = tls_task_priority_for_current_thread.Get().Get();
    return priority ? *priority : TaskPriority::USER_BLOCKING;
}

}  // namespace internal
}  // namespace base
