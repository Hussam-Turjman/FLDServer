if (IS_WIN)
    set(WINDOWS_SOURCES
            "file_enumerator_win.cc"
            "file_path_watcher_win.cc"
            "file_util_win.cc"
            "file_win.cc"
            "memory_mapped_file_win.cc"
            )
endif ()

if (IS_POSIX)
    set(POSIX_SOURCES
            "file_descriptor_watcher_posix.cc"
            "file_enumerator_posix.cc"
            "file_posix.cc"
            "file_util_posix.cc"
            "memory_mapped_file_posix.cc"
            )
endif ()

if (IS_LINUX)
    set(LINUX_SOURCES
            "file_util_linux.cc"
            "file_path_watcher_linux.cc"
            )
endif ()

fldserver_cc_library(
        NAME
        files
        HDRS
        "dir_reader_fallback.h"
        "dir_reader_linux.h"
        "dir_reader_posix.h"
        "file.h"
        "file_descriptor_watcher_posix.h"
        "file_enumerator.h"
        "file_path.h"
        "file_path_watcher.h"
        "file_path_watcher_linux.h"
        "file_proxy.h"
        "file_tracing.h"
        "file_util.h"
        "important_file_writer.h"
        "important_file_writer_cleaner.h"
        "memory_mapped_file.h"
        "platform_file.h"
        "scoped_file.h"
        "scoped_temp_dir.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "file.cc"
        "file_enumerator.cc"
        "file_path.cc"
        "file_path_constants.cc"
        "file_path_watcher.cc"

        # "file_path_watcher_stub.cc"
        "file_proxy.cc"
        "file_tracing.cc"
        "file_util.cc"
        "important_file_writer.cc"
        "important_file_writer_cleaner.cc"
        "memory_mapped_file.cc"
        "scoped_file.cc"
        "scoped_temp_dir.cc"
        "${WINDOWS_SOURCES}"
        "${POSIX_SOURCES}"
        "${LINUX_SOURCES}"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)