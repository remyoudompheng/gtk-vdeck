include_directories(${GTKMM_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(OUTPUT editor_xml.h
        COMMAND ${XXD} -i editor.xml | ${SED} 's/unsigned char/const char/' > ${CMAKE_CURRENT_BINARY_DIR}/editor_xml.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        MAIN_DEPENDENCY editor.xml)

add_library(vdeck_editor EditWindow.cpp EditWindow.hpp editor_xml.h)

add_executable(vcard_gtk vcard_gtk.cpp)
target_link_libraries(vcard_gtk vdeck_editor vcards ${GTKMM_LIBRARIES})

