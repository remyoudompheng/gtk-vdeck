add_executable(test test.cc)
add_executable(test_split test_split.cc)

target_link_libraries(test vcards ${GLIBMM_LIBRARIES})
target_link_libraries(test_split vcards ${GLIBMM_LIBRARIES})
