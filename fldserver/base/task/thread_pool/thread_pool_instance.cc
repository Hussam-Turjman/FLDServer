// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/task/thread_pool/thread_pool_instance.h"

#include "fldserver/base/check.h"
#include "fldserver/base/memory/ptr_util.h"
#include "fldserver/base/system/sys_info.h"
#include "fldserver/base/task/thread_pool/thread_pool_impl.h"
#include "fldserver/base/threading/platform_thread.h"
#include "fldserver/base/time/time.h"

#include <algorithm>

namespace base
{
namespace
{
// |g_thread_pool| is intentionally leaked on shutdown.
ThreadPoolInstance* g_thread_pool = nullptr;

}  // namespace

ThreadPoolInstance::InitParams::InitParams(int max_num_foreground_threads_in) :
    max_num_foreground_threads(max_num_foreground_threads_in)
{
}

ThreadPoolInstance::InitParams::~InitParams() = default;

ThreadPoolInstance::ScopedExecutionFence::ScopedExecutionFence()
{
    DCHECK(g_thread_pool);
    g_thread_pool->BeginFence();
}

ThreadPoolInstance::ScopedExecutionFence::~ScopedExecutionFence()
{
    DCHECK(g_thread_pool);
    g_thread_pool->EndFence();
}

ThreadPoolInstance::ScopedBestEffortExecutionFence::ScopedBestEffortExecutionFence()
{
    DCHECK(g_thread_pool);
    g_thread_pool->BeginBestEffortFence();
}

ThreadPoolInstance::ScopedBestEffortExecutionFence::~ScopedBestEffortExecutionFence()
{
    DCHECK(g_thread_pool);
    g_thread_pool->EndBestEffortFence();
}

#if !defined(OS_NACL)
// static
void
ThreadPoolInstance::CreateAndStartWithDefaultParams(StringPiece name)
{
    Create(name);
    g_thread_pool->StartWithDefaultParams();
}

void
ThreadPoolInstance::StartWithDefaultParams()
{
    // Values were chosen so that:
    // * There are few background threads.
    // * Background threads never outnumber foreground threads.
    // * The system is utilized maximally by foreground threads.
    // * The main thread is assumed to be busy, cap foreground workers at
    //   |num_cores - 1|.
    const int num_cores = SysInfo::NumberOfProcessors();
    const int max_num_foreground_threads = std::max(3, num_cores - 1);
    Start({max_num_foreground_threads});
}
#endif  // !defined(OS_NACL)

void
ThreadPoolInstance::Create(StringPiece name)
{
    Set(std::make_unique<internal::ThreadPoolImpl>(name));
}

// static
void
ThreadPoolInstance::Set(std::unique_ptr<ThreadPoolInstance> thread_pool)
{
    delete g_thread_pool;
    g_thread_pool = thread_pool.release();
}

// static
ThreadPoolInstance*
ThreadPoolInstance::Get()
{
    return g_thread_pool;
}

}  // namespace base
