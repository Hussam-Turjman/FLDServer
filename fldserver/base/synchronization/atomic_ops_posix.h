// Copyright 2021 The Blitz Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Original Author : Mohammed Hussam Al Turjman

#ifndef CORE_BASE_SYNC_ATOMIC_OPS_POSIX_H
#define CORE_BASE_SYNC_ATOMIC_OPS_POSIX_H

#include "fldserver/fldserver_config.h"

__CORE_BEGIN_DECLARATION

CORE_EXPORT int
AtomicCompareExchangeStrong(volatile int* obj, int* expected, int desired);
__CORE_END_DECLARATION

#endif  // CORE_BASE_SYNC_ATOMIC_OPS_POSIX_H
