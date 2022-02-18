// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRINGS_SYS_STRING_CONVERSIONS_H_
#define BASE_STRINGS_SYS_STRING_CONVERSIONS_H_

// Provides system-dependent string type conversions for cases where it's
// necessary to not use ICU. Generally, you should not need this in Chrome,
// but it is used in some shared code. Dependencies should be minimal.

#include <stdint.h>

#include <string>

#include "fldserver/fldserver_config.h"
#include "fldserver/base/strings/string_piece.h"
#include "fldserver/fldserver_config.h"

#if IS_APPLE
#include <CoreFoundation/CoreFoundation.h>

#include "fldserver/base/mac/scoped_cftyperef.h"

#ifdef __OBJC__
@class NSString;
#else
class NSString;
#endif
#endif  // OS_APPLE

namespace base {

// Converts between wide and UTF-8 representations of a string. On error, the
// result is system-dependent.
CORE_EXPORT std::string SysWideToUTF8(const std::wstring& wide)
    WARN_UNUSED_RESULT;
CORE_EXPORT std::wstring SysUTF8ToWide(StringPiece utf8) WARN_UNUSED_RESULT;

// Converts between wide and the system multi-byte representations of a string.
// DANGER: This will lose information and can change (on Windows, this can
// change between reboots).
CORE_EXPORT std::string SysWideToNativeMB(const std::wstring& wide)
    WARN_UNUSED_RESULT;
CORE_EXPORT std::wstring SysNativeMBToWide(StringPiece native_mb)
    WARN_UNUSED_RESULT;

// Windows-specific ------------------------------------------------------------

#if IS_WIN

// Converts between 8-bit and wide strings, using the given code page. The
// code page identifier is one accepted by the Windows function
// MultiByteToWideChar().
CORE_EXPORT std::wstring SysMultiByteToWide(StringPiece mb, uint32_t code_page)
    WARN_UNUSED_RESULT;
CORE_EXPORT std::string SysWideToMultiByte(const std::wstring& wide,
                                           uint32_t code_page)
    WARN_UNUSED_RESULT;

#endif  // IS_WIN

// Mac-specific ----------------------------------------------------------------

#if IS_APPLE

// Converts between STL strings and CFStringRefs/NSStrings.

// Creates a string, and returns it with a refcount of 1. You are responsible
// for releasing it. Returns NULL on failure.
CORE_EXPORT ScopedCFTypeRef<CFStringRef> SysUTF8ToCFStringRef(StringPiece utf8)
    WARN_UNUSED_RESULT;
CORE_EXPORT ScopedCFTypeRef<CFStringRef> SysUTF16ToCFStringRef(
    StringPiece16 utf16) WARN_UNUSED_RESULT;

// Same, but returns an autoreleased NSString.
CORE_EXPORT NSString* SysUTF8ToNSString(StringPiece utf8) WARN_UNUSED_RESULT;
CORE_EXPORT NSString* SysUTF16ToNSString(StringPiece16 utf16)
    WARN_UNUSED_RESULT;

// Converts a CFStringRef to an STL string. Returns an empty string on failure.
CORE_EXPORT std::string SysCFStringRefToUTF8(CFStringRef ref)
    WARN_UNUSED_RESULT;
CORE_EXPORT std::u16string SysCFStringRefToUTF16(CFStringRef ref)
    WARN_UNUSED_RESULT;

// Same, but accepts NSString input. Converts nil NSString* to the appropriate
// string type of length 0.
CORE_EXPORT std::string SysNSStringToUTF8(NSString* ref) WARN_UNUSED_RESULT;
CORE_EXPORT std::u16string SysNSStringToUTF16(NSString* ref) WARN_UNUSED_RESULT;

#endif  // IS_APPLE

}  // namespace base

#endif  // BASE_STRINGS_SYS_STRING_CONVERSIONS_H_
