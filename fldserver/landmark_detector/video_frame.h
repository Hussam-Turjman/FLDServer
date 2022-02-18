// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#ifndef FLDSERVER_VIDEO_FRAME_H
#define FLDSERVER_VIDEO_FRAME_H

#include "fldserver/fldserver_config.h"
#include <opencv2/core.hpp>

namespace LandmarkDetector
{
class CLNF;
class FLDSERVER_EXPORT VideoFrame
{
public:
    VideoFrame() = delete;
    VideoFrame(const VideoFrame& other) = delete;
    VideoFrame&
    operator=(const VideoFrame& other) = delete;
    explicit VideoFrame(cv::Mat frame, CLNF* face_model);
    void
    SetCameraIntrinsics(float fx, float fy, float cx, float cy);
    int
    width() const
    {
        return frame_.size().width;
    }
    int
    height() const
    {
        return frame_.size().height;
    }
    cv::Mat
    ConvertToGrayscale_8bit();
    ~VideoFrame();

    const cv::Mat&
    frame() const
    {
        return frame_;
    }

    const std::vector<cv::Point2f>&
    all_eye_landmarks_2d() const
    {
        return all_eye_landmarks_;
    };
    const std::vector<cv::Point3f>&
    all_eye_landmarks_3d() const
    {
        return all_3d_eye_landmarks_;
    };
    const cv::Mat_<float>&
    detected_landmarks_3d() const
    {
        return landmarks_3d_;
    };
    cv::Vec6d
    pose_estimate() const;
    const cv::Mat_<float>&
    detected_landmarks_2d() const;
    void
    CalculateGazeDirections();
    void
    CalculateEyeLandmarks();
    void
    CalculateLandMarks3D();
    const cv::Point3f&
    gaze_direction0() const
    {
        return gaze_direction0_;
    };
    const cv::Point3f&
    gaze_direction1() const
    {
        return gaze_direction1_;
    };
    const cv::Vec2d&
    gaze_angle() const
    {
        return gaze_angle_;
    };

    float
    fx() const
    {
        return fx_;
    }
    float
    fy() const
    {
        return fy_;
    }
    float
    cx() const
    {
        return cx_;
    }
    float
    cy() const
    {
        return cy_;
    }

private:
    CLNF* const face_model_;
    std::vector<cv::Point2f> all_eye_landmarks_;
    std::vector<cv::Point3f> all_3d_eye_landmarks_;
    cv::Mat_<float> landmarks_3d_;
    cv::Point3f gaze_direction0_;
    cv::Point3f gaze_direction1_;
    cv::Vec2d gaze_angle_;

    cv::Mat frame_;

    /*
     * -fx <focal length in x>
     * -fy <focal length in y>
     * -cx <optical centre in x>
     * -cy <optical centre in y>
     * */
    float fx_, fy_, cx_, cy_;
};

}  // namespace LandmarkDetector

#endif  // FLDSERVER_VIDEO_FRAME_H
