include_directories(${GTKMM_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(OUTPUT vdeck_xml.h
        COMMAND ${XXD} -i vdeck.xml | ${SED} 's/unsigned char/const char/' > ${CMAKE_CURRENT_BINARY_DIR}/vdeck_xml.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        MAIN_DEPENDENCY vdeck.xml)

add_executable(vdeck
  main.cpp vdeck_xml.h
  MainWindow.cpp MainWindow.hpp
  ListView.cpp ListView.hpp)
target_link_libraries(vdeck vdeck_editor vcards ${GTKMM_LIBRARIES})

