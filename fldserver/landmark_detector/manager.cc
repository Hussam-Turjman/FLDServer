// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#include "fldserver/base/logging.h"

#include "fldserver/landmark_detector/manager.h"

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
FLDManager::~FLDManager() = default;
FLDManager::FLDManager(std::vector<std::string> arguments) : arguments_(std::move(arguments))
{
}
bool
FLDManager::Init()
{
    // Load the modules that are being used for tracking and face analysis
    // Load face landmark detector
    det_parameters_.reset(new FaceModelParameters(arguments_));

    // Always track gaze in feature extraction
    face_model_.reset(new CLNF(det_parameters_->model_location));

    if (!face_model_->loaded_successfully)
    {
        LOG(ERROR) << "Could not load the landmark detector";
        return false;
    }

    // Load facial feature extractor and AU analyser
    face_analysis_params_.reset(new FaceAnalysis::FaceAnalyserParameters(arguments_));
    face_analyser_.reset(new FaceAnalysis::FaceAnalyser(*face_analysis_params_));

    if (!face_model_->eye_model)
    {
        LOG(ERROR) << "no eye model found";
        return false;
    }

    if (face_analyser_->GetAUClassNames().size() == 0
        && face_analyser_->GetAUClassNames().size() == 0)
    {
        LOG(ERROR) << "ERROR: no Action Unit models found.";
        return false;
    }

    return true;
}
}  // namespace LandmarkDetector