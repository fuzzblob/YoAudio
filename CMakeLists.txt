cmake_minimum_required(VERSION 3.5...3.31)
# The version number.
set (YOA_VERSION_MAJOR 0)
set (YOA_VERSION_MINOR 1)
set (YOA_VERSION_PATCH 0)
project("YoAudio" VERSION "${YOA_VERSION_MAJOR}.${YOA_VERSION_MINOR}.${YOA_VERSION_PATCH}" DESCRIPTION "YoAudio Meta project")


##########################
#
#		Options
#
##########################

option(BUILD_LIBRARY "Enable building library" ON)
# option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_EDITOR "Enable building editor" ON)
option(STARTUP_IN_EDITOR "Startup in Editor Project" ON)

# should we allow logging?
option (LOGGING_ENABLED "enable logging" ON) 

message(STATUS "###################################################################################################################")
message(STATUS "")
message(STATUS "Now building the project for ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "")
message(STATUS "###################################################################################################################")

##########################
#
#		Compiler
#
##########################

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

##########################
#
#	Dependencies
#
##########################

#
# Set up SDL2 dependency stuff
#

#set(SDL2_BUILDING_LIBRARY "true")
set(SDL2_CUSTOM_SEARCH_PATH engine/dependencies/SDL2/)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/engine/dependencies)
MESSAGE(STATUS "Looking for SDL2 (SDL2_CUSTOM_SEARCH_PATH = ${SDL2_CUSTOM_SEARCH_PATH})...")
find_package(SDL2 REQUIRED)

# Check if we found it!
IF ( SDL2_INCLUDE_DIR AND SDL2_LIBRARY )
	SET( SDL2_FOUND true )
ELSE ( SDL2_INCLUDE_DIR AND SDL2_LIBRARY  )
	SET( SDL2_FOUND false )
	MESSAGE(STATUS "Looking for SDL2 includes at path: ${SDL2_INCLUDE_DIR}")
	MESSAGE(STATUS "Looking for SDL2 library at path: ${SDL2_LIBRARY}")
ENDIF ( SDL2_INCLUDE_DIR AND SDL2_LIBRARY )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )

if(BUILD_LIBRARY)
    add_subdirectory(engine)
	set(STARTUP_PROJECT ${EGINE_NAME})
endif()

if(BUILD_EDITOR)
    add_subdirectory(editor)
	if (${STARTUP_IN_EDITOR})
		if(${EDITOR_NAME} STREQUAL "")
			MESSAGE (STATUS "EDITOR_NAME seems unset. cant set startup project to Editor.")
		else()
			MESSAGE (STATUS "setting editor project (${EDITOR_NAME})...")
			set(STARTUP_PROJECT ${EDITOR_NAME})
		endif()
	endif()
endif()

if(MSVC)
	if(NOT ${STARTUP_PROJECT} STREQUAL "")
		set(STARTUP_DIR ${CMAKE_CURRENT_SOURCE_DIR})
		MESSAGE(STATUS "setting ${STARTUP_PROJECT} as startup project (${STARTUP_DIR})")
		set_property(DIRECTORY ${STARTUP_DIR} PROPERTY VS_STARTUP_PROJECT ${STARTUP_PROJECT})
		# set_property(DIRECTORY ${PROJ_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
		# set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	else()
		MESSAGE(STATUS "STARTUP_PROJECT (${STARTUP_PROJECT}) variable not set!")
		MESSAGE(STATUS "( ${EDITOR_NAME} / ${ENGINE_NAME} )")
		MESSAGE(STATUS "( ${PROJ_SOURCE_DIR} / ${CMAKE_CURRENT_SOURCE_DIR} )")
	endif()
endif()

MESSAGE("YoAudio CMakeLists.txt end")