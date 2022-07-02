# Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
# source code is governed by a license that can be found in the LICENSE file.


fldserver_cc_library(
        NAME
        landmark_detection
        HDRS
        "sequence_capture.h"
        "image_manipulation_helpers.h"
        "stdafx_ut.h"
        "recorder_openface.h"
        "concurrent_queue.h"
        "csv_recorder.h"
        "hog_recorder.h"
        "recorder_openface_parameters.h"
        "detection_manager.h"
        "features_extractor.h"
        COPTS
        ${CORE_DEFAULT_COPTS}
        SRCS
        "features_extractor.cc"
        "detection_manager.cc"
        "recorder_openface_parameters.cc"
        "hog_recorder.cc"
        "csv_recorder.cc"
        "recorder_openface.cc"
        "sequence_capture.cc"
        DEPS
        Threads::Threads
        dlib::dlib
        ${OpenCV_LIBS}
        ${OpenFace_LIBRARIES}
        ${CMAKE_DL_LIBS}
        PUBLIC
)