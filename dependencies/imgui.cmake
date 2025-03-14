#
# Set up imgui dependency stuff
#
set(GUI_SEARCH_PATHS dependencies/ ../dependencies/)
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
# collect imgui files to pull into editor executable
#
file(GLOB IMGUI_SOURCES
    ${GUI_INCLUDE_DIRS}/*.h
    ${GUI_INCLUDE_DIRS}/*.cpp
	${GUI_INCLUDE_DIRS}/misc/cpp/imgui_stdlib.* # to easily use InputText with std::string.
	${GUIIMPL_INCLUDE_DIRS}/imgui_impl_sdl2.h
	${GUIIMPL_INCLUDE_DIRS}/imgui_impl_sdl2.cpp
	${GUIIMPL_INCLUDE_DIRS}/imgui_impl_opengl3.h
	${GUIIMPL_INCLUDE_DIRS}/imgui_impl_opengl3.cpp
)
#
# improve the debugging experience in VisualStudio
#
if(MSVC)
	set(IMGUI_SOURCES ${IMGUI_SOURCES}
		${GUI_INCLUDE_DIRS}/misc/debuggers/imgui.natvis 
		${GUI_INCLUDE_DIRS}/misc/debuggers/imgui.natstepfilter
	)
endif()