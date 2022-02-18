// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_RANDOM_H_
#define CRYPTO_RANDOM_H_

#include <stddef.h>

#include "fldserver/base/containers/span.h"
#include "fldserver/base/crypto/crypto_export.h"

namespace crypto
{
// Fills the given buffer with |length| random bytes of cryptographically
// secure random numbers.
// |length| must be positive.
CORE_EXPORT void
RandBytes(void* bytes, size_t length);

// Fills |bytes| with cryptographically-secure random bits.
CORE_EXPORT void
RandBytes(base::span<uint8_t> bytes);
}  // namespace crypto

#endif  // CRYPTO_RANDOM_H_
