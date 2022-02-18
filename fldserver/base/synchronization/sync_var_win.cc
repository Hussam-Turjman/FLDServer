// Copyright 2021 The Blitz Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Original Author : Mohammed Hussam Al Turjman

#include "fldserver/base/synchronization/sync_var_win.h"

#include "fldserver/base/check.h"
#include "fldserver/base/logging.h"
#include "fldserver/base/synchronization/waitable_event.h"

namespace base
{
SyncVarWin::SyncVarWin() : handle_()
{
}

SyncVarWin::~SyncVarWin()
{
}
bool
SyncVarWin::Initialize()
{
    handle_.Set(CreateEvent(nullptr, TRUE /*bManualReset*/, FALSE /*bInitialState*/, nullptr));
    return handle_.IsValid();
}
std::unique_ptr<SyncVarWin>
SyncVarWin::Create()
{
    std::unique_ptr<SyncVarWin> instance(new (std::nothrow) SyncVarWin());
    if (instance && instance->Initialize())
        return instance;
    return nullptr;
}
void
SyncVarWin::Wait(void* data)
{
    CHECK(handle_.IsValid());

    buffer_ = data;
    DWORD result = WaitForSingleObject(handle_.Get(), INFINITE);
    // It is most unexpected that this should ever fail.  Help consumers learn
    // about it if it should ever fail.
    CHECK(result != WAIT_FAILED);
    CHECK_EQ(WAIT_OBJECT_0, result);
    CHECK(ResetEvent(handle_.Get()));
}
void
SyncVarWin::Signal(void** ret_data)
{
    CHECK(handle_.IsValid());

    if (ret_data)
    {
        *ret_data = buffer_;
    }
    CHECK(SetEvent(handle_.Get()));
}
}  // namespace base