cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")
include_directories (".")

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

# List all the files that are generated as part of the Bindings build process
set (BINDING_OUTPUTS
    ${CMAKE_CURRENT_SOURCE_DIR}/Bindings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Bindings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/LuaState_Call.inc
)

set(BINDING_DEPENDENCIES
    tolua
    ../Bindings/virtual_method_hooks.lua
    ../Bindings/AllToLua.pkg
    ../Bindings/gen_LuaState_Call.lua
    ../Bindings/LuaFunctions.h
    ../Bindings/LuaState_Call.inc
    ../Bindings/LuaWindow.h
    ../Bindings/Plugin.h
    ../Bindings/PluginLua.h
    ../Bindings/PluginManager.h
    ../Bindings/WebPlugin.h
    ../BiomeDef.h
    ../BlockArea.h
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
    ../Entities/EntityEffect.h
    ../Entities/Entity.h
    ../Entities/Floater.h
    ../Entities/Pawn.h
    ../Entities/Painting.h
    ../Entities/Pickup.h
    ../Entities/Player.h
    ../Entities/ProjectileEntity.h
    ../Entities/ArrowEntity.h
    ../Entities/ThrownEggEntity.h
    ../Entities/ThrownEnderPearlEntity.h
    ../Entities/ExpBottleEntity.h
    ../Entities/ThrownSnowballEntity.h
    ../Entities/FireChargeEntity.h
    ../Entities/FireworkEntity.h
    ../Entities/GhastFireballEntity.h
    ../Entities/TNTEntity.h
    ../Entities/ExpOrb.h
    ../Entities/HangingEntity.h
    ../Entities/ItemFrame.h
    ../Generating/ChunkDesc.h
    ../Group.h
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
set_source_files_properties(Bindings/Bindings.cpp PROPERTIES GENERATED TRUE)
set_source_files_properties(Bindings/Bindings.h PROPERTIES GENERATED TRUE)

if(NOT MSVC)
    add_library(Bindings ${SRCS} ${HDRS})

    target_link_libraries(Bindings lua sqlite tolualib polarssl)
endif()
