# Auto generated file by blitz 2021-04-29-22:22:31. Blitz is a tool for generating cmake 
# templates, written by (Mohammed Hussam Al Turjman, hussam.turjman@gmail.com). 
# For more information please visit https://github.com/Hussam-Turjman/LibraryTemplate

# Copyright (c) 2021 The Blitz project Authors. All rights reserved. Use of this source 
# code is governed by a BSD-style license that can be found in the LICENSE file.

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
