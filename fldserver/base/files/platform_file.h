// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILES_PLATFORM_FILE_H_
#define BASE_FILES_PLATFORM_FILE_H_

#include "fldserver/base/files/scoped_file.h"
#include "fldserver/fldserver_config.h"

#if IS_WIN
#include "fldserver/base/win/scoped_handle.h"
#include "fldserver/base/win/windows_types.h"
#endif

// This file defines platform-independent types for dealing with
// platform-dependent files. If possible, use the higher-level base::File class
// rather than these primitives.

namespace base
{
#if IS_WIN

using PlatformFile = HANDLE;
using ScopedPlatformFile = ::base::win::ScopedHandle;

// It would be nice to make this constexpr but INVALID_HANDLE_VALUE is a
// ((void*)(-1)) which Clang rejects since reinterpret_cast is technically
// disallowed in constexpr. Visual Studio accepts this, however.
const PlatformFile kInvalidPlatformFile = INVALID_HANDLE_VALUE;

#elif IS_POSIX || defined(OS_FUCHSIA)

using PlatformFile = int;
using ScopedPlatformFile = ::base::ScopedFD;

constexpr PlatformFile kInvalidPlatformFile = -1;

#endif

}  // namespace base

#endif  // BASE_FILES_PLATFORM_FILE_H_
