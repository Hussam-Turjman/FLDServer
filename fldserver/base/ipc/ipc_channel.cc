// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/ipc/ipc_channel.h"

#include "fldserver/base/atomic_sequence_num.h"
#include "fldserver/base/rand_util.h"
#include "fldserver/base/strings/stringprintf.h"
#include "fldserver/fldserver_config.h"

#include <limits>
#include <stddef.h>
#include <stdint.h>

namespace
{
// Global atomic used to guarantee channel IDs are unique.
base::AtomicSequenceNumber g_last_id;

}  // namespace

namespace IPC
{
// static
constexpr size_t Channel::kMaximumMessageSize;

// static
std::string
Channel::GenerateUniqueRandomChannelID()
{
    // Note: the string must start with the current process id, this is how
    // some child processes determine the pid of the parent.
    //
    // This is composed of a unique incremental identifier, the process ID of
    // the creator, an identifier for the child instance, and a strong random
    // component. The strong random component prevents other processes from
    // hijacking or squatting on predictable channel names.
#if defined(OS_NACL_NONSFI)
    // The seccomp sandbox disallows use of getpid(), so we provide a
    // dummy PID.
    int process_id = -1;
#else
    int process_id = base::GetCurrentProcId();
#endif
    return base::StringPrintf("%d.%u.%d",
                              process_id,
                              g_last_id.GetNext(),
                              base::RandInt(0, std::numeric_limits<int32_t>::max()));
}

}  // namespace IPC
