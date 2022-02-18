# Auto generated file by blitz 2021-04-22-20:11:59. blitz is a tool for generating cmake
# templates, written by (Mohammed Hussam Al Turjman, hussam.turjman@gmail.com). 
# For more information please visit https://github.com/Hussam-Turjman/LibraryTemplate

#
# Copyright 2017 The Abseil Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# See fldserver/copts/copts.py and fldserver/copts/generate_copts.py
include(GENERATED_FLDServerCopts)

set(FLDSERVER_LSAN_LINKOPTS "")
set(FLDSERVER_HAVE_LSAN OFF)
set(FLDSERVER_DEFAULT_LINKOPTS "")


if(BUILD_DLL)
  set(FLDSERVER_BUILD_DLL TRUE)
endif()

if("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "x86_64|amd64|AMD64")
  if (MSVC)
    set(FLDSERVER_RANDOM_RANDEN_COPTS "${FLDSERVER_RANDOM_HWAES_MSVC_X64_FLAGS}")
  else()
    set(FLDSERVER_RANDOM_RANDEN_COPTS "${FLDSERVER_RANDOM_HWAES_X64_FLAGS}")
  endif()
elseif("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm.*|aarch64")
  if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    set(FLDSERVER_RANDOM_RANDEN_COPTS "${FLDSERVER_RANDOM_HWAES_ARM64_FLAGS}")
  elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    set(FLDSERVER_RANDOM_RANDEN_COPTS "${FLDSERVER_RANDOM_HWAES_ARM32_FLAGS}")
  else()
    message(WARNING "Value of CMAKE_SIZEOF_VOID_P (${CMAKE_SIZEOF_VOID_P}) is not supported.")
  endif()
else()
  message(WARNING "Value of CMAKE_SYSTEM_PROCESSOR (${CMAKE_SYSTEM_PROCESSOR}) is unknown and cannot be used to set FLDSERVER_RANDOM_RANDEN_COPTS")
  set(FLDSERVER_RANDOM_RANDEN_COPTS "")
endif()


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(FLDSERVER_DEFAULT_COPTS "${FLDSERVER_GCC_FLAGS}")
  set(FLDSERVER_TEST_COPTS "${FLDSERVER_GCC_FLAGS};${FLDSERVER_GCC_TEST_FLAGS}")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  # MATCHES so we get both Clang and AppleClang
  if(MSVC)
    # clang-cl is half MSVC, half LLVM
    set(FLDSERVER_DEFAULT_COPTS "${FLDSERVER_CLANG_CL_FLAGS}")
    set(FLDSERVER_TEST_COPTS "${FLDSERVER_CLANG_CL_FLAGS};${FLDSERVER_CLANG_CL_TEST_FLAGS}")
    set(FLDSERVER_DEFAULT_LINKOPTS "${FLDSERVER_MSVC_LINKOPTS}")
  else()
    set(FLDSERVER_DEFAULT_COPTS "${FLDSERVER_LLVM_FLAGS}")
    set(FLDSERVER_TEST_COPTS "${FLDSERVER_LLVM_FLAGS};${FLDSERVER_LLVM_TEST_FLAGS}")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      # AppleClang doesn't have lsan
      # https://developer.apple.com/documentation/code_diagnostics
      if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.5)
        set(FLDSERVER_LSAN_LINKOPTS "-fsanitize=leak")
        set(FLDSERVER_HAVE_LSAN ON)
      endif()
    endif()
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(FLDSERVER_DEFAULT_COPTS "${FLDSERVER_MSVC_FLAGS}")
  set(FLDSERVER_TEST_COPTS "${FLDSERVER_MSVC_FLAGS};${FLDSERVER_MSVC_TEST_FLAGS}")
  set(FLDSERVER_DEFAULT_LINKOPTS "${FLDSERVER_MSVC_LINKOPTS}")
else()
  message(WARNING "Unknown compiler: ${CMAKE_CXX_COMPILER}.  Building with no default flags")
  set(FLDSERVER_DEFAULT_COPTS "")
  set(FLDSERVER_TEST_COPTS "")
endif()

set(FLDSERVER_CXX_STANDARD "${CMAKE_CXX_STANDARD}")

