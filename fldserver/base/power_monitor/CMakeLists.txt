if (IS_WIN)
    set(WINDOWS_SOURCES
            "power_monitor_device_source_win.cc"
            )
endif ()

if (IS_POSIX)
    set(POSIX_SOURCES
            "power_monitor_device_source_stub.cc"
            )
endif ()

if (IS_LINUX)
    set(LINUX_SOURCES
            )
endif ()

fldserver_cc_library(
        NAME
        power_monitor
        HDRS
        "power_monitor.h"
        "power_monitor_device_source.h"
        "power_monitor_source.h"
        "power_observer.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "power_monitor.cc"
        "power_monitor_device_source.cc"
        "power_monitor_source.cc"
        "${WINDOWS_SOURCES}"
        "${POSIX_SOURCES}"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)

