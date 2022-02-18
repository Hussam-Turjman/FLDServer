# Auto generated file by blitz 2021-04-29-22:22:31. Blitz is a tool for generating cmake 
# templates, written by (Mohammed Hussam Al Turjman, hussam.turjman@gmail.com). 
# For more information please visit https://github.com/Hussam-Turjman/LibraryTemplate

# Copyright (c) 2021 The Blitz project Authors. All rights reserved. Use of this source 
# code is governed by a BSD-style license that can be found in the LICENSE file.

cmake_minimum_required(VERSION 3.17)
project(ImportTest)

find_package(Dummy REQUIRED)

message(STATUS "DUMMY_INCLUDE_DIR : ${DUMMY_INCLUDE_DIR}")
include_directories(BEFORE PRIVATE ${DUMMY_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME} ${DUMMY})
