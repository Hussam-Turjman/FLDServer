# Copyright (c) 2021 The Core project Authors. All rights reserved. Use of this
# source code is governed by a BSD-style license that can be found in the LICENSE file.


fldserver_cc_library(
        NAME
        nspr
        HDRS
        "prtime.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "prtime.cc"
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)