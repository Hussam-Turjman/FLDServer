// Copyright 2021 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_HASH_SHA1_BORINGSSL_H_
#define BASE_HASH_SHA1_BORINGSSL_H_

#include <openssl/sha.h>

#include <stdint.h>

namespace base
{
// Used for storing intermediate data during an SHA1 computation. Callers
// should not access the data.
using SHA1Context = SHA_CTX;
}  // namespace base

#endif  // BASE_HASH_SHA1_BORINGSSL_H_
