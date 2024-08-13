set(MODEL_SOURCES
    model.cpp
)

set(MODEL_HEADERS
    model.hpp
)

add_library(model ${MODEL_SOURCES} ${MODEL_HEADERS})

find_library(GLAD_LIBRARY glad PATHS /nix/store)

target_link_libraries(model PUBLIC glm::glm glfw assimp ${GLAD_LIBRARY})

target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})