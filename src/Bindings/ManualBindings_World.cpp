
// ManualBindings_World.cpp

// Implements the manual Lua API bindings for the cWorld class

#include "Globals.h"
#include "tolua++/include/tolua++.h"
#include "../World.h"
#include "../Broadcaster.h"
#include "../UUID.h"
#include "ManualBindings.h"
#include "LuaState.h"
#include "PluginLua.h"
#include "LuaChunkStay.h"





static int tolua_cWorld_BroadcastParticleEffect(lua_State * tolua_S)
{
	/* Function signature:
	World:BroadcastParticleEffect("Name", PosX, PosY, PosZ, OffX, OffY, OffZ, ParticleData, ParticleAmount, [ExcludeClient], [OptionalParam1], [OptionalParam2]
	*/
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cWorld") ||
		!L.CheckParamString  (2) ||
		!L.CheckParamNumber  (3, 10)
	)
	{
		return 0;
	}

	// Read the params:
	cWorld * World = nullptr;
	AString Name;
	float PosX, PosY, PosZ, OffX, OffY, OffZ;
	float ParticleData;
	int ParticleAmount;
	cClientHandle * ExcludeClient = nullptr;
	L.GetStackValues(1, World, Name, PosX, PosY, PosZ, OffX, OffY, OffZ, ParticleData, ParticleAmount, ExcludeClient);
	if (World == nullptr)
	{
		LOGWARNING("World:BroadcastParticleEffect(): invalid world parameter");
		L.LogStackTrace();
		return 0;
	}

	// Read up to 2 more optional data params:
	std::array<int, 2> data;
	for (int i = 0; (i < 2) && L.IsParamNumber(11 + i); i++)
	{
		L.GetStackValue(11 + i, data[static_cast<size_t>(i)]);
	}

	World->GetBroadcaster().BroadcastParticleEffect(Name, Vector3f(PosX, PosY, PosZ), Vector3f(OffX, OffY, OffZ), ParticleData, ParticleAmount, ExcludeClient);

	return 0;
}





static int tolua_cWorld_ChunkStay(lua_State * tolua_S)
{
	/* Function signature:
	World:ChunkStay(ChunkCoordTable, OnChunkAvailable, OnAllChunksAvailable)
	ChunkCoordTable == { {Chunk1x, Chunk1z}, {Chunk2x, Chunk2z}, ... }
	*/

	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType     (1, "cWorld") ||
		!L.CheckParamTable        (2) ||
		!L.CheckParamFunctionOrNil(3, 4)
	)
	{
		return 0;
	}

	// Read the params:
	cWorld * world;
	cLuaState::cStackTablePtr chunkCoords;
	cLuaState::cOptionalCallbackPtr onChunkAvailable, onAllChunksAvailable;  // Callbacks may be unassigned at all - as a request to load / generate chunks
	if (!L.GetStackValues(1, world, chunkCoords, onChunkAvailable, onAllChunksAvailable))
	{
		LOGWARNING("cWorld:ChunkStay(): Cannot read parameters, bailing out.");
		L.LogStackTrace();
		L.LogStackValues("Values on the stack");
		return 0;
	}
	if (world == nullptr)
	{
		LOGWARNING("World:ChunkStay(): invalid world parameter");
		L.LogStackTrace();
		return 0;
	}
	ASSERT(chunkCoords != nullptr);  // If the table was invalid, GetStackValues() would have failed

	// Read the chunk coords:
	auto chunkStay = cpp14::make_unique<cLuaChunkStay>();
	if (!chunkStay->AddChunks(*chunkCoords))
	{
		return 0;
	}

	// Activate the ChunkStay:
	chunkStay.release()->Enable(*world->GetChunkMap(), std::move(onChunkAvailable), std::move(onAllChunksAvailable));
	return 0;
}





static int tolua_cWorld_DoExplosionAt(lua_State * tolua_S)
{
	/* Function signature:
	World:DoExplosionAt(ExplosionSize, BlockX, BlockY, BlockZ, CanCauseFire, SourceType, [SourceData])
	*/

	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType     (1, "cWorld") ||
		!L.CheckParamNumber       (2, 5) ||
		!L.CheckParamBool         (6) ||
		!L.CheckParamNumber       (7) ||
		!L.CheckParamEnd          (9)
	)
	{
		return 0;
	}

	// Read the params:
	cWorld * World;
	double ExplosionSize;
	int BlockX, BlockY, BlockZ;
	bool CanCauseFire;
	int SourceTypeInt;
	if (!L.GetStackValues(1, World, ExplosionSize, BlockX, BlockY, BlockZ, CanCauseFire, SourceTypeInt))
	{
		LOGWARNING("World:DoExplosionAt(): invalid parameters");
		L.LogStackTrace();
		return 0;
	}
	if ((SourceTypeInt < 0) || (SourceTypeInt >= esMax))
	{
		LOGWARNING("World:DoExplosionAt(): Invalid source type");
		L.LogStackTrace();
		return 0;
	}
	eExplosionSource SourceType;
	void * SourceData;
	switch (SourceTypeInt)
	{
		case esBed:
		{
			// esBed receives a Vector3i SourceData param:
			Vector3i * pos = nullptr;
			L.GetStackValue(8, pos);
			SourceType = esBed;
			SourceData = pos;
			break;
		}

		case esEnderCrystal:
		case esGhastFireball:
		case esMonster:
		case esPrimedTNT:
		case esWitherBirth:
		case esWitherSkull:
		{
			// These all receive a cEntity descendant SourceData param:
			cEntity * ent = nullptr;
			L.GetStackValue(8, ent);
			SourceType = static_cast<eExplosionSource>(SourceTypeInt);
			SourceData = ent;
			break;
		}

		case esOther:
		case esPlugin:
		{
			// esOther and esPlugin ignore their SourceData params
			SourceType = static_cast<eExplosionSource>(SourceTypeInt);
			SourceData = nullptr;
			break;
		}

		default:
		{
			LOGWARNING("cWorld:DoExplosionAt(): invalid SourceType parameter: %d", SourceTypeInt);
			L.LogStackTrace();
			return 0;
		}
	}

	// Create the actual explosion:
	World->DoExplosionAt(ExplosionSize, BlockX, BlockY, BlockZ, CanCauseFire, SourceType, SourceData);

	return 0;
}





static int tolua_cWorld_DoWithPlayerByUUID(lua_State * tolua_S)
{
	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamSelf("cWorld") ||
		!L.CheckParamUUID(2) ||
		!L.CheckParamFunction(3) ||
		!L.CheckParamEnd(4)
	)
	{
		return 0;
	}

	// Get parameters:
	cWorld * Self;
	cUUID PlayerUUID;
	cLuaState::cRef FnRef;
	L.GetStackValues(1, Self, PlayerUUID, FnRef);

	if (PlayerUUID.IsNil())
	{
		return L.ApiParamError("Expected a non-nil UUID for parameter #1");
	}
	if (!FnRef.IsValid())
	{
		return L.ApiParamError("Expected a valid callback function for parameter #2");
	}

	// Call the function:
	bool res = Self->DoWithPlayerByUUID(PlayerUUID, [&](cPlayer & a_Player)
		{
			bool ret = false;
			L.Call(FnRef, &a_Player, cLuaState::Return, ret);
			return ret;
		}
	);

	// Push the result as the return value:
	L.Push(res);
	return 1;
}





static int tolua_cWorld_ForEachLoadedChunk(lua_State * tolua_S)
{
	// Exported manually, because tolua doesn't support converting functions to functor types.
	// Function signature: ForEachLoadedChunk(callback) -> bool

	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cWorld") ||
		!L.CheckParamFunction(2)
		)
	{
		return 0;
	}

	cPluginLua * Plugin = cManualBindings::GetLuaPlugin(tolua_S);
	if (Plugin == nullptr)
	{
		return 0;
	}

	// Read the params:
	cWorld * World = reinterpret_cast<cWorld *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (World == nullptr)
	{
		LOGWARNING("World:ForEachLoadedChunk(): invalid world parameter");
		L.LogStackTrace();
		return 0;
	}
	cLuaState::cRef FnRef;
	L.GetStackValues(2, FnRef);
	if (!FnRef.IsValid())
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Could not get function reference of parameter #2");
	}

	// Call the enumeration:
	bool ret = World->ForEachLoadedChunk(
		[&L, &FnRef](int a_ChunkX, int a_ChunkZ) -> bool
		{
			bool res = false;  // By default continue the enumeration
			L.Call(FnRef, a_ChunkX, a_ChunkZ, cLuaState::Return, res);
			return res;
		}
	);

	// Push the return value:
	L.Push(ret);
	return 1;
}





static int tolua_cWorld_GetBlockInfo(lua_State * tolua_S)
{
	// Exported manually, because tolua would generate useless additional parameters (a_BlockType .. a_BlockSkyLight)
	// Function signature: GetBlockInfo(BlockX, BlockY, BlockZ) -> BlockValid, [BlockType, BlockMeta, BlockSkyLight, BlockBlockLight]

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cWorld") ||
		!L.CheckParamNumber(2, 4) ||
		!L.CheckParamEnd(5)
	)
	{
		return 0;
	}

	// Get params:
	cWorld * Self = nullptr;
	int BlockX = 0;
	int BlockY = 0;
	int BlockZ = 0;
	L.GetStackValues(1, Self, BlockX, BlockY, BlockZ);
	if (Self == nullptr)
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Invalid 'self'");
	}

	// Call the function:
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta, BlockSkyLight, BlockBlockLight;
	bool res = Self->GetBlockInfo(BlockX, BlockY, BlockZ, BlockType, BlockMeta, BlockSkyLight, BlockBlockLight);

	// Push the returned values:
	L.Push(res);
	if (res)
	{
		L.Push(BlockType, BlockMeta, BlockSkyLight, BlockBlockLight);
		return 5;
	}
	return 1;
}





static int tolua_cWorld_GetBlockTypeMeta(lua_State * tolua_S)
{
	// Exported manually, because tolua would generate useless additional parameters (a_BlockType, a_BlockMeta)
	// Function signature: GetBlockTypeMeta(BlockX, BlockY, BlockZ) -> BlockValid, [BlockType, BlockMeta]

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cWorld") ||
		!L.CheckParamNumber(2, 4) ||
		!L.CheckParamEnd(5)
	)
	{
		return 0;
	}

	// Get params:
	cWorld * Self = nullptr;
	int BlockX = 0;
	int BlockY = 0;
	int BlockZ = 0;
	L.GetStackValues(1, Self, BlockX, BlockY, BlockZ);
	if (Self == nullptr)
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Invalid 'self'");
	}

	// Call the function:
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	bool res = Self->GetBlockTypeMeta(BlockX, BlockY, BlockZ, BlockType, BlockMeta);

	// Push the returned values:
	L.Push(res);
	if (res)
	{
		L.Push(BlockType, BlockMeta);
		return 3;
	}
	return 1;
}





static int tolua_cWorld_GetSignLines(lua_State * tolua_S)
{
	// Exported manually, because tolua would generate useless additional parameters (a_Line1 .. a_Line4)

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cWorld") ||
		!L.CheckParamNumber(2, 4) ||
		!L.CheckParamEnd(5)
	)
	{
		return 0;
	}

	// Get params:
	cWorld * Self = nullptr;
	int BlockX = 0;
	int BlockY = 0;
	int BlockZ = 0;
	L.GetStackValues(1, Self, BlockX, BlockY, BlockZ);
	if (Self == nullptr)
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Invalid 'self'");
	}

	// Call the function:
	AString Line1, Line2, Line3, Line4;
	bool res = Self->GetSignLines(BlockX, BlockY, BlockZ, Line1, Line2, Line3, Line4);

	// Push the returned values:
	L.Push(res);
	if (res)
	{
		L.Push(Line1, Line2, Line3, Line4);
		return 5;
	}
	return 1;
}





static int tolua_cWorld_PrepareChunk(lua_State * tolua_S)
{
	/* Function signature:
	World:PrepareChunk(ChunkX, ChunkZ, Callback)
	*/

	// Check the param types:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType     (1, "cWorld") ||
		!L.CheckParamNumber       (2, 3) ||
		!L.CheckParamFunctionOrNil(4)
	)
	{
		return 0;
	}

	// Wrap the Lua callback inside a C++ callback class:
	class cCallback:
		public cChunkCoordCallback
	{
	public:
		// cChunkCoordCallback override:
		virtual void Call(int a_CBChunkX, int a_CBChunkZ, bool a_IsSuccess) override
		{
			m_LuaCallback.Call(a_CBChunkX, a_CBChunkZ, a_IsSuccess);
		}

		cLuaState::cOptionalCallback m_LuaCallback;
	};

	// Read the params:
	cWorld * world = nullptr;
	int chunkX = 0;
	int chunkZ = 0;
	auto Callback = cpp14::make_unique<cCallback>();
	L.GetStackValues(1, world, chunkX, chunkZ, Callback->m_LuaCallback);
	if (world == nullptr)
	{
		LOGWARNING("World:PrepareChunk(): invalid world parameter");
		L.LogStackTrace();
		return 0;
	}

	// Call the chunk preparation:
	world->PrepareChunk(chunkX, chunkZ, std::move(Callback));
	return 0;
}





static int tolua_cWorld_QueueTask(lua_State * tolua_S)
{
	// Function signature:
	// World:QueueTask(Callback)

	// Retrieve the args:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cWorld") ||
		!L.CheckParamFunction(2)
	)
	{
		return 0;
	}
	cWorld * World;
	cLuaState::cCallbackSharedPtr Task;
	if (!L.GetStackValues(1, World, Task))
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Cannot read parameters");
	}
	if (World == nullptr)
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Not called on an object instance");
	}
	if (!Task->IsValid())
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Could not store the callback parameter");
	}

	World->QueueTask([Task](cWorld & a_World)
		{
			Task->Call(&a_World);
		}
	);
	return 0;
}





static int tolua_cWorld_SetSignLines(lua_State * tolua_S)
{
	// Exported manually, because tolua would generate useless additional return values (a_Line1 .. a_Line4)

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cWorld") ||
		!L.CheckParamNumber(2, 4) ||
		!L.CheckParamString(5, 8) ||
		!L.CheckParamEnd(9)
	)
	{
		return 0;
	}

	// Get params:
	cWorld * Self = nullptr;
	int BlockX = 0;
	int BlockY = 0;
	int BlockZ = 0;
	AString Line1, Line2, Line3, Line4;
	L.GetStackValues(1, Self, BlockX, BlockY, BlockZ, Line1, Line2, Line3, Line4);
	if (Self == nullptr)
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Invalid 'self'");
	}

	// Call the function:
	bool res = Self->SetSignLines(BlockX, BlockY, BlockZ, Line1, Line2, Line3, Line4);

	// Push the returned values:
	L.Push(res);
	return 1;
}





static int tolua_cWorld_ScheduleTask(lua_State * tolua_S)
{
	// Function signature:
	// World:ScheduleTask(NumTicks, Callback)

	// Retrieve the args:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cWorld") ||
		!L.CheckParamNumber  (2) ||
		!L.CheckParamFunction(3)
	)
	{
		return 0;
	}
	cWorld * World;
	int NumTicks;
	auto Task = std::make_shared<cLuaState::cCallback>();
	if (!L.GetStackValues(1, World, NumTicks, Task))
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Cannot read parameters");
	}
	if (World == nullptr)
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Not called on an object instance");
	}
	if (!Task->IsValid())
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Could not store the callback parameter");
	}

	World->ScheduleTask(NumTicks, [Task](cWorld & a_World)
		{
			Task->Call(&a_World);
		}
	);
	return 0;
}






static int tolua_cWorld_TryGetHeight(lua_State * tolua_S)
{
	/* Exported manually, because tolua would require the out-only param a_Height to be used when calling
	Function signature: world:TryGetHeight(a_World, a_BlockX, a_BlockZ) -> IsValid, Height
	*/

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cWorld") ||
		!L.CheckParamNumber(2, 3) ||
		!L.CheckParamEnd(4)
	)
	{
		return 0;
	}

	// Get params:
	cWorld * self = nullptr;
	int BlockX = 0;
	int BlockZ = 0;
	L.GetStackValues(1, self, BlockX, BlockZ);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "Invalid 'self' in function 'TryGetHeight'", nullptr);
		return 0;
	}

	// Call the implementation:
	int Height = 0;
	bool res = self->TryGetHeight(BlockX, BlockZ, Height);
	L.Push(res);
	if (res)
	{
		L.Push(Height);
		return 2;
	}
	return 1;
}





void cManualBindings::BindWorld(lua_State * tolua_S)
{
	tolua_beginmodule(tolua_S, nullptr);
		tolua_beginmodule(tolua_S, "cWorld");
			tolua_function(tolua_S, "BroadcastParticleEffect",    tolua_cWorld_BroadcastParticleEffect);
			tolua_function(tolua_S, "ChunkStay",                  tolua_cWorld_ChunkStay);
			tolua_function(tolua_S, "DoExplosionAt",              tolua_cWorld_DoExplosionAt);
			tolua_function(tolua_S, "DoWithBeaconAt",             DoWithXYZ<cWorld, cBeaconEntity,       &cWorld::DoWithBeaconAt>);
			tolua_function(tolua_S, "DoWithBedAt",                DoWithXYZ<cWorld, cBedEntity,          &cWorld::DoWithBedAt>);
			tolua_function(tolua_S, "DoWithBlockEntityAt",        DoWithXYZ<cWorld, cBlockEntity,        &cWorld::DoWithBlockEntityAt>);
			tolua_function(tolua_S, "DoWithBrewingstandAt",       DoWithXYZ<cWorld, cBrewingstandEntity, &cWorld::DoWithBrewingstandAt>);
			tolua_function(tolua_S, "DoWithChestAt",              DoWithXYZ<cWorld, cChestEntity,        &cWorld::DoWithChestAt>);
			tolua_function(tolua_S, "DoWithCommandBlockAt",       DoWithXYZ<cWorld, cCommandBlockEntity, &cWorld::DoWithCommandBlockAt>);
			tolua_function(tolua_S, "DoWithDispenserAt",          DoWithXYZ<cWorld, cDispenserEntity,    &cWorld::DoWithDispenserAt>);
			tolua_function(tolua_S, "DoWithDropSpenserAt",        DoWithXYZ<cWorld, cDropSpenserEntity,  &cWorld::DoWithDropSpenserAt>);
			tolua_function(tolua_S, "DoWithDropperAt",            DoWithXYZ<cWorld, cDropperEntity,      &cWorld::DoWithDropperAt>);
			tolua_function(tolua_S, "DoWithEntityByID",           DoWithID< cWorld, cEntity,             &cWorld::DoWithEntityByID>);
			tolua_function(tolua_S, "DoWithFlowerPotAt",          DoWithXYZ<cWorld, cFlowerPotEntity,    &cWorld::DoWithFlowerPotAt>);
			tolua_function(tolua_S, "DoWithFurnaceAt",            DoWithXYZ<cWorld, cFurnaceEntity,      &cWorld::DoWithFurnaceAt>);
			tolua_function(tolua_S, "DoWithMobHeadAt",            DoWithXYZ<cWorld, cMobHeadEntity,      &cWorld::DoWithMobHeadAt>);
			tolua_function(tolua_S, "DoWithNoteBlockAt",          DoWithXYZ<cWorld, cNoteEntity,         &cWorld::DoWithNoteBlockAt>);
			tolua_function(tolua_S, "DoWithPlayer",               DoWith<   cWorld, cPlayer,             &cWorld::DoWithPlayer>);
			tolua_function(tolua_S, "DoWithPlayerByUUID",         tolua_cWorld_DoWithPlayerByUUID);
			tolua_function(tolua_S, "FindAndDoWithPlayer",        DoWith<   cWorld, cPlayer,             &cWorld::FindAndDoWithPlayer>);
			tolua_function(tolua_S, "ForEachBlockEntityInChunk",  ForEachInChunk<cWorld, cBlockEntity,   &cWorld::ForEachBlockEntityInChunk>);
			tolua_function(tolua_S, "ForEachBrewingstandInChunk", ForEachInChunk<cWorld, cBrewingstandEntity, &cWorld::ForEachBrewingstandInChunk>);
			tolua_function(tolua_S, "ForEachChestInChunk",        ForEachInChunk<cWorld, cChestEntity,   &cWorld::ForEachChestInChunk>);
			tolua_function(tolua_S, "ForEachEntity",              ForEach<       cWorld, cEntity,        &cWorld::ForEachEntity>);
			tolua_function(tolua_S, "ForEachEntityInBox",         ForEachInBox<  cWorld, cEntity,        &cWorld::ForEachEntityInBox>);
			tolua_function(tolua_S, "ForEachEntityInChunk",       ForEachInChunk<cWorld, cEntity,        &cWorld::ForEachEntityInChunk>);
			tolua_function(tolua_S, "ForEachFurnaceInChunk",      ForEachInChunk<cWorld, cFurnaceEntity, &cWorld::ForEachFurnaceInChunk>);
			tolua_function(tolua_S, "ForEachLoadedChunk",         tolua_cWorld_ForEachLoadedChunk);
			tolua_function(tolua_S, "ForEachPlayer",              ForEach<       cWorld, cPlayer,        &cWorld::ForEachPlayer>);
			tolua_function(tolua_S, "GetBlockInfo",               tolua_cWorld_GetBlockInfo);
			tolua_function(tolua_S, "GetBlockTypeMeta",           tolua_cWorld_GetBlockTypeMeta);
			tolua_function(tolua_S, "GetSignLines",               tolua_cWorld_GetSignLines);
			tolua_function(tolua_S, "PrepareChunk",               tolua_cWorld_PrepareChunk);
			tolua_function(tolua_S, "QueueTask",                  tolua_cWorld_QueueTask);
			tolua_function(tolua_S, "ScheduleTask",               tolua_cWorld_ScheduleTask);
			tolua_function(tolua_S, "SetSignLines",               tolua_cWorld_SetSignLines);
			tolua_function(tolua_S, "TryGetHeight",               tolua_cWorld_TryGetHeight);
		tolua_endmodule(tolua_S);
	tolua_endmodule(tolua_S);
}




