// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SCOPED_CLEAR_LAST_ERROR_H_
#define BASE_SCOPED_CLEAR_LAST_ERROR_H_

#include "fldserver/fldserver_config.h"

#include <errno.h>
// TODO(crbug.com/1010217) Remove once no #includers are getting base/macros.h
// by including this header.
#include "fldserver/base/macros.h"
#include "fldserver/fldserver_config.h"

namespace base
{
// ScopedClearLastError stores and resets the value of thread local error codes
// (errno, GetLastError()), and restores them in the destructor. This is useful
// to avoid side effects on these values in instrumentation functions that
// interact with the OS.

// Common implementation of ScopedClearLastError for all platforms. Use
// ScopedClearLastError instead.
class CORE_EXPORT ScopedClearLastErrorBase
{
public:
    ScopedClearLastErrorBase() : last_errno_(errno)
    {
        errno = 0;
    }
    ScopedClearLastErrorBase(const ScopedClearLastErrorBase&) = delete;
    ScopedClearLastErrorBase&
    operator=(const ScopedClearLastErrorBase&) = delete;
    ~ScopedClearLastErrorBase()
    {
        errno = last_errno_;
    }

private:
    const int last_errno_;
};

#if IS_WIN

// Windows specific implementation of ScopedClearLastError.
class CORE_EXPORT ScopedClearLastError : public ScopedClearLastErrorBase
{
public:
    ScopedClearLastError();
    ScopedClearLastError(const ScopedClearLastError&) = delete;
    ScopedClearLastError&
    operator=(const ScopedClearLastError&) = delete;
    ~ScopedClearLastError();

private:
    const unsigned long last_system_error_;
};

#elif IS_POSIX || defined(OS_FUCHSIA)

using ScopedClearLastError = ScopedClearLastErrorBase;

#endif  // IS_WIN

}  // namespace base

#endif  // BASE_SCOPED_CLEAR_LAST_ERROR_H_
