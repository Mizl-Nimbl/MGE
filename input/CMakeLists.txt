set(INPUT_SOURCES
    input.cpp
)

set(INPUT_HEADERS
    input.hpp
)

add_library(input ${INPUT_SOURCES} ${INPUT_HEADERS})

find_library(GLAD_LIBRARY glad PATHS /nix/store)

target_link_libraries(input PUBLIC glm::glm glfw ${GLAD_LIBRARY})

target_include_directories(input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})