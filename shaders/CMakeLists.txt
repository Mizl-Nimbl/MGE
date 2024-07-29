set(SHADERS_SOURCES
    shader.cpp
)

set(SHADERS_HEADERS
    shader.hpp
)

add_library(shaders ${SHADERS_SOURCES} ${SHADERS_HEADERS})

find_library(GLAD_LIBRARY glad PATHS /nix/store)

target_link_libraries(shaders PUBLIC glm::glm glfw ${GLAD_LIBRARY})

target_include_directories(shaders PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})