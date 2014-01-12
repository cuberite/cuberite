
cmake_minimum_required (VERSION 2.6)
project (MCServer)

# NOTE: This CMake file is processed only for Unix builds; Windows(MSVC) builds handle all the subfolders in /src in a single file, /src/CMakeLists.txt

include_directories ("${PROJECT_SOURCE_DIR}/../")

ADD_CUSTOM_COMMAND(
	# add any new generated bindings here
	OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/Bindings.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Bindings.h
	
	# command execuded to regerate bindings
	COMMAND tolua -L virtual_method_hooks.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	
	# add any new generation dependencies here
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/virtual_method_hooks.lua ${CMAKE_CURRENT_SOURCE_DIR}/AllToLua.pkg tolua
)

#add cpp files here
add_library(Bindings PluginManager LuaState WebPlugin Bindings ManualBindings LuaWindow Plugin PluginLua WebPlugin)

target_link_libraries(Bindings lua sqlite tolualib)
