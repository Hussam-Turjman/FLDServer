// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_FOR_IO_H_
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_FOR_IO_H_

// This header is a forwarding header to coalesce the various platform specific
// types representing MessagePumpForIO.

#include "fldserver/fldserver_config.h"

#if IS_WIN
#include "fldserver/base/message_loop/message_pump_win.h"
#elif defined(OS_IOS)
#include "fldserver/base/message_loop/message_pump_io_ios.h"
#elif defined(OS_MAC)
#include "fldserver/base/message_loop/message_pump_kqueue.h"
#elif defined(OS_NACL_SFI)
#include "fldserver/base/message_loop/message_pump_default.h"
#elif defined(OS_FUCHSIA)
#include "fldserver/base/message_loop/message_pump_fuchsia.h"
#elif IS_POSIX
#include "fldserver/base/message_loop/message_pump_libevent.h"
#endif

namespace base
{
#if IS_WIN
// Windows defines it as-is.
using MessagePumpForIO = MessagePumpForIO;
#elif defined(OS_IOS)
using MessagePumpForIO = MessagePumpIOSForIO;
#elif defined(OS_MAC)
using MessagePumpForIO = MessagePumpKqueue;
#elif defined(OS_NACL_SFI)
using MessagePumpForIO = MessagePumpDefault;
#elif defined(OS_FUCHSIA)
using MessagePumpForIO = MessagePumpFuchsia;
#elif IS_POSIX
using MessagePumpForIO = MessagePumpLibevent;
#else
#error Platform does not define MessagePumpForIO
#endif

}  // namespace base

#endif  // BASE_MESSAGE_LOOP_MESSAGE_PUMP_FOR_IO_H_
