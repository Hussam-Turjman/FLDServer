// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TIMER_HI_RES_TIMER_MANAGER_H_
#define BASE_TIMER_HI_RES_TIMER_MANAGER_H_

#include "fldserver/base/macros.h"
#include "fldserver/base/memory/ref_counted.h"
#include "fldserver/base/power_monitor/power_observer.h"
#include "fldserver/base/timer/timer.h"
#include "fldserver/fldserver_config.h"

namespace base
{
// Ensures that the Windows high resolution timer is only used
// when not running on battery power.
class CORE_EXPORT HighResolutionTimerManager : public base::PowerSuspendObserver,
                                               public base::PowerStateObserver
{
public:
    HighResolutionTimerManager();
    ~HighResolutionTimerManager() override;

    // base::PowerStateObserver methods.
    void
    OnPowerStateChange(bool on_battery_power) override;
    // base::PowerSuspendObserver methods.
    void
    OnSuspend() override;
    void
    OnResume() override;

    // Returns true if the hi resolution clock could be used right now.
    bool
    hi_res_clock_available() const
    {
        return hi_res_clock_available_;
    }

private:
    // Enable or disable the faster multimedia timer.
    void
    UseHiResClock(bool use);

    bool hi_res_clock_available_;

#if IS_WIN
    // Timer for polling the high resolution timer usage.
    base::RepeatingTimer timer_;
#endif

    DISALLOW_COPY_AND_ASSIGN(HighResolutionTimerManager);
};

}  // namespace base

#endif  // BASE_TIMER_HI_RES_TIMER_MANAGER_H_
