
cmake_minimum_required (VERSION 2.6)
project (MCServer)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++03")
endif()

include_directories (SYSTEM "${PROJECT_SOURCE_DIR}/../lib/")
include_directories (SYSTEM "${PROJECT_SOURCE_DIR}/../lib/jsoncpp/include")

add_subdirectory(OSSupport)
add_subdirectory(HTTPServer)
add_subdirectory(Bindings)
add_subdirectory(Items)
add_subdirectory(Blocks)
add_subdirectory(Protocol)
add_subdirectory(Generating)
add_subdirectory(WorldStorage)
add_subdirectory(Mobs)
add_subdirectory(Entities)
add_subdirectory(Simulator)

set(SOURCE main Root MCLogger Authenticator StringUtils Server DeadlockDetect WebAdmin GroupManager)
set(SOURCE ${SOURCE} CraftingRecipes FurnaceRecipe MonsterConfig World CommandOutput RCONServer)
set(SOURCE ${SOURCE} Log ClientHandle ChatColor Group Enchantments MonsterConfig ChunkMap ChunkSender)
set(SOURCE ${SOURCE} LightingThread MobCensus MobSpawner MobProximityCounter Noise)

add_executable(../MCServer/MCServer ${SOURCE})

target_link_libraries(../MCServer/MCServer OSSupport HTTPServer iniFile Bindings Items Blocks)
target_link_libraries(../MCServer/MCServer Protocol Generating WorldStorage jsoncpp cryptopp)
target_link_libraries(../MCServer/MCServer Mobs Entities Simulator)
