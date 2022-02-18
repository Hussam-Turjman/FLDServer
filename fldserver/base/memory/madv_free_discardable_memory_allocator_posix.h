// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MEMORY_MADV_FREE_DISCARDABLE_MEMORY_ALLOCATOR_POSIX_H_
#define BASE_MEMORY_MADV_FREE_DISCARDABLE_MEMORY_ALLOCATOR_POSIX_H_

#include "fldserver/base/bind.h"
#include "fldserver/base/callback.h"
#include "fldserver/base/macros.h"
#include "fldserver/base/memory/discardable_memory.h"
#include "fldserver/base/memory/discardable_memory_allocator.h"
#include "fldserver/base/memory/madv_free_discardable_memory_posix.h"

#include "fldserver/fldserver_config.h"

#include <stddef.h>

#include <atomic>
#include <memory>

namespace base
{
class CORE_EXPORT MadvFreeDiscardableMemoryAllocatorPosix : public DiscardableMemoryAllocator
{
public:
    MadvFreeDiscardableMemoryAllocatorPosix();
    ~MadvFreeDiscardableMemoryAllocatorPosix() override;

    std::unique_ptr<DiscardableMemory>
    AllocateLockedDiscardableMemory(size_t size) override;

    size_t
    GetBytesAllocated() const override;

    void
    ReleaseFreeMemory() override
    {
        // Do nothing, since MADV_FREE discardable memory does not keep any memory
        // overhead that can be released.
    }

private:
    std::atomic<size_t> bytes_allocated_{0};

    DISALLOW_COPY_AND_ASSIGN(MadvFreeDiscardableMemoryAllocatorPosix);
};
}  // namespace base

#endif  // BASE_MEMORY_MADV_FREE_DISCARDABLE_MEMORY_ALLOCATOR_POSIX_H_
