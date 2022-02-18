// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/crypto/mac_security_services_lock.h"

#include "fldserver/base/no_destructor.h"
#include "fldserver/base/synchronization/lock.h"

namespace crypto
{
base::Lock&
GetMacSecurityServicesLock()
{
    static base::NoDestructor<base::Lock> lock;
    return *lock;
}

}  // namespace crypto
