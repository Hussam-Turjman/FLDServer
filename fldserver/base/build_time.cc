// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/build_time.h"

// Imports the generated build date, i.e. BUILD_DATE.
#include "fldserver/base/check.h"
#include "fldserver/base/time/time.h"

#define BUILD_DATE "May 02 2021 05:00:00"

namespace base
{
Time
GetBuildTime()
{
    Time integral_build_time;
    // BUILD_DATE is exactly "Mmm DD YYYY HH:MM:SS".
    // See //build/write_build_date_header.py. "HH:MM:SS" is expected to
    // be "05:00:00" in non-official builds but is not enforced here.
    bool result = Time::FromUTCString(BUILD_DATE, &integral_build_time);
    DCHECK(result);
    return integral_build_time;
}

}  // namespace base
