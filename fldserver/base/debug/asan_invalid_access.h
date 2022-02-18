// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Defines some functions that intentionally do an invalid memory access in
// order to trigger an AddressSanitizer (ASan) error report.

#ifndef BASE_DEBUG_ASAN_INVALID_ACCESS_H_
#define BASE_DEBUG_ASAN_INVALID_ACCESS_H_

#include "fldserver/base/compiler_specific.h"
#include "fldserver/fldserver_config.h"

namespace base
{
namespace debug
{
#if defined(ADDRESS_SANITIZER) || 0

// Generates an heap buffer overflow.
CORE_EXPORT NOINLINE void
AsanHeapOverflow();

// Generates an heap buffer underflow.
CORE_EXPORT NOINLINE void
AsanHeapUnderflow();

// Generates an use after free.
CORE_EXPORT NOINLINE void
AsanHeapUseAfterFree();

// The "corrupt-block" and "corrupt-heap" classes of bugs is specific to
// Windows.
#if IS_WIN
// Corrupts a memory block and makes sure that the corruption gets detected when
// we try to free this block.
CORE_EXPORT NOINLINE void
AsanCorruptHeapBlock();

// Corrupts the heap and makes sure that the corruption gets detected when a
// crash occur.
CORE_EXPORT NOINLINE void
AsanCorruptHeap();

#endif  // OS_WIN
#endif  // ADDRESS_SANITIZER

}  // namespace debug
}  // namespace base

#endif  // BASE_DEBUG_ASAN_INVALID_ACCESS_H_
