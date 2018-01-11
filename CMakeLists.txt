cmake_minimum_required(VERSION 3.6)
project(YoAudioEditor)

message(STATUS "Hello, we are now building ${PROJECT_NAME}")

# set CMAKE_BUILD_TYPE if empty
# for differentiation between debug and release builds.
if ( CMAKE_BUILD_TYPE STREQUAL "" )
	set(
		CMAKE_BUILD_TYPE
			"Debug"
		CACHE STRING
			"Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) \"Debug\" \"Release\" \"RelWithDebInfo\" \"MinSizeRel\"."
		FORCE
	)
endif ( CMAKE_BUILD_TYPE STREQUAL "" )

#
# Set up SDL2 dependency stuff
#
set(SDL2_SEARCH_PATHS ../YoAudio/dependencies/SDL2)
 
# SDL2 include
FIND_PATH( SDL2_INCLUDE_DIRS
	NAMES
		SDL.h
	PATHS
		${SDL2_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where SDL.h resides"
)

# SDL2 lib
FIND_LIBRARY( SDL2_LIBRARIES
	NAMES
		SDL2 SDL2main
	PATHS
		${SDL2_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x86
		lib/x64
	DOC
		"The SDL2 library"
)

# Check if we found it!
IF ( SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES )
	SET( SDL2_FOUND TRUE )
	MESSAGE(STATUS "Looking for SDL2 - found")
ELSE ( SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES  )
	SET( SDL2_FOUND FALSE )
	MESSAGE(STATUS "Looking for SDL2 - not found")
ENDIF ( SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES )

file(GLOB_RECURSE HDRS
	src/*.h
)

file(GLOB_RECURSE SRCS
	src/*.cpp
	src/*.c
)

include_directories(
	${SDL2_INCLUDE_DIRS}
)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin )

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

set_property(DIRECTORY ${PROJ_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
