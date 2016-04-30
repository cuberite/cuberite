cmake_minimum_required (VERSION 2.6)
project (Cuberite)

include_directories ("${PROJECT_SOURCE_DIR}/../")
include_directories (".")

SET (SRCS
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
	ManualBindings_Network.cpp
	ManualBindings_RankManager.cpp
	ManualBindings_World.cpp
	Plugin.cpp
	PluginLua.cpp
	PluginManager.cpp
	WebPlugin.cpp
)

SET (HDRS
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
	WebPlugin.h
	tolua++.h
)

get_directory_property(BINDING_OUTPUTS DIRECTORY ../ DEFINITION BINDING_OUTPUTS)
set_source_files_properties(${BINDING_OUTPUTS} PROPERTIES GENERATED TRUE)

set_source_files_properties(${CMAKE_SOURCE_DIR}/src/Bindings/Bindings.cpp PROPERTIES COMPILE_FLAGS -Wno-error)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set_source_files_properties(${CMAKE_SOURCE_DIR}/src/Bindings/Bindings.cpp PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-old-style-cast -Wno-missing-prototypes")
	set_source_files_properties(LuaWindow.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch-enum")
endif()

if(NOT MSVC)
	add_library(Bindings ${SRCS} ${HDRS})
	add_dependencies(Bindings BindingsGeneration)
	target_link_libraries(Bindings lua sqlite tolualib mbedtls HTTPServer)
endif()
