// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#ifndef FLDSERVER_DETECTION_MANAGER_H
#define FLDSERVER_DETECTION_MANAGER_H

#include <OpenFace/LandmarkCoreIncludes.h>

#include <OpenFace/Face_utils.h>
#include <OpenFace/FaceAnalyser.h>
#include <OpenFace/GazeEstimation.h>

#include <OpenFace/Visualizer.h>
#include <OpenFace/VisualizationUtils.h>
#include <memory>

#include "fldserver/landmark_detection/recorder_openface.h"
#include "fldserver/landmark_detection/recorder_openface_parameters.h"
#include "fldserver/landmark_detection/sequence_capture.h"
#include "fldserver/fldserver_config.h"
#include "fldserver/base/values.h"

namespace LandmarkDetector
{
class FLDSERVER_EXPORT DetectionManager
{
public:
    DetectionManager();
    ~DetectionManager();

    bool
    Init(int argc, char** argv);
    bool
    Init(const std::vector<std::string>& arguments);
    bool
    OpenSequenceReader(double fps_guess,
                       int image_width,
                       int image_height,
                       float fx = -1,
                       float fy = -1,
                       float cx = -1,
                       float cy = -1);
    bool
    DetectFeatures(const cv::Mat& remote_image,
                   base::Value* json_out,
                   cv::Mat* vis_image = nullptr,
                   bool show_observation = false);

private:
    void
    Reset();

private:
    std::vector<std::string> arguments_;
    std::unique_ptr<FaceModelParameters> det_parameters_;
    std::unique_ptr<CLNF> face_model_;
    std::unique_ptr<FaceAnalysis::FaceAnalyserParameters> face_analysis_params_;
    std::unique_ptr<FaceAnalysis::FaceAnalyser> face_analyser_;
    std::unique_ptr<Utilities::SequenceCapture> sequence_reader_;
    std::unique_ptr<Utilities::Visualizer> visualizer_;
    std::unique_ptr<Utilities::FpsTracker> fps_tracker_;
    std::unique_ptr<Utilities::RecorderOpenFace> recorder_openface_;
    Utilities::RecorderOpenFaceParameters record_openface_params_;
};
}  // namespace LandmarkDetector

#endif  // FLDSERVER_DETECTION_MANAGER_H
