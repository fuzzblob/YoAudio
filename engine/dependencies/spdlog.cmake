#
# Set up spdlog dependency stuff
#
IF ( LOGGING_ENABLED )
	FIND_PATH( SPDLOG_INCLUDE_DIRS
		NAMES
			spdlog.h
		PATHS
			dependencies/spdlog/include
		PATH_SUFFIXES
			spdlog
		DOC
			"The directory where spdlog.h resides"
	)
	MESSAGE(STATUS "SPDLOG_INCLUDE_DIRS = ${SPDLOG_INCLUDE_DIRS}")
	# Check if we found it!
	IF ( SPDLOG_INCLUDE_DIRS )
		SET (SPDLOG_FOUND true)
		MESSAGE(STATUS "Looking for sdpLog - found: ${SPDLOG_INCLUDE_DIRS}")
	ELSE ( SPDLOG_INCLUDE_DIRS )
		SET (SPDLOG_FOUND false)
		MESSAGE(STATUS "Looking for sdpLog - not found: ${SPDLOG_INCLUDE_DIRS}")
	ENDIF ( SPDLOG_INCLUDE_DIRS )
ELSE ( LOGGING_ENABLED )
	SET (SPDLOG_FOUND false)
	MESSAGE(STATUS "disabled sdpLog")
ENDIF ( LOGGING_ENABLED )