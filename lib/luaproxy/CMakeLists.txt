
# This project adds a Lua Proxy DLL on Windows
# By an unfortunate choice in the popular LuaBinaries distribution, there are two names for the Lua DLL on Windows: lua51.dll and lua5.1.dll.
# Some binary Lua packages are built for one, the others for the other. Messy!
# In order to support both package flavors, we create a "proxy DLL":
# Basically the lua5.1.dll has its PE Exports section manipulated so that it points each exported function to its lua51.dll implementation.
# Effectively, this forwards all calls from lua5.1.dll to lua51.dll without any performance costs (the forwarding is done in the Windows PE loader on app start).

# This project creates the proxy DLL by using a specially crafted .DEF file that is used to link the Proxy DLL.
# Note that it has been tested only on MSVC, it might not work with other compilers.
# The initial implementation was taken from http://lua-users.org/wiki/LuaProxyDllFour , but adapted to MSVC




if (WIN32)

	if (MSVC)
		# Tell the linker to use the DEF file to generate the proxy:
		set(CMAKE_EXE_LINKER_FLAGS_RELEASE    "${CMAKE_EXE_LINKER_FLAGS_RELEASE}    /NOENTRY /DEF:lua5.1.def /MANIFEST:NO")
		set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /NOENTRY /DEF:lua5.1.def /MANIFEST:NO")
		set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} /NOENTRY /DEF:lua5.1.def /MANIFEST:NO")
		set(CMAKE_EXE_LINKER_FLAGS_DEBUG      "${CMAKE_EXE_LINKER_FLAGS_DEBUG}      /NOENTRY /DEF:lua5.1.def /MANIFEST:NO")
		set(CMAKE_SHARED_LINKER_FLAGS_DEBUG   "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}   /NOENTRY /DEF:lua5.1.def /MANIFEST:NO")
		set(CMAKE_MODULE_LINKER_FLAGS_DEBUG   "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}   /NOENTRY /DEF:lua5.1.def /MANIFEST:NO")
	elseif (MINGW)
		# MinGW requires no further flags and has been tested
	else()
		message ("LuaProxy: This cmake code has not been tested on your compiler. Please report your success or failure in the forum.")
	endif()

	add_library(luaproxy SHARED "lua5.1.def" "Dummy.c")
	set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Server)
	set_target_properties(luaproxy PROPERTIES
		OUTPUT_NAME "lua5.1"
		PREFIX ""
	)
	target_link_libraries(luaproxy lua)

	# Output the executable into the $/Server folder, so that Cuberite can find it:
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Server)
	SET_TARGET_PROPERTIES(luaproxy PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/Server
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/Server
		ARCHIVE_OUTPUT_DIRECTORY_DEBUGPROFILE   ${CMAKE_SOURCE_DIR}/Server
		ARCHIVE_OUTPUT_DIRECTORY_RELEASEPROFILE ${CMAKE_SOURCE_DIR}/Server
		LIBRARY_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/Server
		LIBRARY_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/Server
		LIBRARY_OUTPUT_DIRECTORY_DEBUGPROFILE   ${CMAKE_SOURCE_DIR}/Server
		LIBRARY_OUTPUT_DIRECTORY_RELEASEPROFILE ${CMAKE_SOURCE_DIR}/Server
		RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/Server
		RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/Server
		RUNTIME_OUTPUT_DIRECTORY_DEBUGPROFILE   ${CMAKE_SOURCE_DIR}/Server
		RUNTIME_OUTPUT_DIRECTORY_RELEASEPROFILE ${CMAKE_SOURCE_DIR}/Server
	)

else()

	message (FATAL_ERROR "This project is needed only for Windows, modify your cmake file not to include it on Linux")

endif()
