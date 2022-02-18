// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#include "fldserver/base/base64.h"
#include "fldserver/base/logging.h"
#include "fldserver/base/json/json_writer.h"

#include <OpenFace/LandmarkCoreIncludes.h>

#include <OpenFace/Face_utils.h>
#include <OpenFace/FaceAnalyser.h>
#include <OpenFace/GazeEstimation.h>
#include <OpenFace/RecorderOpenFace.h>
#include <OpenFace/RecorderOpenFaceParameters.h>
#include <OpenFace/SequenceCapture.h>
#include <OpenFace/Visualizer.h>
#include <OpenFace/VisualizationUtils.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "fldserver/landmark_detector/features_extractor.h"
#include "fldserver/landmark_detector/manager.h"

#include "fldserver/landmark_detector/video_frame.h"
#include "fldserver/landmark_detector/features_recorder.h"

#include <iostream>
#include <codecvt>
namespace LandmarkDetector
{
FeaturesExtractor::FeaturesExtractor(FLDManager* manager) : manager_(manager)
{
}
FeaturesExtractor::~FeaturesExtractor()
{
}
std::optional<std::string>
FeaturesExtractor::OnFrame(int frame_id,
                           double timestamp,
                           int width,
                           int height,
                           const std::string& image,
                           size_t size)
{
    std::string decoded_image;
    if (!base::Base64Decode(image, &decoded_image))
    {
        LOG(ERROR) << "Failed to decode the given image from base64";
        return std::nullopt;
    }

    std::vector<uchar> data(decoded_image.data(), decoded_image.data() + decoded_image.size());
    cv::Mat mat = cv::imdecode(cv::Mat(data), 1);
    auto features = ExtractFeatures(mat, frame_id, timestamp);
    if (features.has_value())
    {
        std::string json;
        if (!base::JSONWriter::Write(features.value(), &json))
        {
            LOG(ERROR) << "Failed to convert json to string";
            return std::nullopt;
        }

        return json;
    }
    return std::nullopt;
}
std::optional<base::Value>
FeaturesExtractor::ExtractFeatures(const cv::Mat& image, int frame_num, double time_stamp)
{
    VideoFrame video_frame(image, manager_->face_model());
    video_frame.SetCameraIntrinsics(-1, -1, -1, -1);
    cv::Mat gray_scale = video_frame.ConvertToGrayscale_8bit();

    // The actual facial landmark detection / tracking
    bool detection_success = LandmarkDetector::DetectLandmarksInVideo(
            video_frame.frame(), *manager_->face_model(), *manager_->det_parameters(), gray_scale);
    if (!detection_success)
    {
        IncreaseFailedTries();
        // LOG(WARNING) << "Failed to DetectLandmarksInVideo";
    }
    else
    {
        IncreaseSuccessfulTries();
    }

    video_frame.CalculateEyeLandmarks();
    video_frame.CalculateGazeDirections();
    video_frame.CalculateLandMarks3D();

    int num_face_landmarks = video_frame.detected_landmarks_2d().rows / 2;
    int num_eye_landmarks = (int) video_frame.all_eye_landmarks_2d().size();
    int num_model_modes = manager_->face_model()->params_local.rows;
    std::vector<std::pair<std::string, double>> au_intensities =
            manager_->face_analyser()->GetCurrentAUsReg();
    std::vector<std::pair<std::string, double>> au_occurences =
            manager_->face_analyser()->GetCurrentAUsClass();
    std::vector<std::string> au_names_class;
    for (const auto& au : au_occurences)
    {
        au_names_class.push_back(au.first);
    }
    std::vector<std::string> au_names_reg;
    for (const auto& au : au_intensities)
    {
        au_names_reg.push_back(au.first);
    }

    FeaturesRecorder recorder(true,
                              true,
                              true,
                              true,
                              true,
                              true,
                              true,
                              num_face_landmarks,
                              num_model_modes,
                              num_eye_landmarks,
                              au_names_class,
                              au_names_reg);

    cv::Vec6d pose_estimate = video_frame.pose_estimate();
    base::Value value = recorder.ConvertToJson(0,
                                               frame_num,
                                               time_stamp,
                                               detection_success,
                                               manager_->face_model()->detection_certainty,
                                               video_frame.detected_landmarks_2d(),
                                               video_frame.detected_landmarks_3d(),
                                               manager_->face_model()->params_local,
                                               manager_->face_model()->params_global,
                                               pose_estimate,
                                               video_frame.gaze_direction0(),
                                               video_frame.gaze_direction1(),
                                               video_frame.gaze_angle(),
                                               video_frame.all_eye_landmarks_2d(),
                                               video_frame.all_eye_landmarks_3d(),
                                               au_intensities,
                                               au_occurences);
    Utilities::Visualizer visualizer(true, false, false, false);
    visualizer.SetImage(video_frame.frame(),
                        video_frame.fx(),
                        video_frame.fy(),
                        video_frame.cx(),
                        video_frame.cy());
    visualizer.SetObservationActionUnits(au_intensities, au_occurences);
    visualizer.SetObservationGaze(video_frame.gaze_direction0(),
                                  video_frame.gaze_direction1(),
                                  video_frame.all_eye_landmarks_2d(),
                                  video_frame.all_eye_landmarks_3d(),
                                  manager_->face_model()->detection_certainty);
    visualizer.SetObservationPose(pose_estimate, manager_->face_model()->detection_certainty);
    visualizer.SetObservationLandmarks(video_frame.detected_landmarks_2d(),
                                       manager_->face_model()->detection_certainty);
    // visualizer.ShowObservation();
    cv::Mat vis_image = visualizer.GetVisImage();
    std::vector<uchar> vis_image_encoded;
    if (cv::imencode(".jpg", vis_image, vis_image_encoded))
    {
        std::string base64_encoded;
        std::string vis_image_encoded_str(vis_image_encoded.begin(), vis_image_encoded.end());

        base::Base64Encode(vis_image_encoded_str, &base64_encoded);

        value.SetKey("vis_image", base::Value(base64_encoded));
        value.SetKey("vis_image_width", base::Value(vis_image.size().width));
        value.SetKey("vis_image_height", base::Value(vis_image.size().height));
    }
    value.SetKey("fx", base::Value(video_frame.fx()));
    value.SetKey("fy", base::Value(video_frame.fy()));
    value.SetKey("cx", base::Value(video_frame.cx()));
    value.SetKey("cy", base::Value(video_frame.cy()));

    return value;
}
}  // namespace LandmarkDetector