cmake_minimum_required(VERSION 3.6)
# set project name
project(YoAudio)
# The version number.
set (YOA_VERSION_MAJOR 0)
set (YOA_VERSION_MINOR 0)

message(STATUS "Now building the project for ${PROJECT_NAME} version ${YOA_VERSION_MAJOR} ${YOA_VERSION_MINOR}")

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

# should we allow logging?
option (SPDLOG_ENABLED 
        "enable logging" ON) 

IF ( SPDLOG_ENABLED )
	set( SPDLOG_INCLUDE_DIRS dependencies/spdlog/include )
	# Check if we found it!
	IF ( SPDLOG_INCLUDE_DIRS )
		SET( SPDLOG_ENABLED true )
		MESSAGE(STATUS "Looking for sdpLog - found")
		include_directories(
			include
			${SPDLOG_INCLUDE_DIRS}
		)
	ELSE ( SPDLOG_INCLUDE_DIRS )
		SET( SPDLOG_ENABLED false )
		MESSAGE(STATUS "Looking for sdpLog - not found")
	ENDIF ( SPDLOG_INCLUDE_DIRS )
ELSE ( SPDLOG_ENABLED )
	SET( SPDLOG_ENABLED false )
	MESSAGE(STATUS "disabled sdpLog")
ENDIF ( SPDLOG_ENABLED )

#
# Set up SDL2 dependency stuff
#

# SDL2 include headers
FIND_PATH( SDL2_INCLUDE_DIRS
	NAMES
		SDL.h
	PATHS
		# TODO: find path recursively instead of per version number
		dependencies/SDL2/
		dependencies/SDL2/SDL2-2.0.9/
        /usr/local/opt/sdl2/include/SDL2
	PATH_SUFFIXES
		include
	DOC
		"The directory where SDL.h resides"
)

# SDL2 development library
FIND_LIBRARY( SDL2_LIBRARIES
	NAMES
		SDL2 SDL2main
	PATHS
		# TODO: find path recursively instead of per version number
		dependencies/SDL2/
		dependencies/SDL2/SDL2-2.0.9/
        /usr/local/opt/sdl2/lib
	PATH_SUFFIXES
		lib
		lib/x86
		lib64
		lib/x64
	DOC
		"The SDL2 library"
)

# Check if we found it!
IF ( SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES )
	SET( SDL2_FOUND true )
	MESSAGE(STATUS "Looking for SDL2 - found")
ELSE ( SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES  )
	SET( SDL2_FOUND false )
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
)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/include/YoaConfig.h.in"
  "${PROJECT_SOURCE_DIR}/include/YoaConfig.h"
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
