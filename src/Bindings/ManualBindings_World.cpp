
// ManualBindings_World.cpp

// Implements the manual Lua API bindings for the cWorld class

#include "Globals.h"
#include "tolua++/include/tolua++.h"
#include "../World.h"
#include "../Broadcaster.h"
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
	int ParticleAmmount;
	cClientHandle * ExcludeClient = nullptr;
	L.GetStackValues(1, World, Name, PosX, PosY, PosZ, OffX, OffY, OffZ, ParticleData, ParticleAmmount, ExcludeClient);
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

	World->GetBroadcaster().BroadcastParticleEffect(Name, Vector3f(PosX, PosY, PosZ), Vector3f(OffX, OffY, OffZ), ParticleData, ParticleAmmount, ExcludeClient);

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
	
	cPluginLua * Plugin = cManualBindings::GetLuaPlugin(tolua_S);
	if (Plugin == nullptr)
	{
		return 0;
	}
	
	// Read the params:
	cWorld * World = reinterpret_cast<cWorld *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (World == nullptr)
	{
		LOGWARNING("World:ChunkStay(): invalid world parameter");
		L.LogStackTrace();
		return 0;
	}

	cLuaChunkStay * ChunkStay = new cLuaChunkStay(*Plugin);

	if (!ChunkStay->AddChunks(2))
	{
		delete ChunkStay;
		ChunkStay = nullptr;
		return 0;
	}

	ChunkStay->Enable(*World->GetChunkMap(), 3, 4);
	return 0;
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
		L.Push(BlockType);
		L.Push(BlockMeta);
		L.Push(BlockSkyLight);
		L.Push(BlockBlockLight);
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
		L.Push(BlockType);
		L.Push(BlockMeta);
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
		L.Push(Line1);
		L.Push(Line2);
		L.Push(Line3);
		L.Push(Line4);
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
	
	// Read the params:
	cWorld * world = nullptr;
	int chunkX = 0;
	int chunkZ = 0;
	L.GetStackValues(1, world, chunkX, chunkZ);
	if (world == nullptr)
	{
		LOGWARNING("World:PrepareChunk(): invalid world parameter");
		L.LogStackTrace();
		return 0;
	}

	// Wrap the Lua callback inside a C++ callback class:
	class cCallback:
		public cChunkCoordCallback
	{
	public:
		cCallback(lua_State * a_LuaState):
			m_LuaState(a_LuaState),
			m_Callback(m_LuaState, 4)
		{
		}

		// cChunkCoordCallback override:
		virtual void Call(int a_CBChunkX, int a_CBChunkZ) override
		{
			if (m_Callback.IsValid())
			{
				m_LuaState.Call(m_Callback, a_CBChunkX, a_CBChunkZ);
			}

			// This is the last reference of this object, we must delete it so that it doesn't leak:
			delete this;
		}

	protected:
		cLuaState m_LuaState;
		cLuaState::cRef m_Callback;
	};

	// Call the chunk preparation:
	world->PrepareChunk(chunkX, chunkZ, cpp14::make_unique<cCallback>(tolua_S));
	return 0;
}





class cLuaWorldTask :
	public cPluginLua::cResettable
{
public:
	cLuaWorldTask(cPluginLua & a_Plugin, int a_FnRef) :
		cPluginLua::cResettable(a_Plugin),
		m_FnRef(a_FnRef)
	{
	}

	void Run(cWorld & a_World)
	{
		cCSLock Lock(m_CSPlugin);
		if (m_Plugin != nullptr)
		{
			m_Plugin->Call(m_FnRef, &a_World);
		}
	}

protected:
	int m_FnRef;
};





static int tolua_cWorld_QueueTask(lua_State * tolua_S)
{
	// Binding for cWorld::QueueTask
	// Params: function
	
	// Retrieve the cPlugin from the LuaState:
	cPluginLua * Plugin = cManualBindings::GetLuaPlugin(tolua_S);
	if (Plugin == nullptr)
	{
		// An error message has been already printed in GetLuaPlugin()
		return 0;
	}

	// Retrieve the args:
	cWorld * self = reinterpret_cast<cWorld *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Not called on an object instance");
	}
	if (!lua_isfunction(tolua_S, 2))
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a function for parameter #1");
	}

	// Create a reference to the function:
	int FnRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
	if (FnRef == LUA_REFNIL)
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Could not get function reference of parameter #1");
	}

	auto ResettableTask = std::make_shared<cLuaWorldTask>(*Plugin, FnRef);
	Plugin->AddResettable(ResettableTask);
	self->QueueTask(std::bind(&cLuaWorldTask::Run, ResettableTask, std::placeholders::_1));
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
	// Binding for cWorld::ScheduleTask
	// Params: function, Ticks
	
	// Retrieve the cPlugin from the LuaState:
	cPluginLua * Plugin = cManualBindings::GetLuaPlugin(tolua_S);
	if (Plugin == nullptr)
	{
		// An error message has been already printed in GetLuaPlugin()
		return 0;
	}

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
	cWorld * World = reinterpret_cast<cWorld *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (World == nullptr)
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Not called on an object instance");
	}

	// Create a reference to the function:
	int FnRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
	if (FnRef == LUA_REFNIL)
	{
		return cManualBindings::lua_do_error(tolua_S, "Error in function call '#funcname#': Could not get function reference of parameter #1");
	}
	
	auto ResettableTask = std::make_shared<cLuaWorldTask>(*Plugin, FnRef);
	Plugin->AddResettable(ResettableTask);
	World->ScheduleTask(static_cast<int>(tolua_tonumber(tolua_S, 2, 0)), std::bind(&cLuaWorldTask::Run, ResettableTask, std::placeholders::_1));
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
			tolua_function(tolua_S, "BroadcastParticleEffect",   tolua_cWorld_BroadcastParticleEffect);
			tolua_function(tolua_S, "ChunkStay",                 tolua_cWorld_ChunkStay);
			tolua_function(tolua_S, "DoWithBlockEntityAt",       DoWithXYZ<cWorld, cBlockEntity,        &cWorld::DoWithBlockEntityAt>);
			tolua_function(tolua_S, "DoWithBeaconAt",            DoWithXYZ<cWorld, cBeaconEntity,       &cWorld::DoWithBeaconAt>);
			tolua_function(tolua_S, "DoWithChestAt",             DoWithXYZ<cWorld, cChestEntity,        &cWorld::DoWithChestAt>);
			tolua_function(tolua_S, "DoWithDispenserAt",         DoWithXYZ<cWorld, cDispenserEntity,    &cWorld::DoWithDispenserAt>);
			tolua_function(tolua_S, "DoWithDropSpenserAt",       DoWithXYZ<cWorld, cDropSpenserEntity,  &cWorld::DoWithDropSpenserAt>);
			tolua_function(tolua_S, "DoWithDropperAt",           DoWithXYZ<cWorld, cDropperEntity,      &cWorld::DoWithDropperAt>);
			tolua_function(tolua_S, "DoWithEntityByID",          DoWithID< cWorld, cEntity,             &cWorld::DoWithEntityByID>);
			tolua_function(tolua_S, "DoWithFurnaceAt",           DoWithXYZ<cWorld, cFurnaceEntity,      &cWorld::DoWithFurnaceAt>);
			tolua_function(tolua_S, "DoWithNoteBlockAt",         DoWithXYZ<cWorld, cNoteEntity,         &cWorld::DoWithNoteBlockAt>);
			tolua_function(tolua_S, "DoWithCommandBlockAt",      DoWithXYZ<cWorld, cCommandBlockEntity, &cWorld::DoWithCommandBlockAt>);
			tolua_function(tolua_S, "DoWithMobHeadAt",           DoWithXYZ<cWorld, cMobHeadEntity,      &cWorld::DoWithMobHeadAt>);
			tolua_function(tolua_S, "DoWithFlowerPotAt",         DoWithXYZ<cWorld, cFlowerPotEntity,    &cWorld::DoWithFlowerPotAt>);
			tolua_function(tolua_S, "DoWithPlayer",              DoWith<   cWorld, cPlayer,             &cWorld::DoWithPlayer>);
			tolua_function(tolua_S, "FindAndDoWithPlayer",       DoWith<   cWorld, cPlayer,             &cWorld::FindAndDoWithPlayer>);
			tolua_function(tolua_S, "DoWithPlayerByUUID",        DoWith<   cWorld, cPlayer,             &cWorld::DoWithPlayerByUUID>);
			tolua_function(tolua_S, "ForEachBlockEntityInChunk", ForEachInChunk<cWorld, cBlockEntity,   &cWorld::ForEachBlockEntityInChunk>);
			tolua_function(tolua_S, "ForEachChestInChunk",       ForEachInChunk<cWorld, cChestEntity,   &cWorld::ForEachChestInChunk>);
			tolua_function(tolua_S, "ForEachEntity",             ForEach<       cWorld, cEntity,        &cWorld::ForEachEntity>);
			tolua_function(tolua_S, "ForEachEntityInBox",        ForEachInBox<  cWorld, cEntity,        &cWorld::ForEachEntityInBox>);
			tolua_function(tolua_S, "ForEachEntityInChunk",      ForEachInChunk<cWorld, cEntity,        &cWorld::ForEachEntityInChunk>);
			tolua_function(tolua_S, "ForEachFurnaceInChunk",     ForEachInChunk<cWorld, cFurnaceEntity, &cWorld::ForEachFurnaceInChunk>);
			tolua_function(tolua_S, "ForEachPlayer",             ForEach<       cWorld, cPlayer,        &cWorld::ForEachPlayer>);
			tolua_function(tolua_S, "ForEachLoadedChunk",        tolua_cWorld_ForEachLoadedChunk);
			tolua_function(tolua_S, "GetBlockInfo",              tolua_cWorld_GetBlockInfo);
			tolua_function(tolua_S, "GetBlockTypeMeta",          tolua_cWorld_GetBlockTypeMeta);
			tolua_function(tolua_S, "GetSignLines",              tolua_cWorld_GetSignLines);
			tolua_function(tolua_S, "PrepareChunk",              tolua_cWorld_PrepareChunk);
			tolua_function(tolua_S, "QueueTask",                 tolua_cWorld_QueueTask);
			tolua_function(tolua_S, "ScheduleTask",              tolua_cWorld_ScheduleTask);
			tolua_function(tolua_S, "SetSignLines",              tolua_cWorld_SetSignLines);
			tolua_function(tolua_S, "TryGetHeight",              tolua_cWorld_TryGetHeight);
		tolua_endmodule(tolua_S);
	tolua_endmodule(tolua_S);
}




