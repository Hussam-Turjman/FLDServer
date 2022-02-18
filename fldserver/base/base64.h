// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_BASE64_H_
#define BASE_BASE64_H_

#include "fldserver/base/containers/span.h"
#include "fldserver/base/strings/string_piece.h"
#include "fldserver/fldserver_config.h"

#include <string>

namespace base
{
// Encodes the input binary data in base64.
CORE_EXPORT std::string
Base64Encode(span<const uint8_t> input);

// Encodes the input string in base64.
CORE_EXPORT void
Base64Encode(const StringPiece& input, std::string* output);

// Decodes the base64 input string.  Returns true if successful and false
// otherwise. The output string is only modified if successful. The decoding can
// be done in-place.
CORE_EXPORT bool
Base64Decode(const StringPiece& input, std::string* output);

}  // namespace base

#endif  // BASE_BASE64_H_
