cmake_minimum_required(VERSION 3.7.0)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(MGE VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(assimp REQUIRED)
find_package(tinyxml2 REQUIRED)
find_package(Freetype REQUIRED)
find_library(GLAD_LIBRARY glad PATHS /nix/store)

add_compile_options(-Wno-unused-result)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS} ${TinyXML2_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(input)
add_subdirectory(render)
add_subdirectory(shaders)
add_subdirectory(systems)
add_subdirectory(textures)
add_subdirectory(mesh)
add_subdirectory(model)
add_subdirectory(light)
add_subdirectory(scene)
add_subdirectory(audio)
add_subdirectory(audiobank)
add_subdirectory(signalemitter)
add_subdirectory(font)

include(CTest)
enable_testing()

add_executable(MGE "main.cpp")

set_property(TARGET MGE PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "~/Documents/MGE")

set(ZLIB_LIBRARIES zlibstatic)
set(ENABLE_BOOST_WORKAROUND ON)
set(BUILD_STATIC_LIB ON)

target_link_libraries(MGE OpenGL::GL)
target_link_libraries(MGE glm::glm)
target_link_libraries(MGE glfw)
target_link_libraries(MGE assimp)
target_link_libraries(MGE tinyxml2)
target_link_libraries(MGE Freetype::Freetype)
target_link_libraries(MGE ${GLAD_LIBRARY})
