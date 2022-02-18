// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#ifndef FLDSERVER_LANDMARK_DETCTOR_FEATURES_RECORDER_H
#define FLDSERVER_LANDMARK_DETCTOR_FEATURES_RECORDER_H

#include "fldserver/base/values.h"

#include "fldserver/fldserver_config.h"
#include <vector>
#include <string>
#include <opencv2/core.hpp>

namespace LandmarkDetector
{
class FLDSERVER_EXPORT FeaturesRecorder
{
public:
    FeaturesRecorder() = delete;
    FeaturesRecorder(const FeaturesRecorder& other) = delete;
    FeaturesRecorder&
    operator=(const FeaturesRecorder& other) = delete;
    FeaturesRecorder(bool is_sequence,
                     bool output_2D_landmarks,
                     bool output_3D_landmarks,
                     bool output_model_params,
                     bool output_pose,
                     bool output_AUs,
                     bool output_gaze,
                     int num_face_landmarks,
                     int num_model_modes,
                     int num_eye_landmarks,
                     const std::vector<std::string>& au_names_class,
                     const std::vector<std::string>& au_names_reg);

    base::Value
    ConvertToJson(int face_id,
                  int frame_num,
                  double time_stamp,
                  bool landmark_detection_success,
                  double landmark_confidence,
                  const cv::Mat_<float>& landmarks_2D,
                  const cv::Mat_<float>& landmarks_3D,
                  const cv::Mat_<float>& pdm_model_params,
                  const cv::Vec6f& rigid_shape_params,
                  const cv::Vec6f& pose_estimate,
                  const cv::Point3f& gazeDirection0,
                  const cv::Point3f& gazeDirection1,
                  const cv::Vec2f& gaze_angle,
                  const std::vector<cv::Point2f>& eye_landmarks2d,
                  const std::vector<cv::Point3f>& eye_landmarks3d,
                  const std::vector<std::pair<std::string, double>>& au_intensities,
                  const std::vector<std::pair<std::string, double>>& au_occurences);

    ~FeaturesRecorder();

private:
    const bool is_sequence_;
    const bool output_2D_landmarks_;
    const bool output_3D_landmarks_;
    const bool output_model_params_;
    const bool output_pose_;
    const bool output_AUs_;
    const bool output_gaze_;
    const int num_face_landmarks_;
    const int num_model_modes_;
    const int num_eye_landmarks_;
    std::vector<std::string> au_names_class_;
    std::vector<std::string> au_names_reg_;
};
}  // namespace LandmarkDetector

#endif  // FLDSERVER_LANDMARK_DETCTOR_FEATURES_RECORDER_H
