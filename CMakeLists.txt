cmake_minimum_required(VERSION 3.5)
# set project name
project(YoAudio)
# The version number.
set (YOA_VERSION_MAJOR 0)
set (YOA_VERSION_MINOR 0)

message(STATUS "Now building the project for ${PROJECT_NAME} version ${YOA_VERSION_MAJOR} ${YOA_VERSION_MINOR}")

# set CMAKE_BUILD_TYPE if empty
# for differentiation between debug and release builds.
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -g -std=c++14")
endif()

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
option (LOGGING_ENABLED 
        "enable logging" ON) 

IF ( LOGGING_ENABLED )
	set( SPDLOG_INCLUDE_DIRS dependencies/spdlog/include )
	# Check if we found it!
	IF ( SPDLOG_INCLUDE_DIRS )
		SET( LOGGING_ENABLED true )
		MESSAGE(STATUS "Looking for sdpLog - found")
		include_directories(
			include
			${SPDLOG_INCLUDE_DIRS}
		)
	ELSE ( SPDLOG_INCLUDE_DIRS )
		SET( LOGGING_ENABLED false )
		MESSAGE(STATUS "Looking for sdpLog - not found")
	ENDIF ( SPDLOG_INCLUDE_DIRS )
ELSE ( LOGGING_ENABLED )
	SET( LOGGING_ENABLED false )
	MESSAGE(STATUS "disabled sdpLog")
ENDIF ( LOGGING_ENABLED )

#
# Set up SDL2 dependency stuff
#

# SDL2 include headers
FIND_PATH( SDL2_INCLUDE_DIRS
	NAMES
		SDL.h
	PATHS
		# TODO: find path recursively instead of per version number
		dependencies/SDL2/include/
		# Mac OS (when using Homebrew)
        /usr/local/opt/sdl2/include/SDL2
        # Linux (when using apt)
        /usr/inculde/SDL2/
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
		# Mac OS (when using Homebrew)
        /usr/local/opt/sdl2/lib
        # Linux (when using apt)
        /usr/bin/
        /usr/share/
	PATH_SUFFIXES
        # TODO: detect architecture to link against
		lib
		lib/x86
		lib64
		lib/x64
	DOC
		"The SDL2 library"
)

set(SDL2_BUILDING_LIBRARY "true")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/dependencies)

# Linux using "/usr/lib/i386-linux-gnu/cmake/SDL2/sdl2-config.cmake"
# TODO: possible on Windows and Mac OS?
if ( SDL2_INCLUDE_DIRS STREQUAL "SDL2_INCLUDE_DIRS-NOTFOUND" )
	find_package(SDL2 REQUIRED)
	string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
endif ( SDL2_INCLUDE_DIRS STREQUAL "SDL2_INCLUDE_DIRS-NOTFOUND" )

# Check if we found it!
IF ( SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES )
	SET( SDL2_FOUND true )
	MESSAGE(STATUS "Looking for SDL2 - found")
ELSE ( SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES  )
	SET( SDL2_FOUND false )
	MESSAGE(STATUS "Looking for SDL2 includes - ${SDL2_INCLUDE_DIRS}")
	MESSAGE(STATUS "Looking for SDL2 library  - ${SDL2_LIBRARIES}")
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
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ../lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin )

add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

set_property(DIRECTORY ${PROJ_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
