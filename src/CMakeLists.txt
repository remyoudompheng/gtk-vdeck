include_directories(${GTKMM_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR}/editor)
link_directories(
${CMAKE_BINARY_DIR}/editor
${GTKMM_LIBRARY_DIRS})

add_executable(vdeck
  main.cpp
  MainWindow.cpp MainWindow.hpp
  ListView.cpp ListView.hpp)
target_link_libraries(vdeck vdeck_editor cardinal vcards ${GTKMM_LIBRARIES})

vala_add_executable(vdeck2 vdeck.vala
    MainWindow.vala DeckView.vala
    PACKAGES cardinal vdeck_editor gtk+-2.0
    VAPI_DIRS ${CMAKE_BINARY_DIR}/parser ${CMAKE_BINARY_DIR}/editor
)
target_link_libraries(vdeck2 vdeck_editor cardinal ${GTK_LIBRARIES})

install(TARGETS vdeck RUNTIME DESTINATION bin)
