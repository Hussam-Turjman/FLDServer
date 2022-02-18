// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PROCESS_PROCESS_INFO_H_
#define BASE_PROCESS_PROCESS_INFO_H_

#include "fldserver/fldserver_config.h"

namespace base
{
#if IS_WIN
enum IntegrityLevel
{
    INTEGRITY_UNKNOWN,
    UNTRUSTED_INTEGRITY,
    LOW_INTEGRITY,
    MEDIUM_INTEGRITY,
    HIGH_INTEGRITY,
};

// Returns the integrity level of the process. Returns INTEGRITY_UNKNOWN in the
// case of an underlying system failure.
CORE_EXPORT IntegrityLevel
GetCurrentProcessIntegrityLevel();

// Determines whether the current process is elevated.
CORE_EXPORT bool
IsCurrentProcessElevated();

#endif  // IS_WIN

}  // namespace base

#endif  // BASE_PROCESS_PROCESS_INFO_H_
