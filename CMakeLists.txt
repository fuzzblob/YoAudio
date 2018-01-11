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

file(GLOB_RECURSE HDRS
	include/*.h
)

file(GLOB_RECURSE SRCS
	src/*.cpp
	src/*.c
)

message(STATUS "${HDRS}")
message(STATUS "${SRCS}")

include_directories(
	include
)

add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set_property(DIRECTORY ${PROJ_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
