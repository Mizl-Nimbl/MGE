set(LIGHT_SOURCES
    light.cpp
)

set(LIGHT_HEADERS
    light.hpp
)

add_library(light ${LIGHT_SOURCES} ${LIGHT_HEADERS})

find_library(GLAD_LIBRARY glad PATHS /nix/store)

target_link_libraries(light PUBLIC glm::glm glfw assimp ${GLAD_LIBRARY})

target_include_directories(light PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})