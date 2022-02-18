// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRINGS_STRCAT_WIN_H_
#define BASE_STRINGS_STRCAT_WIN_H_

#include "fldserver/base/compiler_specific.h"
#include "fldserver/base/containers/span.h"
#include "fldserver/base/strings/string_piece.h"
#include "fldserver/fldserver_config.h"

#include <initializer_list>
#include <string>

namespace base
{
// The following section contains overloads of the cross-platform APIs for
// std::wstring and base::WStringPiece.
CORE_EXPORT void
StrAppend(std::wstring* dest, span<const WStringPiece> pieces);
CORE_EXPORT void
StrAppend(std::wstring* dest, span<const std::wstring> pieces);

inline void
StrAppend(std::wstring* dest, std::initializer_list<WStringPiece> pieces)
{
    StrAppend(dest, make_span(pieces));
}

CORE_EXPORT std::wstring
StrCat(span<const WStringPiece> pieces) WARN_UNUSED_RESULT;
CORE_EXPORT std::wstring
StrCat(span<const std::wstring> pieces) WARN_UNUSED_RESULT;

inline std::wstring
StrCat(std::initializer_list<WStringPiece> pieces)
{
    return StrCat(make_span(pieces));
}

}  // namespace base

#endif  // BASE_STRINGS_STRCAT_WIN_H_
