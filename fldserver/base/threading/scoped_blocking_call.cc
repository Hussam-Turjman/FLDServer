// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/threading/scoped_blocking_call.h"

#include "fldserver/base/lazy_instance.h"
#include "fldserver/base/threading/thread_local.h"
#include "fldserver/base/threading/thread_restrictions.h"
#include "fldserver/base/time/time.h"

#include "fldserver/fldserver_config.h"

#if 0
#include "third_party/perfetto/protos/perfetto/trace/track_event/source_location.pbzero.h"  // nogncheck
#endif                                                                                      // 0

namespace base
{
namespace
{
#if DCHECK_IS_ON()
// Used to verify that the trace events used in the constructor do not result in
// instantiating a ScopedBlockingCall themselves (which would cause an infinite
// reentrancy loop).
LazyInstance<ThreadLocalBoolean>::Leaky tls_construction_in_progress = LAZY_INSTANCE_INITIALIZER;
#endif

}  // namespace

ScopedBlockingCall::ScopedBlockingCall(const Location& from_here, BlockingType blocking_type) :
    UncheckedScopedBlockingCall(
            from_here, blocking_type, UncheckedScopedBlockingCall::BlockingCallType::kRegular)
{
#if DCHECK_IS_ON()
    DCHECK(!tls_construction_in_progress.Get().Get());
    tls_construction_in_progress.Get().Set(true);
#endif

    internal::AssertBlockingAllowed();

#if DCHECK_IS_ON()
    tls_construction_in_progress.Get().Set(false);
#endif
}

ScopedBlockingCall::~ScopedBlockingCall()
{
}

namespace internal
{
ScopedBlockingCallWithBaseSyncPrimitives::ScopedBlockingCallWithBaseSyncPrimitives(
        const Location& from_here, BlockingType blocking_type) :
    UncheckedScopedBlockingCall(from_here,
                                blocking_type,
                                UncheckedScopedBlockingCall::BlockingCallType::kBaseSyncPrimitives)
{
#if DCHECK_IS_ON()
    DCHECK(!tls_construction_in_progress.Get().Get());
    tls_construction_in_progress.Get().Set(true);
#endif

    internal::AssertBaseSyncPrimitivesAllowed();

#if DCHECK_IS_ON()
    tls_construction_in_progress.Get().Set(false);
#endif
}

ScopedBlockingCallWithBaseSyncPrimitives::~ScopedBlockingCallWithBaseSyncPrimitives()
{
}

}  // namespace internal

}  // namespace base
