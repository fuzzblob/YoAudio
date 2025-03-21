# set project name
project("YOA_Editor" VERSION ${PROJECT_VERSION})
SET(EDITOR_NAME ${PROJECT_NAME} CACHE INTERNAL "Name of ${PROJECT_NAME} project")

message(STATUS "###################################################################################################################")
message(STATUS "")
message(STATUS "Now building the project for ${EDITOR_NAME} version ${PROJECT_VERSION}")
message(STATUS "")
message(STATUS "###################################################################################################################")

# set C++ standard
include(../compiler.cmake)
#
# handle dependencies
#
include(../dependencies/YoAudio.cmake)
include(../dependencies/imgui.cmake)
# find OpenGL for rendering pixels to the screen
find_package(OpenGL REQUIRED)
# configure editor config header
configure_file (
	"${CMAKE_CURRENT_SOURCE_DIR}/src/EditorConfig.h.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/EditorConfig.h" )
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
    ${GUIIMPL_INCLUDE_DIRS})
#
# setup the executable
#
add_executable(${EDITOR_NAME} ${SRCS} ${HDRS} ${IMGUI_SOURCES})
target_compile_features(${EDITOR_NAME} PRIVATE cxx_std_20)
#
# link extermal libraries
#
target_link_libraries(${EDITOR_NAME} OpenGL::GL ${SDL2_LIBRARY} )
# YoAudio platform specifics
if(MSVC)
	#link_directories( ${YOA_LIB})
	set_target_properties(${EDITOR_NAME} PROPERTIES
	  IMPORTED_LOCATION "${YOA_LIB}/${ENGINE_NAME}.lib"
	  INTERFACE_INCLUDE_DIRECTORIES "${YOA_INCLUDE_DIR}"
	)
else()
	#link_directories(${YOA_SO})
	set_target_properties(${EDITOR_NAME} PROPERTIES
	  IMPORTED_LOCATION "${YOA_SO}/${ENGINE_NAME}.so"
	  INTERFACE_INCLUDE_DIRECTORIES "${YOA_INCLUDE_DIR}"
	)
endif()
target_link_libraries(${EDITOR_NAME} ${ENGINE_NAME} ${COMMON_NAME}) # also adds the required include path
# when referencing YoAudio.h the DLL macros should expand to import, not export
target_compile_definitions(${EDITOR_NAME} PRIVATE BUILD_DLL=false)
# search for SDL2.dll
set(SDL2_DLL_SEARCH_PATH ../dependencies/SDL2/)
FIND_PATH( SDL_DLL_PATH
	NAMES SDL2.dll
	PATHS ${SDL2_DLL_SEARCH_PATH}
	PATH_SUFFIXES lib/x64
	DOC "The SDL2.dll file location")
message(STATUS "SDL2.dll path detected is: " ${SDL_DLL_PATH} )
# copy SDL.dll to Runtime directory so the editor can run properly
IF ( SDL_DLL_PATH )
	add_custom_command(TARGET ${EDITOR_NAME} PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${SDL_DLL_PATH}/SDL2.dll $<TARGET_FILE_DIR:${EDITOR_NAME}>
		COMMAND_EXPAND_LISTS)
ELSE ( )
	message(STATUS "could not locate SDL2.dll path in ${SDL2_DLL_SEARCH_PATH}" )
ENDIF ( )
#
# Windows platform specific stuff
#
if(MSVC)
	if(${Code_Analysis})
		# do code analysis
		set_target_properties(${EDITOR_NAME} PROPERTIES
			VS_GLOBAL_RunCodeAnalysis true
			VS_GLOBAL_EnableMicrosoftCodeAnalysis true)
	endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_${lang}_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    message(STATUS "TODO: contemplate copying assets to buid directory if needed?")
	# add_custom_command(
		# TARGET ${EDITOR_NAME}
	    # PRE_BUILD
		# COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${EDITOR_NAME}>/assets )
endif()

# copy assets folder to build directory to enable easy playback
set(ASSETS_DIR ${CMAKE_CURRENT_LIST_DIR}/assets/)
message(STATUS "custom command to copy `${ASSETS_DIR}` to buid directory pre-build.")
add_custom_command(TARGET ${EDITOR_NAME} PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${ASSETS_DIR} $<TARGET_FILE_DIR:${EDITOR_NAME}>/assets/)

message(STATUS "########## ${ENGINE_NAME} CMakeLists.txt end ##########")
message(STATUS " ")