fldserver_cc_library(
        NAME
        cityhash
        HDRS
        "city.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "city.cc"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)
