# Copyright (c) 2021 The Core project Authors. All rights reserved. Use of this
# source code is governed by a BSD-style license that can be found in the LICENSE file.


fldserver_cc_library(
        NAME
        xdg_user_dir_lookup
        HDRS
        "xdg_user_dir_lookup.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "xdg_user_dir_lookup.cc"
        DEPS
        Threads::Threads
        PUBLIC
)
