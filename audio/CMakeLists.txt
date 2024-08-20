set(AUDIO_SOURCES
    audio.cpp
)

set(AUDIO_HEADERS
    audio.hpp
)

add_library(audio ${AUDIO_SOURCES} ${AUDIO_HEADERS})

find_library(GLAD_LIBRARY glad PATHS /nix/store)

target_link_libraries(audio PUBLIC glm::glm glfw ${GLAD_LIBRARY})

target_include_directories(audio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})