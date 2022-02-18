// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/process/memory.h"

#if IS_WIN
#include <windows.h>
#else
#include <unistd.h>
#endif  // IS_WIN

#include "fldserver/base/debug/alias.h"
#include "fldserver/base/immediate_crash.h"
#include "fldserver/base/logging.h"
#include "fldserver/base/stl_util.h"

#include <string.h>
#if 0

#endif
#include "fldserver/fldserver_config.h"

namespace base
{
size_t g_oom_size = 0U;

namespace internal
{
// Crash server classifies base::internal::OnNoMemoryInternal as OOM.
NOINLINE void
OnNoMemoryInternal(size_t size)
{
    g_oom_size = size;
#if IS_WIN
    // Kill the process. This is important for security since most of code
    // does not check the result of memory allocation.
    // https://msdn.microsoft.com/en-us/library/het71c37.aspx
    // Pass the size of the failed request in an exception argument.
    ULONG_PTR exception_args[] = {size};
    ::RaiseException(base::win::kOomExceptionCode,
                     EXCEPTION_NONCONTINUABLE,
                     base::size(exception_args),
                     exception_args);

    // Safety check, make sure process exits here.
    _exit(win::kOomExceptionCode);
#else
    size_t tmp_size = size;
    base::debug::Alias(&tmp_size);

    // Note: Don't add anything that may allocate here. Depending on the
    // allocator, this may be called from within the allocator (e.g. with
    // PartitionAlloc), and would deadlock as our locks are not recursive.
    //
    // Additionally, this is unlikely to work, since allocating from an OOM
    // handler is likely to fail.
    //
    // Use IMMEDIATE_CRASH() so that the top frame in the crash is our code,
    // rather than using abort() or similar; this avoids the crash server needing
    // to be able to successfully unwind through libc to get to the correct
    // address, which is particularly an issue on Android.
    IMMEDIATE_CRASH();
#endif  // IS_WIN
}

}  // namespace internal

void
TerminateBecauseOutOfMemory(size_t size)
{
    internal::OnNoMemoryInternal(size);
}

// Defined in memory_mac.mm for Mac.
#if !IS_APPLE

bool
UncheckedCalloc(size_t num_items, size_t size, void** result)
{
    const size_t alloc_size = num_items * size;

    // Overflow check
    if (size && ((alloc_size / size) != num_items))
    {
        *result = nullptr;
        return false;
    }

    if (!UncheckedMalloc(alloc_size, result))
        return false;

    memset(*result, 0, alloc_size);
    return true;
}

#endif  // IS_APPLE

namespace internal
{
bool
ReleaseAddressSpaceReservation()
{
#if 0
    return ReleaseReservation();
#else
    return false;
#endif
}
}  // namespace internal

}  // namespace base
