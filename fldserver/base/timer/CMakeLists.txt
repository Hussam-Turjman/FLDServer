if (IS_WIN)
    set(WINDOWS_SOURCES
            "hi_res_timer_manager_win.cc"
            )
endif ()

if (IS_POSIX)
    set(POSIX_SOURCES
            "hi_res_timer_manager_posix.cc"
            )
endif ()

if (IS_LINUX)
    set(LINUX_SOURCES

            )
endif ()

fldserver_cc_library(
        NAME
        timer
        HDRS
        "elapsed_timer.h"
        "hi_res_timer_manager.h"
        "lap_timer.h"
        "timer.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "elapsed_timer.cc"
        "lap_timer.cc"
        "timer.cc"
        "${POSIX_SOURCES}"
        "${WINDOWS_SOURCES}"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)