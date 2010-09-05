include_directories(${GTKMM_INCLUDE_DIRS})

add_executable(vdeck main.cpp)
target_link_libraries(vdeck ${GTKMM_LIBRARIES})

