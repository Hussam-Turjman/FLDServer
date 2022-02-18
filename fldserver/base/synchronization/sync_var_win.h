// Copyright 2021 The Blitz Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Original Author : Mohammed Hussam Al Turjman

#ifndef CORE_SYNCHRONIZATION_BASE_SYNC_VAR_WIN_H
#define CORE_SYNCHRONIZATION_BASE_SYNC_VAR_WIN_H

#include "fldserver/base/synchronization/sync_var.h"
#include "fldserver/fldserver_config.h"
#include "fldserver/base/win/scoped_handle.h"

#include <windows.h>

#include <memory>
#include <atomic>

namespace base
{
class WaitableEvent;
class CORE_EXPORT SyncVarWin : public SyncVar
{
public:
    static std::unique_ptr<SyncVarWin>
    Create();

    CORE_DISALLOW_COPY_AND_ASSIGN(SyncVarWin);

    void
    Wait(void* data) override;
    void
    Signal(void** ret_data) override;

    ~SyncVarWin() override;

private:
    SyncVarWin();
    bool
    Initialize();

private:
    void* buffer_ = nullptr;
    win::ScopedHandle handle_;
};
}  // namespace base
#endif  // CORE_SYNCHRONIZATION_BASE_SYNC_VAR_WIN_H
