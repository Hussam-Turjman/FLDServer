// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ENTERPRISE_UTIL_H_
#define BASE_ENTERPRISE_UTIL_H_

#include "fldserver/fldserver_config.h"

namespace base
{
// Returns true if an outside entity manages the current machine. This includes
// but is not limited to the presence of user accounts from a centralized
// directory or the presence of dynamically updatable machine policies from an
// outside administrator.
CORE_EXPORT bool
IsMachineExternallyManaged();

#if IS_APPLE

// Returns true if the device is being managed by an MDM system. Uses an old API
// not intended for the purpose.
enum class MacDeviceManagementStateOld
{
    kFailureAPIUnavailable = 0,
    kFailureUnableToParseResult = 1,
    kNoEnrollment = 2,
    kMDMEnrollment = 3,

    kMaxValue = kMDMEnrollment
};
CORE_EXPORT MacDeviceManagementStateOld
IsDeviceRegisteredWithManagementOld();

// Returns the state of the management of the device. Uses a new API so results
// aren't always available. For more details, this is documented at
// https://blog.fleetsmith.com/what-is-user-approved-mdm-uamdm/ .

// These values are persisted to logs. Entries must not be renumbered and
// numeric values must never be reused.
enum class MacDeviceManagementStateNew
{
    kFailureAPIUnavailable = 0,
    kFailureUnableToParseResult = 1,
    kNoEnrollment = 2,
    kLimitedMDMEnrollment = 3,
    kFullMDMEnrollment = 4,
    kDEPMDMEnrollment = 5,

    kMaxValue = kDEPMDMEnrollment
};
CORE_EXPORT MacDeviceManagementStateNew
IsDeviceRegisteredWithManagementNew();

// Returns whether the device and/or the current user is enrolled to a domain.
struct DeviceUserDomainJoinState
{
    bool device_joined;
    bool user_joined;
};
CORE_EXPORT DeviceUserDomainJoinState
AreDeviceAndUserJoinedToDomain();

#endif  // OS_APPLE

}  // namespace base

#endif  // BASE_ENTERPRISE_UTIL_H_
