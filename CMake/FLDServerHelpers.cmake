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
#    https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

include(CMakeParseArguments)
include(FLDServerConfigureCopts)
include(FLDServerDll)
include(FLDServerInstallDirs)

# The IDE folder for FLDServer that will be used if FLDServer is included in a CMake
# project that sets
#    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# For example, Visual Studio supports folders.
if(NOT DEFINED FLDSERVER_IDE_FOLDER)
  set(FLDSERVER_IDE_FOLDER FLDServer)
endif()

# fldserver_cc_library()
#
# CMake function to imitate Bazel's cc_library rule.
#
# Parameters:
# NAME: name of target (see Note)
# HDRS: List of public header files for the library
# SRCS: List of source files for the library
# DEPS: List of other libraries to be linked in to the binary targets
# COPTS: List of private compile options
# DEFINES: List of public defines
# LINKOPTS: List of link options
# PUBLIC: Add this so that this library will be exported under fldserver::
# Also in IDE, target will appear in FLDServer folder while non PUBLIC will be in FLDServer/internal.
# TESTONLY: When added, this target will only be built if user passes -DFLDSERVER_RUN_TESTS=ON to CMake.
#
# Note:
# By default, fldserver_cc_library will always create a library named fldserver_${NAME},
# and alias target fldserver::${NAME}.  The fldserver:: form should always be used.
# This is to reduce namespace pollution.
#
# fldserver_cc_library(
#   NAME
#     awesome
#   HDRS
#     "a.h"
#   SRCS
#     "a.cc"
# )
# fldserver_cc_library(
#   NAME
#     fantastic_lib
#   SRCS
#     "b.cc"
#   DEPS
#     fldserver::awesome # not "awesome" !
#   PUBLIC
# )
#
# fldserver_cc_library(
#   NAME
#     main_lib
#   ...
#   DEPS
#     fldserver::fantastic_lib
# )
#
# TODO: Implement "ALWAYSLINK"
function(fldserver_cc_library)
    cmake_parse_arguments(FLDSERVER_CC_LIB
            "DISABLE_INSTALL;PUBLIC;TESTONLY;OBJECT_LIB"
            "NAME"
            "HDRS;SRCS;COPTS;DEFINES;LINKOPTS;DEPS;OBJECT_DEPS"
            ${ARGN}
            )

    if(FLDSERVER_CC_LIB_TESTONLY AND NOT FLDSERVER_RUN_TESTS)
        return()
    endif()

    if(FLDSERVER_ENABLE_INSTALL)
        set(_NAME "${FLDSERVER_CC_LIB_NAME}")
    else()
        set(_NAME "fldserver_${FLDSERVER_CC_LIB_NAME}")
    endif()

    # Check if this is a header-only library
    set(FLDSERVER_CC_SRCS "${FLDSERVER_CC_LIB_SRCS}")
    # list(FILTER FLDSERVER_CC_SRCS EXCLUDE REGEX ".*\.(h|inc)")


    if("${FLDSERVER_CC_SRCS}" STREQUAL "")
        set(FLDSERVER_CC_LIB_IS_INTERFACE 1)
    else()
        set(FLDSERVER_CC_LIB_IS_INTERFACE 0)
    endif()

    if(BUILD_SHARED_LIBS)
        set(_build_type "shared")
    else()
        set(_build_type "static")
    endif()



    if(NOT FLDSERVER_CC_LIB_IS_INTERFACE)

        if(${_build_type} STREQUAL "static" OR ${_build_type} STREQUAL "shared")
            set(_OBJECT_LIBS)
            if (FLDSERVER_CC_LIB_OBJECT_LIB)
                add_library(${_NAME} OBJECT ${_OBJECT_LIBS})
            else()

                foreach(_M IN LISTS FLDSERVER_CC_LIB_OBJECT_DEPS)

                    list(APPEND _OBJECT_LIBS $<TARGET_OBJECTS:${_M}>)
                endforeach()
                add_library(${_NAME} "")
            endif ()

            target_sources(${_NAME} PRIVATE ${FLDSERVER_CC_LIB_SRCS} ${FLDSERVER_CC_LIB_HDRS} ${_OBJECT_LIBS})
            target_link_libraries(${_NAME}
                    PUBLIC ${FLDSERVER_CC_LIB_DEPS}
                    PRIVATE
                    ${FLDSERVER_CC_LIB_LINKOPTS}
                    ${FLDSERVER_DEFAULT_LINKOPTS}
                    )
            if (${_build_type} STREQUAL "shared")
                target_compile_definitions(
                        ${_NAME}
                        PRIVATE
                        EXPORTING_FLDSERVER
                        PUBLIC
                        FLDSERVER_BUILD_SHARED_LIBS
                )
            endif ()


        else()
            message(FATAL_ERROR "Invalid build type: ${_build_type}")
        endif()

        # Linker language can be inferred from sources, but in the case of DLLs we
        # don't have any .cc files so it would be ambiguous. We could set it
        # explicitly only in the case of DLLs but, because "CXX" is always the
        # correct linker language for static or for shared libraries, we set it
        # unconditionally.
        set_property(TARGET ${_NAME} PROPERTY LINKER_LANGUAGE "CXX")

        target_include_directories(${_NAME}
                PUBLIC
                "$<BUILD_INTERFACE:${FLDSERVER_COMMON_INCLUDE_DIRS}>"
                $<INSTALL_INTERFACE:${FLDSERVER_INSTALL_INCLUDEDIR}>
                )
        target_compile_options(${_NAME}
                PRIVATE ${FLDSERVER_CC_LIB_COPTS})
        target_compile_definitions(${_NAME} PUBLIC ${FLDSERVER_CC_LIB_DEFINES})

        # Add all FLDServer targets to a a folder in the IDE for organization.
        if(FLDSERVER_CC_LIB_PUBLIC)
            set_property(TARGET ${_NAME} PROPERTY FOLDER ${FLDSERVER_IDE_FOLDER})
        elseif(FLDSERVER_CC_LIB_TESTONLY)
            set_property(TARGET ${_NAME} PROPERTY FOLDER ${FLDSERVER_IDE_FOLDER}/test)
        else()
            set_property(TARGET ${_NAME} PROPERTY FOLDER ${FLDSERVER_IDE_FOLDER}/internal)
        endif()

        # INTERFACE libraries can't have the CXX_STANDARD property set
        set_property(TARGET ${_NAME} PROPERTY CXX_STANDARD ${FLDSERVER_CXX_STANDARD})
        set_property(TARGET ${_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

        # When being installed, we lose the fldserver_ prefix.  We want to put it back
        # to have properly named lib files.  This is a no-op when we are not being
        # installed.
        if(FLDSERVER_ENABLE_INSTALL)
            set_target_properties(${_NAME} PROPERTIES
                    OUTPUT_NAME "fldserver_${_NAME}"
                    )
        endif()
    else()
        # Generating header-only library
        add_library(${_NAME} INTERFACE)
        target_include_directories(${_NAME}
                INTERFACE
                "$<BUILD_INTERFACE:${FLDSERVER_COMMON_INCLUDE_DIRS}>"
                $<INSTALL_INTERFACE:${FLDSERVER_INSTALL_INCLUDEDIR}>
                )

        target_link_libraries(${_NAME}
                INTERFACE
                ${FLDSERVER_CC_LIB_DEPS}
                ${FLDSERVER_CC_LIB_LINKOPTS}
                ${FLDSERVER_DEFAULT_LINKOPTS}
                )
        target_compile_definitions(${_NAME} INTERFACE ${FLDSERVER_CC_LIB_DEFINES})
    endif()

    # TODO currently we don't install googletest alongside fldserver sources, so
    # installed fldserver can't be tested.
    if(NOT FLDSERVER_CC_LIB_OBJECT_LIB AND NOT FLDSERVER_CC_LIB_TESTONLY AND FLDSERVER_ENABLE_INSTALL)

        install(TARGETS ${_NAME} EXPORT ${PROJECT_NAME}Targets
                RUNTIME DESTINATION ${FLDSERVER_INSTALL_BINDIR}
                LIBRARY DESTINATION ${FLDSERVER_INSTALL_LIBDIR}
                ARCHIVE DESTINATION ${FLDSERVER_INSTALL_LIBDIR}
                )


        list(APPEND FLDSERVER_MODULES ${_NAME})
        list(REMOVE_DUPLICATES FLDSERVER_MODULES)
        set(FLDSERVER_MODULES "${FLDSERVER_MODULES}" CACHE INTERNAL "FLDSERVER_MODULES")


    endif()

    add_library(fldserver::${FLDSERVER_CC_LIB_NAME} ALIAS ${_NAME})
endfunction()


# fldserver_cc_test()
#
# CMake function to imitate Bazel's cc_test rule.
#
# Parameters:
# NAME: name of target (see Usage below)
# SRCS: List of source files for the binary
# DEPS: List of other libraries to be linked in to the binary targets
# COPTS: List of private compile options
# DEFINES: List of public defines
# LINKOPTS: List of link options
#
# Note:
# By default, fldserver_cc_test will always create a binary named fldserver_${NAME}.
# This will also add it to ctest list as fldserver_${NAME}.
#
# Usage:
# fldserver_cc_library(
#   NAME
#     awesome
#   HDRS
#     "a.h"
#   SRCS
#     "a.cc"
#   PUBLIC
# )
#
# fldserver_cc_test(
#   NAME
#     awesome_test
#   SRCS
#     "awesome_test.cc"
#   DEPS
#     fldserver::awesome
#     gmock
#     gtest_main
# )
function(fldserver_cc_test)
  if(NOT FLDSERVER_RUN_TESTS)
    return()
  endif()

  cmake_parse_arguments(FLDSERVER_CC_TEST
    ""
    "NAME"
    "SRCS;COPTS;DEFINES;LINKOPTS;DEPS"
    ${ARGN}
  )

  set(_NAME "fldserver_${FLDSERVER_CC_TEST_NAME}_test")

  add_executable(${_NAME} "")

  if (WIN32 AND MSVC)
      # MultiThreaded Compile with -MT or equivalent flag(s) to
      # use a multi-threaded statically-linked runtime library.
      set(__COPTS__ "/MT")
      target_compile_options(${_NAME}
              PRIVATE ${__COPTS__})
  endif ()

  target_sources(${_NAME} PRIVATE ${FLDSERVER_CC_TEST_SRCS})
  target_include_directories(${_NAME}
    PUBLIC ${FLDSERVER_COMMON_INCLUDE_DIRS}
    PRIVATE ${GMOCK_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS}
  )

  target_compile_definitions(${_NAME}
          PUBLIC
          ${FLDSERVER_CC_TEST_DEFINES}
          )
  target_compile_options(${_NAME}
    PRIVATE ${FLDSERVER_CC_TEST_COPTS}
  )

  target_link_libraries(${_NAME}
    PUBLIC ${FLDSERVER_CC_TEST_DEPS}
    PRIVATE ${FLDSERVER_CC_TEST_LINKOPTS}
  )
  # Add all FLDServer targets to a folder in the IDE for organization.
  set_property(TARGET ${_NAME} PROPERTY FOLDER ${FLDSERVER_IDE_FOLDER}/test)

  set_property(TARGET ${_NAME} PROPERTY CXX_STANDARD ${FLDSERVER_CXX_STANDARD})
  set_property(TARGET ${_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

  add_test(NAME ${_NAME} COMMAND ${_NAME})
endfunction()


function(check_target my_target)
  if(NOT TARGET ${my_target})
    message(FATAL_ERROR " FLDSERVER: compiling fldserver requires a ${my_target} CMake target in your project,
                   see CMake/README.md for more details")
  endif(NOT TARGET ${my_target})
endfunction()
