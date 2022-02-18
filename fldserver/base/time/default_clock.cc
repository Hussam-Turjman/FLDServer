// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/time/default_clock.h"

#include "fldserver/base/no_destructor.h"

namespace base
{
DefaultClock::~DefaultClock() = default;

Time
DefaultClock::Now() const
{
    return Time::Now();
}

// static
DefaultClock*
DefaultClock::GetInstance()
{
    static base::NoDestructor<DefaultClock> instance;
    return instance.get();
}

}  // namespace base
