if (IS_WIN)
    set(WINDOWS_SOURCES
            # error C2712: Cannot use __try in functions that require object unwinding
            "close_handle_hook_win.cc"
            "gdi_debug_util_win.cc"
            # Assembly errors
            "invalid_access_win.cc"
            "debugger_win.cc"
            "stack_trace_win.cc"

            )
endif ()

if (IS_POSIX)
    set(POSIX_SOURCES
            "debugger_posix.cc"
            "stack_trace_posix.cc"

            )
endif ()

if (IS_LINUX)
    set(LINUX_SOURCES
            "proc_maps_linux.cc"
            "elf_reader.cc"
            )
endif ()

fldserver_cc_library(
        NAME
        debug
        HDRS
        "activity_analyzer.h"
        "activity_tracker.h"
        "alias.h"
        "asan_invalid_access.h"
        "close_handle_hook_win.h"
        "crash_logging.h"
        "debugger.h"
        "dump_without_crashing.h"
        "elf_reader.h"
        "gdi_debug_util_win.h"
        "invalid_access_win.h"
        "leak_annotations.h"
        "proc_maps_linux.h"
        "profiler.h"
        "stack_trace.h"
        "task_trace.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "activity_analyzer.cc"
        "activity_tracker.cc"
        "alias.cc"
        "asan_invalid_access.cc"
        "crash_logging.cc"
        "debugger.cc"
        "dump_without_crashing.cc"
        "profiler.cc"
        "task_trace.cc"
        # '__builtin_frame_address': identifier not found on windows
        "stack_trace.cc"
        "${WINDOWS_SOURCES}"
        "${POSIX_SOURCES}"
        "${LINUX_SOURCES}"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)