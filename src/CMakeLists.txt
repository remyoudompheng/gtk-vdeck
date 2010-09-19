include_directories(${GTKMM_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(vdeck
  main.cpp
  MainWindow.cpp MainWindow.hpp
  ListView.cpp ListView.hpp)
target_link_libraries(vdeck vdeck_editor vcards ${GTKMM_LIBRARIES})

install(TARGETS vdeck RUNTIME DESTINATION bin)
