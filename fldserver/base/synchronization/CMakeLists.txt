if (IS_WIN)
    set(WINDOWS_SOURCES
            "condition_variable_win.cc"
            "lock_impl_win.cc"
            "waitable_event_watcher_win.cc"
            "waitable_event_win.cc"
            "sync_var_win.cc"
            )
endif ()

if (IS_POSIX)
    set(POSIX_SOURCES
            "lock_impl_posix.cc"
            "waitable_event_posix.cc"
            "condition_variable_posix.cc"
            "waitable_event_watcher_posix.cc"
            "sync_var_posix.cc"
            "atomic_ops_posix.c"
            )
endif ()

if (IS_LINUX)
    set(LINUX_SOURCES

            )
endif ()

fldserver_cc_library(
        NAME
        synchronization
        HDRS
        "atomic_flag.h"
        "condition_variable.h"
        "lock.h"
        "lock_impl.h"
        "waitable_event.h"
        "waitable_event_watcher.h"
        "sync_var.h"
        "sync_var_posix.h"
        "atomic_ops_posix.h"
        "sync_var_win.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "sync_var.cc"
        "atomic_flag.cc"
        "lock.cc"
        "${WINDOWS_SOURCES}"
        "${POSIX_SOURCES}"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)