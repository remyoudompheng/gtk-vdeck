include_directories(${CMAKE_CURRENT_BINARY_DIR})

vala_add_library(cardinal STATIC
                 Vcard.vala Vdeck.vala VcardTypes.vala config.vala
                 GENERATE_VAPI cardinal.vapi
                 GENERATE_HEADER cardinal.h)

vala_add_executable(vdeck_query mutt-query.vala
                    PACKAGES cardinal
                    VAPI_DIRS ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(vdeck_query cardinal ${GLIB_LIBRARIES})

install(TARGETS vdeck_query RUNTIME DESTINATION bin)
