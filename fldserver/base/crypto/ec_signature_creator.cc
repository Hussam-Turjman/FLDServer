// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/crypto/ec_signature_creator.h"

#include "fldserver/base/check.h"
#include "fldserver/base/memory/ptr_util.h"
#include "fldserver/base/crypto/ec_signature_creator_impl.h"

namespace crypto
{
namespace
{
ECSignatureCreatorFactory* g_factory_ = nullptr;

}  // namespace

// static
std::unique_ptr<ECSignatureCreator>
ECSignatureCreator::Create(ECPrivateKey* key)
{
    if (g_factory_)
        return g_factory_->Create(key);
    return std::make_unique<ECSignatureCreatorImpl>(key);
}

// static
void
ECSignatureCreator::SetFactoryForTesting(ECSignatureCreatorFactory* factory)
{
    // We should always clear the factory after each test to avoid
    // use-after-free problems.
    DCHECK(!g_factory_ || !factory);
    g_factory_ = factory;
}

}  // namespace crypto
