# set project name
project(YoAudio_Editor)
message(STATUS "Now building the project for ${PROJECT_NAME} version ${YOA_VERSION_MAJOR}.${YOA_VERSION_MINOR}.${YOA_VERSION_PATCH}")

message(STATUS "###################################################################################################################")
message(STATUS "")
message(STATUS "Now building the project for ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "")
message(STATUS "###################################################################################################################")

#
# handle dependencies
#
include(dependencies/YoAudio.cmake)
include(dependencies/imgui.cmake)
# find OpenGL for rendering pixels to the screen
find_package(OpenGL REQUIRED)

#
# input files for this project
#
file(GLOB_RECURSE HDRS
	src/*.h
	src/*.hpp
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
    # ${GUI_INCLUDE_DIRS}/backends
    ${GUIIMPL_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS} ${IMGUI_SOURCES})
#target_include_directories(${PROJECT_NAME} PRIVATE  ${GUI_INCLUDE_DIRS} ${GUIIMPL_INCLUDE_DIRS})  # Include the IMGUI headers
target_link_libraries(${PROJECT_NAME} OpenGL::GL ${SDL2_LIBRARY} )


# The modern CMake version that doesn't add the -I and -L flags to every compiler invocation would be to use imported libraries:
# add_library("YoAudio-Engine" SHARED IMPORTED) # or STATIC instead of SHARED
if(MSVC)
	#link_directories( ${YOA_LIB})
	set_target_properties("YoAudio-Engine" PROPERTIES
	  IMPORTED_LOCATION "${YOA_LIB}/YoAudio-Engine.lib"
	  INTERFACE_INCLUDE_DIRECTORIES "${YOA_INCLUDE_DIR}"
	)
else()
	#link_directories(${YOA_SO})
	set_target_properties("YoAudio-Engine" PROPERTIES
	  IMPORTED_LOCATION "${YOA_SO}/YoAudio-Engine.so"
	  INTERFACE_INCLUDE_DIRECTORIES "${YOA_INCLUDE_DIR}"
	)
endif()
target_link_libraries(${PROJECT_NAME} "YoAudio-Engine") # also adds the required include path
# If setting the INTERFACE_INCLUDE_DIRECTORIES doesn't add the path, older versions of CMake also allow you to use target_include_directories(bar PUBLIC /path/to/include). However, this no longer works with CMake 3.6 or newer.

# The simplest way of doing this would be to add
# include_directories(${YOA_INCLUDE_DIR})

# add_executable(${PROJECT_NAME} ${FOO_SRCS})
# target_link_libraries(${PROJECT_NAME} bar) # libbar.so is found in ${CMAKE_SOURCE_DIR}/lib



if(MSVC)
	target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_DLL=false)
	
    # add_custom_command(
        # TARGET ${PROJECT_NAME}
        # POST_BUILD
        # COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2_BINARIES}/SDL2.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    # add_custom_command(
        # TARGET ${PROJECT_NAME}
        # PRE_BUILD
        # COMMAND ${CMAKE_COMMAND} -E copy_if_different "${YOA_BINARIES}/YoAudio.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    # add_custom_command(
        # TARGET ${PROJECT_NAME}
        # PRE_BUILD
        # COMMAND ${CMAKE_COMMAND} -E copy_if_different "${YOA_BINARIES}/YoAudio.pdb" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

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

MESSAGE("YoAudio Editor CMakeLists.txt end")