cmake_minimum_required(VERSION 3.6)
project(YoAudio)

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
# Set up spdlog dependency stuff
#

set( SPDLOG_INCLUDE_DIRS dependencies/spdlog/include )

# Check if we found it!
IF ( SPDLOG_INCLUDE_DIRS )
	#SET( _FOUND TRUE )
	MESSAGE(STATUS "Looking for sdpLog - found")
ELSE ( SPDLOG_INCLUDE_DIRS )
	#SET( _FOUND FALSE )
	MESSAGE(STATUS "Looking for sdpLog - not found")
ENDIF ( SPDLOG_INCLUDE_DIRS )

#
# Set up SDL2 dependency stuff
#

# SDL2 include
FIND_PATH( SDL2_INCLUDE_DIRS
	NAMES
		SDL.h
	PATHS
		dependencies/SDL2
        /usr/local/opt/sdl2/include/SDL2
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
		dependencies/SDL2
        /usr/local/opt/sdl2/lib
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
	include/*.h
	src/*.h
)

file(GLOB_RECURSE SRCS
	src/*.cpp
	src/*.c
)

include_directories(
	include
	${SDL2_INCLUDE_DIRS}
	${SPDLOG_INCLUDE_DIRS}
)

add_definitions(-DBUILD_DLL)

#set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../lib )
#set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ../lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin )

add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

set_property(DIRECTORY ${PROJ_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
