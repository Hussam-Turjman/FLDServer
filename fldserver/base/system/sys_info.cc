// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/system/sys_info.h"

#include "fldserver/base/base_switches.h"
#include "fldserver/base/bind.h"
#include "fldserver/base/callback.h"
#include "fldserver/base/command_line.h"
#include "fldserver/base/location.h"
#include "fldserver/base/no_destructor.h"
#include "fldserver/base/notreached.h"
#include "fldserver/base/system/sys_info_internal.h"
#include "fldserver/base/task/post_task.h"
#include "fldserver/base/task/task_traits.h"
#include "fldserver/base/task/thread_pool.h"
#include "fldserver/base/time/time.h"
#include "fldserver/fldserver_config.h"

#include <algorithm>

namespace base
{
namespace
{
static const int kLowMemoryDeviceThresholdMB = 512;
}  // namespace

// static
int64_t
SysInfo::AmountOfPhysicalMemory()
{
    if (base::CommandLine::ForCurrentProcess()->HasSwitch(switches::kEnableLowEndDeviceMode))
    {
        return kLowMemoryDeviceThresholdMB * 1024 * 1024;
    }

    return AmountOfPhysicalMemoryImpl();
}

// static
int64_t
SysInfo::AmountOfAvailablePhysicalMemory()
{
    if (base::CommandLine::ForCurrentProcess()->HasSwitch(switches::kEnableLowEndDeviceMode))
    {
        // Estimate the available memory by subtracting our memory used estimate
        // from the fake |kLowMemoryDeviceThresholdMB| limit.
        size_t memory_used = AmountOfPhysicalMemoryImpl() - AmountOfAvailablePhysicalMemoryImpl();
        size_t memory_limit = kLowMemoryDeviceThresholdMB * 1024 * 1024;
        // std::min ensures no underflow, as |memory_used| can be > |memory_limit|.
        return memory_limit - std::min(memory_used, memory_limit);
    }

    return AmountOfAvailablePhysicalMemoryImpl();
}

bool
SysInfo::IsLowEndDevice()
{
    if (base::CommandLine::ForCurrentProcess()->HasSwitch(switches::kEnableLowEndDeviceMode))
    {
        return true;
    }

    return IsLowEndDeviceImpl();
}

#if !defined(OS_ANDROID)

bool
DetectLowEndDevice()
{
    CommandLine* command_line = CommandLine::ForCurrentProcess();
    if (command_line->HasSwitch(switches::kEnableLowEndDeviceMode))
        return true;
    if (command_line->HasSwitch(switches::kDisableLowEndDeviceMode))
        return false;

    int ram_size_mb = SysInfo::AmountOfPhysicalMemoryMB();
    return (ram_size_mb > 0 && ram_size_mb <= kLowMemoryDeviceThresholdMB);
}

// static
bool
SysInfo::IsLowEndDeviceImpl()
{
    static base::NoDestructor<internal::LazySysInfoValue<bool, DetectLowEndDevice>> instance;
    return instance->value();
}
#endif

#if !IS_APPLE
std::string
SysInfo::HardwareModelName()
{
    return std::string();
}
#endif

void
SysInfo::GetHardwareInfo(base::OnceCallback<void(HardwareInfo)> callback)
{
#if IS_WIN || defined(OS_ANDROID) || IS_APPLE
    base::ThreadPool::PostTaskAndReplyWithResult(
            FROM_HERE, {base::MayBlock()},
            base::BindOnce(&GetHardwareInfoSync),
            std::move(callback));
#elif IS_LINUX || defined(OS_CHROMEOS)
    base::ThreadPool::PostTaskAndReplyWithResult(FROM_HERE,
                                                 {base::MayBlock()},
                                                 base::BindOnce(&GetHardwareInfoSync),
                                                 std::move(callback));
#else
    NOTIMPLEMENTED();
    base::ThreadPool::PostTask(FROM_HERE, {}, base::BindOnce(std::move(callback), HardwareInfo()));
#endif
}

// static
base::TimeDelta
SysInfo::Uptime()
{
    // This code relies on an implementation detail of TimeTicks::Now() - that
    // its return value happens to coincide with the system uptime value in
    // microseconds, on Win/Mac/iOS/Linux/ChromeOS and Android.
    int64_t uptime_in_microseconds = TimeTicks::Now().ToInternalValue();
    return base::TimeDelta::FromMicroseconds(uptime_in_microseconds);
}

// static
std::string
SysInfo::ProcessCPUArchitecture()
{
#if defined(ARCH_CPU_X86)
    return "x86";
#elif defined(ARCH_CPU_X86_64)
    return "x86_64";
#elif defined(ARCH_CPU_ARMEL)
    return "ARM";
#elif defined(ARCH_CPU_ARM64)
    return "ARM_64";
#else
    return std::string();
#endif
}

}  // namespace base
