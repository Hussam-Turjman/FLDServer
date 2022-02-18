// Copyright (c) 2021 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MEMORY_NONSCANNABLE_MEMORY_H_
#define BASE_MEMORY_NONSCANNABLE_MEMORY_H_

#include "fldserver/fldserver_config.h"

#include <atomic>
#include <cstdint>
#include <memory>

#if 0
#include "fldserver/base/allocator/partition_allocator/partition_alloc.h"
#include "fldserver/base/allocator/partition_allocator/starscan/metadata_allocator.h"
#endif

// This file contains allocation/deallocation functions for memory that doesn't
// need to be scanned by PCScan. Such memory should only contain "data" objects,
// i.e. objects that don't have pointers/references to other objects. An example
// would be strings or socket/IPC/file buffers. Use with caution.
namespace base
{
#if 0
template <typename>
class NoDestructor;

namespace internal
{
// Represents allocator that contains memory for data-like objects (that don't
// contain pointers) and therefore doesn't require scanning.
class CORE_EXPORT NonScannableAllocator final
{
public:
    static NonScannableAllocator&
    Instance();

    NonScannableAllocator(const NonScannableAllocator&) = delete;
    NonScannableAllocator&
    operator=(const NonScannableAllocator&) = delete;

    void*
    Alloc(size_t size);
    static void
    Free(void*);

    // Returns PartitionRoot corresponding to the allocator, or nullptr if the
    // allocator is not enabled.
    ThreadSafePartitionRoot*
    root()
    {
        if (!allocator_.get())
            return nullptr;
        return allocator_->root();
    }

    void
    EnablePCScan();

private:
    template <typename>
    friend class base::NoDestructor;

    NonScannableAllocator();
    ~NonScannableAllocator();

    std::unique_ptr<base::PartitionAllocator, PCScanMetadataDeleter> allocator_;
    std::atomic_bool pcscan_enabled_{false};
};

}  // namespace internal
#endif  // 0

// Allocate/free non-scannable memory.
CORE_EXPORT void*
AllocNonScannable(size_t size);
CORE_EXPORT void
FreeNonScannable(void* ptr);

// Deleters to be used with std::unique_ptr.
struct NonScannableDeleter
{
    void
    operator()(void* ptr) const
    {
        FreeNonScannable(ptr);
    }
};

}  // namespace base

#endif  // BASE_MEMORY_NONSCANNABLE_MEMORY_H_
