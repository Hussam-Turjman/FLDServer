// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#ifndef FLDSERVER_HANDLER_FEATURES_EXTRACTOR_H
#define FLDSERVER_HANDLER_FEATURES_EXTRACTOR_H

#include "fldserver/base/values.h"

#include "fldserver/fldserver_config.h"
#include "fldserver/service/service_observer.h"

#include <optional>

namespace base
{
class Value;
}
namespace cv
{
class Mat;
}
namespace LandmarkDetector
{
class FLDManager;
class FLDSERVER_EXPORT FeaturesExtractor : public service::ServiceObserver
{
public:
    FeaturesExtractor() = delete;
    FeaturesExtractor(const FeaturesExtractor& other) = delete;
    FeaturesExtractor&
    operator=(const FeaturesExtractor&) = delete;
    explicit FeaturesExtractor(FLDManager* manager);
    ~FeaturesExtractor() override;
    std::optional<std::string>
    OnFrame(int frame_id,
            double timestamp,
            int width,
            int height,
            const std::string& image,
            size_t size) override;

private:
    std::optional<base::Value>
    ExtractFeatures(const cv::Mat& image, int frame_num, double time_stamp);

private:
    FLDManager* const manager_;
};

}  // namespace LandmarkDetector

#endif  // FLDSERVER_HANDLER_FEATURES_EXTRACTOR_H
