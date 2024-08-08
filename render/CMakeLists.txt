set(RENDER_SOURCES
    render.cpp
)

set(RENDER_HEADERS
    render.hpp
)

add_library(render ${RENDER_SOURCES} ${RENDER_HEADERS})

find_library(GLAD_LIBRARY glad PATHS /nix/store)

target_link_libraries(render PUBLIC glm::glm glfw assimp ${GLAD_LIBRARY})

target_include_directories(render PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})