cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")
include_directories (".")

SET (SRCS
	Bindings.cpp
	DeprecatedBindings.cpp
	LuaChunkStay.cpp
	LuaNameLookup.cpp
	LuaServerHandle.cpp
	LuaState.cpp
	LuaTCPLink.cpp
	LuaWindow.cpp
	ManualBindings.cpp
	ManualBindings_Network.cpp
	ManualBindings_RankManager.cpp
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
	LuaNameLookup.h
	LuaServerHandle.h
	LuaState.h
	LuaTCPLink.h
	LuaWindow.h
	ManualBindings.h
	Plugin.h
	PluginLua.h
	PluginManager.h
	WebPlugin.h
	tolua++.h
)

# List all the files that are generated as part of the Bindings build process
set (BINDING_OUTPUTS
	${CMAKE_CURRENT_SOURCE_DIR}/Bindings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Bindings.h
)

set(BINDING_DEPENDENCIES
	tolua
	../Bindings/virtual_method_hooks.lua
	../Bindings/AllToLua.pkg
	../Bindings/LuaFunctions.h
	../Bindings/LuaWindow.h
	../Bindings/Plugin.h
	../Bindings/PluginLua.h
	../Bindings/PluginManager.h
	../Bindings/WebPlugin.h
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
		COMMAND tolua -L virtual_method_hooks.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}

		# add any new generation dependencies here
		DEPENDS ${BINDING_DEPENDENCIES}
	)
endif ()

set_source_files_properties(${CMAKE_SOURCE_DIR}/src/Bindings/Bindings.cpp PROPERTIES GENERATED TRUE)
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/Bindings/Bindings.h PROPERTIES GENERATED TRUE)

set_source_files_properties(${CMAKE_SOURCE_DIR}/src/Bindings/Bindings.cpp PROPERTIES COMPILE_FLAGS -Wno-error)

if(NOT MSVC)
	add_library(Bindings ${SRCS} ${HDRS})

	target_link_libraries(Bindings lua sqlite tolualib polarssl)
endif()
