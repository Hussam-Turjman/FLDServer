// Copyright 2021 The Blitz Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Original Author : Mohammed Hussam Al Turjman

#ifndef CORE_BASE_SYNC_VAR_POSIX_H
#define CORE_BASE_SYNC_VAR_POSIX_H

#include "fldserver/fldserver_config.h"
#include "fldserver/base/synchronization/sync_var.h"
#include <pthread.h>
#include <semaphore.h>

#include <memory>

namespace base
{
class CORE_EXPORT SyncVarPosix : public SyncVar
{
public:
    static std::unique_ptr<SyncVarPosix>
    Create();

    CORE_DISALLOW_COPY_AND_ASSIGN(SyncVarPosix);

    void
    Wait(void* data) override;
    void
    Signal(void** ret_data) override;

    ~SyncVarPosix() override;

private:
    SyncVarPosix();
    bool
    Initialize();

private:
    void* buffer = nullptr;
    int active_;
    pthread_cond_t cond_var_;
    pthread_mutex_t cond_mutex_;
    sem_t waiter_;
    sem_t poster_;
    bool initialized_;
};
}  // namespace base
#endif  // CORE_BASE_SYNC_VAR_POSIX_H
