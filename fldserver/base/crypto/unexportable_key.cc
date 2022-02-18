// Copyright (c) 2021 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/crypto/unexportable_key.h"

#include "fldserver/base/bind.h"
#include "fldserver/base/check.h"
#include "fldserver/base/metrics/histogram_functions.h"
#include "fldserver/base/task/task_traits.h"
#include "fldserver/base/task/thread_pool.h"
#include "fldserver/fldserver_config.h"

namespace crypto
{
UnexportableSigningKey::~UnexportableSigningKey() = default;
UnexportableKeyProvider::~UnexportableKeyProvider() = default;

#if IS_WIN
std::unique_ptr<UnexportableKeyProvider>
GetUnexportableKeyProviderWin();
#endif

static std::unique_ptr<UnexportableKeyProvider> (*g_mock_provider)() = nullptr;

std::unique_ptr<UnexportableKeyProvider>
GetUnexportableKeyProvider()
{
    if (g_mock_provider)
    {
        return g_mock_provider();
    }

#if IS_WIN
    return GetUnexportableKeyProviderWin();
#else
    return nullptr;
#endif
}

#if IS_WIN

// Note that values here are used in a recorded histogram. Don't change the
// values of existing members.
enum TPMSupport
{
    kNone = 0,
    kRSA = 1,
    kECDSA = 2,
    kMaxValue = 2,
};

void
MeasureTPMAvailability()
{
    // Temporarily measure the fraction of Windows machines that have TPMs, and
    // what the best supported algorithm is.
    // TODO: remove by 2021-04-01.
    base::ThreadPool::PostTask(
            FROM_HERE, {base::MayBlock(), base::TaskPriority::BEST_EFFORT}, base::BindOnce([]() {
                TPMSupport result = TPMSupport::kNone;
                std::unique_ptr<UnexportableKeyProvider> provider = GetUnexportableKeyProvider();
                if (provider)
                {
                    const SignatureVerifier::SignatureAlgorithm kAllAlgorithms[] = {
                            SignatureVerifier::SignatureAlgorithm::ECDSA_SHA256,
                            SignatureVerifier::SignatureAlgorithm::RSA_PKCS1_SHA256,
                    };
                    base::Optional<SignatureVerifier::SignatureAlgorithm> algo =
                            provider->SelectAlgorithm(kAllAlgorithms);
                    if (algo)
                    {
                        switch (*algo)
                        {
                            case SignatureVerifier::SignatureAlgorithm::ECDSA_SHA256:
                                result = TPMSupport::kECDSA;
                                break;
                            case SignatureVerifier::SignatureAlgorithm::RSA_PKCS1_SHA256:
                                result = TPMSupport::kRSA;
                                break;
                            default: break;
                        }
                    }
                }

                base::UmaHistogramEnumeration("Crypto.TPMSupport", result);
            }));
}

#endif  // OS_WIN

namespace internal
{
void SetUnexportableKeyProviderForTesting(std::unique_ptr<UnexportableKeyProvider> (*func)())
{
    if (g_mock_provider)
    {
        // Nesting ScopedMockUnexportableSigningKeyForTesting is not supported.
        CHECK(!func);
        g_mock_provider = nullptr;
    }
    else
    {
        g_mock_provider = func;
    }
}

}  // namespace internal
}  // namespace crypto
