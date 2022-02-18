// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_HKDF_H_
#define CRYPTO_HKDF_H_

#include <stddef.h>

#include <string>

#include "fldserver/base/containers/span.h"
#include "fldserver/base/strings/string_piece.h"
#include "fldserver/base/crypto/crypto_export.h"

namespace crypto
{
CORE_EXPORT
std::string
HkdfSha256(base::StringPiece secret,
           base::StringPiece salt,
           base::StringPiece info,
           size_t derived_key_size);

CORE_EXPORT
std::vector<uint8_t>
HkdfSha256(base::span<const uint8_t> secret,
           base::span<const uint8_t> salt,
           base::span<const uint8_t> info,
           size_t derived_key_size);

}  // namespace crypto

#endif  // CRYPTO_HKDF_H_
