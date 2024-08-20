set(AUDIOBANKBANK_SOURCES
    audiobank.cpp
)

set(AUDIOBANK_HEADERS
    audiobank.hpp
)

add_library(audiobank ${AUDIOBANK_SOURCES} ${AUDIOBANK_HEADERS})

find_library(GLAD_LIBRARY glad PATHS /nix/store)

target_link_libraries(audiobank PUBLIC glm::glm glfw ${GLAD_LIBRARY})

target_include_directories(audiobank PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})