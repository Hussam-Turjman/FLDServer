// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_DEFAULT_H_
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_DEFAULT_H_

#include "fldserver/base/macros.h"
#include "fldserver/base/message_loop/message_pump.h"
#include "fldserver/base/synchronization/waitable_event.h"
#include "fldserver/base/time/time.h"
#include "fldserver/fldserver_config.h"

namespace base
{
class CORE_EXPORT MessagePumpDefault : public MessagePump
{
public:
    MessagePumpDefault();
    ~MessagePumpDefault() override;

    // MessagePump methods:
    void
    Run(Delegate* delegate) override;
    void
    Quit() override;
    void
    ScheduleWork() override;
    void
    ScheduleDelayedWork(const TimeTicks& delayed_work_time) override;
#if IS_APPLE
    void
    SetTimerSlack(TimerSlack timer_slack) override;
#endif

private:
    // This flag is set to false when Run should return.
    bool keep_running_;

    // Used to sleep until there is more work to do.
    WaitableEvent event_;

    DISALLOW_COPY_AND_ASSIGN(MessagePumpDefault);
};

}  // namespace base

#endif  // BASE_MESSAGE_LOOP_MESSAGE_PUMP_DEFAULT_H_
