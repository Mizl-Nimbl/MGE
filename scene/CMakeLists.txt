set(SCENE_SOURCES
    scene.cpp
)

set(SCENE_HEADERS
    scene.hpp
)

add_library(scene ${SCENE_SOURCES} ${SCENE_HEADERS})

find_library(GLAD_LIBRARY glad PATHS /nix/store)

target_link_libraries(scene PUBLIC glm::glm glfw ${GLAD_LIBRARY})

target_include_directories(scene PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})