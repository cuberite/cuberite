
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#define LUA_USE_POSIX
#include "cPlugin_NewLua.h"
#include "cMCLogger.h"
#include "cWebPlugin_Lua.h"

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
{
	m_Directory = a_PluginName;
}

cPlugin_NewLua::~cPlugin_NewLua()
{
	cCSLock Lock( m_CriticalSection );
	for( WebPluginList::iterator itr = m_WebPlugins.begin(); itr != m_WebPlugins.end(); ++itr )
	{
		delete *itr;
	}
	m_WebPlugins.clear();

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

	std::string PluginPath = std::string("Plugins/") + m_Directory + "/";

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





bool cPlugin_NewLua::OnDisconnect( std::string a_Reason, cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );
	if( !PushFunction("OnDisconnect") )
		return false;

	tolua_pushstring( m_LuaState, a_Reason.c_str() );
	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");

	if( !CallFunction(2, 1, "OnDisconnect") )
		return false;

	bool bRetVal = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bRetVal;
}





bool cPlugin_NewLua::OnBlockPlace( cPacket_BlockPlace* a_PacketData, cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );
	if( !PushFunction("OnBlockPlace") )
		return false;

	tolua_pushusertype(m_LuaState, a_PacketData, "cPacket_BlockPlace");
	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");

	if( !CallFunction(2, 1, "OnBlockPlace") )
		return false;

	bool bRetVal = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bRetVal;
}





bool cPlugin_NewLua::OnBlockDig( cPacket_BlockDig* a_PacketData, cPlayer* a_Player, cItem* a_PickupItem )
{
	cCSLock Lock( m_CriticalSection );
	if( !PushFunction("OnBlockDig") )
		return false;

	tolua_pushusertype(m_LuaState, a_PacketData, "cPacket_BlockDig");
	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");
	tolua_pushusertype(m_LuaState, a_PickupItem, "cItem");

	if( !CallFunction(3, 1, "OnBlockDig") )
		return false;

	bool bRetVal = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bRetVal;
}





bool cPlugin_NewLua::OnChat( const char* a_Chat, cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );
	if( !PushFunction("OnChat") )
		return false;

	tolua_pushstring( m_LuaState, a_Chat );
	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");

	if( !CallFunction(2, 1, "OnChat") )
		return false;

	bool bRetVal = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bRetVal;
}





bool cPlugin_NewLua::OnLogin( cPacket_Login* a_PacketData )
{
	cCSLock Lock( m_CriticalSection );
	if( !PushFunction("OnLogin") )
		return false;

	tolua_pushusertype(m_LuaState, a_PacketData, "cPacket_Login");

	if( !CallFunction(1, 1, "OnLogin") )
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





cWebPlugin_Lua* cPlugin_NewLua::CreateWebPlugin(lua_State* a_LuaState)
{
	cCSLock Lock( m_CriticalSection );
	if( a_LuaState != m_LuaState )
	{
		LOGERROR("Not allowed to create a WebPlugin from another plugin but your own!");
		return 0;
	}
	cWebPlugin_Lua* WebPlugin = new cWebPlugin_Lua( this );

	m_WebPlugins.push_back( WebPlugin );

	return WebPlugin;
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