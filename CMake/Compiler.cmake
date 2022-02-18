# Copyright (c) 2021 The FLDServer project Authors. All rights reserved. Use of this
# source code is governed by a license that can be found in the LICENSE file.

include(CheckCCompilerFlag)

# source : https://github.com/libevent/libevent
macro(add_compiler_flags)
    foreach(flag ${ARGN})
        string(REGEX REPLACE "[-.+/:= ]" "_" _flag_esc "${flag}")

        check_c_compiler_flag("${flag}" check_c_compiler_flag_${_flag_esc})

        if (check_c_compiler_flag_${_flag_esc})
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
        endif()
    endforeach()
endmacro()
