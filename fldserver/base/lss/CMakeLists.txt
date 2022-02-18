# Copyright (c) 2021 The Core project Authors. All rights reserved. Use of this
# source code is governed by a BSD-style license that can be found in the LICENSE file.


fldserver_cc_library(
        NAME
        lss
        HDRS
        "linux_syscall_support.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "dummy.c"
        DEPS
        Threads::Threads
        PUBLIC
)