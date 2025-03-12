# set project name
project(YoAudio_Editor)
message(STATUS "Now building the project for ${PROJECT_NAME} version ${YOA_VERSION_MAJOR}.${YOA_VERSION_MINOR}.${YOA_VERSION_PATCH}")

#
# Set up YoAudio dependency stuff
#
set(YOA_SEARCH_PATHS ../engine/)

# YOA include
FIND_PATH( YOA_INCLUDE_DIR
	NAMES
		YoAudio.h
	PATHS
		${YOA_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where YoAudio.h resides"
)

# YOA binaries
if(MSVC)
    FIND_PATH( YOA_LIBRARIES
	    NAMES
		    YoAudio.dll
	    PATHS
		    ${YOA_SEARCH_PATHS} ..
	    PATH_SUFFIXES
		    bin/Debug
			bin/Release
			bin
	    DOC
		    "The YoAudio.dll file"
    )
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    FIND_PATH( YOA_LIBRARIES
	    NAMES
            libYoAudio.so
	    PATHS
		    ${YOA_SEARCH_PATHS}..
	    PATH_SUFFIXES
		    bin
	    DOC
		    "The libYoAudio.so file"
    )
endif()

# Check if we found it!
IF ( YOA_INCLUDE_DIR AND YOA_LIBRARIES )
	SET( YOA_FOUND TRUE )
	MESSAGE(STATUS "Looking for YoAudio - found")
ELSE ( YOA_INCLUDE_DIR AND YOA_LIBRARIES )
	SET( YOA_FOUND FALSE )
	MESSAGE(STATUS "Looking for YoAudio - not found" )
	MESSAGE(STATUS "Looking for YoAudio (includes) at path: " ${} )
	MESSAGE(STATUS "Looking for YoAudio (library) at path: " ${YOA_LIBRARIES} )
ENDIF ( YOA_INCLUDE_DIR AND YOA_LIBRARIES )

IF( YOA_FOUND )
	message("-- Linking YoAudio lib")
	if(MSVC)
		add_library(YoAudio_lib ${YOA_LIBRARIES}/YoAudio.dll)
	else()
		add_library(YoAudio_lib ${YOA_LIBRARIES}/libYoAudio.so)
	endif()
	set_target_properties(YoAudio_lib PROPERTIES LINKER_LANGUAGE C)
ENDIF()

#
# Set up imgui dependency stuff
#
set(GUI_SEARCH_PATHS dependencies )

# imgui include
FIND_PATH( GUI_INCLUDE_DIRS
	NAMES
    	imgui.h
	PATHS
		${GUI_SEARCH_PATHS}
    PATH_SUFFIXES
		imgui
	DOC
		"The directory where imgui.h resides"
)

# Check if we found it!
IF ( GUI_INCLUDE_DIRS )
	SET( GUI_FOUND TRUE )
	MESSAGE(STATUS "Looking for imgui.h - found")
ELSE ( GUI_INCLUDE_DIRS )
	SET( GUI_FOUND FALSE )
	MESSAGE(STATUS "Looking for imgui.h in ${GUI_SEARCH_PATHS} - not found")
ENDIF ( GUI_INCLUDE_DIRS )

# gui impl include
FIND_PATH( GUIIMPL_INCLUDE_DIRS
	NAMES
        imgui_impl_sdl2.h   
	PATHS
		${GUI_SEARCH_PATHS}
	PATH_SUFFIXES
		imgui/backends
	DOC
		"The directory where imgui_impl_sdl2.h resides"
)

# Check if we found it!
IF ( GUIIMPL_INCLUDE_DIRS )
	SET( GUI_FOUND TRUE )
	MESSAGE(STATUS "Looking for imgui_impl_sdl2.h - found")
ELSE ( GUIIMPL_INCLUDE_DIRS )
	SET( GUI_FOUND FALSE )
	MESSAGE(STATUS "Looking for imgui_impl_sdl2.h - not found")
ENDIF ( GUIIMPL_INCLUDE_DIRS )
 
#
# input files for this project
#
file(GLOB_RECURSE HDRS
	src/*.h
	src/*.hpp
    dependencies/imgui/*.h
)

file(GLOB_RECURSE SRCS
	src/*.cpp
	src/*.c
)

set(CMAKE_INCLUDE_DIRECTORIES_BEFORE SYSTEM)
include_directories(
	${SDL2_INCLUDE_DIR}
	${YOA_INCLUDE_DIR}
    ${GUI_INCLUDE_DIRS}
    ${GUIIMPL_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${YoAudio_lib})

if(MSVC)
	if(NOT BUILD_LIBRARY)
		set_property(DIRECTORY ${PROJ_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	endif()
	
    # add_custom_command(
        # TARGET ${PROJECT_NAME}
        # POST_BUILD
        # COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2_BINARIES}/SDL2.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    # add_custom_command(
        # TARGET ${PROJECT_NAME}
        # PRE_BUILD
        # COMMAND ${CMAKE_COMMAND} -E copy_if_different "${YOA_LIBRARIES}/YoAudio.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    # add_custom_command(
        # TARGET ${PROJECT_NAME}
        # PRE_BUILD
        # COMMAND ${CMAKE_COMMAND} -E copy_if_different "${YOA_LIBRARIES}/YoAudio.pdb" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    # add_custom_command(
	    # TARGET ${PROJECT_NAME}
	    # PRE_BUILD
        # COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets )
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    MESSAGE("TODO: contemplate copying assets to buid directory if needed?")
#    add_custom_command(
#	    TARGET ${PROJECT_NAME}
#	    PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets )
endif()
