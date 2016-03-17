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
	tolua++.h
)

# List all the files that are generated as part of the Bindings build process
set (BINDING_OUTPUTS
	${CMAKE_CURRENT_SOURCE_DIR}/Bindings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Bindings.h
	${CMAKE_CURRENT_SOURCE_DIR}/LuaState_Declaration.inc
	${CMAKE_CURRENT_SOURCE_DIR}/LuaState_Implementation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LuaState_Typedefs.inc
)

set(BINDING_DEPENDENCIES
	tolua
	../Bindings/AllToLua.pkg
	../Bindings/BindingsProcessor.lua
	../Bindings/LuaFunctions.h
	../Bindings/LuaWindow.h
	../Bindings/Plugin.h
	../Bindings/PluginLua.h
	../Bindings/PluginManager.h
	../BiomeDef.h
	../BlockArea.h
	../BlockEntities/BeaconEntity.h
	../BlockEntities/BlockEntity.h
	../BlockEntities/BlockEntityWithItems.h
	../BlockEntities/ChestEntity.h
	../BlockEntities/DispenserEntity.h
	../BlockEntities/DropSpenserEntity.h
	../BlockEntities/DropperEntity.h
	../BlockEntities/FurnaceEntity.h
	../BlockEntities/HopperEntity.h
	../BlockEntities/JukeboxEntity.h
	../BlockEntities/NoteEntity.h
	../BlockEntities/SignEntity.h
	../BlockEntities/MobHeadEntity.h
	../BlockEntities/FlowerPotEntity.h
	../BlockID.h
	../BoundingBox.h
	../ChatColor.h
	../ChunkDef.h
	../ClientHandle.h
	../CraftingRecipes.h
	../Cuboid.h
	../Defines.h
	../Enchantments.h
	../Entities/ArrowEntity.h
	../Entities/Entity.h
	../Entities/EntityEffect.h
	../Entities/ExpBottleEntity.h
	../Entities/FireChargeEntity.h
	../Entities/FireworkEntity.h
	../Entities/Floater.h
	../Entities/GhastFireballEntity.h
	../Entities/HangingEntity.h
	../Entities/ItemFrame.h
	../Entities/Pawn.h
	../Entities/Player.h
	../Entities/Painting.h
	../Entities/Pickup.h
	../Entities/ProjectileEntity.h
	../Entities/SplashPotionEntity.h
	../Entities/ThrownEggEntity.h
	../Entities/ThrownEnderPearlEntity.h
	../Entities/ThrownSnowballEntity.h
	../Entities/TNTEntity.h
	../Entities/WitherSkullEntity.h
	../Generating/ChunkDesc.h
	../Inventory.h
	../Item.h
	../ItemGrid.h
	../Mobs/Monster.h
	../OSSupport/File.h
	../Root.h
	../Server.h
	../StringUtils.h
	../Tracer.h
	../UI/Window.h
	../Vector3.h
	../WebAdmin.h
	../World.h
)

if (NOT MSVC)
	ADD_CUSTOM_COMMAND(
		# add any new generated bindings here
		OUTPUT ${BINDING_OUTPUTS}

		# Regenerate bindings:
		COMMAND tolua -L BindingsProcessor.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}

		# add any new generation dependencies here
		DEPENDS ${BINDING_DEPENDENCIES}
	)
endif ()

set_source_files_properties(${BINDING_OUTPUTS} PROPERTIES GENERATED TRUE)

set_source_files_properties(${CMAKE_SOURCE_DIR}/src/Bindings/Bindings.cpp PROPERTIES COMPILE_FLAGS -Wno-error)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set_source_files_properties(${CMAKE_SOURCE_DIR}/src/Bindings/Bindings.cpp PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-old-style-cast -Wno-missing-prototypes")
	set_source_files_properties(LuaWindow.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch-enum")
endif()

if(NOT MSVC)
	add_library(Bindings ${SRCS} ${HDRS})
	target_link_libraries(Bindings lua sqlite tolualib mbedtls HTTPServer)
endif()
