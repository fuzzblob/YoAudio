# Adding imgui as a compiled lirary as described here:
# https://github.com/cpm-cmake/CPM.cmake/issues/368#issuecomment-1177476658
#CPMAddPackage(gh:ocornut/imgui@1.91.9)
#add_library(imgui STATIC
#    ${imgui_SOURCE_DIR}/imgui.cpp
#    ${imgui_SOURCE_DIR}/imgui_demo.cpp # optionally comment this out
#    ${imgui_SOURCE_DIR}/imgui_draw.cpp
#    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
#    ${imgui_SOURCE_DIR}/imgui_tables.cpp
#)
#target_include_directories(imgui INTERFACE ${imgui_SOURCE_DIR})
#target_compile_definitions(imgui PUBLIC -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS) # optional imgui setting
#set_target_properties(imgui PROPERTIES FOLDER third-party) # optoinal IDE dir

CPMAddPackage(gh:ocornut/imgui@1.91.9)

#
# set the include directory
#
find_path(GUI_INCLUDE_DIRS
	NAMES imgui.h   
	PATHS ${imgui_SOURCE_DIR}
	PATH_SUFFIXES imgui
	DOC "The directory where imgui.h resides"
	)
#
# log whether it was found
#
if ( GUI_INCLUDE_DIRS )
	set( GUI_FOUND TRUE )
	message(STATUS "Looking for imgui.h - found")
else( GUI_INCLUDE_DIRS )
	set( GUI_FOUND FALSE )
	message(STATUS "Looking for imgui.h - not found")
endif( GUI_INCLUDE_DIRS )

#
# set the implementation directory
#
find_path( GUIIMPL_INCLUDE_DIRS
	NAMES imgui_impl_sdl2.h   
	PATHS ${imgui_SOURCE_DIR}/backends
	DOCc "The directory where imgui_impl_sdl2.h resides"
	)
#
# log whether it was found
#
if( GUIIMPL_INCLUDE_DIRS )
	set( GUI_FOUND TRUE )
	message(STATUS "Looking for imgui_impl_sdl2.h - found")
else( GUIIMPL_INCLUDE_DIRS )
	set( GUI_FOUND FALSE )
	message(STATUS "Looking for imgui_impl_sdl2.h - not found")
endif( GUIIMPL_INCLUDE_DIRS )
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