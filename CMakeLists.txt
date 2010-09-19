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
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

Include(InstallRequiredSystemLibraries)
INCLUDE(CPack)

Include(FindPkgConfig)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)
pkg_check_modules(GTKMM REQUIRED gtkmm-2.4>=2.16)

find_program(XXD xxd)
find_program(SED sed)

SET(CMAKE_CXX_FLAGS "-Wall")
SET(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -Wextra -DDEBUG")

include_directories(${GLIBMM_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/parser
  ${CMAKE_SOURCE_DIR}/editor)
link_directories(${CMAKE_BINARY_DIR}/parser)

configure_file(config.h.cmake config.h)
add_subdirectory(parser)
add_subdirectory(src)
add_subdirectory(editor)
add_subdirectory(tests)
add_subdirectory(share)

