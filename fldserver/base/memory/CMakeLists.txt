if (IS_WIN)
    set(WINDOWS_SOURCES
            "page_size_win.cc"
            "platform_shared_memory_region_win.cc"
            )
endif ()

if (IS_POSIX)
    set(POSIX_SOURCES
            "madv_free_discardable_memory_allocator_posix.cc"
            "madv_free_discardable_memory_posix.cc"
            "platform_shared_memory_region_posix.cc"
            "page_size_posix.cc"
            )
endif ()

if (IS_LINUX)
    set(LINUX_SOURCES

            )
endif ()

fldserver_cc_library(
        NAME
        base_memory
        HDRS
        "aligned_memory.h"
        "bytes.h"
        "checked_ptr.h"
        "discardable_memory.h"
        "discardable_memory_allocator.h"
        "discardable_memory_internal.h"
        "discardable_shared_memory.h"
        "free_deleter.h"
        "madv_free_discardable_memory_allocator_posix.h"
        "madv_free_discardable_memory_posix.h"
        "memory_pressure_listener.h"
        "memory_pressure_monitor.h"
        "nonscannable_memory.h"
        "page_size.h"
        "platform_shared_memory_region.h"
        "ptr_util.h"
        "raw_scoped_refptr_mismatch_checker.h"
        "read_only_shared_memory_region.h"
        "ref_counted.h"
        "ref_counted_delete_on_sequence.h"
        "ref_counted_memory.h"
        "scoped_policy.h"
        "scoped_refptr.h"
        "shared_memory_hooks.h"
        "shared_memory_mapping.h"
        "shared_memory_security_policy.h"
        "singleton.h"
        "tagging.h"
        "unsafe_shared_memory_pool.h"
        "unsafe_shared_memory_region.h"
        "weak_ptr.h"
        "writable_shared_memory_region.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "aligned_memory.cc"
        "checked_ptr.cc"
        "discardable_memory.cc"
        "discardable_memory_allocator.cc"
        "discardable_shared_memory.cc"
        "memory_pressure_listener.cc"
        "memory_pressure_monitor.cc"
        "nonscannable_memory.cc"
        "platform_shared_memory_region.cc"
        "read_only_shared_memory_region.cc"
        "ref_counted.cc"
        "ref_counted_memory.cc"
        "shared_memory_mapping.cc"
        "shared_memory_security_policy.cc"
        "tagging.cc"
        "unsafe_shared_memory_pool.cc"
        "unsafe_shared_memory_region.cc"
        "weak_ptr.cc"
        "writable_shared_memory_region.cc"
        "${WINDOWS_SOURCES}"
        "${POSIX_SOURCES}"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)
