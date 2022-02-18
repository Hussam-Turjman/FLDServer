# Auto generated file by blitz 2021-04-29-22:22:31. Blitz is a tool for generating cmake 
# templates, written by (Mohammed Hussam Al Turjman, hussam.turjman@gmail.com). 
# For more information please visit https://github.com/Hussam-Turjman/LibraryTemplate

# Copyright (c) 2021 The Blitz project Authors. All rights reserved. Use of this source 
# code is governed by a BSD-style license that can be found in the LICENSE file.

# Disable in-source builds to prevent source tree corruption.
if (" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR "FATAL: In-source builds are not allowed.
            You should create a separate directory for build files.")
endif ()

cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(Dummy LANGUAGES C CXX VERSION 0.0.0)

set(ABI_VERSION 0.0.0)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

set(Dummy_DEBUG_MODE 1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/source)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(BIN_DIR ${PROJECT_BINARY_DIR})
set(INSTALL_DIR ${CMAKE_INSTALL_PREFIX})

set(PROJECT_NAME_CAPITAL DUMMY)
set(LIBNAME "dummy")



# Control where the static and shared libraries are built so that on windows we don't need
# to tinker with the path to run the executable

if (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}/bin)
endif ()

if (NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
endif ()

if (NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}/archive)
endif ()


list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

include(arch)
include(compiler)


if(APPLE)
  set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
elseif(UNIX)
  set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()


# Basic compiler warning flags
# ---------------------------------------------------------------------------
add_library(dummy_compiler_flags INTERFACE)
target_compile_features(dummy_compiler_flags INTERFACE cxx_std_17)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(dummy_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

configure_file(DummyConfig.h.in ${INCLUDE_DIR}/DummyConfig.h)

# ---------------------------------------------------------------------------
# Create the library
add_library(${LIBNAME} STATIC ${SOURCE_DIR}/Foo/Foo.cc)
# State that anybody linking to us needs to include the current source dir
# to find the public headers, while we don't.
target_include_directories(${LIBNAME}
                        BEFORE PUBLIC
                        $<BUILD_INTERFACE:${INCLUDE_DIR}>
                        $<INSTALL_INTERFACE:include/${PROJECT_NAME}/>
                        )
target_include_directories(${LIBNAME}
                BEFORE PRIVATE
                $<BUILD_INTERFACE:${SOURCE_DIR}>
                $<INSTALL_INTERFACE:include/${PROJECT_NAME}/>
                )
target_link_libraries(${LIBNAME} PUBLIC dummy_compiler_flags)

# Define the symbol stating we are using the declspec(dllexport) when building on windows.
target_compile_definitions(${LIBNAME} PRIVATE "EXPORTING_DUMMY")
target_compile_definitions(${LIBNAME} PRIVATE "DLL_DUMMY")

# Setup the version numbering
set_property(TARGET ${LIBNAME} PROPERTY VERSION "${ABI_VERSION}")
set_property(TARGET ${LIBNAME} PROPERTY SOVERSION "${VERSION_MAJOR}")

# Install rules
set(installable_libs ${LIBNAME}  dummy_compiler_flags)
install(TARGETS ${installable_libs}
        DESTINATION lib
        EXPORT DummyTargets)

install(FILES include/Foo/Foo.h DESTINATION include/${PROJECT_NAME}/)
# ---------------------------------------------------------------------------

# Create the uninstall target.
# https://gitlab.kitware.com/cmake/community/wikis/FAQ#can-i-do-make-uninstall-with-cmake
if (NOT TARGET uninstall)
    configure_file(${PROJECT_SOURCE_DIR}/cmake/Uninstall.cmake.in
            ${BIN_DIR}/Uninstall.cmake
            @ONLY)

    add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${BIN_DIR}/Uninstall.cmake)
endif ()

install(FILES "${INCLUDE_DIR}/DummyConfig.h"
  DESTINATION include/${PROJECT_NAME}/
  )
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
include(CPack)

# Install the configuration targets
install(EXPORT DummyTargets
  FILE DummyTargets.cmake
  DESTINATION lib/cmake/Dummy
)

include(CMakePackageConfigHelpers)
# Generate the config file that includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/DummyConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/example"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )

# Generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/DummyConfigVersion.cmake"
  VERSION "${VERSION_MAJOR}.${VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

# Install the configuration file
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/DummyConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/DummyConfigVersion.cmake
  DESTINATION lib/cmake/Dummy
  )

# Generate the export targets for the build tree
# needs to be after the install(TARGETS ) command
export(EXPORT DummyTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/DummyTargets.cmake"
)
