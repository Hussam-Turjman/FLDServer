// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILES_SCOPED_FILE_H_
#define BASE_FILES_SCOPED_FILE_H_

#include "fldserver/base/scoped_generic.h"
#include "fldserver/fldserver_config.h"

#include <stdio.h>

#include <memory>

namespace base
{
namespace internal
{
#if defined(OS_ANDROID)
// Use fdsan on android.
struct CORE_EXPORT ScopedFDCloseTraits : public ScopedGenericOwnershipTracking
{
    static int
    InvalidValue()
    {
        return -1;
    }
    static void
    Free(int);
    static void
    Acquire(const ScopedGeneric<int, ScopedFDCloseTraits>&, int);
    static void
    Release(const ScopedGeneric<int, ScopedFDCloseTraits>&, int);
};
#elif IS_POSIX || defined(OS_FUCHSIA)
struct CORE_EXPORT ScopedFDCloseTraits
{
    static int
    InvalidValue()
    {
        return -1;
    }
    static void
    Free(int fd);
};
#endif

// Functor for |ScopedFILE| (below).
struct ScopedFILECloser
{
    inline void
    operator()(FILE* x) const
    {
        if (x)
            fclose(x);
    }
};

}  // namespace internal

// -----------------------------------------------------------------------------

#if IS_POSIX || defined(OS_FUCHSIA)
// A low-level Posix file descriptor closer class. Use this when writing
// platform-specific code, especially that does non-file-like things with the
// FD (like sockets).
//
// If you're writing low-level Windows code, see base/win/scoped_handle.h
// which provides some additional functionality.
//
// If you're writing cross-platform code that deals with actual files, you
// should generally use base::File instead which can be constructed with a
// handle, and in addition to handling ownership, has convenient cross-platform
// file manipulation functions on it.
typedef ScopedGeneric<int, internal::ScopedFDCloseTraits> ScopedFD;
#endif

// Automatically closes |FILE*|s.
typedef std::unique_ptr<FILE, internal::ScopedFILECloser> ScopedFILE;

}  // namespace base

#endif  // BASE_FILES_SCOPED_FILE_H_
