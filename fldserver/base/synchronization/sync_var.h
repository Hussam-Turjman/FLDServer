// Copyright 2021 The Blitz Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Original Author : Mohammed Hussam Al Turjman

#ifndef CORE_BASE_SYNCHRONIZATION_SYNC_VAR_H
#define CORE_BASE_SYNCHRONIZATION_SYNC_VAR_H

#include "fldserver/fldserver_config.h"
#include <memory>

namespace base
{
class CORE_EXPORT SyncVar
{
public:
    static std::unique_ptr<SyncVar>
    Create();
    virtual ~SyncVar(){};

    virtual void
    Wait(void* data) = 0;

    virtual void
    Signal(void** ret_data) = 0;
};
}  // namespace base

#endif  // CORE_BASE_SYNCHRONIZATION_SYNC_VAR_H
