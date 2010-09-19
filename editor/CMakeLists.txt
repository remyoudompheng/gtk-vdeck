include_directories(${GTKMM_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
link_directories(${GTKMM_LIBRARY_DIRS})

add_library(vdeck_editor EditWindow.cpp EditWindow.hpp)

add_executable(vcard_gtk vcard_gtk.cpp)
target_link_libraries(vcard_gtk vdeck_editor vcards ${GTKMM_LIBRARIES})

install(TARGETS vcard_gtk RUNTIME DESTINATION bin)
