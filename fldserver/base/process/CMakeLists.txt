if (IS_WIN)
    set(WINDOWS_SOURCES
            "kill_win.cc"
            "launch_win.cc"
            "memory_win.cc"
            "process_handle_win.cc"
            "process_info_win.cc"
            "process_iterator_win.cc"
            "process_win.cc"
            "process_metrics_win.cc"
            )
endif ()

if (IS_POSIX)
    set(POSIX_SOURCES
            "process_metrics_posix.cc"
            "kill_posix.cc"
            "process_posix.cc"
            "launch_posix.cc"
            "process_handle_posix.cc"
            )
endif ()

if (IS_LINUX)
    set(LINUX_SOURCES
            "internal_linux.cc"
            "memory_linux.cc"
            "process_handle_linux.cc"
            "process_iterator_linux.cc"
            "process_linux.cc"
            "process_metrics_linux.cc"
            )
endif ()

fldserver_cc_library(
        NAME
        process
        HDRS
        "environment_internal.h"
        "internal_linux.h"
        "kill.h"
        "launch.h"
        "memory.h"

        "process.h"
        "process_handle.h"
        "process_info.h"
        "process_iterator.h"
        "process_metrics.h"
        "process_metrics_iocounters.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "environment_internal.cc"
        "kill.cc"
        "launch.cc"
        "memory.cc"
        "process_handle.cc"
        "process_iterator.cc"
        "process_metrics.cc"
        "${WINDOWS_SOURCES}"
        "${POSIX_SOURCES}"
        "${LINUX_SOURCES}"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)


