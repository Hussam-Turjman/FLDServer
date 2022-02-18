// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/threading/scoped_thread_priority.h"

#include "fldserver/base/location.h"
#include "fldserver/base/threading/platform_thread.h"

#include "fldserver/fldserver_config.h"

namespace base
{
namespace internal
{
ScopedMayLoadLibraryAtBackgroundPriority::ScopedMayLoadLibraryAtBackgroundPriority(
        const Location& from_here, std::atomic_bool* already_loaded)
#if IS_WIN
    :
    already_loaded_(already_loaded)
#endif  // OS_WIN
{
#if IS_WIN
    if (already_loaded_ && already_loaded_->load(std::memory_order_relaxed))
        return;

    const base::ThreadPriority priority = PlatformThread::GetCurrentThreadPriority();
    if (priority == base::ThreadPriority::BACKGROUND)
    {
        original_thread_priority_ = priority;
        PlatformThread::SetCurrentThreadPriority(base::ThreadPriority::NORMAL);
    }
#endif  // OS_WIN
}

ScopedMayLoadLibraryAtBackgroundPriority::~ScopedMayLoadLibraryAtBackgroundPriority()
{
    // Trace events must be closed in reverse order of opening so that they nest
    // correctly.
#if IS_WIN
    if (original_thread_priority_)
    {
        PlatformThread::SetCurrentThreadPriority(original_thread_priority_.value());
    }

    if (already_loaded_)
        already_loaded_->store(true, std::memory_order_relaxed);
#endif  // OS_WIN
}

}  // namespace internal
}  // namespace base
