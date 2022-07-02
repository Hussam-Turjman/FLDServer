// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#include "fldserver/base/base64.h"
#include "fldserver/base/logging.h"
#include "fldserver/base/json/json_writer.h"
#include "fldserver/service/sessions_handler.h"
#include "fldserver/landmark_detection/features_extractor.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <codecvt>

namespace LandmarkDetector
{
FeaturesExtractor::FeaturesExtractor()
{
}

FeaturesExtractor::~FeaturesExtractor()
{
}

std::optional<std::string>
FeaturesExtractor::OnFrame(const std::string& session_id,
                           double fps,
                           int width,
                           int height,
                           const std::string& image,
                           size_t size,
                           bool with_vis_image)
{
    bool should_be_initialized = false;
    auto session = service::SessionsHandler::Get()->GetSession(session_id, &should_be_initialized);
    if (!session)
    {
        LOG(ERROR) << "Failed to get detection session !!";
        return std::nullopt;
    }
    if (should_be_initialized)
    {
        LOG(INFO) << "Should be initialized .. ";
        const bool open_status = session->manager()->OpenSequenceReader(fps, width, height);
        CHECK(open_status);
    }
    std::string decoded_image;
    if (!base::Base64Decode(image, &decoded_image))
    {
        LOG(ERROR) << "Failed to decode the given image from base64";
        return std::nullopt;
    }

    std::vector<uchar> data(decoded_image.data(), decoded_image.data() + decoded_image.size());
    cv::Mat remote_image = cv::imdecode(cv::Mat(data), 1);
    base::Value json_root;
    cv::Mat vis_image;

    const bool extraction_status =
            session->manager()->DetectFeatures(remote_image, &json_root, with_vis_image?&vis_image:nullptr, false);

    CHECK(extraction_status) << "Failed to extract features !!";
    if (with_vis_image){
        std::vector<uchar> vis_image_encoded;
        if (cv::imencode(".jpg", vis_image, vis_image_encoded))
        {
            std::string base64_encoded;
            std::string vis_image_encoded_str(vis_image_encoded.begin(), vis_image_encoded.end());

            base::Base64Encode(vis_image_encoded_str, &base64_encoded);

            json_root.SetKey("vis_image", base::Value(base64_encoded));
            json_root.SetKey("vis_image_width", base::Value(vis_image.size().width));
            json_root.SetKey("vis_image_height", base::Value(vis_image.size().height));
        }
    }

    std::string json;

    if (!base::JSONWriter::Write(json_root, &json))
    {
        LOG(ERROR) << "Failed to convert json to string";
        return std::nullopt;
    }

    return json;
}

}  // namespace LandmarkDetector