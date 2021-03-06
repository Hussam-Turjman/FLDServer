// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/crypto/secure_util.h"

#include <openssl/crypto.h>

//#include "openssl/mem.h"

namespace crypto
{
bool
SecureMemEqual(const void* s1, const void* s2, size_t n)
{
    return CRYPTO_memcmp(s1, s2, n) == 0;
}

}  // namespace crypto
