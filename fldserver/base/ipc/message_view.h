// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_MESSAGE_VIEW_H_
#define IPC_MESSAGE_VIEW_H_

#include "fldserver/base/component_export.h"
#include "fldserver/base/containers/span.h"
#include "fldserver/base/macros.h"
#include "fldserver/base/ipc/ipc_message.h"

#include <vector>
#if 0
#include "mojo/public/interfaces/bindings/native_struct.mojom-forward.h"
#endif
namespace IPC
{
class COMPONENT_EXPORT(IPC_MOJOM) MessageView
{
public:
    MessageView();
#if 0
    MessageView(base::span<const uint8_t> bytes,
                base::Optional<std::vector<mojo::native::SerializedHandlePtr>> handles);
#endif
    MessageView(MessageView&&);
    ~MessageView();

    MessageView&
    operator=(MessageView&&);
#if 0
    base::span<const uint8_t>
    bytes() const
    {
        return bytes_;
    }
    base::Optional<std::vector<mojo::native::SerializedHandlePtr>>
    TakeHandles();

private:
    base::span<const uint8_t> bytes_;
    base::Optional<std::vector<mojo::native::SerializedHandlePtr>> handles_;
#endif
    DISALLOW_COPY_AND_ASSIGN(MessageView);
};

}  // namespace IPC

#endif  // IPC_MESSAGE_VIEW_H_
