# Copyright (c) 2021 The FLDServer project Authors. All rights reserved. Use of this
# source code is governed by a license that can be found in the LICENSE file.

include(CMakeParseArguments)

macro(find_fldserver_package)
    cmake_parse_arguments(FLDSERVER_DEPENDENCY
            ""
            ${ARGN}
            )

    find_package(${ARGN})


    set(_temp)
    list(APPEND _temp ${ARGN} STOP)
    list(APPEND FLDSERVER_DEPENDENCIES ${_temp})
    unset(_temp)

endmacro()

macro(import_fldserver_library)
    cmake_parse_arguments(FLDSERVER_IMPORTED_LIBRARY
            ""
            "TARGET;TYPE;NAME_WIN;NAME_UNIX"
            ""
            ${ARGN} )

    if ("${FLDSERVER_IMPORTED_LIBRARY_TYPE}" STREQUAL "SHARED")
        elseif("${FLDSERVER_IMPORTED_LIBRARY_TYPE}" STREQUAL "STATIC")
        else()
        message(FATAL_ERROR "FLDSERVER_IMPORTED_LIBRARY_TYPE must be either SHARED or STATIC . But got : ${FLDSERVER_IMPORTED_LIBRARY_TYPE}")
    endif ()

    if ("${FLDSERVER_IMPORTED_LIBRARY_TARGET}" STREQUAL "")
        message(FATAL_ERROR "FLDSERVER_IMPORTED_LIBRARY_TARGET is empty")
    endif ()

    if ("${FLDSERVER_IMPORTED_LIBRARY_NAME_WIN}" STREQUAL "")
        message(FATAL_ERROR "FLDSERVER_IMPORTED_LIBRARY_NAME_WIN is empty")
    endif ()

    if ("${FLDSERVER_IMPORTED_LIBRARY_NAME_UNIX}" STREQUAL "")
        message(FATAL_ERROR "FLDSERVER_IMPORTED_LIBRARY_NAME_UNIX is empty")
    endif ()


    add_library("${FLDSERVER_IMPORTED_LIBRARY_TARGET}" "${FLDSERVER_IMPORTED_LIBRARY_TYPE}" IMPORTED)

    list(GET CMAKE_PREFIX_PATH 0  THIRD_PARTY_LIB_PATH)

    if (WIN32)
        set(_imported_lib_path "${THIRD_PARTY_LIB_PATH}/lib/${FLDSERVER_IMPORTED_LIBRARY_NAME_WIN}")
    else()
        set(_imported_lib_path "${THIRD_PARTY_LIB_PATH}/lib/${FLDSERVER_IMPORTED_LIBRARY_NAME_UNIX}")
    endif ()

    if (NOT EXISTS "${_imported_lib_path}")
        message(FATAL_ERROR "The following path does not exist : ${_imported_lib_path}")
    endif ()

    file(TO_CMAKE_PATH  "${_imported_lib_path}" _imported_lib_cmake_path)

    set_property(TARGET "${FLDSERVER_IMPORTED_LIBRARY_TARGET}" PROPERTY
            IMPORTED_LOCATION "${_imported_lib_path}")


    set(_temp)
    list(APPEND _temp ${FLDSERVER_IMPORTED_LIBRARY_TARGET} ${FLDSERVER_IMPORTED_LIBRARY_TYPE} ${FLDSERVER_IMPORTED_LIBRARY_NAME_WIN} ${FLDSERVER_IMPORTED_LIBRARY_NAME_UNIX} ${_imported_lib_cmake_path} STOP)
    list(APPEND FLDSERVER_IMPORTED_LIBS ${_temp})
    unset(_temp)
    unset(_imported_lib_path)
    unset(_imported_lib_cmake_path)

endmacro()
