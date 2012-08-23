
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#define LUA_USE_POSIX
#include "cPlugin_NewLua.h"
#include "cMCLogger.h"
#include "LuaItems.h"

extern "C"
{
#include "lualib.h"
}

#include "tolua++.h"
#include "Bindings.h"
#include "ManualBindings.h"

#ifdef _WIN32
// #include "wdirent.h"
#else
#include <dirent.h>
#endif





extern bool report_errors(lua_State* lua, int status);





cPlugin_NewLua::cPlugin_NewLua( const char* a_PluginName )
	: m_LuaState( 0 )
	, cWebPlugin()
{
	m_Directory = a_PluginName;
}





cPlugin_NewLua::~cPlugin_NewLua()
{
	cCSLock Lock( m_CriticalSection );

	if( m_LuaState )
	{
		lua_close( m_LuaState );
		m_LuaState = 0;
	}
}





bool cPlugin_NewLua::Initialize()
{
	cCSLock Lock( m_CriticalSection );
	if( !m_LuaState ) 
	{	
		m_LuaState = lua_open();
		luaL_openlibs( m_LuaState );
		tolua_AllToLua_open(m_LuaState);
		ManualBindings::Bind( m_LuaState );
	}

	std::string PluginPath = FILE_IO_PREFIX + std::string("Plugins/") + m_Directory + "/";

	// Load all files for this plugin, and execute them
	AStringList Files = GetDirectoryContents(PluginPath.c_str());
	for (AStringList::const_iterator itr = Files.begin(); itr != Files.end(); ++itr)
	{
		if (itr->rfind(".lua") == AString::npos)
		{
			continue;
		}
		AString Path = PluginPath + *itr;
		int s = luaL_loadfile(m_LuaState, Path.c_str() );
		if( report_errors( m_LuaState, s ) )
		{
			LOGERROR("Can't load plugin %s because of an error in file %s", m_Directory.c_str(), Path.c_str() );
			lua_close( m_LuaState );
			m_LuaState = 0;
			return false;
		}

		s = lua_pcall(m_LuaState, 0, LUA_MULTRET, 0);
		if( report_errors( m_LuaState, s ) )
		{
			LOGERROR("Error in plugin %s in file %s", m_Directory.c_str(), Path.c_str() );
			lua_close( m_LuaState );
			m_LuaState = 0;
			return false;
		}
	}  // for itr - Files[]

	// Call intialize function
	if( !PushFunction("Initialize") )
	{
		lua_close( m_LuaState );
		m_LuaState = 0;
		return false;
	}

	tolua_pushusertype(m_LuaState, this, "cPlugin_NewLua");
	
	if( !CallFunction(1, 1, "Initialize") ) 
	{
		lua_close( m_LuaState );
		m_LuaState = 0;
		return false;
	}

	if( !lua_isboolean( m_LuaState, -1 ) )
	{
		LOGWARN("Error in plugin %s Initialize() must return a boolean value!", m_Directory.c_str() );
		lua_close( m_LuaState );
		m_LuaState = 0;
		return false;
	}

	bool bSuccess = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bSuccess;
}





void cPlugin_NewLua::OnDisable()
{
	cCSLock Lock( m_CriticalSection );
	if( !PushFunction("OnDisable", false) ) // false = don't log error if not found
		return;

	CallFunction(0, 0, "OnDisable");
}





void cPlugin_NewLua::Tick(float a_Dt)
{
	cCSLock Lock( m_CriticalSection );
	if( !PushFunction("Tick") )
		return;

	tolua_pushnumber( m_LuaState, a_Dt );

	CallFunction(1, 0, "Tick");
}





bool cPlugin_NewLua::OnCollectItem( cPickup* a_Pickup, cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );
	if( !PushFunction("OnCollectItem") )
		return false;

	tolua_pushusertype(m_LuaState, a_Pickup, "cPickup");
	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");

	if( !CallFunction(2, 1, "OnCollectItem") )
		return false;

	bool bRetVal = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bRetVal;
}





bool cPlugin_NewLua::OnDisconnect(cPlayer * a_Player, const AString & a_Reason)
{
	cCSLock Lock(m_CriticalSection);
	if (!PushFunction("OnDisconnect"))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");
	tolua_pushstring  (m_LuaState, a_Reason.c_str());

	if (!CallFunction(2, 1, "OnDisconnect"))
	{
		return false;
	}

	return (tolua_toboolean( m_LuaState, -1, 0) > 0);
}





bool cPlugin_NewLua::OnBlockPlace(cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, const cItem & a_HeldItem)
{
	cCSLock Lock(m_CriticalSection);
	if (!PushFunction("OnBlockPlace"))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushnumber  (m_LuaState, a_BlockFace);
	tolua_pushusertype(m_LuaState, (void *)&a_HeldItem, "cItem");

	if (!CallFunction(6, 1, "OnBlockPlace"))
	{
		return false;
	}

	return (tolua_toboolean( m_LuaState, -1, 0) > 0);
}





bool cPlugin_NewLua::OnBlockDig(cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status, BLOCKTYPE a_OldBlock, NIBBLETYPE a_OldMeta)
{
	cCSLock Lock(m_CriticalSection);
	if (!PushFunction("OnBlockDig"))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushnumber  (m_LuaState, a_BlockFace);
	tolua_pushnumber  (m_LuaState, a_Status);
	tolua_pushnumber  (m_LuaState, a_OldBlock);
	tolua_pushnumber  (m_LuaState, a_OldMeta);

	if (!CallFunction(8, 1, "OnBlockDig"))
	{
		return false;
	}

	return (tolua_toboolean( m_LuaState, -1, 0) > 0);
}





bool cPlugin_NewLua::OnChat(cPlayer * a_Player, const AString & a_Message)
{
	cCSLock Lock(m_CriticalSection);
	if (!PushFunction("OnChat"))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");
	tolua_pushstring  (m_LuaState, a_Message.c_str());

	if (!CallFunction(2, 1, "OnChat"))
	{
		return false;
	}

	return (tolua_toboolean( m_LuaState, -1, 0) > 0);
}





bool cPlugin_NewLua::OnLogin(cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username)
{
	cCSLock Lock( m_CriticalSection );
	if( !PushFunction("OnLogin") )
		return false;

	tolua_pushusertype (m_LuaState, a_Client, "cClientHandle");
	tolua_pushnumber   (m_LuaState, a_ProtocolVersion);
	tolua_pushcppstring(m_LuaState, a_Username);

	if (!CallFunction(3, 1, "OnLogin"))
		return false;

	bool bRetVal = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bRetVal;
}





void cPlugin_NewLua::OnPlayerSpawn( cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );
	if( !PushFunction("OnPlayerSpawn") )
		return;

	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");

	CallFunction(1, 0, "OnPlayerSpawn");
}





bool cPlugin_NewLua::OnPlayerJoin( cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );
	if( !PushFunction("OnPlayerJoin") )
		return false;

	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");

	if( !CallFunction(1, 1, "OnPlayerJoin") )
		return false;

	bool bRetVal = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bRetVal;
}





void cPlugin_NewLua::OnPlayerMove( cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );
	if( !PushFunction("OnPlayerMove") )
		return;

	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");

	CallFunction(1, 0, "OnPlayerMove");
}





void cPlugin_NewLua::OnTakeDamage( cPawn* a_Pawn, TakeDamageInfo* a_TakeDamageInfo )
{
	cCSLock Lock( m_CriticalSection );
	if( !PushFunction("OnTakeDamage") )
		return;

	tolua_pushusertype(m_LuaState, a_Pawn, "cPawn");
	tolua_pushusertype(m_LuaState, a_TakeDamageInfo, "TakeDamageInfo");

	CallFunction(2, 0, "OnTakeDamage");
}





bool cPlugin_NewLua::OnKilled( cPawn* a_Killed, cEntity* a_Killer )
{
	cCSLock Lock( m_CriticalSection );
	if( !PushFunction("OnKilled") )
		return false;

	tolua_pushusertype(m_LuaState, a_Killed, "cPawn");
	tolua_pushusertype(m_LuaState, a_Killer, "cEntity");

	if( !CallFunction(2, 1, "OnKilled") )
		return false;

	bool bRetVal = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bRetVal;
}





void cPlugin_NewLua::OnChunkGenerated(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CriticalSection);
	if (!PushFunction("OnChunkGenerated"))
	{
		return;
	}
	
	tolua_pushusertype(m_LuaState, a_World, "cWorld");
	tolua_pushnumber  (m_LuaState, a_ChunkX);
	tolua_pushnumber  (m_LuaState, a_ChunkZ);
	
	CallFunction(3, 0, "OnChunkGenerated");
}





bool cPlugin_NewLua::OnChunkGenerating(cWorld * a_World, int a_ChunkX, int a_ChunkZ, cLuaChunk * a_pLuaChunk)
{
	cCSLock Lock(m_CriticalSection);
	if (!PushFunction("OnChunkGenerating"))
		return false;

	tolua_pushusertype(m_LuaState, a_World,     "cWorld");
	tolua_pushnumber  (m_LuaState, a_ChunkX);
	tolua_pushnumber  (m_LuaState, a_ChunkZ);
	tolua_pushusertype(m_LuaState, a_pLuaChunk, "cLuaChunk");

	if( !CallFunction(3, 1, "OnChunkGenerating") )
		return false;

	bool bRetVal = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bRetVal;
}





bool cPlugin_NewLua::OnPreCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	cCSLock Lock(m_CriticalSection);
	if (!PushFunction("OnPreCrafting"))
		return false;

	tolua_pushusertype(m_LuaState, (void *)a_Player, "cPlayer");
	tolua_pushusertype(m_LuaState, (void *)a_Grid,   "cCraftingGrid");
	tolua_pushusertype(m_LuaState, (void *)a_Recipe, "cCraftingRecipe");

	if (!CallFunction(3, 1, "OnPreCrafting"))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bRetVal;
}





bool cPlugin_NewLua::OnCraftingNoRecipe(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	cCSLock Lock(m_CriticalSection);
	if (!PushFunction("OnCraftingNoRecipe"))
		return false;

	tolua_pushusertype(m_LuaState, (void *)a_Player, "cPlayer");
	tolua_pushusertype(m_LuaState, (void *)a_Grid,   "cCraftingGrid");
	tolua_pushusertype(m_LuaState, (void *)a_Recipe, "cCraftingRecipe");

	if (!CallFunction(3, 1, "OnCraftingNoRecipe"))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bRetVal;
}





bool cPlugin_NewLua::OnPostCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	cCSLock Lock(m_CriticalSection);
	if (!PushFunction("OnPostCrafting"))
		return false;

	tolua_pushusertype(m_LuaState, (void *)a_Player, "cPlayer");
	tolua_pushusertype(m_LuaState, (void *)a_Grid,   "cCraftingGrid");
	tolua_pushusertype(m_LuaState, (void *)a_Recipe, "cCraftingRecipe");

	if (!CallFunction(3, 1, "OnPostCrafting"))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bRetVal;
}





bool cPlugin_NewLua::OnBlockToPickup(
	BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, 
	const cPlayer * a_Player, const cItem & a_EquippedItem, cItems & a_Pickups
)
{
	cLuaItems Pickups(a_Pickups);
	cCSLock Lock(m_CriticalSection);
	if (!PushFunction("OnBlockToPickup"))
		return false;

	tolua_pushnumber  (m_LuaState, a_BlockType);
	tolua_pushnumber  (m_LuaState, a_BlockMeta);
	tolua_pushusertype(m_LuaState, (void *)a_Player,        "cPlayer");
	tolua_pushusertype(m_LuaState, (void *)&a_EquippedItem, "cItem");
	tolua_pushusertype(m_LuaState, (void *)&Pickups,        "cLuaItems");

	if (!CallFunction(5, 1, "OnBlockToPickup"))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bRetVal;
}





bool cPlugin_NewLua::OnWeatherChanged(cWorld * a_World)
{
	cCSLock Lock(m_CriticalSection);
	if (!PushFunction("OnWeatherChanged"))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, (void *)a_World, "cWorld");

	if (!CallFunction(1, 1, "OnWeatherChanged"))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bRetVal;
}





bool cPlugin_NewLua::OnUpdatingSign(
	cWorld * a_World, 
	int a_BlockX, int a_BlockY, int a_BlockZ, 
	AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4
)
{
	cCSLock Lock(m_CriticalSection);
	if (!PushFunction("OnUpdatingSign"))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, (void *)a_World, "cWorld");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushstring  (m_LuaState, a_Line1.c_str());
	tolua_pushstring  (m_LuaState, a_Line2.c_str());
	tolua_pushstring  (m_LuaState, a_Line3.c_str());
	tolua_pushstring  (m_LuaState, a_Line4.c_str());

	if (!CallFunction(8, 5, "OnUpdatingSign"))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean( m_LuaState, -5, 0) > 0);
	if (lua_isstring(m_LuaState, -4))
	{
		a_Line1 = tolua_tostring(m_LuaState, -4, "");
	}
	if (lua_isstring(m_LuaState, -3))
	{
		a_Line2 = tolua_tostring(m_LuaState, -3, "");
	}
	if (lua_isstring(m_LuaState, -2))
	{
		a_Line3 = tolua_tostring(m_LuaState, -2, "");
	}
	if (lua_isstring(m_LuaState, -1))
	{
		a_Line4 = tolua_tostring(m_LuaState, -1, "");
	}
	return bRetVal;
}





bool cPlugin_NewLua::OnUpdatedSign(
	cWorld * a_World, 
	int a_BlockX, int a_BlockY, int a_BlockZ, 
	const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4
)
{
	cCSLock Lock(m_CriticalSection);
	if (!PushFunction("OnUpdatedSign"))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, (void *)a_World, "cWorld");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushstring  (m_LuaState, a_Line1.c_str());
	tolua_pushstring  (m_LuaState, a_Line2.c_str());
	tolua_pushstring  (m_LuaState, a_Line3.c_str());
	tolua_pushstring  (m_LuaState, a_Line4.c_str());

	if (!CallFunction(8, 1, "OnUpdatedSign"))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bRetVal;
}





cPlugin_NewLua * cPlugin_NewLua::CreateWebPlugin(lua_State * a_LuaState)
{
	LOGWARN("WARNING: Using deprecated function CreateWebPlugin()! A Lua plugin is a WebPlugin by itself now. (plugin \"%s\" in folder \"%s\")",
		cPlugin::GetName().c_str(), m_Directory.c_str()
	);
	return this;
}





AString cPlugin_NewLua::HandleWebRequest( HTTPRequest * a_Request )
{
	cCSLock Lock( m_CriticalSection );
	std::string RetVal = "";

	std::pair< std::string, std::string > TabName = GetTabNameForRequest(a_Request);
	std::string SafeTabName = TabName.second;
	if( SafeTabName.empty() )
		return "";

	sWebPluginTab* Tab = 0;
	for( TabList::iterator itr = GetTabs().begin(); itr != GetTabs().end(); ++itr )
	{
		if( (*itr)->SafeTitle.compare( SafeTabName ) == 0 ) // This is the one! Rawr
		{
			Tab = *itr;
			break;
		}
	}

	if( Tab )
	{
		//LOGINFO("1. Stack size: %i", lua_gettop(m_LuaState) );
		lua_rawgeti( m_LuaState, LUA_REGISTRYINDEX, Tab->UserData); // same as lua_getref()

		//LOGINFO("2. Stack size: %i", lua_gettop(m_LuaState) );
		// Push HTTPRequest
		tolua_pushusertype( m_LuaState, a_Request, "HTTPRequest" );
		//LOGINFO("Calling bound function! :D");
		int s = lua_pcall( m_LuaState, 1, 1, 0);

		if ( s != 0 )
		{
			std::string err = lua_tostring(m_LuaState, -1);
			LOGERROR("-- %s", err.c_str() );
			lua_pop(m_LuaState, 1);
			LOGINFO("error. Stack size: %i", lua_gettop(m_LuaState) );
			return err;	// Show the error message in the web page, looks cool
		}

		if( !lua_isstring( m_LuaState, -1 ) )
		{
			LOGWARN("WARNING: WebPlugin tab '%s' did not return a string!", Tab->Title.c_str() );
			lua_pop(m_LuaState, 1); // Pop return value
			return std::string("WARNING: WebPlugin tab '") + Tab->Title + std::string("' did not return a string!");
		}

		RetVal += tolua_tostring(m_LuaState, -1, 0);
		lua_pop(m_LuaState, 1); // Pop return value
		//LOGINFO("ok. Stack size: %i", lua_gettop(m_LuaState) );
	}

	return RetVal;
}





bool cPlugin_NewLua::AddWebTab( const AString & a_Title, lua_State * a_LuaState, int a_FunctionReference )
{
	cCSLock Lock( m_CriticalSection );
	if( a_LuaState != m_LuaState )
	{
		LOGERROR("Only allowed to add a tab to a WebPlugin of your own Plugin!");
		return false;
	}
	sWebPluginTab* Tab = new sWebPluginTab();
	Tab->Title = a_Title;
	Tab->SafeTitle = SafeString( a_Title );

	Tab->UserData = a_FunctionReference;

	GetTabs().push_back( Tab );
	return true;
}





// Helper functions
bool cPlugin_NewLua::PushFunction( const char* a_FunctionName, bool a_bLogError /* = true */ )
{
	lua_getglobal(m_LuaState, a_FunctionName);
	if(!lua_isfunction(m_LuaState,-1))
	{
		if( a_bLogError )
		{
			LOGWARN("Error in plugin %s: Could not find function %s()", m_Directory.c_str(), a_FunctionName );
		}
		lua_pop(m_LuaState,1);
		return false;
	}
	return true;
}

bool cPlugin_NewLua::CallFunction( int a_NumArgs, int a_NumResults, const char* a_FunctionName )
{
	int s = lua_pcall(m_LuaState, a_NumArgs, a_NumResults, 0);
	if( report_errors( m_LuaState, s ) )
	{
		LOGWARN("Error in plugin %s calling function %s()", m_Directory.c_str(), a_FunctionName );
		return false;
	}
	return true;
}