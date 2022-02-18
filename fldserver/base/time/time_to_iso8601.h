// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TIME_TIME_TO_ISO8601_H_
#define BASE_TIME_TIME_TO_ISO8601_H_

#include "fldserver/fldserver_config.h"

#include <string>

namespace base
{
class Time;

CORE_EXPORT std::string
TimeToISO8601(const base::Time& t);

}  // namespace base

#endif  // BASE_TIME_TIME_TO_ISO8601_H_
