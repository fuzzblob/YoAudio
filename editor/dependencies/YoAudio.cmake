#
# Set up YoAudio dependency stuff
#
set(YOA_SEARCH_PATHS ../ ../engine/)

#
# include headers
#
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
IF ( YOA_INCLUDE_DIR )
	SET( YOA_FOUND TRUE )
	MESSAGE(STATUS "Looking for YoAudio (includes) at path: " ${YOA_INCLUDE_DIR} )
ELSE ( YOA_INCLUDE_DIR )
	SET( YOA_FOUND FALSE )
	MESSAGE(STATUS "Looking for YoAudio - not found" )
	MESSAGE(STATUS "Looking for YoAudio (includes) at path: " ${YOA_INCLUDE_DIR} )
ENDIF ( YOA_INCLUDE_DIR )
#
# binaries
#
if(MSVC)
    FIND_PATH( YOA_DLL
	    NAMES
		    YoAudio-Engine.dll
	    PATHS
		    ${YOA_SEARCH_PATHS}
	    PATH_SUFFIXES
		    bin/Debug
			bin/Release
			bin
	    DOC
		    "The YoAudio-Engine.dll file"
    )
	FIND_PATH( YOA_LIB
	    NAMES
		    YoAudio-Engine.lib
	    PATHS
		    ${YOA_SEARCH_PATHS}
	    PATH_SUFFIXES
		    bin/Debug
			bin/Release
			bin
	    DOC
		    "The YoAudio-Engine.lib file"
    )
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    FIND_PATH( YOA_SO
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
#
# setup linking
#
IF ( YOA_DLL OR YOA_LIB)
	message("-- Linking YoAudio lib")
	MESSAGE(STATUS "YoAudio DLL: ${YOA_DLL}")
	MESSAGE(STATUS "YoAudio LIB: ${YOA_LIB}")
	link_directories(${YOA_LIB})
	set_target_properties(${YOA_LIBRARY} PROPERTIES LINKER_LANGUAGE C)
ELSEIF( YOA_SO )
	message("-- Linking YoAudio lib")
	MESSAGE(STATUS "YoAudio SO: ${YOA_SO}" )
	link_directories(${YOA_SO})
ELSE ()
	MESSAGE(STATUS "Looking for YoAudio binaries - not found" )
ENDIF()
#
# print status
#
IF( YOA_FOUND )
	message(STATUS "YoAudio Engine configured")
ELSE()
	message(STATUS "YoAudio Engine not fully configured!!!")
ENDIF()