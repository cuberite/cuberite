
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

 ADD_CUSTOM_COMMAND(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/foo.c
   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bar.c ${CMAKE_CURRENT_BINARY_DIR}/foo.c
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/bar.c
   )
execute_process(COMMAND "./tolua++.exe" -L virtual_method_hooks.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg)

add_library(Bindings PluginManager LuaState WebPlugin)
