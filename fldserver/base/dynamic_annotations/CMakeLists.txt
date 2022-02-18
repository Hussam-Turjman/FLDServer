
fldserver_cc_library(
        NAME
        dynamic_annotations
        HDRS
        "dynamic_annotations.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "dynamic_annotations.c"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)

