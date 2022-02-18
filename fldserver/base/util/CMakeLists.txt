if (IS_WIN)
    set(WINDOWS_SOURCES
            "memory_pressure/system_memory_pressure_evaluator_win.cc"
            )
endif ()

if (IS_POSIX)
    set(POSIX_SOURCES

            )
endif ()

if (IS_LINUX)
    set(LINUX_SOURCES
            "memory_pressure/system_memory_pressure_evaluator_linux.cc"
            )
endif ()

fldserver_cc_library(
        NAME
        util
        HDRS
        "memory_pressure/fake_memory_pressure_monitor.h"
        "memory_pressure/memory_pressure_level_reporter.h"
        "memory_pressure/memory_pressure_voter.h"
        "memory_pressure/multi_source_memory_pressure_monitor.h"
        "memory_pressure/system_memory_pressure_evaluator.h"
        "memory_pressure/system_memory_pressure_evaluator_linux.h"
        "memory_pressure/system_memory_pressure_evaluator_win.h"
        "timer/wall_clock_timer.h"
        "type_safety/id_type.h"
        "type_safety/token_type.h"
        "values/values_util.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "memory_pressure/fake_memory_pressure_monitor.cc"
        "memory_pressure/memory_pressure_level_reporter.cc"
        "memory_pressure/memory_pressure_voter.cc"
        "memory_pressure/multi_source_memory_pressure_monitor.cc"
        "memory_pressure/system_memory_pressure_evaluator.cc"
        "timer/wall_clock_timer.cc"
        "values/values_util.cc"
        "${LINUX_SOURCES}"
        "${WINDOWS_SOURCES}"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)



# Sources       : 10
# Headers       : 12
# Deps          : 0
# Defines       : 0
# Platform      : linux
# OS            : posix
# Architecture  : ('64bit', 'ELF')
