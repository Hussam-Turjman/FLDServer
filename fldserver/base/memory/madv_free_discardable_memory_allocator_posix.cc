// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/memory/madv_free_discardable_memory_allocator_posix.h"

#include "fldserver/base/process/process_metrics.h"
#include "fldserver/base/strings/string_number_conversions.h"
#include "fldserver/base/threading/thread_task_runner_handle.h"

#include <sys/mman.h>

#include <inttypes.h>

#if 0
#include "fldserver/base/trace_event/memory_dump_manager.h"  // no-presubmit-check
#endif                                                       // 0

namespace base
{
MadvFreeDiscardableMemoryAllocatorPosix::MadvFreeDiscardableMemoryAllocatorPosix()
{
#if 0
    // Don't register dump provider if ThreadTaskRunnerHandle is not set, such as
    // in tests and Android Webview.
    if (base::ThreadTaskRunnerHandle::IsSet())
    {
        trace_event::MemoryDumpManager::GetInstance()->RegisterDumpProvider(
                this, "MadvFreeDiscardableMemoryAllocator", ThreadTaskRunnerHandle::Get());
    }
#endif  // 0
}

MadvFreeDiscardableMemoryAllocatorPosix::~MadvFreeDiscardableMemoryAllocatorPosix()
{
#if 0
    trace_event::MemoryDumpManager::GetInstance()->UnregisterDumpProvider(this);
#endif  // 0
}

std::unique_ptr<DiscardableMemory>
MadvFreeDiscardableMemoryAllocatorPosix::AllocateLockedDiscardableMemory(size_t size)
{
    return std::make_unique<MadvFreeDiscardableMemoryPosix>(size, &bytes_allocated_);
}

size_t
MadvFreeDiscardableMemoryAllocatorPosix::GetBytesAllocated() const
{
    return bytes_allocated_;
}

}  // namespace base
