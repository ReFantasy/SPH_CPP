cmake_minimum_required(VERSION 3.21)
project(sph)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_BUILD_TYPE Release)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )
link_libraries(${OPENGL_LIBRARIES})

IF (WIN32)
    include_directories("F:/ProjectCode/Simulation/third_party")
    SET(THIRD_LIBS_DIR F:/ProjectCode/Simulation/third_party/WIN/)
	#GLFW
	include_directories("${THIRD_LIBS_DIR}/glfw-3.3.5.bin.WIN64/include")
    link_directories("${THIRD_LIBS_DIR}/glfw-3.3.5.bin.WIN64/lib-vc2019")
	link_libraries(glfw3_mt)
	
	#GLEW
	include_directories("${THIRD_LIBS_DIR}/glew-2.1.0/include")
    link_directories("${THIRD_LIBS_DIR}/glew-2.1.0/lib/Release/x64")
	link_libraries(glew32s)# #define GLEW_STATIC on top of your code to use the static lib
	
ELSEIF (APPLE)
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
ELSEIF (UNIX)
ENDIF ()

add_executable(sph main.cpp particles.h particles.cpp glsl_shader.h sph.cpp sph.h kernel.cpp kernel.h)

