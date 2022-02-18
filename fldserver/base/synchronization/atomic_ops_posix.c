// Copyright 2021 The Blitz Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Original Author : Mohammed Hussam Al Turjman

#include <stdatomic.h>

int
AtomicCompareExchangeStrong(volatile int* obj, int* expected, int desired)
{
    return atomic_compare_exchange_strong(obj, expected, desired);
}