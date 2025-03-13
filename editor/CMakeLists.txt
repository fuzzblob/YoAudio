# set project name
project("YoAudio-Editor" VERSION ${PROJECT_VERSION})
SET(EDITOR_NAME ${PROJECT_NAME} CACHE INTERNAL "Name of ${PROJECT_NAME} project")

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

#
# setup the executable
#
add_executable(${PROJECT_NAME} ${SRCS} ${HDRS} ${IMGUI_SOURCES})
#
# link extermal libraries
#
target_link_libraries(${PROJECT_NAME} OpenGL::GL ${SDL2_LIBRARY} )
# YoAudio platform specifics
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
#
# Windows platform specific stuff
#
if(MSVC)
	# when referencing YoAudio.h the DLL macros should expand to import, not export
	target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_DLL=false)
	# search for SDL2.dll
	set(SDL2_DLL_SEARCH_PATH ../engine/dependencies/SDL2/)
	FIND_PATH( SDL_DLL_PATH
		NAMES SDL2.dll
		PATHS ${SDL2_DLL_SEARCH_PATH}
		PATH_SUFFIXES lib/x64
		DOC "The SDL2.dll file location"
		)
	MESSAGE(STATUS "SDL2.dll path detected is: " ${SDL_DLL_PATH} )
	# copy SDL.dll to Runtime directory so the editor can run properly
	IF ( SDL_DLL_PATH )
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy ${SDL_DLL_PATH}/SDL2.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
			COMMAND_EXPAND_LISTS
			)
	ELSE ( )
		MESSAGE(STATUS "could not locate SDL2.dll path in ${SDL2_DLL_SEARCH_PATH}" )
	ENDIF ( )
	#
	# there should be a better way to make this ditributable 
	#
	
	# install(FILES $<TARGET_RUNTIME_DLLS:${EDITOR_NAME}> TYPE BIN)
	# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		# COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
		# COMMAND_EXPAND_LISTS
		# )
		# add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
        # COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_LIBRARY}/SDL2.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # TODO: Debug, Release etc
	# install(TARGETS ${PROJECT_NAME}
		# RUNTIME ARCHIVE LIBRARY RUNTIME FRAMEWORK BUNDLE PUBLIC_HEADER RESOURCE)
	# install(TARGETS ${SDL2}
		# RUNTIME ARCHIVE LIBRARY RUNTIME FRAMEWORK BUNDLE PUBLIC_HEADER RESOURCE)	
	# install(TARGETS ${PROJECT_NAME}
		# COMPONENT ${PROJECT_NAME}
		# RUNTIME_DEPENDENCIES
		# PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
		# POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
		# DIRECTORIES $<TARGET_FILE_DIR:${PROJECT_NAME}>)
	# install(
		# DIRECTORY "${SDL_DLL_PATH}"
		# TYPE BIN
		# FILES_MATCHING REGEX "[^\\\\/.]\\.[dD][lL][lL]$"
	# )

    MESSAGE("TODO: contemplate copying assets to buid directory if needed?")
    # add_custom_command(
	    # TARGET ${PROJECT_NAME}
	    # PRE_BUILD
        # COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets )
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    MESSAGE("TODO: contemplate copying assets to buid directory if needed?")
	# add_custom_command(
		# TARGET ${PROJECT_NAME}
	    # PRE_BUILD
		# COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets )
endif()

MESSAGE("YoAudio Editor CMakeLists.txt end")