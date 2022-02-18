// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/win/atl_throw.h"

#include "fldserver/base/compiler_specific.h"
#include "fldserver/base/debug/alias.h"
#include "fldserver/base/immediate_crash.h"
#include "fldserver/base/process/memory.h"

#include <winerror.h>

namespace base
{
namespace win
{
NOINLINE void __stdcall AtlThrowImpl(HRESULT hr)
{
    base::debug::Alias(&hr);
    if (hr == E_OUTOFMEMORY)
        base::TerminateBecauseOutOfMemory(0);
    IMMEDIATE_CRASH();
}

}  // namespace win
}  // namespace base
