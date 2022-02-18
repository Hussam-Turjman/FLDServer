if (IS_WIN)
    set(WINDOWS_SOURCES  "time_win.cc")
endif ()

if (IS_POSIX)
    set(POSIX_SOURCES   "time_exploded_posix.cc"
                        "time_now_posix.cc"
                        "time_conversion_posix.cc"
            )
endif ()
fldserver_cc_library(
        NAME
        base_time
        HDRS
        "clock.h"
        "default_clock.h"
        "default_tick_clock.h"
        "tick_clock.h"
        "time.h"
        "time_override.h"
        "time_to_iso8601.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "clock.cc"
        "default_clock.cc"
        "default_tick_clock.cc"
        "tick_clock.cc"
        "time.cc"
        "time_exploded_icu.cc"
        "time_override.cc"
        "time_to_iso8601.cc"
        "${WINDOWS_SOURCES}"
        "${POSIX_SOURCES}"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)
