if (IS_WIN)
    set(WINDOWS_SOURCES
            "thread_local_storage_win.cc"
            "platform_thread_win.cc"
            )
endif ()

if (IS_POSIX)
    set(POSIX_SOURCES
            "platform_thread_internal_posix.cc"
            "platform_thread_posix.cc"
            "thread_local_storage_posix.cc"
            )
endif ()

if (IS_LINUX)
    set(LINUX_SOURCES
            "platform_thread_linux.cc"
            )
endif ()

fldserver_cc_library(
        NAME
        threading
        HDRS
        "hang_watcher.h"
        "platform_thread.h"
        "platform_thread_internal_posix.h"
        "platform_thread_win.h"
        "post_task_and_reply_impl.h"
        "scoped_blocking_call.h"
        "scoped_blocking_call_internal.h"
        "scoped_thread_priority.h"
        "sequence_bound.h"
        "sequence_local_storage_map.h"
        "sequence_local_storage_slot.h"
        "sequenced_task_runner_handle.h"
        "simple_thread.h"
        "thread.h"
        "thread_checker.h"
        "thread_checker_impl.h"
        "thread_collision_warner.h"
        "thread_id_name_manager.h"
        "thread_local.h"
        "thread_local_internal.h"
        "thread_local_storage.h"
        "thread_restrictions.h"
        "thread_task_runner_handle.h"
        "threading_features.h"
        "watchdog.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "hang_watcher.cc"
        "platform_thread.cc"



        "post_task_and_reply_impl.cc"
        "scoped_blocking_call.cc"
        "scoped_blocking_call_internal.cc"
        "scoped_thread_priority.cc"
        "sequence_local_storage_map.cc"
        "sequence_local_storage_slot.cc"
        "sequenced_task_runner_handle.cc"
        "simple_thread.cc"
        "thread.cc"
        "thread_checker_impl.cc"
        "thread_collision_warner.cc"
        "thread_id_name_manager.cc"
        "thread_local_storage.cc"


        "thread_restrictions.cc"
        "thread_task_runner_handle.cc"
        "watchdog.cc"
        "${POSIX_SOURCES}"
        "${LINUX_SOURCES}"
        "${WINDOWS_SOURCES}"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)
