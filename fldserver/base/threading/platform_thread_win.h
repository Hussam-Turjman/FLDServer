// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_PLATFORM_THREAD_WIN_H_
#define BASE_THREADING_PLATFORM_THREAD_WIN_H_

#include "fldserver/base/threading/platform_thread.h"
#include "fldserver/fldserver_config.h"

namespace base
{
namespace internal
{
// Assert that the memory priority of |thread| is |memory_priority|. No-op on
// Windows 7 because ::GetThreadInformation() is not available. Exposed for unit
// tests.
CORE_EXPORT void
AssertMemoryPriority(HANDLE thread, int memory_priority);

}  // namespace internal
}  // namespace base

#endif  // BASE_THREADING_PLATFORM_THREAD_WIN_H_
