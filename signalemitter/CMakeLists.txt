set(SIGNALEMITTER_SOURCES
    signalemitter.cpp
)

set(SIGNALEMITTER_HEADERS
    signalemitter.hpp
)

add_library(signalemitter ${SIGNALEMITTER_SOURCES} ${SIGNALEMITTER_HEADERS})

find_library(GLAD_LIBRARY glad PATHS /nix/store)

target_link_libraries(signalemitter PUBLIC glm::glm glfw assimp ${GLAD_LIBRARY})

target_include_directories(signalemitter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})