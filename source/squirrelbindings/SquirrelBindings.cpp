#include "Globals.h"
#include "SquirrelBindings.h"
#include "SquirrelFunctions.h"

#include "cSquirrelBaseClass.h"
#include "SquirrelArray.h"

#include "../cPlayer.h"

using namespace Sqrat;

#ifdef USE_SQUIRREL
void BindSquirrel(HSQUIRRELVM vm)
{
	RootTable()
		.Bind("Plugin", Class<cSquirrelBaseClass>()
			.Func("AddHook", &cSquirrelBaseClass::AddHook)
			.Func("AddCommand", &cSquirrelBaseClass::AddCommand)
			.Func("BindCommand", &cSquirrelBaseClass::BindCommand)
			);
	
	RootTable().Bind("Vector3f", Class<Vector3f, NoConstructor>()
		.Func("Set", &Vector3f::Set)
		.Func("Normalize", &Vector3f::Normalize)
		.Func("Length", &Vector3f::Length)
		.Func("SqrLength", &Vector3f::SqrLength)
		.Var("x", &Vector3f::x)
		.Var("y", &Vector3f::y)
		.Var("z", &Vector3f::z)
	);
	
	RootTable().Bind("Vector3d", Class<Vector3d, NoConstructor>()
		.Func("Set", &Vector3d::Set)
		.Func("Normalize", &Vector3d::Normalize)
		.Func("Length", &Vector3d::Length)
		.Func("SqrLength", &Vector3d::SqrLength)
		.Var("x", &Vector3d::x)
		.Var("y", &Vector3d::y)
		.Var("z", &Vector3d::z)
	);
	
	RootTable().Bind("cEntity", Class<cEntity, NoConstructor>()
		.Func("GetEntityType", &cEntity::GetEntityType)
		.Func("IsA", &cEntity::IsA)
		.Func("GetWorld", &cEntity::GetWorld)
		.Func("GetPosition", &cEntity::GetPosition)
		.Func("GetPosX", &cEntity::GetPosX)
		.Func("GetPosY", &cEntity::GetPosY)
		.Func("GetPosZ", &cEntity::GetPosZ)
		.Func("GetRot", &cEntity::GetRot)
		.Func("GetRotation", &cEntity::GetRotation)
		.Func("GetPitch", &cEntity::GetPitch)
		.Func("GetRoll", &cEntity::GetRoll)
		.Func("SetRotation", &cEntity::SetRotation)
		.Func("SetPitch", &cEntity::SetPitch)
		.Func("SetRoll", &cEntity::SetRoll)
		.Func("GetLookVector", &cEntity::GetLookVector)
		.Func("GetChunkX", &cEntity::GetChunkX)
		.Func("GetChunkY", &cEntity::GetChunkY)
		.Func("GetChunkZ", &cEntity::GetChunkZ)
		.Func("SetPosX", &cEntity::SetPosX)
		.Func("SetPosY", &cEntity::SetPosY)
		.Func("SetPosZ", &cEntity::SetPosZ)
		.Func<void (cEntity::*) (const double &, const double &, const double &)>("SetPosition", &cEntity::SetPosition)
		.Func("GetUniqueID", &cEntity::GetUniqueID)
		.Func("IsDestroyed", &cEntity::IsDestroyed)
		.Func("Destroy", &cEntity::Destroy)
		.Func("SpawnOn", &cEntity::SpawnOn)

	);
	
	ConstTable().Enum("MetaData", Enumeration()
		.Const("Normal", cPawn::NORMAL)
		.Const("Burning", cPawn::BURNING)
		.Const("Crouched", cPawn::CROUCHED)
		.Const("Riding", cPawn::RIDING)
		.Const("Sprinting", cPawn::SPRINTING)
		.Const("Eating", cPawn::EATING)
		.Const("Blocking", cPawn::BLOCKING)
	);
	
	RootTable().Bind("cPawn", DerivedClass<cPawn, cEntity, NoConstructor>()
		.Func("TeleportToEntity", &cPawn::TeleportToEntity)
		.Func("TeleportTo", &cPawn::TeleportTo)
		.Func("Heal", &cPawn::Heal)
		.Func("TakeDamage", &cPawn::TakeDamage)
		.Func("KilledBy", &cPawn::KilledBy)
		.Func("GetHealth", &cPawn::GetHealth)
		.Func("SetMetaData", &cPawn::SetMetaData)
		.Func("GetMetaData", &cPawn::GetMetaData)
		.Func("SetMaxHealth", &cPawn::SetMaxHealth)
		.Func("GetMaxHealth", &cPawn::GetMaxHealth)
	);
	
	RootTable().Bind("cPlayer", DerivedClass<cPlayer, cPawn, NoConstructor>()
		.Func("GetName", &cPlayer::GetName)
		.Func("SetTouchGround", &cPlayer::SetTouchGround)
		.Func("SetStance", &cPlayer::SetStance)
		.Func("GetEyeHeight", &cPlayer::GetEyeHeight)
		.Func("GetEyePosition", &cPlayer::GetEyePosition)
		.Func("GetFlying", &cPlayer::GetFlying)
		.Func("GetStance", &cPlayer::GetStance)
		//TODO .Func("GetInventory", &cPlayer::GetInventory)
		.Func("TeleportTo", &cPlayer::TeleportTo)
		.Func("GetGameMode", &cPlayer::GetGameMode)
		.Func("GetIP", &cPlayer::GetIP)
		.Func("GetLastBlockActionTime", &cPlayer::GetLastBlockActionTime)
		.Func("GetLastBlockActionCnt", &cPlayer::GetLastBlockActionCnt)
		.Func("SetLastBlockActionCnt", &cPlayer::SetLastBlockActionCnt)
		.Func("SetLastBlockActionTime", &cPlayer::SetLastBlockActionTime)
		.Func("SetGameMode", &cPlayer::SetGameMode)
		.Func("LoginSetGameMode", &cPlayer::LoginSetGameMode)
		.Func("SetIP", &cPlayer::SetIP)
		.Func("MoveTo", &cPlayer::MoveTo)
		.Func("GetClientHandle", &cPlayer::GetClientHandle)
		.Func("SendMessage", &cPlayer::SendMessage)
		.Func("AddToGroup", &cPlayer::AddToGroup)
		.Func("CanUseCommand", &cPlayer::CanUseCommand)
		.Func("HasPermission", &cPlayer::HasPermission)
		.Func("IsInGroup", &cPlayer::IsInGroup)
		.Func("GetColor", &cPlayer::GetColor)
		.Func("TossItem", &cPlayer::TossItem)
		.Func("Heal", &cPlayer::Heal)
		.Func("Feed", &cPlayer::Feed)
		.Func("TakeDamage", &cPlayer::TakeDamage)
		.Func("KilledBy", &cPlayer::KilledBy)
		.Func("Respawn", &cPlayer::Respawn)
		.Func("SetVisible", &cPlayer::SetVisible)
		.Func("IsVisible", &cPlayer::IsVisible)
		.Func("MoveToWorld", &cPlayer::MoveToWorld)
		.Func("GetLoadedWorldName", &cPlayer::GetLoadedWorldName)
		.Func("UseEquippedItem", &cPlayer::UseEquippedItem)
	);
	
	RootTable().Bind("StringArray", Class<SquirrelStringArray>()
		.Func("Get", &SquirrelStringArray::Get)
		.Func("Add", &SquirrelStringArray::Add)
		.Func("Size", &SquirrelStringArray::Size)
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

#endif