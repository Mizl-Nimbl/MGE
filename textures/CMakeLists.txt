set(TEXTURES_SOURCES
    textures.cpp
)

set(TEXTURES_HEADERS
    textures.hpp
)

add_library(textures ${TEXTURES_SOURCES} ${TEXTURES_HEADERS})

find_library(GLAD_LIBRARY glad PATHS /nix/store)

target_link_libraries(textures PUBLIC glm::glm glfw ${GLAD_LIBRARY})

target_include_directories(textures PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})