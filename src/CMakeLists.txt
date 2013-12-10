
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories (SYSTEM "${PROJECT_SOURCE_DIR}/../lib/")
add_subdirectory(OSSupport)
add_subdirectory(HTTPServer)
add_subdirectory(Bindings)

set(SOURCE main Root MCLogger Authenticator StringUtils Server DeadlockDetect WebAdmin GroupManager CraftingRecipes FurnaceRecipe)
set(SOURCE ${SOURCE} MonsterConfig)

add_executable(MCServer ${SOURCE})

target_link_libraries(MCServer OSSupport HTTPServer iniFile Bindings)
