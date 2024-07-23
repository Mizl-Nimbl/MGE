cmake_minimum_required(VERSION 3.7.0)
project(MGE VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 3.3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)

include(CTest)
enable_testing()

add_executable(MGE "main.cpp" "glad.c")

set_property(TARGET MGE PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "~/Documents/MGE")

set(ZLIB_LIBRARIES zlibstatic)
set(ENABLE_BOOST_WORKAROUND ON)
set(BUILD_STATIC_LIB ON)

target_link_libraries(MGE OpenGL::GL)
target_link_libraries(MGE glm::glm)
target_link_libraries(MGE glfw)