if (IS_WIN)
    set(WINDOWS_SOURCES
            "sys_info_win.cc"
            )
endif ()

if (IS_POSIX)
    set(POSIX_SOURCES
            "sys_info_posix.cc"
            )
endif ()

if (IS_LINUX)
    set(LINUX_SOURCES
            "sys_info_linux.cc"
            )
endif ()

fldserver_cc_library(
        NAME
        base_system
        HDRS
        "sys_info.h"
        "sys_info_internal.h"
        "system_monitor.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "sys_info.cc"
        # "sys_info_chromeos.cc"
        # "sys_info_freebsd.cc"
        # "sys_info_openbsd.cc"
        "system_monitor.cc"
        "${LINUX_SOURCES}"
        "${POSIX_SOURCES}"
        "${WINDOWS_SOURCES}"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)
