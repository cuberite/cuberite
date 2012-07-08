#include "Globals.h"
#include "SquirrelBindings.h"
#include "SquirrelFunctions.h"

#include "cSquirrelBaseClass.h"

#include "../cPlayer.h"

using namespace Sqrat;



void BindSquirrel(HSQUIRRELVM vm)
{
	RootTable()
		.Bind("Plugin", Class<cSquirrelBaseClass>()
			.Func("AddHook", &cSquirrelBaseClass::AddHook)
			);
	RootTable().Bind("cPlayer", Class<cPlayer, NoConstructor>()
		.Func("GetName", &cPlayer::GetName)
	);
		

	RootTable().Func("print", &sqPrint);


	ConstTable().Enum("Hook", Enumeration()
		.Const("Tick", cPluginManager::HOOK_TICK)
		.Const("Chat", cPluginManager::HOOK_CHAT)
		.Const("CollectItem", cPluginManager::HOOK_COLLECT_ITEM)
		.Const("BlockDig", cPluginManager::HOOK_BLOCK_DIG)
		.Const("BlockPlace", cPluginManager::HOOK_BLOCK_PLACE)
		.Const("Disconnect", cPluginManager::HOOK_DISCONNECT)
		.Const("Handshake", cPluginManager::HOOK_HANDSHAKE)
		.Const("Login", cPluginManager::HOOK_LOGIN)
		.Const("PlayerSpawn", cPluginManager::HOOK_PLAYER_SPAWN)
		.Const("PlayerJoin", cPluginManager::HOOK_PLAYER_JOIN)
		.Const("PlayerMove", cPluginManager::HOOK_PLAYER_MOVE)
		.Const("TakeDamage", cPluginManager::HOOK_TAKE_DAMAGE)
		.Const("Killed", cPluginManager::HOOK_KILLED)
		.Const("ChunkGenerated", cPluginManager::HOOK_CHUNK_GENERATED)
		.Const("ChunkGenerating", cPluginManager::HOOK_CHUNK_GENERATING)
		.Const("BlockToDrops", cPluginManager::HOOK_BLOCK_TO_DROPS)
		.Const("PreCrafting", cPluginManager::HOOK_PRE_CRAFTING)
		.Const("CraftingNoRecipe", cPluginManager::HOOK_CRAFTING_NO_RECIPE)
		.Const("PostCrafting", cPluginManager::HOOK_POST_CRAFTING)
		.Const("BlockToPickup", cPluginManager::HOOK_BLOCK_TO_PICKUP)
		.Const("WeatherChanged", cPluginManager::HOOK_WEATHER_CHANGED)
		.Const("UpdatingSign", cPluginManager::HOOK_UPDATING_SIGN)
		.Const("UpdatedSign", cPluginManager::HOOK_UPDATED_SIGN));

}