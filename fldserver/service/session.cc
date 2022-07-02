// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.
#include <memory>

#include "fldserver/service/session.h"
#include <random>
#include <sstream>

namespace service
{
Session::Session() : id_(), detection_manager_()
{
}

Session::~Session()
{
}

std::unique_ptr<Session>
Session::Create(const std::vector<std::string>& arguments,
                int fps,
                int frame_width,
                int frame_height,
                float fx,
                float fy,
                float cx,
                float cy)
{
    std::unique_ptr<Session> session;
    session.reset(new Session());

    if (session->detection_manager_.Init(arguments))
    {
        if (session->detection_manager_.OpenSequenceReader(
                    fps, frame_width, frame_height, fx, fy, cx, cy))
        {
            return session;
        }
    }
    return nullptr;
}

}  // namespace service