cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(three_cpp)


# Ported from http://stackoverflow.com/questions/7787823/cmake-how-to-get-the-name-of-all-subdirectories-of-a-directory
# Obtain all subdirectories from current directory, recursively
macro( get_sub_directories result current_directory sub )
	list(APPEND ${result} ${sub} )
	
	file( GLOB children RELATIVE ${current_directory} ${current_directory}/* )

	if( "${sub}" STREQUAL "" )
		foreach( child ${children} )
			if( IS_DIRECTORY ${current_directory}/${child} )
				get_sub_directories( ${result} ${current_directory}/${child} ${child} )
			endif()
		endforeach()
	else()
		foreach( child ${children} )
			if( IS_DIRECTORY ${current_directory}/${child} )
				get_sub_directories( ${result} ${current_directory}/${child} ${sub}/${child} )
			endif()
		endforeach()
	endif()

endmacro()

# Set the necessarry flags for the C / C++ compiler
macro( set_cpp_configs )
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	set(CMAKE_CXX_FLAGS "-std=c++11 -w")

	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/" )
	set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules" )
endmacro()


# Find all the necessary packages
macro( find_packages ) 
	# Finding OpenGL library
	find_package(OpenGL REQUIRED)

	# Finding glfw library
	find_package(PkgConfig REQUIRED )
	pkg_search_module(GLFW REQUIRED glfw3)
	message(STATUS "Found glfw = ${GLFW_INCLUDE_DIRS}}")

	# Finding glm library
	find_package(GLM REQUIRED)

	# Finding Assimp library
	find_package( AssImp REQUIRED )

	# Finding FreeImage library
	find_package( FreeImage REQUIRED )

	# message ( "${OPENGL_LIBRARIES}")
	# message ( "${GLFW_LIBRARIES}")
	# message ( "${GLM_INCLUDE_DIRS}")
	# message ( "${Assimp_LIBRARIES}")
	# message ( "${FreeImage_LIBRARIES}")
endmacro()

set_cpp_configs()
find_packages()

add_subdirectory("Three.cpp Rev.2")