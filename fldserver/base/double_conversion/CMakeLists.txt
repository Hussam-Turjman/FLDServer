
fldserver_cc_library(
        NAME
        double_conversion
        HDRS
        "bignum-dtoa.h"
        "bignum.h"
        "cached-powers.h"
        "diy-fp.h"
        "double-conversion.h"
        "double-to-string.h"
        "fast-dtoa.h"
        "fixed-dtoa.h"
        "ieee.h"
        "string-to-double.h"
        "strtod.h"
        "utils.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "bignum-dtoa.cc"
        "bignum.cc"
        "cached-powers.cc"
        "double-to-string.cc"
        "fast-dtoa.cc"
        "fixed-dtoa.cc"
        "string-to-double.cc"
        "strtod.cc"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)


