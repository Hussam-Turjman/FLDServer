// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/debug/debugger.h"

#include <windows.h>

#include <stdlib.h>

#if 0
#include "fldserver/base/test/clang_profiling.h"
#endif

namespace base
{
namespace debug
{
bool
BeingDebugged()
{
    return ::IsDebuggerPresent() != 0;
}

void
BreakDebugger()
{
#if 0
    WriteClangProfilingProfile();
#endif

    if (IsDebugUISuppressed())
        _exit(1);

    __debugbreak();
}

void
VerifyDebugger()
{
}

}  // namespace debug
}  // namespace base
