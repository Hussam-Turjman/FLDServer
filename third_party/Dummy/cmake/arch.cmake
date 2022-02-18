# Auto generated file by blitz 2021-04-29-22:22:31. Blitz is a tool for generating cmake 
# templates, written by (Mohammed Hussam Al Turjman, hussam.turjman@gmail.com). 
# For more information please visit https://github.com/Hussam-Turjman/LibraryTemplate

# Copyright (c) 2021 The Blitz project Authors. All rights reserved. Use of this source 
# code is governed by a BSD-style license that can be found in the LICENSE file.

include(CheckSymbolExists)
include(CheckVariableExists)
include(CheckIncludeFile)

# include(TestBigEndian)

# test_big_endian(WORDS_BIGENDIAN)

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
