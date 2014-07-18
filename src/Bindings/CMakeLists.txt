cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
    Bindings.cpp
    DeprecatedBindings.cpp
    LuaChunkStay.cpp
    LuaState.cpp
    LuaWindow.cpp
    ManualBindings.cpp
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
    LuaState.h
    LuaWindow.h
    ManualBindings.h
    Plugin.h
    PluginLua.h
    PluginManager.h
    WebPlugin.h
    tolua++.h
)
