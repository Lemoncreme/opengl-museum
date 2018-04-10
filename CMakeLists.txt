project(OpenGLMuseum)
cmake_minimum_required(VERSION 3.11)

set (OpenGLMuseum_VERSION_MAJOR 1)
set (OpenGLMuseum_VERSION_MINOR 0)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(GLAD STATIC lib/glad.c lib/glad/glad.h)

# set(ZLIB_LIBRARIES zlibstatic)
# set(ENABLE_BOOST_WORKAROUND ON)
# set(BUILD_STATIC_LIB ON)
# set(BUILD_ASSIMP_TOOLS  ON)
# set(ASSIMP_BUILD_STATIC_LIB ON)

# add_subdirectory(lib/assimp-3.1.1)



add_executable(Museum src/museum.cpp)

