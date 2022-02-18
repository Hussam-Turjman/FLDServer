// Copyright 2021 The Blitz Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Original Author : Mohammed Hussam Al Turjman

#include "fldserver/base/synchronization/sync_var_posix.h"
#include "fldserver/base/synchronization/atomic_ops_posix.h"
#include "fldserver/base/check_op.h"
#include <new>

namespace base
{
std::unique_ptr<SyncVarPosix>
SyncVarPosix::Create()
{
    std::unique_ptr<SyncVarPosix> instance(new (std::nothrow) SyncVarPosix());
    if (instance->Initialize())
        return instance;

    return nullptr;
}
void
SyncVarPosix::Wait(void* data)
{
    int one;

    CHECK_EQ(pthread_mutex_lock(&cond_mutex_), 0);

    CHECK_EQ(sem_post(&waiter_), 0);

    buffer = data;

    while (true)
    {
        one = 1;

        if (AtomicCompareExchangeStrong(&active_, &one, 0))
        {
            break;
        }

        CHECK_EQ(pthread_cond_wait(&cond_var_, &cond_mutex_), 0);
    }

    CHECK_EQ(pthread_mutex_unlock(&cond_mutex_), 0);

    CHECK_EQ(sem_wait(&poster_), 0);
}
void
SyncVarPosix::Signal(void** ret_data)
{
    int status;
    int zero;

    CHECK_EQ(sem_wait(&waiter_), 0);

    CHECK_EQ(pthread_mutex_lock(&cond_mutex_), 0);

    zero = 0;
    status = AtomicCompareExchangeStrong(&active_, &zero, 1);

    CHECK_NE(status, 0);

    CHECK_EQ(pthread_cond_signal(&cond_var_), 0);

    if (ret_data)
    {
        *ret_data = buffer;
    }

    CHECK_EQ(pthread_mutex_unlock(&cond_mutex_), 0);

    CHECK_EQ(sem_post(&poster_), 0);
}
SyncVarPosix::SyncVarPosix() : active_(0), initialized_(false)
{
}
bool
SyncVarPosix::Initialize()
{
    pthread_mutexattr_t mutexattr;
    pthread_condattr_t condattr;

    if (pthread_mutexattr_init(&mutexattr) != 0)
        goto _err;

    if (pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK) != 0)
        goto _err_1;

    if (pthread_mutex_init(&cond_mutex_, &mutexattr) != 0)
        goto _err_1;

    if (pthread_condattr_init(&condattr) != 0)
        goto _err_2;

    if (pthread_cond_init(&cond_var_, &condattr) != 0)
        goto _err_3;

    if (sem_init(&waiter_, 0, 0) != 0)
        goto _err_4;

    if (sem_init(&poster_, 0, 0) != 0)
        goto _err_5;

    CHECK_EQ(pthread_mutexattr_destroy(&mutexattr), 0);
    CHECK_EQ(pthread_condattr_destroy(&condattr), 0);
    active_ = 0;

    initialized_ = true;

    return true;

_err_5:
    CHECK_EQ(sem_destroy(&waiter_), 0);
_err_4:
    CHECK_EQ(pthread_cond_destroy(&cond_var_), 0);
_err_3:
    CHECK_EQ(pthread_condattr_destroy(&condattr), 0);
_err_2:
    CHECK_EQ(pthread_mutex_destroy(&cond_mutex_), 0);
_err_1:
    CHECK_EQ(pthread_mutexattr_destroy(&mutexattr), 0);
_err:
    return false;
}
SyncVarPosix::~SyncVarPosix()
{
    if (initialized_)
    {
        CHECK_EQ(sem_destroy(&waiter_), 0);
        CHECK_EQ(sem_destroy(&poster_), 0);
        CHECK_EQ(pthread_mutex_destroy(&cond_mutex_), 0);
        CHECK_EQ(pthread_cond_destroy(&cond_var_), 0);
    }
}

}  // namespace base