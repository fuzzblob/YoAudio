cmake_minimum_required(VERSION 3.6)
# set project name
project(YoAudioEditor)
# The version number.
set (YOA_VERSION_MAJOR 0)
set (YOA_VERSION_MINOR 0)
set (YOA_VERSION_PATCH 0)
message(STATUS "Now building the project for ${PROJECT_NAME} version ${YOA_VERSION_MAJOR}.${YOA_VERSION_MINOR}.${YOA_VERSION_PATCH}")

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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -g -std=c++20")
  #-Wall -Wextra -Wpedantic -Wnon-virtual-dtor -pedantic
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
# Set up SDL2 dependency stuff
#

#set(SDL2_BUILDING_LIBRARY "true")
set(SDL2_CUSTOM_SEARCH_PATH "../engine/dependencies/SDL2/" "dependencies")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../engine/dependencies)
find_package(SDL2 REQUIRED)

# Check if we found it!
IF ( SDL2_INCLUDE_DIR AND SDL2_LIBRARY )
	SET( SDL2_FOUND true )
	MESSAGE(STATUS "Looking for SDL2 - found")
ELSE ( SDL2_INCLUDE_DIR AND SDL2_LIBRARY  )
	SET( SDL2_FOUND false )
	include(CMakePrintHelpers)
	cmake_print_variables()
	MESSAGE(STATUS "Looking for SDL2 - not found")
	MESSAGE(STATUS "Looking for SDL2 includes - ${SDL2_INCLUDE_DIR}")
	MESSAGE(STATUS "Looking for SDL2 library  - ${SDL2_LIBRARY}")
ENDIF ( SDL2_INCLUDE_DIR AND SDL2_LIBRARY )

#
# Set up YoAudio dependency stuff
#
set(YOA_SEARCH_PATHS ../engine/)

# YOA include
FIND_PATH( YOA_INCLUDE_DIRS
	NAMES
		YoAudio.h
	PATHS
		${YOA_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where YoAudio.h resides"
)

# YOA binaries
if(MSVC)
    FIND_PATH( YOA_LIBRARIES
	    NAMES
		    YoAudio.dll
	    PATHS
		    ${YOA_SEARCH_PATHS}
	    PATH_SUFFIXES
		    bin/Debug
	    DOC
		    "The YoAudio.dll file"
    )
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    FIND_PATH( YOA_LIBRARIES
	    NAMES
            libYoAudio.so
	    PATHS
		    ${YOA_SEARCH_PATHS}..
	    PATH_SUFFIXES
		    bin
	    DOC
		    "The libYoAudio.so file"
    )
endif()

# Check if we found it!
IF ( YOA_INCLUDE_DIRS AND YOA_LIBRARIES )
	SET( YOA_FOUND TRUE )
	MESSAGE(STATUS "Looking for YoAudio - found")
ELSE ( YOA_INCLUDE_DIRS AND YOA_LIBRARIES )
	SET( YOA_FOUND FALSE )
	MESSAGE(STATUS "Looking for YoAudio - not found" )
	MESSAGE(STATUS "Looking for YoAudio (includes) at path: " ${YOA_INCLUDE_DIRS} )
	MESSAGE(STATUS "Looking for YoAudio (library) at path: " ${YOA_LIBRARIES} )
ENDIF ( YOA_INCLUDE_DIRS AND YOA_LIBRARIES )

#
# Set up imgui dependency stuff
#
set(GUI_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/dependencies )

# imgui include
FIND_PATH( GUI_INCLUDE_DIRS
	NAMES
    	imgui.h
	PATHS
		${GUI_SEARCH_PATHS}
    PATH_SUFFIXES
		imgui
	DOC
		"The directory where imgui.h resides"
)

# Check if we found it!
IF ( GUI_INCLUDE_DIRS )
	SET( GUI_FOUND TRUE )
	MESSAGE(STATUS "Looking for imgui.h - found")
ELSE ( GUI_INCLUDE_DIRS )
	SET( GUI_FOUND FALSE )
	MESSAGE(STATUS "Looking for imgui.h in ${GUI_SEARCH_PATHS} - not found")
ENDIF ( GUI_INCLUDE_DIRS )

# gui impl include
FIND_PATH( GUIIMPL_INCLUDE_DIRS
	NAMES
        imgui_impl_sdl2.h   
	PATHS
		${GUI_SEARCH_PATHS}
	PATH_SUFFIXES
		imgui/backends
	DOC
		"The directory where imgui_impl_sdl2.h resides"
)

# Check if we found it!
IF ( GUIIMPL_INCLUDE_DIRS )
	SET( GUI_FOUND TRUE )
	MESSAGE(STATUS "Looking for imgui_impl_sdl2.h - found")
ELSE ( GUIIMPL_INCLUDE_DIRS )
	SET( GUI_FOUND FALSE )
	MESSAGE(STATUS "Looking for imgui_impl_sdl2.h - not found")
ENDIF ( GUIIMPL_INCLUDE_DIRS )
 
#
# input files for this project
#
file(GLOB_RECURSE HDRS
	src/*.h
	src/*.hpp
    dependencies/imgui/*.h
)

file(GLOB_RECURSE SRCS
	src/*.cpp
	src/*.c
)

set(CMAKE_INCLUDE_DIRECTORIES_BEFORE SYSTEM)
include_directories(
	${SDL2_INCLUDE_DIR}
	${YOA_INCLUDE_DIRS}
    ${GUI_INCLUDE_DIRS}
    ${GUIIMPL_INCLUDE_DIRS}
)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin )

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${YOA_LIBRARIES})

if(MSVC)
    set_property(DIRECTORY ${PROJ_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2_BINARIES}/SDL2.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)

    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${YOA_BINARIES}/YoAudio.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)

    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${YOA_BINARIES}/YoAudio.pdb" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)

    add_custom_command(
	    TARGET ${PROJECT_NAME}
	    PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets )
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    MESSAGE("TODO: contemplate copying assets to buid directory if needed?")
#    add_custom_command(
#	    TARGET ${PROJECT_NAME}
#	    PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets )
endif()
