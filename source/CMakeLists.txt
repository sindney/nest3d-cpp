include_directories(${PROJECT_SOURCE_DIR}/include)

include_directories(${LIB_ASSIMP}/include)
link_directories(${LIB_ASSIMP}/lib)

include_directories(${LIB_GLEW}/include)
link_directories(${LIB_GLEW}/lib)

if(OS_MACOSX)
	find_package(OpenGL REQUIRED)
	include_directories(${OPENGL_INCLUDE_DIR})
endif()

file(GLOB NEST_SRC ${PROJECT_SOURCE_DIR} "*.cpp")

if(BUILD_SHARED_LIBS)
	add_library(nest SHARED ${NEST_SRC})
	if(OS_WINDOWS)
		target_link_libraries(nest glew32 assimp opengl32)
	elseif(OS_MACOSX)
		target_link_libraries(nest glew assimp ${OPENGL_LIBRARIES})
		set_target_properties(nest PROPERTIES BUILD_WITH_INSTALL_RPATH 1 INSTALL_NAME_DIR "@executable_path")
	endif()
else()
	add_library(nest STATIC ${NEST_SRC})
endif()

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION .)
install(FILES ${PROJECT_BINARY_DIR}/NestConfig.h DESTINATION include)
install(TARGETS nest DESTINATION lib)