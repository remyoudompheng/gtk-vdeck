include_directories(${GEE_INCLUDE_DIRS} ${GTK_INCLUDE_DIRS}
                    ${CMAKE_BINARY_DIR}/parser ${CMAKE_BINARY_DIR}/editor)
link_directories(${CMAKE_BINARY_DIR}/editor ${GTK_LIBRARY_DIRS})

vala_add_executable(vdeck vdeck.vala
    MainWindow.vala DeckView.vala
    PACKAGES cardinal vdeck_editor gtk+-2.0 gee-1.0
    VAPI_DIRS ${CMAKE_BINARY_DIR}/parser ${CMAKE_BINARY_DIR}/editor
)
target_link_libraries(vdeck vdeck_editor cardinal ${GTK_LIBRARIES} ${GEE_LIBRARIES})

install(TARGETS vdeck RUNTIME DESTINATION bin)
