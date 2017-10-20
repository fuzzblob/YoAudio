#Copyright (C) 2015 Barry Deeney
#Copyright (c) 2015, Benny Bobaganoosh
#Copyright (c) 2017, Chris Tammik
#All rights reserved.
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are met:
#
#1. Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer. 
#2. Redistributions in binary form must reproduce the above copyright notice,
#  this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
#ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
#ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.2)

if (CMAKE_COMPILER_IS_GNUCC)
	option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)

	if (ENABLE_COVERAGE)
		add_compile_options(--coverage -O0)
	endif()
endif()

if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall) #-Wextra -Wpedantic
	# add_definitions(
		# -c
		# -Wall
	# )
endif()

# Setup the Project
SET(name "PROJ")
project(${name})

# don't leave CMAKE_BUILD_TYPE empty
# differentiation between debug and release builds.
if ( CMAKE_BUILD_TYPE STREQUAL "" )
	set(
		CMAKE_BUILD_TYPE
			"Debug"
		CACHE STRING
			"Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) \"Debug\" \"Release\" \"RelWithDebInfo\" \"MinSizeRel\"."
		FORCE
	)
endif ( CMAKE_BUILD_TYPE STREQUAL "" )

if ( CMAKE_BUILD_TYPE STREQUAL "Release" )
add_definitions( -O2 )
endif ( CMAKE_BUILD_TYPE STREQUAL "Release" )

# Lets LOAD our headers
file(GLOB_RECURSE HDRS 
	${PROJ_SOURCE_DIR}/src/*.h
)

# Lets LOAD our sources
file(GLOB_RECURSE SRCS
	${PROJ_SOURCE_DIR}/src/*.cpp
	${PROJ_SOURCE_DIR}/src/*.c
)

#add_executable(PROJ ${PROJ_SOURCE_DIR}/src/Main.cpp)
add_executable(PROJ ${SRCS} ${HDRS})
#add_executable(${name} WIN32 ${SRCS})
target_compile_features(${name} PRIVATE cxx_lambda_init_captures)
target_link_libraries(${name} --coverage)

# We need a CMAKE_DIR with some code to find external dependencies
SET(PROJ_CMAKE_DIR "${PROJ_SOURCE_DIR}/cmake")

# SDL2
INCLUDE(${PROJ_CMAKE_DIR}/FindSDL2.cmake)

# Define the include DIRs
include_directories(
	${PROJ_SOURCE_DIR}/headers
	${PROJ_SOURCE_DIR}/sources
	${SDL2_INCLUDE_DIRS}
	${SDL2I_INCLUDE_DIRS}
)

# Define the link libraries
target_link_libraries( ${name}
	${SDL2_LIBRARIES}
	${SDL2I_LIBRARIES}
)

#project("tests")
#enable_testing()

#add_executable(tester tester.cpp)
#target_link_libraries(tester --coverage)
#add_test(Tester tester)
