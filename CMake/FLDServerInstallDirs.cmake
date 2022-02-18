# Auto generated file by blitz 2021-04-22-20:11:59. blitz is a tool for generating cmake
# templates, written by (Mohammed Hussam Al Turjman, hussam.turjman@gmail.com). 
# For more information please visit https://github.com/Hussam-Turjman/LibraryTemplate

include(GNUInstallDirs)

# fldserver_VERSION is only set if we are an LTS release being installed, in which
# case it may be into a system directory and so we need to make subdirectories
# for each installed version of FLDServer.  This mechanism is implemented in
# FLDServer's internal Copybara (https://github.com/google/copybara) workflows and
# isn't visible in the CMake buildsystem itself.

if(fldserver_VERSION)
  set(FLDSERVER_SUBDIR "${PROJECT_NAME}_${PROJECT_VERSION}")
  set(FLDSERVER_INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}/${FLDSERVER_SUBDIR}")
  set(FLDSERVER_INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${FLDSERVER_SUBDIR}")
  set(FLDSERVER_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/${FLDSERVER_SUBDIR}")
  set(FLDSERVER_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}/${FLDSERVER_SUBDIR}")
else()
  set(FLDSERVER_INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}")
  set(FLDSERVER_INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
  set(FLDSERVER_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}")
  set(FLDSERVER_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}")
endif()

