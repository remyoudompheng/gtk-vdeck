include_directories(${GTK_INCLUDE_DIRS} ${CMAKE_BINARY_DIR}/parser)
# add_library(vdeck_editor EditWindow.cpp EditWindow.hpp)

vala_add_executable(vcard_gtk
    vcard_gtk.vala EditWindow.vala config.vala
    PACKAGES cardinal gtk+-2.0
    VAPI_DIRS ${CMAKE_BINARY_DIR}/parser
)

target_link_libraries(vcard_gtk cardinal ${GLIB2_LIBRARIES} ${GTK_LIBRARIES})

install(TARGETS vcard_gtk RUNTIME DESTINATION bin)
