project(opengl-museum)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(GLAD STATIC glad.c glad/glad.h)

set(ZLIB_LIBRARIES zlibstatic)
set(ENABLE_BOOST_WORKAROUND ON)
set(BUILD_STATIC_LIB ON)
set(BUILD_ASSIMP_TOOLS  ON)
set(ASSIMP_BUILD_STATIC_LIB ON)

add_subdirectory(assimp-3.1.1)



add_executable(Main main.cpp)

