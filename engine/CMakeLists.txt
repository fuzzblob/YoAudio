cmake_minimum_required(VERSION 3.5...3.31)
# set project name
project(YoAudio_Library)

message(STATUS "Now building the project for ${PROJECT_NAME} version ${YOA_VERSION_MAJOR}.${YOA_VERSION_MINOR}.${YOA_VERSION_PATCH}")

#
# Set up spdlog dependency stuff
#

IF ( LOGGING_ENABLED )
	FIND_PATH( SPDLOG_INCLUDE_DIRS
		NAMES
			spdlog.h
		PATHS
			dependencies/spdlog/include
		PATH_SUFFIXES
			spdlog
		DOC
			"The directory where spdlog.h resides"
	)
	MESSAGE(STATUS "SPDLOG_INCLUDE_DIRS = ${SPDLOG_INCLUDE_DIRS}")
	#set( SPDLOG_INCLUDE_DIRS engine/dependencies/spdlog/include )
	# Check if we found it!
	IF ( SPDLOG_INCLUDE_DIRS )
		SET (SPDLOG_FOUND true)
		MESSAGE(STATUS "Looking for sdpLog - found: ${SPDLOG_INCLUDE_DIRS}")
	ELSE ( SPDLOG_INCLUDE_DIRS )
		SET (SPDLOG_FOUND false)
		MESSAGE(STATUS "Looking for sdpLog - not found: ${SPDLOG_INCLUDE_DIRS}")
	ENDIF ( SPDLOG_INCLUDE_DIRS )
ELSE ( LOGGING_ENABLED )
	SET (SPDLOG_FOUND false)
	MESSAGE(STATUS "disabled sdpLog")
ENDIF ( LOGGING_ENABLED )

file(GLOB_RECURSE HDRS
	include/*.h
	include/*.hpp
	src/*.h
    src/*.hpp
)

file(GLOB_RECURSE SRCS
	src/*.cpp
	src/*.c
)

include_directories(
	include
)

IF ( SDL2_INCLUDE_DIR AND SDL2_LIBRARY )
	include_directories(
		${SDL2_INCLUDE_DIR}
	)
ENDIF ( SDL2_INCLUDE_DIR AND SDL2_LIBRARY )

IF ( SPDLOG_FOUND )
	MESSAGE(STATUS "spdlog found: ${SPDLOG_FOUND}	/${SPDLOG_INCLUDE_DIRS}")
	include_directories(
		${SPDLOG_INCLUDE_DIRS}
	)
ENDIF ( SPDLOG_FOUND )

# configure a header file to pass some of the CMake settings
# to the source code
configure_file ("include/YoaConfig.h.in" "include/YoaConfig.h")

if (MSVC)
	add_definitions(-DBUILD_DLL)
	#set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin )
endif(MSVC)

add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})

if (MSVC)
	if(BUILD_LIBRARY)
		set_property(DIRECTORY ${PROJ_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	endif()
endif (MSVC)