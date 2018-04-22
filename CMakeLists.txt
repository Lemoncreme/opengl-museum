# Project Config
project(OpenGLMuseum)
cmake_minimum_required(VERSION 3.11)
set(OpenGLMuseum_VERSION_MAJOR 1)
set(OpenGLMuseum_VERSION_MINOR 0)
set(CMAKE_BUILD_TYPE Debug)

# Directory vars
set(INCLUDE "./include")
set(SOURCE "./src")
set(LIB "./lib")
set(RESOURCE "./resource")

# Required packages
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(assimp REQUIRED)
find_package(glm REQUIRED)

# Package includes
include_directories(${ASSIMP_INCLUDE_DIR} ${GLFW_INCLUDE_DIRS})

# Executable
add_executable(Museum src/museum.cpp)

# Include paths
include_directories(${INCLUDE} ${SOURCE} ${LIB} ${RESOURCE})

# GLAD
set(GLAD_DIR "${LIB}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(Museum PRIVATE "${GLAD_DIR}/include")
target_link_libraries(Museum "glad" "${CMAKE_DL_LIBS}")

# Link other libs
target_link_libraries(Museum ${ASSIMP_LIBRARIES} ${GLFW_LIBRARIES})
