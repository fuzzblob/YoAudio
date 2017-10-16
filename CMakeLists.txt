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

# Setup the Game Project
project("Game")

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
file(GLOB_RECURSE HDRS ${Game_SOURCE_DIR}/src/*.h)

# Lets LOAD our sources
file(GLOB_RECURSE SRCS
	${Game_SOURCE_DIR}/src/*.cpp
	${Game_SOURCE_DIR}/src/*.c
)

#add_executable(Game ${Game_SOURCE_DIR}/src/Main.cpp)
add_executable(Game ${SRCS} ${HDRS})
target_compile_features(Game PRIVATE cxx_lambda_init_captures)
target_link_libraries(Game --coverage)

# We need a CMAKE_DIR with some code to find external dependencies
SET(Game_CMAKE_DIR "${Game_SOURCE_DIR}/cmake")

# SDL2
INCLUDE(${Game_CMAKE_DIR}/FindSDL2.cmake)

# Define the include DIRs
include_directories(
	${Game_SOURCE_DIR}/headers
	${Game_SOURCE_DIR}/sources
	${SDL2_INCLUDE_DIRS}
	${SDL2I_INCLUDE_DIRS}
)

# Define the link libraries
target_link_libraries( Game
	${SDL2_LIBRARIES}
	${SDL2I_LIBRARIES}
)

#project("tests")
#enable_testing()

#add_executable(tester tester.cpp)
#target_link_libraries(tester --coverage)
#add_test(Tester tester)
