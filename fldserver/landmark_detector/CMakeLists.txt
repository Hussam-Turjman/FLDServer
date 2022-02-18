# Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
# source code is governed by a license that can be found in the LICENSE file.


fldserver_cc_library(
        NAME
        landmark_detector
        HDRS
        "manager.h"
        "features_extractor.h"
        "video_frame.h"
        "features_recorder.h"
        COPTS
        ${CORE_DEFAULT_COPTS}
        SRCS
        "features_recorder.cc"
        "video_frame.cc"
        "features_extractor.cc"
        "manager.cc"
        DEPS
        Threads::Threads
        dlib::dlib
        ${OpenCV_LIBS}
        ${OpenFace_LIBRARIES}
        ${CMAKE_DL_LIBS}
        PUBLIC
)