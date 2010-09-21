cmake_minimum_required(VERSION 2.6)

PROJECT(gtk-vdeck)

# =======================================================
# Information to update before to release this package.

# package version
SET(CPACK_GENERATOR "ZIP" "TGZ")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A viewer/editor for managing vCards")
SET(CPACK_PACKAGE_VENDOR "Rémy Oudompheng")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "1")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

SET(PACKAGE_VERSION "0.1.1")


Include(InstallRequiredSystemLibraries)
INCLUDE(CPack)

Include(FindPkgConfig)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)
pkg_check_modules(GTKMM gtkmm-3.0)
if(NOT GTKMM_FOUND)
 pkg_check_modules(GTKMM gtkmm-2.4>=2.16)
endif()

# Use VS2008 compatible gtkmm libraries
if(WIN32)
set(GLIBMM_LIBRARIES "glibmm-vc90-2_4;sigc-vc90-2_0;glib-2.0")
set(GTKMM_LIBRARIES "gtkmm-vc90-2_4;gtk-win32-2.0;atkmm-vc90-1_6;glibmm-vc90-2_4;sigc-vc90-2_0")
endif(WIN32)

link_directories(${CMAKE_BINARY_DIR}/parser ${GLIBMM_LIBRARY_DIRS})

SET(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")

include_directories(${GLIBMM_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/parser
  ${CMAKE_SOURCE_DIR}/editor)

configure_file(config.h.cmake config.h)
add_subdirectory(parser)
if(GTKMM_FOUND)
add_subdirectory(src)
add_subdirectory(editor)
endif(GTKMM_FOUND)
add_subdirectory(tests)
add_subdirectory(share)

