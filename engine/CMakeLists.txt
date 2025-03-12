cmake_minimum_required(VERSION 3.5...3.31)
# set project name
project("YoAudio-Engine" VERSION ${PROJECT_VERSION})

message(STATUS "###################################################################################################################")
message(STATUS "")
message(STATUS "Now building the project for ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "")
message(STATUS "###################################################################################################################")

# pull in logging library if it was pulled in properly
include(dependencies/spdlog.cmake)

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

add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})

if (MSVC)
	target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_DLL=true)
	#set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin )
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif(MSVC)

if (MSVC AND LIB_IS_STARTUP_PROJECT)
	MESSAGE(STATUS "setting YoAudio Engine as startup project")
	set_property(DIRECTORY ${PROJ_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif ()

MESSAGE("YoAudio Engine CMakeLists.txt end")