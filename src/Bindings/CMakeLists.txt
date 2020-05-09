target_sources(
	${CMAKE_PROJECT_NAME} PRIVATE

	Bindings.cpp
	DeprecatedBindings.cpp
	LuaChunkStay.cpp
	LuaJson.cpp
	LuaNameLookup.cpp
	LuaServerHandle.cpp
	LuaState.cpp
	LuaState_Implementation.cpp
	LuaTCPLink.cpp
	LuaUDPEndpoint.cpp
	LuaWindow.cpp
	ManualBindings.cpp
	ManualBindings_BlockArea.cpp
	ManualBindings_Network.cpp
	ManualBindings_RankManager.cpp
	ManualBindings_World.cpp
	Plugin.cpp
	PluginLua.cpp
	PluginManager.cpp

	Bindings.h
	DeprecatedBindings.h
	LuaChunkStay.h
	LuaFunctions.h
	LuaJson.h
	LuaNameLookup.h
	LuaServerHandle.h
	LuaState.h
	LuaState_Declaration.inc
	LuaState_Typedefs.inc
	LuaTCPLink.h
	LuaUDPEndpoint.h
	LuaWindow.h
	ManualBindings.h
	Plugin.h
	PluginLua.h
	PluginManager.h
	tolua++.h
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set_source_files_properties(
		Bindings.cpp PROPERTIES COMPILE_FLAGS
		"-Wno-error -Wno-old-style-cast -Wno-missing-prototypes -Wno-deprecated-declarations"
	)
endif()
