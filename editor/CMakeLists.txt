include_directories(${GTKMM_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
link_directories(${GTKMM_LIBRARY_DIRS})

add_library(vdeck_editor EditWindow.cpp EditWindow.hpp)

add_executable(vcard_gtk vcard_gtk.cpp)
target_link_libraries(vcard_gtk vdeck_editor vcards ${GTKMM_LIBRARIES})

vala_add_executable(vcard-edit
    vcard_edit.vala EditWindow.vala config.vala
    PACKAGES cardinal gtk+-2.0
    VAPI_DIRS ${CMAKE_BINARY_DIR}/parser
)

target_link_libraries(vcard-edit cardinal ${GLIB2_LIBRARIES} ${GTK_LIBRARIES})

install(TARGETS vcard_gtk RUNTIME DESTINATION bin)
