# Copyright (c) 2021,2022 The FLDServer project Authors. All rights reserved. Use of this
# source code is governed by a license that can be found in the LICENSE file.

add_subdirectory(base)
add_subdirectory(service)
add_subdirectory(landmark_detection)

add_executable(FLDServer "main.cc")
add_executable(Test "test.cc")

foreach(MODULE IN LISTS FLDSERVER_MODULES)
    target_link_libraries(FLDServer
            PUBLIC
            fldserver::${MODULE}
            )
    target_link_libraries(Test
            PUBLIC
            fldserver::${MODULE}
            )
endforeach()


if (FLDSERVER_ENABLE_INSTALL)
    install(TARGETS FLDServer EXPORT ${PROJECT_NAME}Targets
            RUNTIME DESTINATION ${FLDSERVER_INSTALL_BINDIR}
            )
    install(TARGETS Test EXPORT ${PROJECT_NAME}Targets
            RUNTIME DESTINATION ${FLDSERVER_INSTALL_BINDIR}
            )
endif ()  # FLDSERVER_ENABLE_INSTALL