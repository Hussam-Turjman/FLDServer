// Copyright 2021 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WIN_SID_H_
#define BASE_WIN_SID_H_

#include "fldserver/base/optional.h"
#include "fldserver/base/win/windows_types.h"

#include <string>
#include <vector>

namespace base
{
namespace win
{
// Known capabilities defined in Windows 8.
enum class WellKnownCapability
{
    kInternetClient,
    kInternetClientServer,
    kPrivateNetworkClientServer,
    kPicturesLibrary,
    kVideosLibrary,
    kMusicLibrary,
    kDocumentsLibrary,
    kEnterpriseAuthentication,
    kSharedUserCertificates,
    kRemovableStorage,
    kAppointments,
    kContacts
};

// A subset of well known SIDs to create.
enum class WellKnownSid
{
    kNull,
    kWorld,
    kCreatorOwner,
    kNetwork,
    kBatch,
    kInteractive,
    kService,
    kAnonymous,
    kSelf,
    kAuthenticatedUser,
    kRestricted,
    kLocalSystem,
    kLocalService,
    kNetworkService,
    kBuiltinAdministrators,
    kBuiltinUsers,
    kBuiltinGuests,
    kUntrustedLabel,
    kLowLabel,
    kMediumLabel,
    kHighLabel,
    kSystemLabel,
    kWriteRestricted,
    kCreatorOwnerRights,
    kAllApplicationPackages,
    kAllRestrictedApplicationPackages
};

// This class is used to hold and generate SIDs.
class CORE_EXPORT Sid
{
public:
    // Create a Sid from an AppContainer capability name. The name can be
    // completely arbitrary. Only available on Windows 10 and above.
    static Optional<Sid>
    FromNamedCapability(const wchar_t* capability_name);

    // Create a Sid from a known capability enumeration value. The Sids
    // match with the list defined in Windows 8.
    static Optional<Sid>
    FromKnownCapability(WellKnownCapability capability);

    // Create a SID from a well-known type.
    static Optional<Sid>
    FromKnownSid(WellKnownSid type);

    // Create a Sid from a SDDL format string, such as S-1-1-0.
    static Optional<Sid>
    FromSddlString(const wchar_t* sddl_sid);

    // Create a Sid from a PSID pointer.
    static Optional<Sid>
    FromPSID(const PSID sid);

    // Generate a random SID value.
    static Optional<Sid>
    GenerateRandomSid();

    // Create a SID for the current user.
    static Optional<Sid>
    CurrentUser();

    // Create a SID for an integrity level RID.
    static Optional<Sid>
    FromIntegrityLevel(DWORD integrity_level);

    // Create a vector of SIDs from a vector of SDDL format strings.
    static Optional<std::vector<Sid>>
    FromSddlStringVector(const std::vector<const wchar_t*>& sddl_sids);

    // Create a vector of SIDs from a vector of capability names.
    static Optional<std::vector<Sid>>
    FromNamedCapabilityVector(const std::vector<const wchar_t*>& capability_names);

    Sid(const Sid&) = delete;
    Sid&
    operator=(const Sid&) = delete;
    Sid(Sid&& sid);
    ~Sid();

    // Returns sid as a PSID. This should only be used temporarily while the Sid
    // is still within scope.
    PSID
    GetPSID() const;

    // Converts the SID to a SDDL format string.
    Optional<std::wstring>
    ToSddlString() const;

private:
    Sid(const void* sid, size_t length);
    std::vector<char> sid_;
};

}  // namespace win
}  // namespace base

#endif  // BASE_WIN_SID_H_
