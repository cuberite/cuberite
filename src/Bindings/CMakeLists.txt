
cmake_minimum_required (VERSION 2.6)
project (MCServer)

# NOTE: This CMake file is processed only for Unix builds; Windows(MSVC) builds handle all the subfolders in /src in a single file, /src/CMakeLists.txt

#add cpp files here
add_library(Bindings PluginManager LuaState WebPlugin Bindings ManualBindings LuaWindow Plugin PluginLua WebPlugin)

target_link_libraries(Bindings lua sqlite tolualib)
