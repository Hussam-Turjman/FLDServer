// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __FLDSERVER_INTERNAL_BUILD_FLAGS__
#define __FLDSERVER_INTERNAL_BUILD_FLAGS__

// These macros un-mangle the names of the build flags in a way that looks
// natural, and gives errors if the flag is not defined. Normally in the
// preprocessor it's easy to make mistakes that interpret "you haven't done
// the setup to know what the flag is" as "flag is off". Normally you would
// include the generated header rather than include this file directly.
//
// This is for use with generated headers. See build/buildflag_header.gni.

// This dance of two macros does a concatenation of two preprocessor args using
// ## doubly indirectly because using ## directly prevents macros in that
// parameter from being expanded.
#define BUILDFLAG_CAT_INDIRECT(a, b) a##b
#define BUILDFLAG_CAT(a, b) BUILDFLAG_CAT_INDIRECT(a, b)

// Accessor for build flags.
//
// To test for a value, if the build file specifies:
//
//   ENABLE_FOO=true
//
// Then you would check at build-time in source code with:
//
//   #include "foo_flags.h"  // The header the build file specified.
//
//   #if BUILDFLAG(ENABLE_FOO)
//     ...
//   #endif
//
// There will no #define called ENABLE_FOO so if you accidentally test for
// whether that is defined, it will always be negative. You can also use
// the value in expressions:
//
//   const char kSpamServerName[] = BUILDFLAG(SPAM_SERVER_NAME);
//
// Because the flag is accessed as a preprocessor macro with (), an error
// will be thrown if the proper header defining the internal flag value has
// not been included.
#define BUILDFLAG(flag) (BUILDFLAG_CAT(BUILDFLAG_INTERNAL_, flag)())

/* Allocator flags */
#define BUILDFLAG_INTERNAL_USE_ALLOCATOR_SHIM() (0)
#define BUILDFLAG_INTERNAL_USE_TCMALLOC() (0)
#define BUILDFLAG_INTERNAL_USE_PARTITION_ALLOC_AS_MALLOC() (0)

/* Branding flags */
#define BUILDFLAG_INTERNAL_CHROMIUM_BRANDING() (0)
#define BUILDFLAG_INTERNAL_GOOGLE_CHROME_BRANDING() (0)

/* CFI flags */
#define BUILDFLAG_INTERNAL_CFI_CAST_CHECK() (false && false)
#define BUILDFLAG_INTERNAL_CFI_ICALL_CHECK() (false && false)
#define BUILDFLAG_INTERNAL_CFI_ENFORCEMENT_TRAP() (false && !false)
#define BUILDFLAG_INTERNAL_CFI_ENFORCEMENT_DIAGNOSTIC() (false && false && !false)

/* Chromcast flags*/
#define BUILDFLAG_INTERNAL_IS_CHROMECAST() (0)

/* Chromos flags */
#define BUILDFLAG_INTERNAL_IS_CHROMEOS_DEVICE() (0)
#define BUILDFLAG_INTERNAL_IS_CHROMEOS_LACROS() (0)
#define BUILDFLAG_INTERNAL_IS_CHROMEOS_ASH() (0)

/* Clang flags*/
#define BUILDFLAG_INTERNAL_CLANG_PROFILING() (0)
#define BUILDFLAG_INTERNAL_CLANG_PROFILING_INSIDE_SANDBOX() (0)
#define BUILDFLAG_INTERNAL_USE_CLANG_COVERAGE() (0)

/* IPC flags*/
#define BUILDFLAG_INTERNAL_IPC_MESSAGE_LOG_ENABLED() (0)

/* Logging flags */
#define BUILDFLAG_INTERNAL_ENABLE_LOG_ERROR_NOT_REACHED() (0)

/* Net flags */
#define BUILDFLAG_INTERNAL_POSIX_AVOID_MMAP() (0)
#define BUILDFLAG_INTERNAL_DISABLE_FILE_SUPPORT() (0)
#define BUILDFLAG_INTERNAL_DISABLE_FTP_SUPPORT() (0)
#define BUILDFLAG_INTERNAL_ENABLE_MDNS() (1)
#define BUILDFLAG_INTERNAL_ENABLE_REPORTING() (0)
#define BUILDFLAG_INTERNAL_ENABLE_WEBSOCKETS() (0)
#define BUILDFLAG_INTERNAL_INCLUDE_TRANSPORT_SECURITY_STATE_PRELOAD_LIST() (0)
#define BUILDFLAG_INTERNAL_USE_KERBEROS() (1)
#define BUILDFLAG_INTERNAL_USE_EXTERNAL_GSSAPI() (1)
#define BUILDFLAG_INTERNAL_TRIAL_COMPARISON_CERT_VERIFIER_SUPPORTED() (0)
#define BUILDFLAG_INTERNAL_BUILTIN_CERT_VERIFIER_FEATURE_SUPPORTED() (0)

/* Partition allocator flags */
#define BUILDFLAG_INTERNAL_USE_PARTITION_ALLOC() (0)
#define BUILDFLAG_INTERNAL_USE_BACKUP_REF_PTR() (0)
#define BUILDFLAG_INTERNAL_USE_BACKUP_REF_PTR_FAKE() (0)
#define BUILDFLAG_INTERNAL_ENABLE_RUNTIME_BACKUP_REF_PTR_CONTROL() (0)
#define BUILDFLAG_INTERNAL_ENABLE_BACKUP_REF_PTR_SLOW_CHECKS() (0)
#define BUILDFLAG_INTERNAL_PUT_REF_COUNT_IN_PREVIOUS_SLOT() (0)
#define BUILDFLAG_INTERNAL_USE_BRP_POOL_BLOCKLIST() (0)
#define BUILDFLAG_INTERNAL_NEVER_REMOVE_FROM_BRP_POOL_BLOCKLIST() (0)
#define BUILDFLAG_INTERNAL_MAKE_GIGACAGE_GRANULARITY_PARTITION_PAGE_SIZE() (0)
#define BUILDFLAG_INTERNAL_USE_DEDICATED_PARTITION_FOR_ALIGNED_ALLOC() (0)

/* Profiler flags */
#define BUILDFLAG_INTERNAL_ENABLE_ARM_CFI_TABLE() (0)

/* Sanitizer flags */
#define BUILDFLAG_INTERNAL_IS_HWASAN() (0)

/* Synchronization flags */
#define BUILDFLAG_INTERNAL_ENABLE_MUTEX_PRIORITY_INHERITANCE() (0)

/* Tracing flags */
#define BUILDFLAG_INTERNAL_ENABLE_BASE_TRACING() (0)
#define BUILDFLAG_INTERNAL_USE_PERFETTO_CLIENT_LIBRARY() (0)
#define BUILDFLAG_INTERNAL_OPTIONAL_TRACE_EVENTS_ENABLED() (0)

/* Debugging flags */
#define BUILDFLAG_INTERNAL_ENABLE_LOCATION_SOURCE() (1)
#define BUILDFLAG_INTERNAL_FROM_HERE_USES_LOCATION_BUILTINS() (1)
#define BUILDFLAG_INTERNAL_ENABLE_PROFILING() (0)
#define BUILDFLAG_INTERNAL_CAN_UNWIND_WITH_FRAME_POINTERS() (1)
#define BUILDFLAG_INTERNAL_UNSAFE_DEVELOPER_BUILD() (0)
#define BUILDFLAG_INTERNAL_CAN_UNWIND_WITH_CFI_TABLE() (0)
#define BUILDFLAG_INTERNAL_EXCLUDE_UNWIND_TABLES() (0)
#define BUILDFLAG_INTERNAL_ENABLE_GDBINIT_WARNING() (1)
#define BUILDFLAG_INTERNAL_ENABLE_LLDBINIT_WARNING() (0)
#define BUILDFLAG_INTERNAL_EXPENSIVE_DCHECKS_ARE_ON() (0)

/* Blitz url */
#define BUILDFLAG_INTERNAL_USE_PLATFORM_ICU_ALTERNATIVES() (0)

#endif /* __FLDSERVER_INTERNAL_BUILD_FLAGS__ */
