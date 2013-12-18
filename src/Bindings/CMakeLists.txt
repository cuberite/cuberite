
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

if (WIN32)
add_executable(tolua++.exe IMPORTED)
else ()
message(WARNING "cannot rebuild bindings when not on windows")
endif()

 ADD_CUSTOM_COMMAND(
#add any new generated bindings here
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Bindings.cpp ${CMAKE_CURRENT_BINARY_DIR}/Bindings.h
#command execuded to regerate bindings
   COMMAND "./tolua++.exe" -L virtual_method_hooks.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg
#add any new generation dependencies here
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/virtual_method_hooks.lua ${CMAKE_CURRENT_SOURCE_DIR}/AllToLua.pkg tolua++.exe
   )

add_library(Bindings PluginManager LuaState WebPlugin Bindings)
