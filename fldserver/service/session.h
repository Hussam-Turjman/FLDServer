// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#ifndef FLDSERVER_SESSION_H
#define FLDSERVER_SESSION_H

#include "fldserver/fldserver_config.h"

#include <string>
#include <optional>
#include <memory>
#include "fldserver/landmark_detection/detection_manager.h"

namespace LandmarkDetector
{
class DetectionManager;
}
namespace service
{
class FLDSERVER_EXPORT Session
{
public:
    static std::unique_ptr<Session>
    Create(const std::vector<std::string>& arguments,
           int fps,
           int frame_width,
           int frame_height,
           float fx = -1,
           float fy = -1,
           float cx = -1,
           float cy = -1);

    ~Session();

    const std::string&
    id() const
    {
        return id_;
    };

    void
    SetId(const std::string& id)
    {
        id_ = id;
    }

    LandmarkDetector::DetectionManager*
    manager()
    {
        return &detection_manager_;
    }

    Session(const Session& other) = delete;

    Session&
    operator=(const Session& other) = delete;

private:
    Session();

private:
    std::string id_;
    LandmarkDetector::DetectionManager detection_manager_;
};

}  // namespace service

#endif  // FLDSERVER_SESSION_H
