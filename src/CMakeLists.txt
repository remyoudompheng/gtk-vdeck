include_directories(${GTKMM_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR}/editor)
link_directories(
${CMAKE_BINARY_DIR}/editor
${GTKMM_LIBRARY_DIRS})

add_executable(vdeck
  main.cpp
  MainWindow.cpp MainWindow.hpp
  ListView.cpp ListView.hpp)
target_link_libraries(vdeck vdeck_editor cardinal vcards ${GTKMM_LIBRARIES})

install(TARGETS vdeck RUNTIME DESTINATION bin)
