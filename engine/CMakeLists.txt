cmake_minimum_required(VERSION 3.5...3.31)
# set project name
project("YoAudio-Engine" VERSION ${PROJECT_VERSION})
SET(ENGINE_NAME ${PROJECT_NAME} CACHE INTERNAL "Name of ${PROJECT_NAME} project")

message(STATUS "###################################################################################################################")
message(STATUS "")
message(STATUS "Now building the project for ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "")
message(STATUS "###################################################################################################################")

# pull in logging library if it was pulled in properly
include(dependencies/spdlog.cmake)

#
# setup code
#

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
# configure a header file to pass some of the CMake settings
# to the source code
configure_file ("include/YoaConfig.h.in" "include/YoaConfig.h")

#
# include detected dependencies
#
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

#
# setup output binary
#
add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)
# link SDL2
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})

if (MSVC)
	# this expresses the YOA_API macro to express DLL export
	target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_DLL=true)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif(MSVC)

MESSAGE("YoAudio Engine CMakeLists.txt end")