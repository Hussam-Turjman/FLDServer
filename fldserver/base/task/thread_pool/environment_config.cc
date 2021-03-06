// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/task/thread_pool/environment_config.h"

#include "fldserver/base/base_switches.h"
#include "fldserver/base/command_line.h"
#include "fldserver/base/synchronization/lock.h"
#include "fldserver/base/threading/platform_thread.h"
#include "fldserver/fldserver_config.h"

namespace base
{
namespace internal
{
namespace
{
bool
CanUseBackgroundPriorityForWorkerThreadImpl()
{
    // Commandline flag overrides (e.g. for experiments). Note that it may not be
    // initialized yet, e.g. in cronet.
    if (CommandLine::InitializedForCurrentProcess()
        && CommandLine::ForCurrentProcess()->HasSwitch(switches::kEnableBackgroundThreadPool))
    {
        return true;
    }

    // When Lock doesn't handle multiple thread priorities, run all
    // WorkerThread with a normal priority to avoid priority inversion when a
    // thread running with a normal priority tries to acquire a lock held by a
    // thread running with a background priority.
    if (!Lock::HandlesMultipleThreadPriorities())
        return false;

#if !defined(OS_ANDROID)
    // When thread priority can't be increased to NORMAL, run all threads with a
    // NORMAL priority to avoid priority inversions on shutdown (ThreadPoolImpl
    // increases BACKGROUND threads priority to NORMAL on shutdown while resolving
    // remaining shutdown blocking tasks).
    //
    // This is ignored on Android, because it doesn't have a clean shutdown phase.
    if (!PlatformThread::CanIncreaseThreadPriority(ThreadPriority::NORMAL))
        return false;
#endif  // defined(OS_ANDROID)

    return true;
}

}  // namespace

bool
CanUseBackgroundPriorityForWorkerThread()
{
    static const bool can_use_background_priority_for_worker_thread =
            CanUseBackgroundPriorityForWorkerThreadImpl();
    return can_use_background_priority_for_worker_thread;
}

}  // namespace internal
}  // namespace base
