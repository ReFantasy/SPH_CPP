cmake_minimum_required(VERSION 3.21)
project(sph)

set(CMAKE_CXX_STANDARD 11)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )
link_libraries(${OPENGL_LIBRARIES})

#GLFW
include_directories("/usr/local/Cellar/glfw/3.3.6/include")
link_directories("/usr/local/Cellar/glfw/3.3.6/lib")
link_libraries(glfw)

#GLEW
include_directories("/usr/local/Cellar/glew/2.2.0_1/include")
link_directories("/usr/local/Cellar/glew/2.2.0_1/lib")
link_libraries(glew)

#GLM
include_directories("/usr/local/Cellar/glm/0.9.9.8/include")

add_executable(sph main.cpp particles.h particles.cpp glsl_shader.h sph.cpp sph.h kernel.cpp kernel.h)

