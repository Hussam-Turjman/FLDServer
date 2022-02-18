# Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
# source code is governed by a license that can be found in the LICENSE file.


# Disable in-source builds to prevent source tree corruption.
if (" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "FATAL: In-source builds are not allowed.
            You should create a separate directory for build files.")
endif ()

# Most widely used distributions have cmake 3.5 or greater available as of March
# 2019.  A notable exception is RHEL-7 (CentOS7).  You can install a current
# version of CMake by first installing Extra Packages for Enterprise Linux
# (https://fedoraproject.org/wiki/EPEL#Extra_Packages_for_Enterprise_Linux_.28EPEL.29)
# and then issuing `yum install cmake3` on the command line.
cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

# Compiler id for Apple Clang is now AppleClang.
if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif (POLICY CMP0025)

# if command can use IN_LIST
if (POLICY CMP0057)
    cmake_policy(SET CMP0057 NEW)
endif (POLICY CMP0057)

# Project version variables are the empty string if version is unspecified
if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

# option() honor variables
if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif (POLICY CMP0077)


project(fldserver LANGUAGES C CXX)

if (WIN32)
    add_compile_definitions(_HAS_DEPRECATED_RESULT_OF=1)
    add_compile_definitions(NOMINMAX)
    add_compile_definitions(UNICODE)
    add_compile_definitions(WIN32_LEAN_AND_MEAN)
    add_compile_definitions(CERT_CHAIN_PARA_HAS_EXTRA_FIELDS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)
    if (MSVC)
        # MultiThreaded Compile with -MT or equivalent flag(s) to
        # use a multi-threaded statically-linked runtime library.
        add_compile_options(/MT)
    endif ()
endif ()

if (UNIX)
    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mavx2 -mfma")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -mfma")
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# Output directory is correct by default for most build setups. However, when
# building FLDServer as a DLL, it is important to have the DLL in the same
# directory as the executable using it. Thus, we put all executables in a single
# /bin directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Control where the static and shared libraries are built so that on windows we don't need
# to tinker with the path to run the executable

if (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif ()

if (NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif ()

if (NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/archive)
endif ()


# when fldserver is included as subproject (i.e. using add_subdirectory(FLDServer))
# in the source tree of a project that uses it, install rules are disabled.
if (NOT "^${CMAKE_SOURCE_DIR}$" STREQUAL "^${PROJECT_SOURCE_DIR}$")
    option(FLDSERVER_ENABLE_INSTALL "Enable install rule" OFF)
else ()
    option(FLDSERVER_ENABLE_INSTALL "Enable install rule" ON)
endif ()

option(BUILD_DLL "Build a single shared library of all fldserver modules. This library has the alias name fldserver::fldserver_dll" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries instead of static" OFF)
option(FLDSERVER_RUN_TESTS "If ON, FLDServer tests will be run." OFF)
option(ADD_NET_MODULE "Add net module" OFF)

if (BUILD_SHARED_LIBS)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # Position-independent-code
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    endif ()
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        # Position-independent-code
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    endif ()
    set(FLDSERVER_LIBS_TYPE "shared")
else ()
    set(FLDSERVER_LIBS_TYPE "static")
endif ()

list(APPEND CMAKE_MODULE_PATH
        ${CMAKE_CURRENT_LIST_DIR}/CMake
        ${CMAKE_CURRENT_LIST_DIR}/fldserver/copts
        )

include(CMakePackageConfigHelpers)
include(FLDServerInstallDirs)
include(DecentPackage)
include(FLDServerDll)
include(FLDServerHelpers)
include(Arch)
include(Compiler)
include(Version)

set_this_version(
        MAJOR 0
        MINOR 0
        PATCH 0
        NUMERIC 0x00000000
        TYPE dev
        STAGE alpha)

set(FLDSERVER_DEPENDENCIES)
set(FLDSERVER_MODULES)
set(FLDSERVER_IMPORTED_LIBS)

# Add CPack support
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${FLDSERVER_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${FLDSERVER_VERSION_MINOR}")
include(CPack)

configure_file(fldserver_config.h.in ${PROJECT_SOURCE_DIR}/fldserver/fldserver_config.h)


##
## Using fldserver targets
##
## all public fldserver targets are
## exported with the fldserver:: prefix
##
## e.g fldserver::module_a fldserver::module_b  ....
##
## DO NOT rely on the internal targets outside of the prefix


# include current path
list(APPEND FLDSERVER_COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(FLDSERVER_USING_CLANG ON)
else ()
    set(FLDSERVER_USING_CLANG OFF)
endif ()

# Find dependencies

## pthread
find_package(Threads REQUIRED)

if (WIN32)
    find_fldserver_package(Dummy REQUIRED)
else ()
    find_package(Dummy REQUIRED)
endif ()

include_directories(BEFORE PUBLIC ${DUMMY_INCLUDE_DIR})

# OpenCV
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# DLib
find_package(dlib REQUIRED)

# Boost libs
find_package( Boost 1.5.9 COMPONENTS filesystem system)
message(STATUS "Boost targets : ${Boost_LIBRARIES}")

# OpenFace
find_package(OpenFace REQUIRED)
include_directories(BEFORE PUBLIC ${OpenFace_INCLUDE_DIRS})


## Abseil
find_package(absl REQUIRED)

## Libevent
find_package(Libevent REQUIRED)
message(STATUS "Libevent targets : ${LIBEVENT_LIBRARY}")

# SQLite3
find_package (SQLite3 REQUIRED)
message(STATUS "SQLite3 targets : ${SQLite3_LIBRARIES}")

# OpenSSL
find_package(OpenSSL REQUIRED)
message(STATUS "OpenSSL targets : ${OPENSSL_LIBRARIES}")

# ZLIB
find_package(ZLIB REQUIRED)
message(STATUS "ZLIB targets : ${ZLIB_LIBRARIES}")

# LibXml2
find_package(LibXml2 REQUIRED)
message(STATUS "LibXml2 targets : ${LIBXML2_LIBRARIES}")

# Protobuf
find_package(Protobuf REQUIRED)
message(STATUS "Protobuf targets : ${Protobuf_LIBRARIES}")


## ICU
find_package(ICU REQUIRED  COMPONENTS uc dt in io)
message(STATUS "ICU : ${ICU_LIBRARIES}")
message(STATUS "ICU include : ${ICU_INCLUDE_DIRS}")
include_directories(BEFORE PUBLIC ${ICU_INCLUDE_DIRS})

if (${FLDSERVER_RUN_TESTS})

    # enable CTest.  This will set BUILD_TESTING to ON unless otherwise specified
    # on the command line
    include(CTest)

    enable_testing()
    find_package(GTest REQUIRED)
    include_directories(BEFORE PRIVATE ${GTEST_INCLUDE_DIR})


    list(APPEND FLDSERVER_TEST_COMMON_LIBRARIES
            GTest::gtest_main
            GTest::gtest
            GTest::gmock
            Threads::Threads
            )
endif ()


add_subdirectory(fldserver)

if (FLDSERVER_ENABLE_INSTALL)

    # install as a subdirectory only
    install(EXPORT ${PROJECT_NAME}Targets
            NAMESPACE fldserver::
            DESTINATION "${FLDSERVER_INSTALL_CONFIGDIR}"
            )

    configure_package_config_file(
            CMake/fldserverConfig.cmake.in
            "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
            INSTALL_DESTINATION "${FLDSERVER_INSTALL_CONFIGDIR}"
    )
    install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
            DESTINATION "${FLDSERVER_INSTALL_CONFIGDIR}"
            )

    # FLDServer only has a version in stable releases.
    if (FLDSERVER_STABLE_RELEASE)
        write_basic_package_version_file(
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
                VERSION "${FLDSERVER_VERSION}"
                COMPATIBILITY ExactVersion
        )

        install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
                DESTINATION "${FLDSERVER_INSTALL_CONFIGDIR}"
                )
    endif ()  # fldserver_VERSION

    if (UNIX)
        install(DIRECTORY fldserver
                DESTINATION ${FLDSERVER_INSTALL_INCLUDEDIR}
                FILES_MATCHING
                PATTERN "*.inc"
                PATTERN "*.h"
                PATTERN "tests" EXCLUDE
                PATTERN "copts" EXCLUDE
                PATTERN "testdata" EXCLUDE
                PATTERN "win" EXCLUDE
                )
    elseif (WIN32)
        install(DIRECTORY fldserver
                DESTINATION ${FLDSERVER_INSTALL_INCLUDEDIR}
                FILES_MATCHING
                PATTERN "*.inc"
                PATTERN "*.h"
                PATTERN "tests" EXCLUDE
                PATTERN "copts" EXCLUDE
                PATTERN "testdata" EXCLUDE
                PATTERN "posix" EXCLUDE
                )
    else ()
        message(FATAL_ERROR "Unsupported platform")
    endif ()

endif ()  # FLDSERVER_ENABLE_INSTALL

# Create the uninstall target.
# https://gitlab.kitware.com/cmake/community/wikis/FAQ#can-i-do-make-uninstall-with-cmake
if (NOT TARGET uninstall)
    configure_file(${PROJECT_SOURCE_DIR}/CMake/Uninstall.cmake.in
            ${PROJECT_BINARY_DIR}/Uninstall.cmake
            @ONLY)

    add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${PROJECT_BINARY_DIR}/Uninstall.cmake)
endif ()


include(PrintInfo)
print_info()

