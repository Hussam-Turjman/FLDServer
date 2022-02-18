# Copyright (c) 2021 The Core project Authors. All rights reserved. Use of this
# source code is governed by a BSD-style license that can be found in the LICENSE file.


fldserver_cc_library(
        NAME
        modp_b64
        HDRS
        "modp_b64.h"
        "modp_b64_data.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "modp_b64.cc"
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)