cmake_minimum_required(VERSION 3.0)

project(NEST3D_CPP)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(OS_WINDOWS 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(OS_MACOSX 1)
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(LIB_ASSIMP "" CACHE PATH "Location of library assimp.")
set(LIB_GLEW "" CACHE PATH "Location of library glew.")

set(NEST_VERSION_MAJOR 1)
set(NEST_VERSION_MINOR 6)
set(NEST_VERSION_PATCH 0)

configure_file (
	${PROJECT_SOURCE_DIR}/Nest3dConfig.h.in
	${PROJECT_BINARY_DIR}/Nest3dConfig.h
)

add_subdirectory(source)