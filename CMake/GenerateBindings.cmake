function(enable_bindings_generation)
	# Enumerate every Lua-exported class.
	# Changes to these files will cause binding regen:
	set(BINDING_DEPENDENCIES
		Bindings/AllToLua.pkg
		Bindings/BindingsProcessor.lua
		Bindings/LuaFunctions.h
		Bindings/LuaWindow.h
		Bindings/Plugin.h
		Bindings/PluginLua.h
		Bindings/PluginManager.h
		BiomeDef.h
		BlockArea.h
		BlockEntities/BeaconEntity.h
		BlockEntities/BedEntity.h
		BlockEntities/BlockEntity.h
		BlockEntities/BlockEntityWithItems.h
		BlockEntities/BrewingstandEntity.h
		BlockEntities/ChestEntity.h
		BlockEntities/CommandBlockEntity.h
		BlockEntities/DispenserEntity.h
		BlockEntities/DropSpenserEntity.h
		BlockEntities/DropperEntity.h
		BlockEntities/FurnaceEntity.h
		BlockEntities/HopperEntity.h
		BlockEntities/JukeboxEntity.h
		BlockEntities/MobSpawnerEntity.h
		BlockEntities/NoteEntity.h
		BlockEntities/SignEntity.h
		BlockEntities/MobHeadEntity.h
		BlockEntities/FlowerPotEntity.h
		BlockType.h
		BlockInfo.h
		BoundingBox.h
		ChatColor.h
		ChunkDef.h
		ClientHandle.h
		Color.h
		CompositeChat.h
		CraftingRecipes.h
		Cuboid.h
		Defines.h
		EffectID.h
		Enchantments.h
		Entities/Boat.h
		Entities/ArrowEntity.h
		Entities/Entity.h
		Entities/ExpOrb.h
		Entities/EntityEffect.h
		Entities/ExpBottleEntity.h
		Entities/FallingBlock.h
		Entities/FireChargeEntity.h
		Entities/FireworkEntity.h
		Entities/Floater.h
		Entities/GhastFireballEntity.h
		Entities/HangingEntity.h
		Entities/ItemFrame.h
		Entities/LeashKnot.h
		Entities/Pawn.h
		Entities/Player.h
		Entities/Painting.h
		Entities/Pickup.h
		Entities/ProjectileEntity.h
		Entities/SplashPotionEntity.h
		Entities/ThrownEggEntity.h
		Entities/ThrownEnderPearlEntity.h
		Entities/ThrownSnowballEntity.h
		Entities/TNTEntity.h
		Entities/WitherSkullEntity.h
		Generating/ChunkDesc.h
		IniFile.h
		Inventory.h
		Item.h
		ItemGrid.h
		Map.h
		MapManager.h
		Mobs/Monster.h
		Mobs/MonsterTypes.h
		OSSupport/File.h
		Protocol/MojangAPI.h
		Root.h
		Scoreboard.h
		Server.h
		Statistics.h
		StringUtils.h
		UI/Window.h
		UUID.h
		Vector3.h
		WebAdmin.h
		World.h
	)

	# List all the files that are generated as part of the Bindings build process:
	set(BINDING_OUTPUTS
		Bindings.cpp
		Bindings.h
		LuaState_Declaration.inc
		LuaState_Implementation.cpp
		LuaState_Typedefs.inc
	)

	# Make the file paths absolute and pointing to the bindings folder:
	set(BINDINGS_FOLDER "${PROJECT_SOURCE_DIR}/src/Bindings/")
	list(TRANSFORM BINDING_OUTPUTS PREPEND ${BINDINGS_FOLDER})
	list(TRANSFORM BINDING_DEPENDENCIES PREPEND "${PROJECT_SOURCE_DIR}/src/")

	# Generate the bindings:
	add_custom_command(
		OUTPUT ${BINDING_OUTPUTS}
		COMMAND lua BindingsProcessor.lua
		WORKING_DIRECTORY ${BINDINGS_FOLDER}
		DEPENDS ${BINDING_DEPENDENCIES}
	)
endfunction()
