# Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
# source code is governed by a license that can be found in the LICENSE file.


fldserver_cc_library(
        NAME
        service
        HDRS
        "service_observer.h"
        "fields_alloc.h"
        "http_worker.h"
        "runner.h"
        "sessions_handler.h"
        "session.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "session.cc"
        "sessions_handler.cc"
        "runner.cc"
        "http_worker.cc"
        "service_observer.cc"
        DEPS
        Threads::Threads
        fldserver::base
        ${Boost_LIBRARIES}
        PUBLIC
)