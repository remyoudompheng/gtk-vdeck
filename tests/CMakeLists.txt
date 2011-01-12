vala_add_executable(tests tests.vala
    PACKAGES cardinal
    VAPI_DIRS ${CMAKE_BINARY_DIR}/parser)

target_link_libraries(tests cardinal ${GLIB_LIBRARIES})

add_test(NAME test COMMAND tests)

