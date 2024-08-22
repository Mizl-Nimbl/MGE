set(FONT_SOURCES
    font.cpp
)

set(FONT_HEADERS
    font.hpp
)

add_library(font ${FONT_SOURCES} ${FONT_HEADERS})

find_library(GLAD_LIBRARY glad PATHS /nix/store)

target_link_libraries(font PUBLIC glm::glm glfw ${GLAD_LIBRARY})

target_include_directories(font PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})