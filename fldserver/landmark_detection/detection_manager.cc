// Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
// source code is governed by a license that can be found in the LICENSE file.

#include "fldserver/landmark_detection/detection_manager.h"
#include <memory>
#include <vector>
#include <string>
#include "fldserver/base/logging.h"

namespace LandmarkDetector
{
namespace
{
std::vector<std::string>
get_arguments(int argc, char** argv)
{
    std::vector<std::string> arguments;

    // First argument is reserved for the name of the executable
    for (int i = 0; i < argc; ++i)
    {
        arguments.push_back(std::string(argv[i]));
    }
    return arguments;
}
}  // namespace

DetectionManager::DetectionManager() :
    det_parameters_(),
    record_openface_params_(true, true, true, true, true, true, true, true, true, true, true)
{
    record_openface_params_.setOutputAlignedFaces(false);
    record_openface_params_.setOutputTracked(false);
    record_openface_params_.setOutputHog(false);
}

DetectionManager::~DetectionManager()
{
    Reset();
    LOG(INFO) << "Destroying DetectionManager  .. ";
};

bool
DetectionManager::Init(int argc, char** argv)
{
    return Init(get_arguments(argc, argv));
}

bool
DetectionManager::OpenSequenceReader(
        double fps_guess, int image_width, int image_height, float fx, float fy, float cx, float cy)
{
    if (!sequence_reader_)
    {
        LOG(FATAL) << "Detection Manager was not initialized !!!";
        return false;
    }
    return sequence_reader_->OpenRemoteWebcam(fps_guess, image_width, image_height, fx, fy, cx, cy);
}

bool
DetectionManager::DetectFeatures(const cv::Mat& captured_image,
                                 base::Value* json_out,
                                 cv::Mat* vis_image,
                                 bool show_observation)
{
    if (!sequence_reader_)
    {
        LOG(FATAL) << "Detection Manager was not initialized !!!";
        return false;
    }
    if (!json_out)
    {
        LOG(FATAL) << "Null pointer !!";
        return false;
    }

    sequence_reader_->SetRemoteFrame(captured_image);

    // Converting to grayscale
    cv::Mat_<uchar> grayscale_image = sequence_reader_->GetGrayFrame();

    // The actual facial landmark detection / tracking
    bool detection_success = LandmarkDetector::DetectLandmarksInVideo(
            captured_image, *face_model_, *det_parameters_, grayscale_image);

    // Gaze tracking, absolute gaze direction
    cv::Point3f gazeDirection0(0, 0, 0);
    cv::Point3f gazeDirection1(0, 0, 0);
    cv::Vec2d gazeAngle(0, 0);

    if (detection_success && face_model_->eye_model)
    {
        GazeAnalysis::EstimateGaze(*face_model_,
                                   gazeDirection0,
                                   sequence_reader_->fx,
                                   sequence_reader_->fy,
                                   sequence_reader_->cx,
                                   sequence_reader_->cy,
                                   true);
        GazeAnalysis::EstimateGaze(*face_model_,
                                   gazeDirection1,
                                   sequence_reader_->fx,
                                   sequence_reader_->fy,
                                   sequence_reader_->cx,
                                   sequence_reader_->cy,
                                   false);
        gazeAngle = GazeAnalysis::GetGazeAngle(gazeDirection0, gazeDirection1);
    }

    // Do face alignment
    cv::Mat sim_warped_img;
    cv::Mat_<double> hog_descriptor;
    int num_hog_rows = 0, num_hog_cols = 0;

    // Perform AU detection and HOG feature extraction, as this can be expensive only compute it if
    // needed by output or visualization
    if (record_openface_params_.outputAlignedFaces() || record_openface_params_.outputHOG()
        || record_openface_params_.outputAUs() || visualizer_->vis_align || visualizer_->vis_hog
        || visualizer_->vis_aus)
    {
        face_analyser_->AddNextFrame(captured_image,
                                     face_model_->detected_landmarks,
                                     face_model_->detection_success,
                                     sequence_reader_->time_stamp,
                                     sequence_reader_->IsRemoteWebcam());
        face_analyser_->GetLatestAlignedFace(sim_warped_img);
        face_analyser_->GetLatestHOG(hog_descriptor, num_hog_rows, num_hog_cols);
    }

    // Work out the pose of the head from the tracked model
    cv::Vec6d pose_estimate = LandmarkDetector::GetPose(*face_model_,
                                                        sequence_reader_->fx,
                                                        sequence_reader_->fy,
                                                        sequence_reader_->cx,
                                                        sequence_reader_->cy);

    // Keeping track of FPS
    fps_tracker_->AddFrame();

    // Displaying the tracking visualizations
    visualizer_->SetImage(captured_image,
                          sequence_reader_->fx,
                          sequence_reader_->fy,
                          sequence_reader_->cx,
                          sequence_reader_->cy);
    visualizer_->SetObservationFaceAlign(sim_warped_img);
    visualizer_->SetObservationHOG(hog_descriptor, num_hog_rows, num_hog_cols);
    visualizer_->SetObservationLandmarks(face_model_->detected_landmarks,
                                         face_model_->detection_certainty,
                                         face_model_->GetVisibilities());
    visualizer_->SetObservationPose(pose_estimate, face_model_->detection_certainty);
    visualizer_->SetObservationGaze(gazeDirection0,
                                    gazeDirection1,
                                    LandmarkDetector::CalculateAllEyeLandmarks(*face_model_),
                                    LandmarkDetector::Calculate3DEyeLandmarks(*face_model_,
                                                                              sequence_reader_->fx,
                                                                              sequence_reader_->fy,
                                                                              sequence_reader_->cx,
                                                                              sequence_reader_->cy),
                                    face_model_->detection_certainty);
    visualizer_->SetObservationActionUnits(face_analyser_->GetCurrentAUsReg(),
                                           face_analyser_->GetCurrentAUsClass());
    visualizer_->SetFps(fps_tracker_->GetFPS());

    if (show_observation)
    {
        visualizer_->vis_aus = true;

        // detect key presses
        char character_press = visualizer_->ShowObservation();

        // quit processing the current sequence (useful when in Webcam mode)
        if (character_press == 'q')
        {
            // break;
        }
    }

    // Setting up the recorder output
    // open_face_rec.SetObservationHOG(detection_success, hog_descriptor, num_hog_rows,
    // num_hog_cols, 31); // The number of channels in HOG is fixed at the moment, as using FHOG
    recorder_openface_->SetObservationVisualization(visualizer_->GetVisImage());
    recorder_openface_->SetObservationActionUnits(face_analyser_->GetCurrentAUsReg(),
                                                  face_analyser_->GetCurrentAUsClass());
    recorder_openface_->SetObservationLandmarks(face_model_->detected_landmarks,
                                                face_model_->GetShape(sequence_reader_->fx,
                                                                      sequence_reader_->fy,
                                                                      sequence_reader_->cx,
                                                                      sequence_reader_->cy),
                                                face_model_->params_global,
                                                face_model_->params_local,
                                                face_model_->detection_certainty,
                                                detection_success);
    recorder_openface_->SetObservationPose(pose_estimate);
    recorder_openface_->SetObservationGaze(
            gazeDirection0,
            gazeDirection1,
            gazeAngle,
            LandmarkDetector::CalculateAllEyeLandmarks(*face_model_),
            LandmarkDetector::Calculate3DEyeLandmarks(*face_model_,
                                                      sequence_reader_->fx,
                                                      sequence_reader_->fy,
                                                      sequence_reader_->cx,
                                                      sequence_reader_->cy));
    recorder_openface_->SetObservationTimestamp(sequence_reader_->time_stamp);
    recorder_openface_->SetObservationFaceID(0);
    recorder_openface_->SetObservationFrameNumber(sequence_reader_->GetFrameNumber());
    recorder_openface_->SetObservationFaceAlign(sim_warped_img);

    recorder_openface_->WriteObservation(json_out);

    if (vis_image)
    {
        *vis_image = visualizer_->GetVisImage();
    }
    return true;
}

void
DetectionManager::Reset()
{
    if (face_model_)
    {
        recorder_openface_->Close();

        sequence_reader_->Close();

        if (record_openface_params_.outputAUs())
        {
            // INFO_STREAM("Postprocessing the Action Unit predictions");
            // face_analyser.PostprocessOutputFile(open_face_rec.GetCSVFile());
        }

        // Reset the models for the next video
        face_analyser_->Reset();
        face_model_->Reset();
    }
}

bool
DetectionManager::Init(const std::vector<std::string>& arguments)
{
    Reset();

    arguments_ = arguments;
    // no arguments: output usage
    if (arguments_.size() == 1)
    {
        // LOG(ERROR) << "For command line arguments see:";
        // LOG(ERROR) << "
        // https://github.com/TadasBaltrusaitis/OpenFace/wiki/Command-line-arguments"; return false;
    }

    // Load the modules that are being used for tracking and face analysis
    // Load face landmark detector
    det_parameters_ = std::make_unique<FaceModelParameters>(arguments_);
    // Always track gaze in feature extraction
    face_model_ = std::make_unique<CLNF>(det_parameters_->model_location);

    if (!face_model_->loaded_successfully)
    {
        LOG(ERROR) << "ERROR: Could not load the landmark detector";
        return false;
    }

    // Load facial feature extractor and AU analyser
    face_analysis_params_ = std::make_unique<FaceAnalysis::FaceAnalyserParameters>(arguments_);
    face_analyser_ = std::make_unique<FaceAnalysis::FaceAnalyser>(*face_analysis_params_);

    if (!face_model_->eye_model)
    {
        LOG(WARNING) << "WARNING: no eye model found";
    }

    if (face_analyser_->GetAUClassNames().empty() && face_analyser_->GetAUClassNames().empty())
    {
        LOG(WARNING) << "WARNING: no Action Unit models found";
    }

    sequence_reader_ = std::make_unique<Utilities::SequenceCapture>();

    // A utility for visualizing the results
    visualizer_ = std::make_unique<Utilities::Visualizer>(arguments_);

    // Tracking FPS for visualization
    fps_tracker_ = std::make_unique<Utilities::FpsTracker>();
    fps_tracker_->AddFrame();

    if (!face_model_->eye_model)
    {
        record_openface_params_.setOutputGaze(false);
    }
    if (record_openface_params_.outputGaze() && !face_model_->eye_model)
    {
        LOG(WARNING) << "WARNING: no eye model defined, but outputting gaze";
    }
    recorder_openface_ =
            std::make_unique<Utilities::RecorderOpenFace>(record_openface_params_, arguments_);

    return true;
}
}  // namespace LandmarkDetector