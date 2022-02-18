// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/notreached.h"

// This is a widely included header and its size has significant impact on
// build time. Try not to raise this limit unless absolutely necessary. See
// https://chromium.googlesource.com/chromium/src/+/HEAD/docs/wmax_tokens.md
#ifndef NACL_TC_REV
#pragma clang max_tokens_here 17000
#endif

#include "fldserver/base/logging.h"

namespace logging
{
CORE_EXPORT void
LogErrorNotReached(const char* file, int line)
{
    LogMessage(file, line, LOG_ERROR).stream() << "NOTREACHED() hit.";
}

}  // namespace logging
