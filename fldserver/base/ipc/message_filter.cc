// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/ipc/message_filter.h"

#include "fldserver/base/memory/ref_counted.h"
#include "fldserver/base/ipc/ipc_channel.h"

#include <stdint.h>

namespace IPC
{
MessageFilter::MessageFilter() = default;

void
MessageFilter::OnFilterAdded(Channel* channel)
{
}

void
MessageFilter::OnFilterRemoved()
{
}

void
MessageFilter::OnChannelConnected(int32_t peer_pid)
{
}

void
MessageFilter::OnChannelError()
{
}

void
MessageFilter::OnChannelClosing()
{
}

bool
MessageFilter::OnMessageReceived(const Message& message)
{
    return false;
}

bool
MessageFilter::GetSupportedMessageClasses(
        std::vector<uint32_t>* /*supported_message_classes*/) const
{
    return false;
}

MessageFilter::~MessageFilter() = default;

}  // namespace IPC
