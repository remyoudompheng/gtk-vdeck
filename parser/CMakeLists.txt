add_library(vcards STATIC
            VCard.cpp VCard.hpp
            VDeck.cpp VDeck.hpp
            StructuredText.cpp StructuredText.hpp)
target_link_libraries(vcards ${GLIBMM_LIBRARIES})

add_executable(vdeck_query mutt-query.cc)
target_link_libraries(vdeck_query vcards ${GLIBMM_LIBRARIES})

install(TARGETS vdeck_query RUNTIME DESTINATION bin)
