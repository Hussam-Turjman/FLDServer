if (IS_WIN)
    set(WINDOWS_SOURCES
            "message_pump_win.cc"
            )
endif ()

if (IS_POSIX)
    set(POSIX_SOURCES
            "watchable_io_message_pump_posix.cc"
            "message_pump_libevent.cc"
            )
endif ()

if (IS_LINUX)
    set(LINUX_SOURCES

            )
endif ()

fldserver_cc_library(
        NAME
        message_loop
        HDRS
        "message_pump.h"
        "message_pump_default.h"
        "message_pump_for_io.h"
        "message_pump_for_ui.h"
        "message_pump_libevent.h"
        
        "message_pump_type.h"
        "message_pump_win.h"
        "timer_slack.h"
        "watchable_io_message_pump_posix.h"
        "work_id_provider.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "message_pump.cc"
        "message_pump_default.cc"

        "timer_slack.cc"
        "work_id_provider.cc"
        "${WINDOWS_SOURCES}"
        "${POSIX_SOURCES}"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)