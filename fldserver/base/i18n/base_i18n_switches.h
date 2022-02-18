// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_BASE_I18N_SWITCHES_H_
#define BASE_I18N_BASE_I18N_SWITCHES_H_

#include "fldserver/base/i18n/base_i18n_export.h"

namespace switches
{
CORE_EXPORT extern const char kForceUIDirection[];
CORE_EXPORT extern const char kForceTextDirection[];

// kForce*Direction choices for the switches above.
CORE_EXPORT extern const char kForceDirectionLTR[];
CORE_EXPORT extern const char kForceDirectionRTL[];

}  // namespace switches

#endif  // BASE_I18N_BASE_I18N_SWITCHES_H_
