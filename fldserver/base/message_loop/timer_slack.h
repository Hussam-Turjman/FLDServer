// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_TIMER_SLACK_H_
#define BASE_MESSAGE_LOOP_TIMER_SLACK_H_

#include "fldserver/base/time/time.h"
#include "fldserver/fldserver_config.h"

namespace base
{
struct Feature;

// Amount of timer slack to use for delayed timers.  Increasing timer slack
// allows the OS to coalesce timers more effectively.
enum TimerSlack
{
    // Lowest value for timer slack allowed by OS.
    TIMER_SLACK_NONE,

    // Maximal value for timer slack allowed by OS.
    TIMER_SLACK_MAXIMUM
};

// Returns true if the ludicrous timer slack experiment is enabled.
CORE_EXPORT bool
IsLudicrousTimerSlackEnabled();

// Returns the slack for the experiment.
CORE_EXPORT base::TimeDelta
GetLudicrousTimerSlack();

namespace features
{
// Exposed for testing.
#if 0
CORE_EXPORT extern const base::Feature kLudicrousTimerSlack;
#endif
}  // namespace features

}  // namespace base

#endif  // BASE_MESSAGE_LOOP_TIMER_SLACK_H_
