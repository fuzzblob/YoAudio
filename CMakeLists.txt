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
option(Warnings_As_Errors "Treat compiler warnings as errors" OFF)
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
#	Analysis Tools
#
##########################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# cppcheck
if(Code_Analysis)
	find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
	if (CMAKE_CXX_CPPCHECK)
		message(STATUS "Setting cppcheck flags.")
		# example cppcheck --enable=warning -Dfalse=0 --inconclusive --force --project=build_ninja/compile_commands.json --inline-suppr --suppressions-list=cppcheck-supressions.txt --xml --output-file=cppcheck-results.xml
		# --enable=all, checks for everything cppcheck knows about
		# -Dfalse=0, tell cppcheck that the boolean word 'false' equals the zero value
		# --force, prevents toomanyconfig errors by forcing cppcheck to run to the end of nested config lists
		# --project, specifices the json file created by MAKE_EXPORT_COMPILE_COMMANDS which outlines the code structure
		# --inline-suppr, uses inline 'cppcheck-suppress' comments to suppress specific checks
		# --suppressions-list, specifies this file, which suppresses check for certain file
		list( APPEND CMAKE_CXX_CPPCHECK
			"--enable=warning"
			"-Dfalse=0"
			"--inconclusive"
			"--force"
			# ignore slow input file
			#"-i${CMAKE_SOURCE_DIR}/editor/src/InputManager.cpp"
			#"--project=${CMAKE_SOURCE_DIR}/compile_commands.json"
			"--inline-suppr"
			"--suppressions-list=${CMAKE_SOURCE_DIR}/cppcheck-supressions.txt"
			"--xml"
			"--output-file=${CMAKE_SOURCE_DIR}/cppcheck-results.xml"
			)
	endif()
	find_program(CMAKE_CXX_CLANGTIDY NAMES cland-tidy)
	if(CMAKE_CXX_CLANGTIDY)
		message(STATUS "Setting clang-tidy command.")
		set(CMAKE_CXX_CLANG_TIDY
			clang-tidy;
			--config-file=.clang-tidy;
			-format-style='file';
			-header-filter=.; # -header-filter=${CMAKE_CURRENT_SOURCE_DIR};
			-checks=*;
			)
	endif()
endif()
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
# example usage: target_link_libraries(${PROJECT_NAME} OpenGL::GL ${SDL2_LIBRARY} )

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

message(STATUS "########## ${PROJECT_NAME} CMakeLists.txt end ##########")

message(STATUS "")
message(STATUS "###################################################################################################################")
message(STATUS "")
message(STATUS "				${PROJECT_NAME} configuration complete!")
message(STATUS "")
message(STATUS "###################################################################################################################")