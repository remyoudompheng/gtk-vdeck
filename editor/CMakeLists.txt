include_directories(${GTK_INCLUDE_DIRS} ${CMAKE_BINARY_DIR}/parser ${CMAKE_BINARY_DIR}/editor)
# add_library(vdeck_editor EditWindow.cpp EditWindow.hpp)

vala_add_library(vdeck_editor SHARED
                 EditWindow.vala config.vala
                 PACKAGES cardinal gtk+-2.0
                 GENERATE_VAPI vdeck_editor.vapi
                 GENERATE_HEADER vdeck_editor.h)

vala_add_executable(vcard_gtk vcard_gtk.vala
    PACKAGES cardinal vdeck_editor gtk+-2.0
    VAPI_DIRS ${CMAKE_BINARY_DIR}/parser ${CMAKE_BINARY_DIR}/editor
)

target_link_libraries(vcard_gtk vdeck_editor cardinal ${GLIB2_LIBRARIES} ${GTK_LIBRARIES})

install(TARGETS vcard_gtk RUNTIME DESTINATION bin)
