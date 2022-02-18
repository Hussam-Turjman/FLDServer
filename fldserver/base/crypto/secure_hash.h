// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_SECURE_HASH_H_
#define CRYPTO_SECURE_HASH_H_

#include <stddef.h>

#include <memory>

#include "fldserver/base/macros.h"
#include "fldserver/base/crypto/crypto_export.h"

namespace crypto
{
// A wrapper to calculate secure hashes incrementally, allowing to
// be used when the full input is not known in advance. The end result will the
// same as if we have the full input in advance.
class CORE_EXPORT SecureHash
{
public:
    enum Algorithm
    {
        SHA256,
    };
    virtual ~SecureHash()
    {
    }

    static std::unique_ptr<SecureHash>
    Create(Algorithm type);

    virtual void
    Update(const void* input, size_t len) = 0;
    virtual void
    Finish(void* output, size_t len) = 0;
    virtual size_t
    GetHashLength() const = 0;

    // Create a clone of this SecureHash. The returned clone and this both
    // represent the same hash state. But from this point on, calling
    // Update()/Finish() on either doesn't affect the state of the other.
    virtual std::unique_ptr<SecureHash>
    Clone() const = 0;

protected:
    SecureHash()
    {
    }

private:
    DISALLOW_COPY_AND_ASSIGN(SecureHash);
};

}  // namespace crypto

#endif  // CRYPTO_SECURE_HASH_H_
