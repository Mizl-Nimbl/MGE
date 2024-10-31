set(LUALINKER_SOURCES
    lualinker.cpp
)

set(LUALINKER_HEADERS
    lualinker.hpp
)

add_library(lualinker ${LUALINKER_SOURCES} ${LUALINKER_HEADERS})

find_library(GLAD_LIBRARY glad PATHS /nix/store)

target_link_libraries(lualinker PUBLIC glm::glm glfw assimp ${GLAD_LIBRARY})

target_include_directories(lualinker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})