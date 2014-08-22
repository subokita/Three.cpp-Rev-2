# Include all the subdirectories, for the sake of the headers
get_sub_directories( sub_dirs ${CMAKE_CURRENT_SOURCE_DIR} "" )
foreach( dir ${sub_dirs} )
	include_directories( ${dir} )
endforeach()

#Include all the cpp files
file( GLOB_RECURSE THREE_SOURCES *.cpp )
file( GLOB_RECURSE THREE_HEADERS *.h )

#Copy all the header files into a folder
string( REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" THREE_HEADERS "${THREE_HEADERS}")
foreach( header_file ${THREE_HEADERS} )
	configure_file( ${header_file} ${CMAKE_BINARY_DIR}/includes/${header_file} COPYONLY)
endforeach()

add_library(three_cpp STATIC
	${THREE_SOURCES}
)

add_executable(main 
	main.cpp
)

target_link_libraries(main 
	${ALL_LIBS}
	${GLFW_LIBRARIES}
	${OPENGL_LIBRARIES}
	${Assimp_LIBRARIES}
	${FreeImage_LIBRARIES}
	three_cpp
)
