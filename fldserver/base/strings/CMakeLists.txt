if (IS_WIN)
    set(WINDOWS_SOURCES
            "string_number_conversions_win.cc"
            "strcat_win.cc"
            "string_split_win.cc"
            "sys_string_conversions_win.cc"
            "string_util_win.cc"
            )
endif ()

if (IS_POSIX)
    set(POSIX_SOURCES
            "sys_string_conversions_posix.cc"
            )
endif ()

if (IS_LINUX)
    set(LINUX_SOURCES

            )
endif ()

fldserver_cc_library(
        NAME
        base_strings
        HDRS
        "abseil_string_conversions.h"
        "char_traits.h"
        "escape.h"
        "latin1_string_conversions.h"
        "pattern.h"
        "safe_sprintf.h"
        "strcat.h"
        "strcat_internal.h"
        "strcat_win.h"
        "string_number_conversions.h"
        "string_number_conversions_internal.h"
        "string_number_conversions_win.h"
        "string_piece.h"
        "string_piece_forward.h"
        "string_split.h"
        "string_split_internal.h"
        "string_split_win.h"
        "string_tokenizer.h"
        "string_util.h"
        "string_util_internal.h"
        "string_util_posix.h"
        "string_util_win.h"
        "stringize_macros.h"
        "stringprintf.h"
        "sys_string_conversions.h"
        "utf_offset_string_conversions.h"
        "utf_string_conversion_utils.h"
        "utf_string_conversions.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "abseil_string_conversions.cc"
        "escape.cc"
        "latin1_string_conversions.cc"
        "pattern.cc"
        "safe_sprintf.cc"
        "strcat.cc"
        "string_number_conversions.cc"
        "string_piece.cc"
        "string_split.cc"
        "string_util.cc"
        "string_util_constants.cc"
        "stringprintf.cc"
        "utf_offset_string_conversions.cc"
        "utf_string_conversion_utils.cc"
        "utf_string_conversions.cc"
        "${WINDOWS_SOURCES}"
        "${POSIX_SOURCES}"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)




# Sources       : 21
# Headers       : 28
# Deps          : 0
# Defines       : 0
# Platform      : linux
# OS            : posix
# Architecture  : ('64bit', 'ELF')
