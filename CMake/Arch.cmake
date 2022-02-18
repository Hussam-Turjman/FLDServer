# Copyright (c) 2021 The Core project Authors. All rights reserved. Use of this
# source code is governed by a BSD-style license that can be found in the LICENSE file.

include(CheckSymbolExists)
include(CheckVariableExists)
include(CheckIncludeFile)

include(TestBigEndian)



cmake_host_system_information(RESULT NUMBER_OF_LOGICAL_CORES QUERY  NUMBER_OF_LOGICAL_CORES)
cmake_host_system_information(RESULT NUMBER_OF_PHYSICAL_CORES QUERY  NUMBER_OF_PHYSICAL_CORES)
cmake_host_system_information(RESULT HOSTNAME QUERY  HOSTNAME)
cmake_host_system_information(RESULT FQDN QUERY  FQDN)
cmake_host_system_information(RESULT TOTAL_VIRTUAL_MEMORY QUERY  TOTAL_VIRTUAL_MEMORY)
cmake_host_system_information(RESULT AVAILABLE_VIRTUAL_MEMORY QUERY  AVAILABLE_VIRTUAL_MEMORY)
cmake_host_system_information(RESULT TOTAL_PHYSICAL_MEMORY QUERY  TOTAL_PHYSICAL_MEMORY)
cmake_host_system_information(RESULT AVAILABLE_PHYSICAL_MEMORY QUERY  AVAILABLE_PHYSICAL_MEMORY)
cmake_host_system_information(RESULT IS_64BIT QUERY  IS_64BIT)
cmake_host_system_information(RESULT HAS_FPU QUERY  HAS_FPU)
cmake_host_system_information(RESULT HAS_MMX QUERY  HAS_MMX)
cmake_host_system_information(RESULT HAS_MMX_PLUS QUERY  HAS_MMX_PLUS)
cmake_host_system_information(RESULT HAS_SSE QUERY  HAS_SSE)
cmake_host_system_information(RESULT HAS_SSE2 QUERY  HAS_SSE2)
cmake_host_system_information(RESULT HAS_SSE_FP QUERY  HAS_SSE_FP)
cmake_host_system_information(RESULT HAS_SSE_MMX QUERY  HAS_SSE_MMX)
cmake_host_system_information(RESULT HAS_AMD_3DNOW QUERY  HAS_AMD_3DNOW)
cmake_host_system_information(RESULT HAS_AMD_3DNOW_PLUS QUERY  HAS_AMD_3DNOW_PLUS)
cmake_host_system_information(RESULT HAS_IA64 QUERY  HAS_IA64)
cmake_host_system_information(RESULT HAS_SERIAL_NUMBER QUERY  HAS_SERIAL_NUMBER)
cmake_host_system_information(RESULT PROCESSOR_SERIAL_NUMBER QUERY  PROCESSOR_SERIAL_NUMBER)
cmake_host_system_information(RESULT PROCESSOR_NAME QUERY  PROCESSOR_NAME)
cmake_host_system_information(RESULT PROCESSOR_DESCRIPTION QUERY  PROCESSOR_DESCRIPTION)
cmake_host_system_information(RESULT OS_NAME QUERY  OS_NAME)
cmake_host_system_information(RESULT OS_RELEASE QUERY  OS_RELEASE)
cmake_host_system_information(RESULT OS_VERSION QUERY  OS_VERSION)
cmake_host_system_information(RESULT OS_PLATFORM QUERY  OS_PLATFORM)


# Initialization:
set(IS_X64 0)
set(IS_X86 0)
set(IS_POSIX 0)
set(IS_WIN 0)
set(IS_APPLE 0)
set(IS_LINUX 0)

if (UNIX)
    set(IS_POSIX 1)
endif (UNIX)

if (WIN32)
    set(IS_WIN 1)
endif ()

if (APPLE)
    set(IS_APPLE 1)
endif()

if (UNIX AND NOT APPLE)
    set(IS_LINUX 1)
endif()

if (IS_APPLE AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(IS_MAC 1)
else()
    set(IS_MAC 0)
endif()

# Check for 64-bit x86 (aka x64):
check_symbol_exists(__x86_64   "stddef.h" IS__x86_64)

if(NOT IS__x86_64)
    set(IS__x86_64 0)
endif()

check_symbol_exists(__x86_64__ "stddef.h" IS__x86_64__)

if(NOT IS__x86_64__)
    set(IS__x86_64__ 0)
endif()

check_symbol_exists(__amd64    "stddef.h" IS__amd64)

if(NOT IS__amd64)
    set(IS__amd64 0)
endif()

check_symbol_exists(_M_X64     "stddef.h" IS_M_X64)

if(NOT IS_M_X64)
    set(IS_M_X64 0)
endif()

if ((IS__x86_64 OR IS__x86_64__) OR (IS__amd64 OR IS_M_X64))
    message(STATUS "Processor architecture is 64-bit x86.")
    set(IS_X64 1)
else()

    # Check for 32-bit x86:
    check_symbol_exists(__i386   "stddef.h" IS__i386)
    check_symbol_exists(__i386__ "stddef.h" IS__i386__)
    check_symbol_exists(_M_IX86  "stddef.h" IS_M_IX86)

    if ((IS__i386 OR IS__i386__) OR IS_M_IX86)
        message(STATUS "Processor architecture is 32-bit x86.")
        set(IS_X86 1)
    else()
        message(FATAL_ERROR " This version only supports x86 and x86-64 architectures ")
    endif() # x86
endif() # x64


if(NOT IS__i386)
    set(IS__i386 0)
endif()

if(NOT IS__i386__)
    set(IS__i386__ 0)
endif()

if(NOT IS_M_IX86)
    set(IS_M_IX86 0)
endif()


if(NOT IS_X86)
    set(IS_X86 0)
endif()

test_big_endian(IS_BIG_ENDIAN)
set(IS_LITTLE_ENDIAN 0)

if (NOT IS_BIG_ENDIAN)
    set(IS_BIG_ENDIAN 0)
    set(IS_LITTLE_ENDIAN 1)
endif ()

check_include_file(fcntl.h HAS_FCNTL)
check_include_file(pthread.h HAS_PTHREAD)
check_include_file(poll.h HAS_POLL)
check_include_file(unistd.h HAS_UNISTD)


if(NOT HAS_UNISTD)
    set(HAS_UNISTD 0)
endif()


if(NOT HAS_FCNTL)
    set(HAS_FCNTL 0)
endif()


if(NOT HAS_PTHREAD)
    set(HAS_PTHREAD 0)
endif()


if(NOT HAS_POLL)
    set(HAS_POLL 0)
endif()
