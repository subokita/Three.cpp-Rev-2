# Find Assimp includes and library
#
# This module defines
#  Assimp_INCLUDE_DIRS
#  Assimp_LIBRARIES, the libraries to link against to use Assimp.
#  Assimp_LIBRARY_DIRS, the location of the libraries
#  Assimp_FOUND, If false, do not try to use Assimp
#
# Copyright Â© 2007, Matt Williams
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

IF (Assimp_LIBRARIES AND Assimp_INCLUDE_DIRS)
	SET(Assimp_FIND_QUIETLY TRUE) # Already in cache, be silent
ELSE(Assimp_LIBRARIES AND Assimp_INCLUDE_DIRS)
	MESSAGE(STATUS "Looking for Assimp")
ENDIF (Assimp_LIBRARIES AND Assimp_INCLUDE_DIRS)

SET(Assimp_INCLUDE_SEARCH_DIRS
	${Assimp_LIBRARY_SEARCH_DIRS}
	${CMAKE_LIBRARY_PATH}
	/usr/include
	/usr/local/include
	/usr/local/include/assimp
	/opt/include
	/opt/assimp/include
)

SET(Assimp_LIBRARY_SEARCH_DIRS
	${Assimp_LIBRARY_SEARCH_DIRS}
	${CMAKE_LIBRARY_PATH}
	/usr/lib
	/usr/local/lib
	/opt/lib
	/opt/assimp/lib
)

FIND_PATH(Assimp_INCLUDE_DIRS ai_assert.h ${Assimp_INCLUDE_SEARCH_DIRS})
FIND_LIBRARY(Assimp_LIBRARIES NAMES assimp PATHS ${Assimp_LIBRARY_SEARCH_DIRS})

#Do some preparation
SEPARATE_ARGUMENTS(Assimp_INCLUDE_DIRS)
SEPARATE_ARGUMENTS(Assimp_LIBRARIES)

MARK_AS_ADVANCED(Assimp_INCLUDE_DIRS Assimp_LIBRARIES Assimp_LIBRARY_DIRS)

IF (Assimp_INCLUDE_DIRS AND Assimp_LIBRARIES)
	SET(Assimp_FOUND TRUE)
ENDIF (Assimp_INCLUDE_DIRS AND Assimp_LIBRARIES)

IF (Assimp_FOUND)
	IF (NOT Assimp_FIND_QUIETLY)
		MESSAGE(STATUS "  libraries : ${Assimp_LIBRARIES} from ${Assimp_LIBRARY_DIRS}")
		MESSAGE(STATUS "  includes  : ${Assimp_INCLUDE_DIRS}")
	ENDIF (NOT Assimp_FIND_QUIETLY)
ELSE (Assimp_FOUND)
	IF (Assimp_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could not find Assimp")
	ENDIF (Assimp_FIND_REQUIRED)
ENDIF (Assimp_FOUND)