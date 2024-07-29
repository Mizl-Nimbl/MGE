set(SYSTEMS_SOURCES
    systems.cpp
)

set(SYSTEMS_HEADERS
    systems.hpp
)

add_library(systems ${SYSTEMS_SOURCES} ${SYSTEMS_HEADERS})

find_library(GLAD_LIBRARY glad PATHS /nix/store)

target_link_libraries(systems PUBLIC glm::glm glfw ${GLAD_LIBRARY})

target_include_directories(systems PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})