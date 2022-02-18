# Copyright (c) 2021 The FLDServer project Authors. All rights reserved. Use of this
# source code is governed by a license that can be found in the LICENSE file.

macro(print_info)
    message(STATUS "")
    message(STATUS "        ( ${PROJECT_NAME}  ${FLDSERVER_PACKAGE_VERSION}  )")
    message(STATUS "")
    message(STATUS "CMAKE_C_STANDARD:               ${CMAKE_C_STANDARD}")
    message(STATUS "CMAKE_C_STANDARD_REQUIRED       ${CMAKE_C_STANDARD_REQUIRED}")
    message(STATUS "CMAKE_CXX_STANDARD:             ${CMAKE_CXX_STANDARD}")
    message(STATUS "CMAKE_CXX_STANDARD_REQUIRED     ${CMAKE_CXX_STANDARD_REQUIRED}")
    message(STATUS "CMAKE_GENERATOR :               ${CMAKE_GENERATOR}")
    message(STATUS "CMAKE_BINARY_DIR:               ${CMAKE_BINARY_DIR}")
    message(STATUS "CMAKE_CURRENT_BINARY_DIR:       ${CMAKE_CURRENT_BINARY_DIR}")
    message(STATUS "CMAKE_SOURCE_DIR:               ${CMAKE_SOURCE_DIR}")
    message(STATUS "CMAKE_CURRENT_SOURCE_DIR:       ${CMAKE_CURRENT_SOURCE_DIR}")
    message(STATUS "PROJECT_BINARY_DIR:             ${PROJECT_BINARY_DIR}")
    message(STATUS "PROJECT_SOURCE_DIR:             ${PROJECT_SOURCE_DIR}")
    message(STATUS "CMAKE_MODULE_PATH:              ${CMAKE_MODULE_PATH}")
    message(STATUS "CMAKE_COMMAND:                  ${CMAKE_COMMAND}")
    message(STATUS "CMAKE_ROOT:                     ${CMAKE_ROOT}")
    message(STATUS "CMAKE_SYSTEM:                   ${CMAKE_SYSTEM}")
    message(STATUS "CMAKE_SYSTEM_NAME:              ${CMAKE_SYSTEM_NAME}")
    message(STATUS "CMAKE_SYSTEM_VERSION:           ${CMAKE_SYSTEM_VERSION}")
    message(STATUS "CMAKE_SYSTEM_PROCESSOR:         ${CMAKE_SYSTEM_PROCESSOR}")
    message(STATUS "CMAKE_SKIP_RPATH:               ${CMAKE_SKIP_RPATH}")
    message(STATUS "CMAKE_VERBOSE_MAKEFILE:         ${CMAKE_VERBOSE_MAKEFILE}")
    message(STATUS "CMAKE_BUILD_TYPE:               ${CMAKE_BUILD_TYPE}")
    message(STATUS "CMAKE_C_FLAGS:                  ${CMAKE_C_FLAGS}")
    message(STATUS "CMAKE_C_COMPILER:               ${CMAKE_C_COMPILER} (id ${CMAKE_C_COMPILER_ID})")
    message(STATUS "CMAKE_CXX_FLAGS:                ${CMAKE_CXX_FLAGS}")
    message(STATUS "CMAKE_CXX_COMPILER:             ${CMAKE_CXX_COMPILER} (id ${CMAKE_CXX_COMPILER_ID})")
    message(STATUS "CMAKE_SYSTEM_PROCESSOR :        ${CMAKE_SYSTEM_PROCESSOR}")

    message(STATUS "WINDOWS_EXPORT_ALL_SYMBOLS :    ${CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS}")
    message(STATUS "CMAKE_AR:                       ${CMAKE_AR}")
    message(STATUS "CMAKE_RANLIB:                   ${CMAKE_RANLIB}")
    message(STATUS "CMAKE_INSTALL_PREFIX :          ${CMAKE_INSTALL_PREFIX}")
    message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
    message(STATUS "CMAKE_PREFIX_PATH:              ${CMAKE_PREFIX_PATH}")
    message(STATUS "--------------------------------------------------------------------")
    message(STATUS "BUILD_SHARED_LIBS:              ${BUILD_SHARED_LIBS}")
    message(STATUS "FLDSERVER_ENABLE_INSTALL:       ${FLDSERVER_ENABLE_INSTALL}")
    message(STATUS "BUILD_DLL                       ${BUILD_DLL}")
    message(STATUS "FLDSERVER_DEFAULT_COPTS         ${FLDSERVER_DEFAULT_COPTS}")
    message(STATUS "--------------------------------------------------------------------")
    message(STATUS "Logical fldservers :                 ${NUMBER_OF_LOGICAL_FLDSERVERS}")
    message(STATUS "Physical fldservers :                ${NUMBER_OF_PHYSICAL_FLDSERVERS}")
    message(STATUS "Host :                          ${HOSTNAME}")
    message(STATUS "Total virtual memory :          ${TOTAL_VIRTUAL_MEMORY}")
    message(STATUS "Available virtual memory :      ${AVAILABLE_VIRTUAL_MEMORY}")
    message(STATUS "Total physical memory :         ${TOTAL_PHYSICAL_MEMORY}")
    message(STATUS "Available physical memory  :    ${AVAILABLE_PHYSICAL_MEMORY}")
    message(STATUS "Processor serial number :       ${PROCESSOR_SERIAL_NUMBER}")
    message(STATUS "Processor name :                ${PROCESSOR_NAME}")
    message(STATUS "Processor description :         ${PROCESSOR_DESCRIPTION}")
    message(STATUS "Is 64 bit :                     ${IS_64BIT}")
    message(STATUS "OS name :                       ${OS_NAME}")
    message(STATUS "OS release :                    ${OS_RELEASE}")
    message(STATUS "OS version :                    ${OS_VERSION}")
    message(STATUS "OS platform :                   ${OS_PLATFORM}")
    message(STATUS "")

endmacro()