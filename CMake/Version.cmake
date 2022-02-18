# Copyright (c) 2021 The FLDServer project Authors. All rights reserved. Use of this
# source code is governed by a license that can be found in the LICENSE file.

macro(set_this_version)
    cmake_parse_arguments(VERSION
            "" # Options
            "MAJOR;MINOR;PATCH" # One val
            "NUMERIC;STAGE;TYPE" # Multi val
            ${ARGN}
            )


    set(FLDSERVER_VERSION_MAJOR ${VERSION_MAJOR})
    set(FLDSERVER_VERSION_MINOR ${VERSION_MINOR})
    set(FLDSERVER_VERSION_PATCH ${VERSION_PATCH})


    set(FLDSERVER_VERSION "${FLDSERVER_VERSION_MAJOR}.${FLDSERVER_VERSION_MINOR}.${FLDSERVER_VERSION_PATCH}")

    set(FLDSERVER_VERSION_NUMERIC           ${VERSION_NUMERIC})

    set(FLDSERVER_VERSION_STAGE ${VERSION_STAGE})
    set(FLDSERVER_VERSION_TYPE  ${VERSION_TYPE})

    set(ALLOWED_STAGE_NAMES
            alpha
            beta
            stable
            )
    set(ALLOWED_TYPE_NAMES
            dev
            release
            )

    list(FIND ALLOWED_STAGE_NAMES "${FLDSERVER_VERSION_STAGE}" STAGE_RET)
    list(FIND ALLOWED_TYPE_NAMES "${FLDSERVER_VERSION_TYPE}" TYPE_RET)

    if (STAGE_RET EQUAL -1)
        message(FATAL_ERROR "stage ${FLDSERVER_VERSION_STAGE} is not allowed")
    endif ()

    if(TYPE_RET EQUAL -1)
        message(FATAL_ERROR "type ${FLDSERVER_VERSION_TYPE} is not allowed")
    endif()

    if(FLDSERVER_VERSION_STAGE STREQUAL "stable")
        if(FLDSERVER_VERSION_TYPE STREQUAL "release")
            set(FLDSERVER_STABLE_RELEASE 1)
        endif()
    endif()

    set(FLDSERVER_PACKAGE_VERSION  "${FLDSERVER_VERSION_TYPE}-${FLDSERVER_VERSION_MAJOR}.${FLDSERVER_VERSION_MINOR}.${FLDSERVER_VERSION_PATCH}-${FLDSERVER_VERSION_STAGE}")

endmacro()