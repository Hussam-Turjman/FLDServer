// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_LISTENER_H_
#define IPC_IPC_LISTENER_H_

#include "fldserver/base/component_export.h"
#include "fldserver/fldserver_config.h"

#include <stdint.h>

#include <string>
#if 0
#include "mojo/public/cpp/bindings/scoped_interface_endpoint_handle.h"
#endif
namespace IPC
{
class Message;

// Implemented by consumers of a Channel to receive messages.
class COMPONENT_EXPORT(IPC) Listener
{
public:
    // Called when a message is received.  Returns true iff the message was
    // handled.
    virtual bool
    OnMessageReceived(const Message& message) = 0;

    // Called when the channel is connected and we have received the internal
    // Hello message from the peer.
    virtual void
    OnChannelConnected(int32_t peer_pid)
    {
    }

    // Called when an error is detected that causes the channel to close.
    // This method is not called when a channel is closed normally.
    virtual void
    OnChannelError()
    {
    }

    // Called when a message's deserialization failed.
    virtual void
    OnBadMessageReceived(const Message& message)
    {
    }
#if 0
    // Called when an associated interface request is received on a Channel and
    // the Channel has no registered handler for it.
    virtual void
    OnAssociatedInterfaceRequest(const std::string& interface_name,
                                 mojo::ScopedInterfaceEndpointHandle handle)
    {
    }
#endif
#if IS_POSIX || defined(OS_FUCHSIA)
    // Called on the server side when a channel that listens for connections
    // denies an attempt to connect.
    virtual void
    OnChannelDenied()
    {
    }

    // Called on the server side when a channel that listens for connections
    // has an error that causes the listening channel to close.
    virtual void
    OnChannelListenError()
    {
    }
#endif  // OS_POSIX || OS_FUCHSIA

protected:
    virtual ~Listener()
    {
    }
};

}  // namespace IPC

#endif  // IPC_IPC_LISTENER_H_
