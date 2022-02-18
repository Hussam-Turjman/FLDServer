// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#include "fldserver/landmark_detector/features_recorder.h"
#include "fldserver/base/json/json_writer.h"

#include <algorithm>
namespace LandmarkDetector
{
FeaturesRecorder::~FeaturesRecorder()
{
}
FeaturesRecorder::FeaturesRecorder(bool is_sequence,
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
                                   const std::vector<std::string>& au_names_reg) :
    is_sequence_(is_sequence),
    output_2D_landmarks_(output_2D_landmarks),
    output_3D_landmarks_(output_3D_landmarks),
    output_model_params_(output_model_params),
    output_pose_(output_pose),
    output_AUs_(output_AUs),
    output_gaze_(output_gaze),
    num_face_landmarks_(num_face_landmarks),
    num_model_modes_(num_model_modes),
    num_eye_landmarks_(num_eye_landmarks),
    au_names_class_(au_names_class),
    au_names_reg_(au_names_reg)
{
}
base::Value
FeaturesRecorder::ConvertToJson(int face_id,
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
                                const std::vector<std::pair<std::string, double>>& au_occurences)
{
    base::Value out(base::Value::Type::DICTIONARY);
    // Different headers if we are writing out the results on a sequence or an individual image
    if (is_sequence_)
    {
        out.SetKey("frame", base::Value(frame_num));
        out.SetKey("face_id", base::Value(face_id));
        out.SetKey("confidence", base::Value(landmark_confidence));

        out.SetKey("timestamp", base::Value(time_stamp));
        out.SetKey("success", base::Value(landmark_detection_success));
    }
    else
    {
        out.SetKey("face_id", base::Value(face_id));
        out.SetKey("confidence", base::Value(landmark_confidence));
    }

    if (output_gaze_)
    {
        out.SetKey("gaze_0_x", base::Value(gazeDirection0.x));
        out.SetKey("gaze_0_y", base::Value(gazeDirection0.y));
        out.SetKey("gaze_0_z", base::Value(gazeDirection0.z));

        out.SetKey("gaze_1_x", base::Value(gazeDirection1.x));
        out.SetKey("gaze_1_y", base::Value(gazeDirection1.y));
        out.SetKey("gaze_1_z", base::Value(gazeDirection1.z));

        // Output gaze angle (same format as head pose angle)
        out.SetKey("gaze_angle_x", base::Value(gaze_angle[0]));
        out.SetKey("gaze_angle_y", base::Value(gaze_angle[1]));

        int count = 0;
        // Output the 2D eye landmarks
        for (auto eye_lmk : eye_landmarks2d)
        {
            out.SetKey("eye_lmk_x_" + std::to_string(count), base::Value(eye_lmk.x));
            out.SetKey("eye_lmk_y_" + std::to_string(count), base::Value(eye_lmk.y));

            count++;
        }

        count = 0;
        // Output the 3D eye landmarks
        for (auto eye_lmk : eye_landmarks3d)
        {
            out.SetKey("eye_lmk_X_" + std::to_string(count), base::Value(eye_lmk.x));
            out.SetKey("eye_lmk_Y_" + std::to_string(count), base::Value(eye_lmk.y));
            out.SetKey("eye_lmk_Z_" + std::to_string(count), base::Value(eye_lmk.z));

            count++;
        }
    }
    if (output_pose_)
    {
        out.SetKey("pose_Tx", base::Value(pose_estimate[0]));
        out.SetKey("pose_Ty", base::Value(pose_estimate[1]));
        out.SetKey("pose_Tz", base::Value(pose_estimate[2]));

        out.SetKey("pose_Rx", base::Value(pose_estimate[3]));
        out.SetKey("pose_Ry", base::Value(pose_estimate[4]));
        out.SetKey("pose_Rz", base::Value(pose_estimate[5]));
    }

    // Output the detected 2D facial landmarks
    if (output_2D_landmarks_)
    {
        auto iter = landmarks_2D.begin();

        for (int i = 0; i < num_face_landmarks_; ++i)
        {
            out.SetKey("face_x_" + std::to_string(i), base::Value(*iter));

            iter++;
        }
        for (int i = 0; i < num_face_landmarks_; ++i)
        {
            out.SetKey("face_y_" + std::to_string(i), base::Value(*iter));
            iter++;
        }
    }

    if (output_3D_landmarks_)
    {
        auto iter = landmarks_3D.begin();
        for (int i = 0; i < num_face_landmarks_; ++i)
        {
            out.SetKey("face_X_" + std::to_string(i), base::Value(*iter));

            iter++;
        }
        for (int i = 0; i < num_face_landmarks_; ++i)
        {
            out.SetKey("face_Y_" + std::to_string(i), base::Value(*iter));

            iter++;
        }
        for (int i = 0; i < num_face_landmarks_; ++i)
        {
            out.SetKey("face_Z_" + std::to_string(i), base::Value(*iter));

            iter++;
        }
    }

    // Outputting model parameters (rigid and non-rigid), the first parameters are the 6 rigid shape
    // parameters, they are followed by the non rigid shape parameters
    if (output_model_params_)
    {
        out.SetKey("p_scale", base::Value(rigid_shape_params[0]));
        out.SetKey("p_rx", base::Value(rigid_shape_params[1]));
        out.SetKey("p_ry", base::Value(rigid_shape_params[2]));
        out.SetKey("p_rz", base::Value(rigid_shape_params[3]));
        out.SetKey("p_tx", base::Value(rigid_shape_params[4]));
        out.SetKey("p_ty", base::Value(rigid_shape_params[5]));

        int count = 0;
        // Output the non_rigid shape parameters
        for (auto lmk : pdm_model_params)
        {
            out.SetKey("pdm_" + std::to_string(count), base::Value(lmk));
            count++;
        }
    }

    if (output_AUs_)
    {
        if (au_intensities.size() == 0)
        {
            for (const std::string& reg_name : au_names_reg_)
            {
                out.SetKey(reg_name + std::string("_r"), base::Value(0));
            }
        }
        else
        {
            for (const std::string& reg_name : au_names_reg_)
            {
                bool found = false;
                for (const auto& au_reg : au_intensities)
                {
                    if (reg_name.compare(au_reg.first) == 0)
                    {
                        out.SetKey(reg_name + std::string("_r"), base::Value(au_reg.second));

                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    out.SetKey(reg_name + std::string("_r"), base::Value(0));
                }
            }
        }

        if (au_occurences.size() == 0)
        {
            for (const std::string& class_name : au_names_class_)
            {
                out.SetKey(class_name + std::string("_c"), base::Value(0));
            }
        }
        else
        {
            for (const std::string& class_name : au_names_class_)
            {
                bool found = false;
                for (const auto& au_class : au_occurences)
                {
                    if (class_name.compare(au_class.first) == 0)
                    {
                        out.SetKey(class_name + std::string("_c"), base::Value(au_class.second));

                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    out.SetKey(class_name + std::string("_c"), base::Value(0));
                }
            }
        }
    }
    return out;
}
}  // namespace LandmarkDetector