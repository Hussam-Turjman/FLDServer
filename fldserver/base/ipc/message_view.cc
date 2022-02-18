// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fldserver/base/ipc/message_view.h"

#if 0
#include "mojo/public/interfaces/bindings/native_struct.mojom.h"
#endif

namespace IPC
{
MessageView::MessageView() = default;
#if 0
MessageView::MessageView(base::span<const uint8_t> bytes,
                         base::Optional<std::vector<mojo::native::SerializedHandlePtr>> handles) :
    bytes_(bytes), handles_(std::move(handles))
{
}

MessageView::MessageView(MessageView&&) = default;

MessageView::~MessageView() = default;

MessageView&
MessageView::operator=(MessageView&&) = default;

base::Optional<std::vector<mojo::native::SerializedHandlePtr>>
MessageView::TakeHandles()
{
    return std::move(handles_);
}
#endif

}  // namespace IPC
