// Copyright (c) 2021 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/memory/nonscannable_memory.h"

#include "fldserver/base/no_destructor.h"

#include <stdlib.h>

#if 0
#include "fldserver/base/allocator/allocator_shim_default_dispatch_to_partition_alloc.h"
#include "fldserver/base/allocator/partition_allocator/starscan/pcscan.h"
#endif

namespace base
{
namespace internal
{
#if 0
NonScannableAllocator::NonScannableAllocator() = default;
NonScannableAllocator::~NonScannableAllocator() = default;

NonScannableAllocator&
NonScannableAllocator::Instance()
{
    static base::NoDestructor<NonScannableAllocator> instance;
    return *instance;
}

void*
NonScannableAllocator::Alloc(size_t size)
{
    // TODO(bikineev): Change to LIKELY once PCScan is enabled by default.
    if (UNLIKELY(pcscan_enabled_.load(std::memory_order_acquire)))
    {
        PA_DCHECK(allocator_.get());
        return allocator_->root()->AllocFlagsNoHooks(0, size);
    }
    // Otherwise, dispatch to default partition.
    return PartitionAllocMalloc::Allocator()->AllocFlagsNoHooks(0, size);
}

void
NonScannableAllocator::Free(void* ptr)
{
    ThreadSafePartitionRoot::FreeNoHooks(ptr);
}

void
NonScannableAllocator::EnablePCScan()
{
    allocator_.reset(MakePCScanMetadata<base::PartitionAllocator>());
    allocator_->init(PartitionOptions(PartitionOptions::AlignedAlloc::kDisallowed,
                                      PartitionOptions::ThreadCache::kDisabled,
                                      PartitionOptions::Quarantine::kAllowed,
                                      PartitionOptions::Cookies::kAllowed,
                                      PartitionOptions::RefCount::kDisallowed));
    PCScan::RegisterNonScannableRoot(allocator_->root());
    pcscan_enabled_.store(true, std::memory_order_release);
}
#endif  // 0

}  // namespace internal

void*
AllocNonScannable(size_t size)
{
#if 0
    return internal::NonScannableAllocator::Instance().Alloc(size);
#else
    return ::malloc(size);
#endif
}

void
FreeNonScannable(void* ptr)
{
#if 0
    internal::NonScannableAllocator::Free(ptr);
#else
    return ::free(ptr);
#endif
}

}  // namespace base
