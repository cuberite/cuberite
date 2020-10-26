function(group_sources)
	# Enable the support for solution folders in MSVC
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	# Put projects into solution folders in MSVC:
	set_target_properties(
		event_core_static
		event_extra_static
		expat
		fmt
		jsoncpp_lib
		lualib
		luaexpat
		mbedcrypto
		mbedtls
		mbedx509
		lsqlite
		sqlite3
		SQLiteCpp
		tolualib
		zlib
		PROPERTIES FOLDER Libraries
	)

	# luaproxy not generated on anything else
	if(WIN32)
		set_target_properties(
			luaproxy
			PROPERTIES FOLDER Support
		)
	endif()

	if(${BUILD_TOOLS})
		set_target_properties(
			MCADefrag
			ProtoProxy
			PROPERTIES FOLDER Tools
		)
	endif()

	# Put all files into one project, separate by the folders:
	get_property(TARGET_SOURCE_FILES TARGET ${CMAKE_PROJECT_NAME} PROPERTY SOURCES)
	source_group(TREE "${PROJECT_SOURCE_DIR}/src" FILES ${TARGET_SOURCE_FILES})
endfunction()
