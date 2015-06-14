CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )

PROJECT( "RapaOpenAL" )

SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake" )

IF( MSVC )
	INCLUDE( RapaConfigureVisualStudio )
ENDIF()

FIND_PACKAGE( OpenAL )
IF( OPENAL_FOUND )
		
	INCLUDE_DIRECTORIES( ${OPENAL_INCLUDE_DIR} )
	INCLUDE_DIRECTORIES( include )
		
	SET	(	HEADERS
			include/ROALRoot.h
			include/ROALDevice.h
			include/ROALPlaybackDevice.h
			include/ROALCaptureDevice.h
			include/ROALContext.h
			include/ROALBuffer.h
			include/ROALListener.h
			include/ROALSource.h
			include/ROALStaticSource.h
			include/ROALStreamingSource.h
		)
		
	SET	(	SOURCES
			src/ROALRoot.cpp
			src/ROALDevice.cpp
			src/ROALPlaybackDevice.cpp
			src/ROALCaptureDevice.cpp
			src/ROALContext.cpp
			src/ROALBuffer.cpp
			src/ROALListener.cpp
			src/ROALSource.cpp
			src/ROALStaticSource.cpp
			src/ROALStreamingSource.cpp
		)

	SOURCE_GROUP("" FILES ${SOURCES} )		# Avoid "Header Files" and "Source Files" virtual folders in VisualStudio

	SET(CMAKE_DEBUG_POSTFIX "d")
	ADD_LIBRARY( ${PROJECT_NAME} STATIC ${SOURCES} )
	TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${OPENAL_LIBRARY} ) 

	#
	# Install
	#
	INSTALL(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
			LIBRARY DESTINATION lib
			ARCHIVE DESTINATION lib
			RUNTIME DESTINATION bin )
			#INCLUDES DESTINATION include )		# If uncommented, the ${PROJECT_NAME} target contains INCLUDE_DIRECTORIES information. Importing the target automatically adds this directory to the INCLUDE_DIRECTORIES.
	SET( TARGET_NAMESPACE Rapa:: )
	INSTALL( FILES ${HEADERS} DESTINATION include COMPONENT Devel )		
	EXPORT( EXPORT ${PROJECT_NAME}Targets FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Targets.cmake" NAMESPACE ${TARGET_NAMESPACE} )
	CONFIGURE_FILE( cmake/${PROJECT_NAME}Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake" @ONLY )
	SET( ConfigPackageLocation lib/cmake/${PROJECT_NAME} )
	INSTALL(EXPORT ${PROJECT_NAME}Targets
			FILE ${PROJECT_NAME}Targets.cmake
			NAMESPACE ${TARGET_NAMESPACE}
			DESTINATION ${ConfigPackageLocation} )
	INSTALL( FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake" DESTINATION ${ConfigPackageLocation} COMPONENT Devel )
	
	
	ADD_SUBDIRECTORY( samples )
	
ELSE()
	MESSAGE("OpenAL not found")
ENDIF()
