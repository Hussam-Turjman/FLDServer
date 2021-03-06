// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_HANDLE_ATTACHMENT_WIN_H_
#define IPC_HANDLE_ATTACHMENT_WIN_H_

#include "fldserver/base/win/scoped_handle.h"
#include "fldserver/base/ipc/handle_win.h"
#include "fldserver/base/ipc/ipc_message_attachment.h"
#include "fldserver/base/ipc/ipc_message_support_export.h"

#include <stdint.h>

namespace IPC
{
namespace internal
{
// This class represents a Windows HANDLE attached to a Chrome IPC message.
class IPC_MESSAGE_SUPPORT_EXPORT HandleAttachmentWin : public MessageAttachment
{
public:
    // This constructor makes a copy of |handle| and takes ownership of the
    // result. Should only be called by the sender of a Chrome IPC message.
    explicit HandleAttachmentWin(const HANDLE& handle);

    enum FromWire
    {
        FROM_WIRE,
    };
    // This constructor takes ownership of |handle|. Should only be called by the
    // receiver of a Chrome IPC message.
    HandleAttachmentWin(const HANDLE& handle, FromWire from_wire);

    // MessageAttachment interface.
    Type
    GetType() const override;

    HANDLE
    Take()
    {
        return handle_.Take();
    }

private:
    ~HandleAttachmentWin() override;

    base::win::ScopedHandle handle_;
};

}  // namespace internal
}  // namespace IPC

#endif  // IPC_HANDLE_ATTACHMENT_WIN_H_
