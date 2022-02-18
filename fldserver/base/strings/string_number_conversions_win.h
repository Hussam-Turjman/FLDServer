// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRINGS_STRING_NUMBER_CONVERSIONS_WIN_H_
#define BASE_STRINGS_STRING_NUMBER_CONVERSIONS_WIN_H_

#include "fldserver/base/strings/string_piece.h"
#include "fldserver/fldserver_config.h"

#include <string>

namespace base
{
CORE_EXPORT std::wstring
NumberToWString(int value);
CORE_EXPORT std::wstring
NumberToWString(unsigned int value);
CORE_EXPORT std::wstring
NumberToWString(long value);
CORE_EXPORT std::wstring
NumberToWString(unsigned long value);
CORE_EXPORT std::wstring
NumberToWString(long long value);
CORE_EXPORT std::wstring
NumberToWString(unsigned long long value);
CORE_EXPORT std::wstring
NumberToWString(double value);

// The following section contains overloads of the cross-platform APIs for
// std::wstring and base::WStringPiece.
CORE_EXPORT bool
StringToInt(WStringPiece input, int* output);
CORE_EXPORT bool
StringToUint(WStringPiece input, unsigned* output);
CORE_EXPORT bool
StringToInt64(WStringPiece input, int64_t* output);
CORE_EXPORT bool
StringToUint64(WStringPiece input, uint64_t* output);
CORE_EXPORT bool
StringToSizeT(WStringPiece input, size_t* output);
CORE_EXPORT bool
StringToDouble(WStringPiece input, double* output);

}  // namespace base

#endif  // BASE_STRINGS_STRING_NUMBER_CONVERSIONS_WIN_H_
