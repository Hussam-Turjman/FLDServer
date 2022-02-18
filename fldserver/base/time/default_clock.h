// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TIME_DEFAULT_CLOCK_H_
#define BASE_TIME_DEFAULT_CLOCK_H_

#include "fldserver/base/compiler_specific.h"
#include "fldserver/base/time/clock.h"
#include "fldserver/fldserver_config.h"

namespace base
{
// DefaultClock is a Clock implementation that uses Time::Now().
class CORE_EXPORT DefaultClock : public Clock
{
public:
    ~DefaultClock() override;

    // Simply returns Time::Now().
    Time
    Now() const override;

    // Returns a shared instance of DefaultClock. This is thread-safe.
    static DefaultClock*
    GetInstance();
};

}  // namespace base

#endif  // BASE_TIME_DEFAULT_CLOCK_H_
