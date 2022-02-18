# Auto generated file by blitz 2021-04-22-20:11:59. blitz is a tool for generating cmake
# templates, written by (Mohammed Hussam Al Turjman, hussam.turjman@gmail.com). 
# For more information please visit https://github.com/Hussam-Turjman/LibraryTemplate

include(CMakeParseArguments)

set(FLDSERVER_DLL_FILES
        "module_a/Foo.h"
        "module_a/Foo.cc"
        "module_a/internal/FooInternal.h"
        "module_a/internal/FooInternal.cc"
        "module_b/Boo.h"
        "module_b/Boo.cc"
        "module_b/internal/BooInternal.cc"
        )


set(FLDSERVER_DLL_EXTERNAL_TARGETS
        "yuv"
        "vpx"
        "Threads::Threads"
        )


function(fldserver_make_dll)
    message(FATAL_ERROR "Not implemented yet")
    
    list(APPEND FLDSERVER_MODULES fldserver_dll)
    list(REMOVE_DUPLICATES FLDSERVER_MODULES)
    set(FLDSERVER_MODULES "${FLDSERVER_MODULES}" CACHE INTERNAL "FLDSERVER_MODULES")

    add_library(
            fldserver_dll
            SHARED
            "${FLDSERVER_DLL_FILES}"
    )
    target_link_libraries(
            fldserver_dll
            PRIVATE
            ${FLDSERVER_DEFAULT_LINKOPTS}
    )

    target_link_libraries(fldserver_dll
            PUBLIC
            ${FLDSERVER_DLL_EXTERNAL_TARGETS}
            )

    set_property(TARGET fldserver_dll PROPERTY LINKER_LANGUAGE "CXX")
    target_include_directories(
            fldserver_dll
            PUBLIC
            "$<BUILD_INTERFACE:${FLDSERVER_COMMON_INCLUDE_DIRS}>"
            $<INSTALL_INTERFACE:${FLDSERVER_INSTALL_INCLUDEDIR}>
    )

    target_compile_options(
            fldserver_dll
            PRIVATE
            ${FLDSERVER_DEFAULT_COPTS}
    )

    target_compile_definitions(
            fldserver_dll
            PRIVATE
            EXPORTING_FLDSERVER
            PUBLIC
            FLDSERVER_BUILD_DLL
            INTERFACE
            ${FLDSERVER_CC_LIB_DEFINES}
    )
    install(TARGETS fldserver_dll EXPORT ${PROJECT_NAME}Targets
            RUNTIME DESTINATION ${FLDSERVER_INSTALL_BINDIR}
            LIBRARY DESTINATION ${FLDSERVER_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${FLDSERVER_INSTALL_LIBDIR}
            )
endfunction()

