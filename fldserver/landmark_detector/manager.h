// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#ifndef FLDSERVER_HANDLER_FLD_MANAGER_H
#define FLDSERVER_HANDLER_FLD_MANAGER_H

#include "fldserver/fldserver_config.h"
#include <vector>
#include <string>
#include <memory>

namespace FaceAnalysis
{
class FaceAnalyserParameters;
class FaceAnalyser;
}  // namespace FaceAnalysis
namespace LandmarkDetector
{
class FaceModelParameters;
class CLNF;

class FLDSERVER_EXPORT FLDManager
{
public:
    explicit FLDManager(std::vector<std::string> arguments);
    FLDManager() = delete;
    FLDManager(const FLDManager& other) = delete;
    FLDManager&
    operator=(const FLDManager& other) = delete;
    FaceModelParameters*
    det_parameters() const
    {
        return det_parameters_.get();
    };
    CLNF*
    face_model() const
    {
        return face_model_.get();
    }
    FaceAnalysis::FaceAnalyserParameters*
    face_analysis_params() const
    {
        return face_analysis_params_.get();
    }
    FaceAnalysis::FaceAnalyser*
    face_analyser() const
    {
        return face_analyser_.get();
    }
    bool
    Init();
    ~FLDManager();

private:
    std::vector<std::string> arguments_;
    std::unique_ptr<FaceModelParameters> det_parameters_;
    std::unique_ptr<CLNF> face_model_;
    std::unique_ptr<FaceAnalysis::FaceAnalyserParameters> face_analysis_params_;
    std::unique_ptr<FaceAnalysis::FaceAnalyser> face_analyser_;
};
}  // namespace LandmarkDetector

#endif  // FLDSERVER_HANDLER_FLD_MANAGER_H
