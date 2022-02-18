// Copyright 2021 The Blitz Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Original Author : Mohammed Hussam Al Turjman

#include "fldserver/base/synchronization/sync_var.h"

#if IS_POSIX
#include "fldserver/base/synchronization/sync_var_posix.h"
#endif

#if IS_WIN
#include "fldserver/base/synchronization/sync_var_win.h"
#endif

namespace base
{
std::unique_ptr<SyncVar>
SyncVar::Create()
{
#if IS_POSIX
    return SyncVarPosix::Create();
#elif IS_WIN
    return SyncVarWin::Create();
#else
    return nullptr;
#endif
}
}  // namespace base