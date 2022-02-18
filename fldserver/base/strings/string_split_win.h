// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRINGS_STRING_SPLIT_WIN_H_
#define BASE_STRINGS_STRING_SPLIT_WIN_H_

#include "fldserver/base/compiler_specific.h"
#include "fldserver/base/strings/string_piece.h"
#include "fldserver/base/strings/string_split.h"
#include "fldserver/fldserver_config.h"

#include <string>
#include <vector>

namespace base
{
// The following section contains overloads of the cross-platform APIs for
// std::wstring and base::WStringPiece.
CORE_EXPORT std::vector<std::wstring>
SplitString(WStringPiece input,
            WStringPiece separators,
            WhitespaceHandling whitespace,
            SplitResult result_type) WARN_UNUSED_RESULT;

CORE_EXPORT std::vector<WStringPiece>
SplitStringPiece(WStringPiece input,
                 WStringPiece separators,
                 WhitespaceHandling whitespace,
                 SplitResult result_type) WARN_UNUSED_RESULT;

CORE_EXPORT std::vector<std::wstring>
SplitStringUsingSubstr(WStringPiece input,
                       WStringPiece delimiter,
                       WhitespaceHandling whitespace,
                       SplitResult result_type) WARN_UNUSED_RESULT;

CORE_EXPORT std::vector<WStringPiece>
SplitStringPieceUsingSubstr(WStringPiece input,
                            WStringPiece delimiter,
                            WhitespaceHandling whitespace,
                            SplitResult result_type) WARN_UNUSED_RESULT;

}  // namespace base

#endif  // BASE_STRINGS_STRING_SPLIT_WIN_H_
