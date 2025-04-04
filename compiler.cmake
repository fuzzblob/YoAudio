##########################
#
#		Compiler
#
##########################

# Test whether an MSVC-like command-line option works.
execute_process(COMMAND "${CMAKE_${lang}_COMPILER}" -?
	RESULT_VARIABLE _clang_result
	OUTPUT_VARIABLE _clang_stdout
	ERROR_VARIABLE _clang_stderr)
if(_clang_result EQUAL 0)
	set(CMAKE_${lang}_COMPILER_FRONTEND_VARIANT "MSVC")
else()
	set(CMAKE_${lang}_COMPILER_FRONTEND_VARIANT "GNU")
endif()
# GNU like compiler
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_${lang}_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
	message(STATUS "GNU compiler options expressing.")
	# specify UTF8 support
	# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "/utf-8")
	# set(CMAKE_C_FLAGS "/utf-8")
	# Compiler warnings
	# Update if necessary
	# For more warning options see: https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md
	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wnon-virtual-dtor -pedantic -g -std=c++20")
	if(Warnings_As_Errors)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
	endif() 
endif()
# Windows toolchain
if(MSVC)
	message(STATUS "MSVC options expressing.")
	# specify UTF8 support
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "/utf-8")
	set(CMAKE_C_FLAGS "/utf-8")
	# Microsoft Visual Studio specifics
	add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
	# Force to always compile with W4
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive-")
	endif()
	# For more warning options see: https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md
	# /Wall is possible thanks to VS2022 enabling to exclude external headers from it
	set(MSVC_FLAGS "/permissive-|/w14640|/Wall|/external:anglebrackets|/external:W0|/std:c++20|/w14242|/w14254|/w14263|/w14265|/w14287|/we4289|/w14296|/w14311|/w14545|/w14546|/w14547|/w14549|/w14555|/w14619|/w14640|/w14826|/w14905|/w14906|/w14928")
	foreach(entry IN LISTS MSVC_FLAGS)
		string(REPLACE "|" ";" entry "${entry}")
		# use "${entry}" normally
		set(CMAKE_CXX_FLAGS ${entry})
	endforeach()
	
	# if(Warnings_As_Errors)
		# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "/WX")
	# endif()
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