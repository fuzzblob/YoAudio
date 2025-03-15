cmake_minimum_required(VERSION 3.8...3.31)
# The version number.
set (YOA_VERSION_MAJOR 0)
set (YOA_VERSION_MINOR 2)
set (YOA_VERSION_PATCH 2)
project("YoAudio" VERSION "${YOA_VERSION_MAJOR}.${YOA_VERSION_MINOR}.${YOA_VERSION_PATCH}" DESCRIPTION "YoAudio Meta project")
##########################
#
#		Options
#
##########################
# Meta Project
set(FORCE_SDL true) # "force the use of SDL2 backend (no other options are implemented at the moment)"
option(Code_Analysis "do code analysis in MS VS" OFF)
option(Warnings_As_Errors "Treat compiler warnings as errors" ON)
# option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
# Editor:
option(BUILD_EDITOR "Enable building editor" ON)
option(STARTUP_IN_EDITOR "Startup in Editor Project" ON)
set(EDITOR_FRAME_RATE 60)			# "Editor rendering frame rate"
# Engine:
option(BUILD_LIBRARY "Enable building library" ON)
# should we allow logging?
set (LOGGING_ENABLED true)			# "enable logging"
set (AUDIO_THREAD_UPDATES false)	# "enable audio thread updates"
set (TICK_RATE 60)					# "audio thread tick rate"
set (MAX_VOICES 0)					# "Maximum audio voices (unlimited when below 1)"
set (TARGET_SAMPLERATE 48000)		# "default audio output sample rate"
set (TARGET_CHANNELS 2)				# "default audio output channel count"
set (TARGET_BUFFER 4096)			# "default audio output buffer length"
set (TARGET_BITDEPTH 16)			# "default audio output bit depth"
# beging project setup
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
# set C++17 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# specify UTF8 support
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "/utf-8")
set(CMAKE_C_FLAGS "/utf-8")
# For more warning options see: https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md
if(MSVC)
	# Microsoft Visual Studio specifics
	add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
	# Force to always compile with W4
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive-")
	endif()
	# /Wall is possible thanks to VS2022 enabling to exclude external headers from it
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive- /w14640 /Wall /external:anglebrackets /external:W0")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 /w14296 /w14311 /w14545")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w14546 /w14547 /w14549 /w14555 /w14619 /w14640 /w14826 /w14905 /w14906 /w14928")
	# if(Warnings_As_Errors)
		# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
	# endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	# Compiler warnings
	# Update if necessary
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wnon-virtual-dtor -pedantic -g -std=c++20")
	if(Warnings_As_Errors)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
	endif() 
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
# set(SDL2_BUILDING_LIBRARY false)
set(SDL2_CUSTOM_SEARCH_PATH ${PROJECT_SOURCE_DIR}/dependencies/SDL2/)
#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/dependencies)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/dependencies ${SDL2_CUSTOM_SEARCH_PATH})
message(STATUS "Looking for SDL2 (SDL2_CUSTOM_SEARCH_PATH = ${SDL2_CUSTOM_SEARCH_PATH})...")
find_package(SDL2 REQUIRED)
# target_link_libraries(${PROJECT_NAME} OpenGL::GL ${SDL2_LIBRARY} )


# Check if we found it!
IF ( SDL2_INCLUDE_DIR AND SDL2_LIBRARY )
	SET( SDL2_FOUND true )
ELSE ( SDL2_INCLUDE_DIR AND SDL2_LIBRARY  )
	SET( SDL2_FOUND false )
	message(STATUS "Looking for SDL2 includes at path: ${SDL2_INCLUDE_DIR}")
	message(STATUS "Looking for SDL2 library at path: ${SDL2_LIBRARY}")
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
			message(STATUS "EDITOR_NAME seems unset. cant set startup project to Editor.")
		else()
			set(STARTUP_PROJECT ${EDITOR_NAME})
		endif()
	endif()
endif()

if(MSVC)
	if(NOT ${STARTUP_PROJECT} STREQUAL "")
		set(STARTUP_DIR ${CMAKE_CURRENT_SOURCE_DIR})
		message(STATUS "setting startup project to `${STARTUP_PROJECT}`")
		set_property(DIRECTORY ${STARTUP_DIR} PROPERTY VS_STARTUP_PROJECT ${STARTUP_PROJECT})
	else()
		message(STATUS "STARTUP_PROJECT (${STARTUP_PROJECT}) variable not set!")
		message(STATUS "editor project name: ${EDITOR_NAME}")
		message(STATUS "engine project name: ${ENGINE_NAME}")
		message(STATUS "PROJ_SOURCE_DIR: ${PROJ_SOURCE_DIR}")
		message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
	endif()
endif()

message(STATUS "YoAudio CMakeLists.txt end")
message(STATUS " ")