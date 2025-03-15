cmake_minimum_required(VERSION 3.5...3.31)
# set project name
project("YoAudio-Engine" VERSION ${PROJECT_VERSION})
SET(ENGINE_NAME ${PROJECT_NAME} CACHE INTERNAL "Name of ${PROJECT_NAME} project")

message(STATUS "###################################################################################################################")
message(STATUS "")
message(STATUS "Now building the project for ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "")
message(STATUS "###################################################################################################################")

#
# Set up spdlog dependency stuff
#
if(LOGGING_ENABLED)
	set(SPDLOG_ROOT  ${PROJECT_SOURCE_DIR}/../dependencies/spdlog)
	find_package(Spdlog REQUIRED)
else()
	set(SPDLOG_FOUND false)
	message(STATUS "logging is disabled. not looking for sdpLog library.")
endif()

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
# configure a header file to pass some of the CMake settings to the source code
# configure_file (include/YoaConfig.h.in include/YoaConfig.h @ONLY)
configure_file (
	"${CMAKE_CURRENT_SOURCE_DIR}/include/YoaConfig.h.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/YoaConfig.h" )

#
# include detected dependencies
#
if(SDL2_INCLUDE_DIR)
	include_directories(
		${SDL2_INCLUDE_DIR}
	)
endif()

if(SPDLOG_FOUND)
	message(STATUS "spdlog found: ${SPDLOG_FOUND}	/${SPDLOG_INCLUDE_DIR}")
	include_directories(${SPDLOG_INCLUDE_DIR})
else()
	message(STATUS "spdlog not found!!!")
endif()

#
# setup output binary
#
add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)
# link SDL2
if(SDL2_LIBRARY)
	target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
endif()

if (MSVC)
	# this expresses the YOA_API macro to express DLL export
	target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_DLL=true)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
	if(${Code_Analysis})
		# do code analysis
		set_target_properties(${PROJECT_NAME} PROPERTIES
			VS_GLOBAL_RunCodeAnalysis true
			VS_GLOBAL_EnableMicrosoftCodeAnalysis true)
	endif()
endif(MSVC)

message(STATUS "YoAudio Engine CMakeLists.txt end")
message(STATUS " ")