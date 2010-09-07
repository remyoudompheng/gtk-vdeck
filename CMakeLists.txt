cmake_minimum_required(VERSION 2.8)

PROJECT(gtk-vdeck)

# =======================================================
# Information to update before to release this package.

# package version
SET(VDECK_MAJOR_VERSION "0")
SET(VDECK_MINOR_VERSION "0")
SET(VDECK_PATCH_VERSION "1")

Include(FindPkgConfig)
pkg_check_modules(GTKMM REQUIRED gtkmm-2.4>=2.16)

find_program(XXD xxd)
find_program(SED sed)

include_directories(${CMAKE_SOURCE_DIR}/parser)
link_directories(${CMAKE_BINARY_DIR}/parser)

add_subdirectory(parser)
add_subdirectory(src)
add_subdirectory(tests)

