set(MESH_SOURCES
    mesh.cpp
)

set(MESH_HEADERS
    mesh.hpp
)

add_library(mesh ${MESH_SOURCES} ${MESH_HEADERS})

find_library(GLAD_LIBRARY glad PATHS /nix/store)

target_link_libraries(mesh PUBLIC glm::glm glfw assimp ${GLAD_LIBRARY})

target_include_directories(mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})