// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_PLATFORM_THREAD_INTERNAL_POSIX_H_
#define BASE_THREADING_PLATFORM_THREAD_INTERNAL_POSIX_H_

#include "fldserver/base/optional.h"
#include "fldserver/base/threading/platform_thread.h"
#include "fldserver/fldserver_config.h"

namespace base
{
namespace internal
{
struct ThreadPriorityToNiceValuePair
{
    ThreadPriority priority;
    int nice_value;
};
// The elements must be listed in the order of increasing priority (lowest
// priority first), that is, in the order of decreasing nice values (highest
// nice value first).
CORE_EXPORT extern const ThreadPriorityToNiceValuePair kThreadPriorityToNiceValueMap[4];

// Returns the nice value matching |priority| based on the platform-specific
// implementation of kThreadPriorityToNiceValueMap.
int
ThreadPriorityToNiceValue(ThreadPriority priority);

// Returns the ThreadPrioirty matching |nice_value| based on the platform-
// specific implementation of kThreadPriorityToNiceValueMap.
CORE_EXPORT ThreadPriority
NiceValueToThreadPriority(int nice_value);

// If non-nullopt, this return value will be used as the platform-specific
// result of CanIncreaseThreadPriority().
Optional<bool>
CanIncreaseCurrentThreadPriorityForPlatform(ThreadPriority priority);

// Allows platform specific tweaks to the generic POSIX solution for
// SetCurrentThreadPriority(). Returns true if the platform-specific
// implementation handled this |priority| change, false if the generic
// implementation should instead proceed.
bool
SetCurrentThreadPriorityForPlatform(ThreadPriority priority);

// If non-null, this return value will be used as the platform-specific result
// of CanIncreaseThreadPriority().
Optional<ThreadPriority>
GetCurrentThreadPriorityForPlatform();

#if IS_LINUX || defined(OS_CHROMEOS)
// Current thread id is cached in thread local storage for performance reasons.
// In some rare cases it's important to clear that cache explicitly (e.g. after
// going through clone() syscall which does not call pthread_atfork()
// handlers).
CORE_EXPORT void
ClearTidCache();
#endif  // IS_LINUX || defined(OS_CHROMEOS)

}  // namespace internal

}  // namespace base

#endif  // BASE_THREADING_PLATFORM_THREAD_INTERNAL_POSIX_H_
