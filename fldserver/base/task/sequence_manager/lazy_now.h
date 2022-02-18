// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TASK_SEQUENCE_MANAGER_LAZY_NOW_H_
#define BASE_TASK_SEQUENCE_MANAGER_LAZY_NOW_H_

#include "fldserver/base/optional.h"
#include "fldserver/base/time/time.h"
#include "fldserver/fldserver_config.h"

namespace base
{
class TickClock;

namespace sequence_manager
{
// Now() is somewhat expensive so it makes sense not to call Now() unless we
// really need to and to avoid subsequent calls if already called once.
// LazyNow objects are expected to be short-living to represent accurate time.
class CORE_EXPORT LazyNow
{
public:
    explicit LazyNow(TimeTicks now);
    explicit LazyNow(const TickClock* tick_clock);
    LazyNow(const LazyNow&) = delete;
    LazyNow&
    operator=(const LazyNow&) = delete;

    LazyNow(LazyNow&& move_from) noexcept;

    // Result will not be updated on any subsesequent calls.
    TimeTicks
    Now();

    bool
    has_value() const
    {
        return !!now_;
    }

private:
    const TickClock* tick_clock_;  // Not owned.
    Optional<TimeTicks> now_;
};

}  // namespace sequence_manager
}  // namespace base

#endif  // BASE_TASK_SEQUENCE_MANAGER_LAZY_NOW_H_
