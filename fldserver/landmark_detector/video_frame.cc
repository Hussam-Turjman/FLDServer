// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#include "fldserver/landmark_detector/video_frame.h"

#include <opencv2/imgproc.hpp>
#include <utility>
#include <OpenFace/LandmarkCoreIncludes.h>
#include <OpenFace/Face_utils.h>
#include <OpenFace/FaceAnalyser.h>
#include <OpenFace/GazeEstimation.h>
#include <OpenFace/RecorderOpenFace.h>
#include <OpenFace/RecorderOpenFaceParameters.h>
#include <OpenFace/SequenceCapture.h>
#include <OpenFace/Visualizer.h>
#include <OpenFace/VisualizationUtils.h>
namespace LandmarkDetector
{
VideoFrame::VideoFrame(cv::Mat frame, CLNF* model) :
    frame_(std::move(frame)),
    fx_(-1),
    fy_(-1),
    cx_(-1),
    cy_(-1),
    face_model_(model),
    gaze_direction0_(0, 0, 0),
    gaze_direction1_(0, 0, 0),
    gaze_angle_(0, 0)
{
}
VideoFrame::~VideoFrame()
{
}

//===========================================================================
// Converting between color spaces and bit depths
//===========================================================================

cv::Mat
VideoFrame::ConvertToGrayscale_8bit()
{
    cv::Mat out;
    if (frame_.channels() == 3)
    {
        // Make sure it's in a correct format
        if (frame_.depth() == CV_16U)
        {
            cv::Mat tmp = frame_ / 256;
            tmp.convertTo(out, CV_8U);
            cv::cvtColor(out, out, cv::COLOR_BGR2GRAY);
        }
        else
        {
            cv::cvtColor(frame_, out, cv::COLOR_BGR2GRAY);
        }
    }
    else if (frame_.channels() == 4)
    {
        if (frame_.depth() == CV_16U)
        {
            cv::Mat tmp = frame_ / 256;
            tmp.convertTo(out, CV_8U);
            cv::cvtColor(out, out, cv::COLOR_BGRA2GRAY);
        }
        else
        {
            cv::cvtColor(frame_, out, cv::COLOR_BGRA2GRAY);
        }
    }
    else
    {
        if (frame_.depth() == CV_16U)
        {
            cv::Mat tmp = frame_ / 256;
            tmp.convertTo(out, CV_8U);
        }
        else if (frame_.depth() == CV_8U)
        {
            out = frame_.clone();
        }
    }
    return out;
}
void
VideoFrame::SetCameraIntrinsics(float fx, float fy, float cx, float cy)
{
    // If optical centers are not defined just use center of image
    if (cx == -1)
    {
        cx_ = width() / 2.0f;
        cy_ = height() / 2.0f;
    }
    else
    {
        cx_ = cx;
        cy_ = cy;
    }
    // Use a rough guess-timate of focal length
    if (fx == -1)
    {
        fx_ = 500.0f * (width() / 640.0f);
        fy_ = 500.0f * (height() / 480.0f);

        fx_ = (fx_ + fy_) / 2.0f;
        fy_ = fx_;
    }
    else
    {
        fx_ = fx;
        fy_ = fy;
    }
}

void
VideoFrame::CalculateGazeDirections()
{
    GazeAnalysis::EstimateGaze(*face_model_, gaze_direction0_, fx_, fy_, cx_, cy_, true);
    GazeAnalysis::EstimateGaze(*face_model_, gaze_direction1_, fx_, fy_, cx_, cy_, false);
    gaze_angle_ = GazeAnalysis::GetGazeAngle(gaze_direction0_, gaze_direction1_);
}
void
VideoFrame::CalculateEyeLandmarks()
{
    all_eye_landmarks_ = CalculateAllEyeLandmarks(*face_model_);
    all_3d_eye_landmarks_ = Calculate3DEyeLandmarks(*face_model_, fx_, fy_, cx_, cy_);
}
void
VideoFrame::CalculateLandMarks3D()
{
    landmarks_3d_ = face_model_->GetShape(fx_, fy_, cx_, cy_);
}
const cv::Mat_<float>&
VideoFrame::detected_landmarks_2d() const
{
    return face_model_->detected_landmarks;
}
cv::Vec6d
VideoFrame::pose_estimate() const
{
    return LandmarkDetector::GetPose(*face_model_, fx_, fy_, cx_, cy_);
}

}  // namespace LandmarkDetector