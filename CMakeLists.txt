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

SET(PACKAGE_VERSION "0.2")

Include(InstallRequiredSystemLibraries)
INCLUDE(CPack)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeFiles)

Include(FindPkgConfig)
pkg_check_modules(GLIB REQUIRED gobject-2.0)
pkg_check_modules(GEE REQUIRED gee-1.0)
pkg_check_modules(GTK REQUIRED gtk+-2.0>=2.16)

Include(FindVala)
Include(UseVala)

SET(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")
#SET(VALA_COMPILE_FLAGS "--enable-experimental-non-null")
SET(VALA_COMPILE_FLAGS "--enable-checking")
SET(VALA_COMPILE_FLAGS_DEBUG "-X -ggdb3")

include_directories(${GLIB_INCLUDE_DIRS} ${CMAKE_BINARY_DIR}/parser)
link_directories(${CMAKE_BINARY_DIR}/parser ${GLIB_LIBRARY_DIRS})

configure_file(config.vala.cmake ${CMAKE_SOURCE_DIR}/parser/config.vala)

add_subdirectory(parser)
add_subdirectory(src)
add_subdirectory(editor)
#add_subdirectory(tests)
add_subdirectory(share)

