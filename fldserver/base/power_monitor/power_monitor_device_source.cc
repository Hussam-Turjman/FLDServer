// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/power_monitor/power_monitor_device_source.h"

namespace base
{
PowerMonitorDeviceSource::PowerMonitorDeviceSource()
{
#if IS_APPLE
    PlatformInit();
#endif
}

PowerMonitorDeviceSource::~PowerMonitorDeviceSource()
{
#if IS_APPLE
    PlatformDestroy();
#endif
}

}  // namespace base
