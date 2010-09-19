add_library(vcards STATIC
            VCard.cpp VCard.hpp
            VDeck.cpp VDeck.hpp
            StructuredText.cpp StructuredText.hpp)
target_link_libraries(vcards ${GLIBMM_LIBRARIES})