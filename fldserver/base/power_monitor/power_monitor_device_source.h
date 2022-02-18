// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_POWER_MONITOR_POWER_MONITOR_DEVICE_SOURCE_H_
#define BASE_POWER_MONITOR_POWER_MONITOR_DEVICE_SOURCE_H_

#include "fldserver/base/macros.h"
#include "fldserver/base/power_monitor/power_monitor_source.h"
#include "fldserver/base/power_monitor/power_observer.h"
#include "fldserver/fldserver_config.h"
#include "fldserver/fldserver_config.h"

#include <memory>
#include <vector>

#if IS_WIN
#include <windows.h>
#endif  // !OS_WIN

#if defined(OS_MAC)
#include "fldserver/base/mac/scoped_cftyperef.h"
#include "fldserver/base/mac/scoped_ionotificationportref.h"
#include "fldserver/base/power_monitor/thermal_state_observer_mac.h"

#include <IOKit/IOTypes.h>
#endif

#if defined(OS_IOS)
#include <objc/runtime.h>
#endif  // OS_IOS

namespace base
{
// A class used to monitor the power state change and notify the observers about
// the change event.
class CORE_EXPORT PowerMonitorDeviceSource : public PowerMonitorSource
{
public:
    PowerMonitorDeviceSource();
    ~PowerMonitorDeviceSource() override;

#if 0
    // On Chrome OS, Chrome receives power-related events from powerd, the system
    // power daemon, via D-Bus signals received on the UI thread. base can't
    // directly depend on that code, so this class instead exposes static methods
    // so that events can be passed in.
    static void
    SetPowerSource(bool on_battery);
    static void
    HandleSystemSuspending();
    static void
    HandleSystemResumed();
    static void
    ThermalEventReceived(PowerThermalObserver::DeviceThermalState state);

    // These two methods is used for handling thermal state update requests, such
    // as asking for initial state when starting lisitening to thermal change.
    PowerThermalObserver::DeviceThermalState
    GetCurrentThermalState() override;
    void
    SetCurrentThermalState(PowerThermalObserver::DeviceThermalState state) override;
#endif

private:
    friend class PowerMonitorDeviceSourceTest;

#if IS_WIN
    // Represents a message-only window for power message handling on Windows.
    // Only allow PowerMonitor to create it.
    class PowerMessageWindow
    {
    public:
        PowerMessageWindow();
        ~PowerMessageWindow();

    private:
        static LRESULT CALLBACK
        WndProcThunk(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
        // Instance of the module containing the window procedure.
        HMODULE instance_;
        // A hidden message-only window.
        HWND message_hwnd_;
    };
#endif  // OS_WIN

#if IS_APPLE
    void
    PlatformInit();
    void
    PlatformDestroy();
#endif  // OS_APPLE

#if defined(OS_MAC)
    // Callback from IORegisterForSystemPower(). |refcon| is the |this| pointer.
    static void
    SystemPowerEventCallback(void* refcon,
                             io_service_t service,
                             natural_t message_type,
                             void* message_argument);
#endif  // OS_MAC

    // Platform-specific method to check whether the system is currently
    // running on battery power.  Returns true if running on batteries,
    // false otherwise.
    bool
    IsOnBatteryPower() override;

#if defined(OS_ANDROID)
    int
    GetRemainingBatteryCapacity() override;
#endif  // defined(OS_ANDROID)

#if defined(OS_MAC)
    // PowerMonitorSource:
    PowerThermalObserver::DeviceThermalState
    GetCurrentThermalState() override;

    // Reference to the system IOPMrootDomain port.
    io_connect_t power_manager_port_ = IO_OBJECT_NULL;

    // Notification port that delivers power (sleep/wake) notifications.
    mac::ScopedIONotificationPortRef notification_port_;

    // Notifier reference for the |notification_port_|.
    io_object_t notifier_ = IO_OBJECT_NULL;

    // Run loop source to observe power-source-change events.
    ScopedCFTypeRef<CFRunLoopSourceRef> power_source_run_loop_source_;

    // Observer of thermal state events: critical temperature etc.
    std::unique_ptr<ThermalStateObserverMac> thermal_state_observer_;
#endif

#if defined(OS_IOS)
    // Holds pointers to system event notification observers.
    std::vector<id> notification_observers_;
#endif

#if IS_WIN
    PowerMessageWindow power_message_window_;
#endif

#if 0
    PowerThermalObserver::DeviceThermalState current_thermal_state_ =
            PowerThermalObserver::DeviceThermalState::kUnknown;
#endif
    DISALLOW_COPY_AND_ASSIGN(PowerMonitorDeviceSource);
};

}  // namespace base

#endif  // BASE_POWER_MONITOR_POWER_MONITOR_DEVICE_SOURCE_H_
