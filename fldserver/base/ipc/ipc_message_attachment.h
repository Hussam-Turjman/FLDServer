// Copyright (c) 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_MESSAGE_ATTACHMENT_H_
#define IPC_IPC_MESSAGE_ATTACHMENT_H_

#include "fldserver/base/macros.h"
#include "fldserver/base/memory/ref_counted.h"
#include "fldserver/base/pickle.h"
#include "fldserver/fldserver_config.h"
#include "fldserver/base/ipc/ipc_message_support_export.h"
#if 0
#include "mojo/public/cpp/system/handle.h"
#endif
namespace IPC
{
// Auxiliary data sent with |Message|. This can be a platform file descriptor
// or a mojo |MessagePipe|. |GetType()| returns the type of the subclass.
class IPC_MESSAGE_SUPPORT_EXPORT MessageAttachment : public base::Pickle::Attachment
{
public:
    enum class Type
    {
        MOJO_HANDLE,
        PLATFORM_FILE,
        WIN_HANDLE,
        MACH_PORT,
        FUCHSIA_HANDLE,
    };

#if 0
    static scoped_refptr<MessageAttachment>
    CreateFromMojoHandle(mojo::ScopedHandle handle, Type type);

    mojo::ScopedHandle
    TakeMojoHandle();

#endif
    virtual Type
    GetType() const = 0;

protected:
    friend class base::RefCountedThreadSafe<MessageAttachment>;
    MessageAttachment();
    ~MessageAttachment() override;

    DISALLOW_COPY_AND_ASSIGN(MessageAttachment);
};

}  // namespace IPC

#endif  // IPC_IPC_MESSAGE_ATTACHMENT_H_
