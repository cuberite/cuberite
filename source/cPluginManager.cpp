#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPluginManager.h"
#include "cPlugin.h"
#include "cPlugin_Lua.h"
#include "cPlugin_NewLua.h"
#include "cPlugin_Squirrel.h"
#include "cWebAdmin.h"
#include "cItem.h"
#include "cRoot.h"
#include "cLuaCommandBinder.h"
#include "cSquirrelCommandBinder.h"
#include "../iniFile/iniFile.h"
#include "tolua++.h"
#include "cPlayer.h"

#include "squirrelbindings/SquirrelBindings.h"
#include "squirrelbindings/SquirrelFunctions.h"

#if USE_SQUIRREL
	#pragma warning(disable:4100;disable:4127;disable:4510;disable:4610;disable:4244;disable:4512) // Getting A LOT of these warnings from SqPlus
	
	#pragma warning(default:4100;default:4127;default:4510;default:4610;default:4244;default:4512)
#endif





cPluginManager* cPluginManager::GetPluginManager()
{
	LOGWARN("WARNING: Using deprecated function cPluginManager::GetPluginManager() use cRoot::Get()->GetPluginManager() instead!");
	return cRoot::Get()->GetPluginManager();
}





cPluginManager::cPluginManager()
	: m_LuaCommandBinder( new cLuaCommandBinder() )
	, m_SquirrelCommandBinder( new cSquirrelCommandBinder() )
	, m_bReloadPlugins(false)
{
}





cPluginManager::~cPluginManager()
{
	UnloadPluginsNow();
	
	delete m_LuaCommandBinder;
	delete m_SquirrelCommandBinder;
}





void cPluginManager::ReloadPlugins()
{
	m_bReloadPlugins = true;
}





void cPluginManager::ReloadPluginsNow()
{
	LOG("Loading plugins");
	m_bReloadPlugins = false;
	UnloadPluginsNow();

	#if USE_SQUIRREL
	CloseSquirrelVM();
	OpenSquirrelVM();
	#endif  // USE_SQUIRREL

	cIniFile IniFile("settings.ini");
	if (!IniFile.ReadFile() )
	{
		LOGWARNING("cPluginManager: Can't find settings.ini, so can't load any plugins.");
	}
		
	unsigned int KeyNum = IniFile.FindKey("Plugins");
	unsigned int NumPlugins = IniFile.GetNumValues( KeyNum );
	if( NumPlugins > 0 )
	{
		for(unsigned int i = 0; i < NumPlugins; i++)
		{
			AString ValueName = IniFile.GetValueName(KeyNum, i );
			if( ValueName.compare("Plugin") == 0 ) // It's a Lua plugin
			{	
				AString PluginFile = IniFile.GetValue(KeyNum, i );
				if( !PluginFile.empty() )
				{
					// allow for comma separated plugin list
					// degrades and works fine for the plugin
					// per line
					AStringVector split = StringSplit( PluginFile, "," );
					for (unsigned int j = 0; j < split.size(); j++)
					{
						cPlugin_Lua* Plugin = new cPlugin_Lua( (split[j] + AString(".lua") ).c_str() );
						if( !AddLuaPlugin( Plugin ) )
						{
							delete Plugin;
						}
					}
				}
			}
			else if( ValueName.compare("NewPlugin") == 0 ) // New plugin style
			{
				AString PluginFile = IniFile.GetValue(KeyNum, i );
				if( !PluginFile.empty() )
				{
					cPlugin_NewLua* Plugin = new cPlugin_NewLua( PluginFile.c_str() );
					if( !AddPlugin( Plugin ) )
					{
						delete Plugin;
					}
				}
			}
			
			#if USE_SQUIRREL
			else if( ValueName.compare("Squirrel") == 0 ) // Squirrel plugin
			{
				AString PluginFile = IniFile.GetValue(KeyNum, i );
				if( !PluginFile.empty() )
				{
					LOGINFO("Loading Squirrel plugin: %s", PluginFile.c_str() );
					
					cPlugin_Squirrel *Plugin = new cPlugin_Squirrel(PluginFile.c_str());

					if( !AddPlugin( Plugin ) )
					{
						delete Plugin;
					}
				}
			}
			#endif  // USE_SQUIRREL
		}
	}

	if( GetNumPlugins() == 0 )
	{
		LOG("No plugins loaded");
	}
	else
	{
		LOG("Loaded %i plugin(s)", GetNumPlugins() );
	}
}





void cPluginManager::Tick(float a_Dt)
{
	if( m_bReloadPlugins )
	{
		ReloadPluginsNow();
	}

	HookMap::iterator Plugins = m_Hooks.find( E_PLUGIN_TICK );
	if( Plugins != m_Hooks.end() )
	{
		for( PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr )
		{
			(*itr)->Tick( a_Dt );
		}
	}
}





bool cPluginManager::CallHook(PluginHook a_Hook, unsigned int a_NumArgs, ...)
{
	HookMap::iterator Plugins = m_Hooks.find( a_Hook );

	// Special case for chat hook, since you can also bind commands (bound commands don't use chat hook)
	if (a_Hook == HOOK_CHAT)
	{
		if (a_NumArgs != 2)
		{ 	
			return false;
		}
		va_list argptr;
		va_start( argptr, a_NumArgs);
		const char * Message = va_arg(argptr, const char* );
		cPlayer * Player = va_arg(argptr, cPlayer * );
		va_end (argptr);

		if (m_SquirrelCommandBinder->HandleCommand( std::string( Message ), Player))
		{
			return true;
		}

		if (m_LuaCommandBinder->HandleCommand( std::string( Message ), Player))
		{
			return true;
		}

		//Check if it was a standard command (starts with a slash)
		if(Message[0] == '/')
		{
			Player->SendMessage("Unknown Command");
			LOGINFO("Player \"%s\" issued command: %s", Player->GetName().c_str(), Message);
			return true;	//Cancel sending
		}

		if (Plugins == m_Hooks.end())
		{
			return false;
		}
		
		for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
		{
			if ((*itr)->OnChat(Message, Player))
			{
				return true;
			}
		}
		
		return false;
	}

	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	
	switch( a_Hook )
	{
		case HOOK_COLLECT_ITEM:
		{
			if( a_NumArgs != 2 ) break;
			va_list argptr;
			va_start( argptr, a_NumArgs);
			cPickup* Pickup = va_arg(argptr, cPickup* );
			cPlayer* Player = va_arg(argptr, cPlayer* );
			va_end (argptr);
			for( PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr )
			{
				if( (*itr)->OnCollectItem( Pickup, Player ) )
					return true;
			}
			break;
		}
		
		case HOOK_BLOCK_DIG:
		{
			if( a_NumArgs != 2 ) break;
			va_list argptr;
			va_start( argptr, a_NumArgs);
			cPacket_BlockDig* Packet = va_arg(argptr, cPacket_BlockDig* );
			cPlayer* Player = va_arg(argptr, cPlayer* );
			cItem* Item = va_arg( argptr, cItem* );
			va_end (argptr);
			for( PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr )
			{
				if( (*itr)->OnBlockDig( Packet, Player, Item ) )
					return true;
			}
			break;
		}
		
		case HOOK_BLOCK_PLACE:
		{
			if( a_NumArgs != 2 ) break;
			va_list argptr;
			va_start( argptr, a_NumArgs);
			cPacket_BlockPlace* Packet = va_arg(argptr, cPacket_BlockPlace* );
			cPlayer* Player = va_arg(argptr, cPlayer* );
			va_end (argptr);
			for( PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr )
			{
				if( (*itr)->OnBlockPlace( Packet, Player ) )
					return true;
			}
			break;
		}
		
		case HOOK_DISCONNECT:
		{
			if( a_NumArgs != 2 ) break;
			va_list argptr;
			va_start( argptr, a_NumArgs);
			const char* Reason = va_arg(argptr, const char* );
			cPlayer* Player = va_arg(argptr, cPlayer* );
			va_end (argptr);
			for( PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr )
			{
				if( (*itr)->OnDisconnect( Reason, Player ) )
					return true;
			}
			break;
		}

		case HOOK_LOGIN:
		{
			if( a_NumArgs != 1 ) break;
			va_list argptr;
			va_start( argptr, a_NumArgs);
			cPacket_Login* Packet = va_arg(argptr, cPacket_Login* );
			va_end (argptr);
			for( PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr )
			{
				if( (*itr)->OnLogin( Packet ) )
					return true;
			}
			break;
		}

		case HOOK_PLAYER_JOIN:
		{
			if( a_NumArgs != 1 ) break;
			va_list argptr;
			va_start( argptr, a_NumArgs);
			cPlayer* Player = va_arg(argptr, cPlayer* );
			va_end (argptr);
			for( PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr )
			{
				if( (*itr)->OnPlayerJoin( Player ) )
					return true;
			}
			break;
		}

		case HOOK_PLAYER_MOVE:
		{
			if( a_NumArgs != 1 ) break;
			va_list argptr;
			va_start( argptr, a_NumArgs);
			cPlayer* Player = va_arg(argptr, cPlayer* );
			va_end (argptr);
			for( PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr )
			{
				(*itr)->OnPlayerMove( Player );
			}
			break;
		}

		case HOOK_TAKE_DAMAGE:
		{
			if( a_NumArgs != 2 ) break;
			va_list argptr;
			va_start( argptr, a_NumArgs);
			cPawn* Pawn = va_arg(argptr, cPawn* );
			TakeDamageInfo* TDI = va_arg(argptr, TakeDamageInfo* );
			va_end (argptr);
			for( PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr )
			{
				(*itr)->OnTakeDamage( Pawn, TDI );
			}
			break;
		}

		case HOOK_KILLED:
		{
			if( a_NumArgs != 2 ) break;
			va_list argptr;
			va_start( argptr, a_NumArgs);
			cPawn* Killed = va_arg(argptr, cPawn* );
			cEntity* Killer = va_arg(argptr, cEntity* );
			va_end (argptr);
			for( PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr )
			{
				if( (*itr)->OnKilled( Killed, Killer ) )
					return true;
			}
			break;
		}
		
		case HOOK_CHUNK_GENERATED:
		{
			if (a_NumArgs != 3)
			{
				break;
			}
			va_list argptr;
			va_start( argptr, a_NumArgs);
			cWorld * World = va_arg(argptr, cWorld *);
			int ChunkX = va_arg(argptr, int);
			int ChunkZ = va_arg(argptr, int);
			va_end (argptr);
			for( PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr )
			{
				(*itr)->OnChunkGenerated(World, ChunkX, ChunkZ);
			}
			break;
		}
		
		case HOOK_PLAYER_SPAWN:
		{
			if (a_NumArgs != 1)
			{
				break;
			}
			va_list argptr;
			va_start( argptr, a_NumArgs);
			cPlayer * Player = va_arg(argptr, cPlayer *);
			va_end (argptr);
			for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
			{
				(*itr)->OnPlayerSpawn(Player);
			}
			break;
		}

		default:
		{
			LOGWARNING("cPluginManager: Calling Unknown hook: %i", a_Hook );
			ASSERT(!"Calling an unknown hook");
			break;
		}
	}  // switch (a_Hook)
	return false;
}





bool cPluginManager::CallHookChunkGenerating(cWorld * a_World, int a_ChunkX, int a_ChunkZ, cLuaChunk * a_LuaChunk)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_CHUNK_GENERATING);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnChunkGenerating(a_World, a_ChunkX, a_ChunkZ, a_LuaChunk))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPreCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PRE_CRAFTING);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPreCrafting(a_Player, a_Grid, a_Recipe))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookCraftingNoRecipe(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_CRAFTING_NO_RECIPE);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnCraftingNoRecipe(a_Player, a_Grid, a_Recipe))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPostCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_POST_CRAFTING);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPostCrafting(a_Player, a_Grid, a_Recipe))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookBlockToPickup(
	BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, 
	const cPlayer * a_Player, const cItem & a_EquippedItem, cItems & a_Pickups
)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_POST_CRAFTING);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnBlockToPickup(a_BlockType, a_BlockMeta, a_Player, a_EquippedItem, a_Pickups))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookWeatherChanged(cWorld * a_World)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_WEATHER_CHANGED);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnWeatherChanged(a_World))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookUpdatingSign(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_UPDATING_SIGN);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnUpdatingSign(a_World, a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookUpdatedSign(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_UPDATED_SIGN);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnUpdatedSign(a_World, a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4))
		{
			return true;
		}
	}
	return false;
}






cPlugin* cPluginManager::GetPlugin( const char* a_Plugin ) const
{
	for( PluginList::const_iterator itr = m_Plugins.begin(); itr != m_Plugins.end(); ++itr )
	{
		if (strcmp((*itr)->GetName(), a_Plugin) == 0 )
		{
			return *itr;
		}
	}
	return 0;
}





const cPluginManager::PluginList & cPluginManager::GetAllPlugins() const
{
	return m_Plugins;
}





void cPluginManager::UnloadPluginsNow()
{
	m_Hooks.clear();

	while( m_LuaPlugins.size() > 0 )
	{
		cPlugin_Lua* LuaPlugin = *m_LuaPlugins.begin();
		if( LuaPlugin )
		{
			cWebAdmin* WebAdmin = cRoot::Get()->GetWebAdmin();
			if( WebAdmin ) WebAdmin->RemovePlugin( LuaPlugin->GetLuaState() );
			delete LuaPlugin;
		}
		m_LuaPlugins.remove( LuaPlugin );
	}

	while( m_Plugins.size() > 0 )
	{
		RemovePlugin( *m_Plugins.begin(), true );
	}

	//SquirrelVM::Shutdown(); // This breaks shit
}





void cPluginManager::RemoveHooks( cPlugin* a_Plugin )
{
	m_Hooks[ E_PLUGIN_TICK].remove             ( a_Plugin );
	m_Hooks[ E_PLUGIN_CHAT].remove             ( a_Plugin );
	m_Hooks[ E_PLUGIN_COLLECT_ITEM].remove     ( a_Plugin );
	m_Hooks[ E_PLUGIN_BLOCK_DIG].remove        ( a_Plugin );
	m_Hooks[ E_PLUGIN_BLOCK_PLACE].remove      ( a_Plugin );
	m_Hooks[ E_PLUGIN_DISCONNECT].remove       ( a_Plugin );
	m_Hooks[ E_PLUGIN_HANDSHAKE].remove        ( a_Plugin );
	m_Hooks[ E_PLUGIN_LOGIN].remove            ( a_Plugin );
	m_Hooks[ E_PLUGIN_PLAYER_SPAWN].remove     ( a_Plugin );
	m_Hooks[ E_PLUGIN_PLAYER_JOIN].remove      ( a_Plugin );
	m_Hooks[ E_PLUGIN_PLAYER_MOVE].remove      ( a_Plugin );
	m_Hooks[ E_PLUGIN_TAKE_DAMAGE].remove      ( a_Plugin );
	m_Hooks[ E_PLUGIN_KILLED].remove           ( a_Plugin );
	m_Hooks[ E_PLUGIN_CHUNK_GENERATED ].remove ( a_Plugin );
	m_Hooks[ E_PLUGIN_CHUNK_GENERATING ].remove( a_Plugin );
	m_Hooks[ E_PLUGIN_BLOCK_TO_DROPS ].remove  ( a_Plugin );
}





void cPluginManager::RemovePlugin( cPlugin* a_Plugin, bool a_bDelete /* = false */ )
{
	if( a_bDelete )
	{
		m_LuaCommandBinder->RemoveBindingsForPlugin( a_Plugin );
		m_SquirrelCommandBinder->RemoveBindingsForPlugin( a_Plugin );
		m_Plugins.remove( a_Plugin );
		RemoveHooks( a_Plugin );
		a_Plugin->OnDisable();

		delete a_Plugin;
	}
	else
	{
		for( LuaPluginList::iterator itr = m_LuaPlugins.begin(); itr != m_LuaPlugins.end(); ++itr )
		{
			(*itr)->RemovePlugin( a_Plugin );
		}
	}
}





bool cPluginManager::AddPlugin( cPlugin* a_Plugin )
{
	a_Plugin->m_bCanBindCommands = true;
	if( a_Plugin->Initialize() )
	{
		m_Plugins.remove( a_Plugin );
		m_Plugins.push_back( a_Plugin );
		return true;
	}

	a_Plugin->m_bCanBindCommands = false;
	RemoveHooks( a_Plugin ); // Undo any damage the Initialize() might have done
	return false;
}





bool cPluginManager::AddPlugin( lua_State* a_LuaState, cPlugin* a_Plugin )
{
	a_Plugin->SetLanguage( cPlugin::E_LUA );
	cPlugin_Lua* LuaPlugin = GetLuaPlugin( a_LuaState );
	if( LuaPlugin == NULL )
	{
		lua_Debug ar;
		lua_getstack(a_LuaState, 1, &ar);
		lua_getinfo(a_LuaState, "nSl", &ar);
		LOGERROR("ERROR: Trying to add an 'old style' plugin from within a 'new style' plugin.\nIn file: %s at line: %i", ar.source, ar.currentline);
	}
	a_Plugin->m_bCanBindCommands = true;
	if( LuaPlugin && a_Plugin->Initialize() )
	{
		m_Plugins.remove( a_Plugin );
		m_Plugins.push_back( a_Plugin );
		LuaPlugin->AddPlugin( a_Plugin );
		return true;
	}

	a_Plugin->m_bCanBindCommands = false;
	return false;
}





bool cPluginManager::AddLuaPlugin( cPlugin_Lua* a_Plugin )
{
	m_LuaPlugins.push_back( a_Plugin ); // It HAS to be in here before calling Initialize, so it can be found by AddPlugin()
	if(a_Plugin->Initialize() )
	{
		return true;
	}
	LOG(">>>>>>> Could not initialize a plugin! ");
	m_LuaPlugins.remove( a_Plugin );
	return false;
}





void cPluginManager::RemoveLuaPlugin( std::string a_FileName )
{
	for( LuaPluginList::iterator itr = m_LuaPlugins.begin(); itr != m_LuaPlugins.end(); ++itr )
	{
		if( (*itr)->GetFileName() == a_FileName )
		{
			cPlugin_Lua* Plugin = *itr;
			m_LuaPlugins.remove( Plugin );
			delete Plugin;
			return;
		}
	}
}





cPlugin_Lua* cPluginManager::GetLuaPlugin( lua_State* a_State )
{
	for( LuaPluginList::iterator itr = m_LuaPlugins.begin(); itr != m_LuaPlugins.end(); ++itr )
	{
		if( (*itr)->GetLuaState() == a_State )
		{
			return *itr;
		}
	}
	return NULL;
}





void cPluginManager::AddHook( cPlugin* a_Plugin, PluginHook a_Hook )
{
	if( !a_Plugin )
	{
		LOGWARN("Called cPluginManager::AddHook while a_Plugin is NULL");
		return;
	}
	PluginList & Plugins = m_Hooks[ a_Hook ];
	Plugins.remove( a_Plugin );
	Plugins.push_back( a_Plugin );
}





unsigned int cPluginManager::GetNumPlugins() const
{
	return m_Plugins.size(); 
}





bool cPluginManager::HasPlugin( cPlugin* a_Plugin ) const
{
	for( PluginList::const_iterator itr = m_Plugins.begin(); itr != m_Plugins.end(); ++itr )
	{
		if( *itr == a_Plugin )
			return true;
	}
	return false;
}