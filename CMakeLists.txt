project(OpenGLMuseum)
cmake_minimum_required(VERSION 3.11)

set (OpenGLMuseum_VERSION_MAJOR 1)
set (OpenGLMuseum_VERSION_MINOR 0)

set(INCLUDE "./include")
set(SOURCE "./src")
set(LIB "./lib")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

add_executable(Museum src/museum.cpp)

include_directories(${INCLUDE} ${SOURCE} ${LIB})

# GLAD
set(GLAD_DIR "${LIB}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(Museum PRIVATE "${GLAD_DIR}/include")
target_link_libraries(Museum "glad" "${CMAKE_DL_LIBS}")



