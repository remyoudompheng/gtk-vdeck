cmake_minimum_required(VERSION 2.8)

PROJECT(gtk-vdeck)

# =======================================================
# Information to update before to release this package.

# package version
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A viewer/editor for managing vCards")
SET(CPACK_PACKAGE_VENDOR "Rémy Oudompheng")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "1")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

IF(WIN32 AND NOT UNIX)
   SET(CPACK_GENERATOR "NSIS")
   SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/test.bmp")
ENDIF(WIN32 AND NOT UNIX)

INCLUDE(CPack)

Include(FindPkgConfig)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)
pkg_check_modules(GTKMM REQUIRED gtkmm-2.4>=2.16)

find_program(XXD xxd)
find_program(SED sed)

SET(CMAKE_CXX_FLAGS "-Wall")

include_directories(${GLIBMM_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/parser
  ${CMAKE_SOURCE_DIR}/editor)
link_directories(${CMAKE_BINARY_DIR}/parser)

add_subdirectory(parser)
add_subdirectory(src)
add_subdirectory(editor)
add_subdirectory(tests)

