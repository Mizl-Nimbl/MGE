set(PHYSICS_SOURCES
    physics.cpp
)

set(PHYSICS_HEADERS
    physics.hpp
)

add_library(physics ${PHYSICS_SOURCES} ${PHYSICS_HEADERS})

find_library(GLAD_LIBRARY glad PATHS /nix/store)

target_link_libraries(physics PUBLIC glm::glm glfw assimp ${GLAD_LIBRARY})

target_include_directories(physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})