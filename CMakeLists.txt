cmake_minimum_required(VERSION 3.5...3.31)
# set project name
project(YoAudio)
# The version number.
set (YOA_VERSION_MAJOR 0)
set (YOA_VERSION_MINOR 0)
set (YOA_VERSION_PATCH 0)

message(STATUS "Now building the project for ${PROJECT_NAME} version ${YOA_VERSION_MAJOR}.${YOA_VERSION_MINOR}.${YOA_VERSION_PATCH}")

# specify UTF8 support
set(CMAKE_CXX_FLAGS "/utf-8")
set(CMAKE_C_FLAGS "/utf-8")

# set CMAKE_BUILD_TYPE if empty
# for differentiation between debug and release builds.
if(MSVC)
  add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic -Wnon-virtual-dtor -pedantic -g -std=c++20")
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
	set( SPDLOG_INCLUDE_DIRS engine/dependencies/spdlog/include )
	# Check if we found it!
	IF ( SPDLOG_INCLUDE_DIRS )
		SET (SPDLOG_FOUND true)
		MESSAGE(STATUS "Looking for sdpLog - found")
	ELSE ( SPDLOG_INCLUDE_DIRS )
		SET (SPDLOG_FOUND false)
		MESSAGE(STATUS "Looking for sdpLog - not found")
	ENDIF ( SPDLOG_INCLUDE_DIRS )
ELSE ( LOGGING_ENABLED )
	SET (SPDLOG_FOUND false)
	MESSAGE(STATUS "disabled sdpLog")
ENDIF ( LOGGING_ENABLED )

#
# Set up SDL2 dependency stuff
#

#set(SDL2_BUILDING_LIBRARY "true")
set(SDL2_CUSTOM_SEARCH_PATH engine/dependencies/SDL2/)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/engine/dependencies)
find_package(SDL2 REQUIRED)

# Check if we found it!
IF ( SDL2_INCLUDE_DIR AND SDL2_LIBRARY )
	SET( SDL2_FOUND true )
	MESSAGE(STATUS "Looking for SDL2 - found")
ELSE ( SDL2_INCLUDE_DIR AND SDL2_LIBRARY  )
	SET( SDL2_FOUND false )
	MESSAGE(STATUS "Looking for SDL2 includes at path: ${SDL2_INCLUDE_DIR}")
	MESSAGE(STATUS "Looking for SDL2 library at path: ${SDL2_LIBRARY}")
ENDIF ( SDL2_INCLUDE_DIR AND SDL2_LIBRARY )

file(GLOB_RECURSE HDRS
	engine/include/*.h
	engine/include/*.hpp
	engine/src/*.h
    engine/src/*.hpp
)

file(GLOB_RECURSE SRCS
	engine/src/*.cpp
	engine/src/*.c
)

include_directories(
	${PROJECT_SOURCE_DIR}/engine/include
	${SDL2_INCLUDE_DIR}
)
if ( SPDLOG_FOUND )
	include_directories(
		${SPDLOG_INCLUDE_DIRS}
	)
endif ( SPDLOG_FOUND )

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/engine/include/YoaConfig.h.in"
  "${PROJECT_SOURCE_DIR}/engine/include/YoaConfig.h"
  )

if (MSVC)
	add_definitions(-DBUILD_DLL)
	#set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin )
endif(MSVC)

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ../bin )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin )

add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})

if (MSVC)
    set_property(DIRECTORY ${PROJ_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif (MSVC)