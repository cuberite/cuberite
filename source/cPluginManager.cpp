#include "cPluginManager.h"
#include "cPlugin.h"
#include "cPlugin_Lua.h"
#include "cPlugin_NewLua.h"
#include "cMCLogger.h"
#include "cWebAdmin.h"
#include "cItem.h"
#include "cRoot.h"
#include "cLuaCommandBinder.h"
#include "../iniFile/iniFile.h"
#include <string> //strcmp
#include <stdarg.h>

#include "SquirrelBindings.h"
#if USE_SQUIRREL
#pragma warning(disable:4100;disable:4127;disable:4510;disable:4610;disable:4244;disable:4512) // Getting A LOT of these warnings from SqPlus
#include <sqplus/sqplus.h>
#pragma warning(default:4100;default:4127;default:4510;default:4610;default:4244;default:4512)
#endif

extern std::vector<std::string> StringSplit(std::string str, std::string delim);

typedef std::list< cPlugin_Lua* > LuaPluginList;
typedef std::map< cPluginManager::PluginHook, cPluginManager::PluginList > HookMap;

struct cPluginManager::sPluginManagerState
{
	LuaPluginList LuaPlugins;
	cPluginManager::PluginList Plugins;
	HookMap Hooks;
};

cPluginManager* cPluginManager::GetPluginManager()
{
	LOGWARN("WARNING: Using deprecated function cPluginManager::GetPluginManager() use cRoot::Get()->GetPluginManager() instead!");
	return cRoot::Get()->GetPluginManager();
}

cPluginManager::cPluginManager()
	: m_pState( new sPluginManagerState )
	, m_LuaCommandBinder( new cLuaCommandBinder() )
	, m_bReloadPlugins(false)
{
}

cPluginManager::~cPluginManager()
{
	UnloadPluginsNow();
	
	delete m_LuaCommandBinder;
	delete m_pState;
}

void cPluginManager::ReloadPlugins()
{
	m_bReloadPlugins = true;
}

void cPluginManager::ReloadPluginsNow()
{
	LOG("--Loading plugins--");
	m_bReloadPlugins = false;
	UnloadPluginsNow();

	
	
#if USE_SQUIRREL
	if( !SquirrelBindings::IsBound )	// Can only do this once apparently, or we're making ambiguous calls in the script
	{
		SquirrelVM::Init();
		SquirrelBindings::Bind( SquirrelVM::GetVMPtr() );
	}
#endif

	cIniFile IniFile("settings.ini");
	if( IniFile.ReadFile() )
	{
		unsigned int KeyNum = IniFile.FindKey("Plugins");
		unsigned int NumPlugins = IniFile.GetNumValues( KeyNum );
		if( NumPlugins > 0 )
		{
			for(unsigned int i = 0; i < NumPlugins; i++)
			{
				std::string ValueName = IniFile.GetValueName(KeyNum, i );
				if( ValueName.compare("Plugin") == 0 ) // It's a Lua plugin
				{	
					std::string PluginFile = IniFile.GetValue(KeyNum, i );
					if( !PluginFile.empty() )
					{
						// allow for comma separated plugin list
						// degrades and works fine for the plugin
						// per line
						std::vector< std::string > split = StringSplit( PluginFile, "," );
						for (unsigned int j = 0; j < split.size(); j++) {
							cPlugin_Lua* Plugin = new cPlugin_Lua( (split[j] + std::string(".lua") ).c_str() );
							if( !AddLuaPlugin( Plugin ) )
							{
								delete Plugin;
							}
						}
					}
				}
				else if( ValueName.compare("NewPlugin") == 0 ) // New plugin style
				{
					std::string PluginFile = IniFile.GetValue(KeyNum, i );
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
					std::string PluginFile = IniFile.GetValue(KeyNum, i );
					if( !PluginFile.empty() )
					{
						LOGINFO("Loading Squirrel plugin: %s", PluginFile.c_str() );
						try 
						{
							SquirrelObject SquirrelScript = SquirrelVM::CompileScript( (std::string("Plugins/") + PluginFile + ".nut").c_str() );
							try 
							{
								SquirrelVM::RunScript( SquirrelScript );
							} 
							catch (SquirrelError & e)
							{
								LOGERROR("Error: %s, %s\n", e.desc, "SquirrelVM::RunScript");
							}
						}
						catch (SquirrelError & e) 
						{
							LOGERROR("Error: %s, %s\n", e.desc, "SquirrelVM::CompileScript");
						}
					}
				}
#endif
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
	else
	{
		LOG("WARNING: Can't find settings.ini, so can't load any plugins.");
	}


	LOG("--Done loading plugins--");
}

void cPluginManager::Tick(float a_Dt)
{
	if( m_bReloadPlugins )
	{
		ReloadPluginsNow();
	}


	HookMap::iterator Plugins = m_pState->Hooks.find( E_PLUGIN_TICK );
	if( Plugins != m_pState->Hooks.end() )
	{
		for( PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr )
		{
			(*itr)->Tick( a_Dt );
		}
	}
}

bool cPluginManager::CallHook( PluginHook a_Hook, unsigned int a_NumArgs, ... )
{
	HookMap::iterator Plugins = m_pState->Hooks.find( a_Hook );

	// Special case for chat hook, since you can also bind commands (bound commands don't use chat hook)
	if( a_Hook == E_PLUGIN_CHAT )
	{
		if( a_NumArgs != 2 ) return false;
		va_list argptr;
		va_start( argptr, a_NumArgs);
		const char* Message = va_arg(argptr, const char* );
		cPlayer* Player = va_arg(argptr, cPlayer* );
		va_end (argptr);

		if( m_LuaCommandBinder->HandleCommand( std::string( Message ), Player ) )
			return true;

		if( Plugins != m_pState->Hooks.end() )
		{
			for( PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr )
			{
				if( (*itr)->OnChat( Message, Player ) )
					return true;
			}
		}

		return false;
	}

	if( Plugins != m_pState->Hooks.end() )
	{
		switch( a_Hook )
		{
		case E_PLUGIN_COLLECT_ITEM:
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
			}
			break;
		case E_PLUGIN_BLOCK_DIG:
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
			}
			break;
		case E_PLUGIN_BLOCK_PLACE:
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
			}
			break;
		case E_PLUGIN_DISCONNECT:
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
			}
			break;
		case E_PLUGIN_LOGIN:
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
			}
			break;
		case E_PLUGIN_PLAYER_JOIN:
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
			}
			break;
		case E_PLUGIN_PLAYER_MOVE:
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
			}
			break;
		case E_PLUGIN_TAKE_DAMAGE:
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
			}
			break;
		case E_PLUGIN_KILLED:
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
			}
			break;
		default:
			LOG("WARNING: Calling Unknown hook: %i", a_Hook );
			break;
		}
	}
	return false;
}

cPlugin* cPluginManager::GetPlugin( const char* a_Plugin )
{
	for( PluginList::iterator itr = m_pState->Plugins.begin(); itr != m_pState->Plugins.end(); ++itr )
	{
		if( std::string( (*itr)->GetName() ).compare( a_Plugin ) == 0 )
		{
			return *itr;
		}
	}
	return 0;
}

const cPluginManager::PluginList & cPluginManager::GetAllPlugins()
{
	return m_pState->Plugins;
}

void cPluginManager::UnloadPluginsNow()
{
	m_pState->Hooks.clear();

	while( m_pState->LuaPlugins.size() > 0 )
	{
		cPlugin_Lua* LuaPlugin = *m_pState->LuaPlugins.begin();
		if( LuaPlugin )
		{
			cWebAdmin* WebAdmin = cRoot::Get()->GetWebAdmin();
			if( WebAdmin ) WebAdmin->RemovePlugin( LuaPlugin->GetLuaState() );
			delete LuaPlugin;
		}
		m_pState->LuaPlugins.remove( LuaPlugin );
	}

	while( m_pState->Plugins.size() > 0 )
	{
		RemovePlugin( *m_pState->Plugins.begin(), true );
	}

	//SquirrelVM::Shutdown(); // This breaks shit
}

void cPluginManager::RemoveHooks( cPlugin* a_Plugin )
{
	m_pState->Hooks[ E_PLUGIN_TICK			].remove( a_Plugin );
	m_pState->Hooks[ E_PLUGIN_CHAT			].remove( a_Plugin );
	m_pState->Hooks[ E_PLUGIN_COLLECT_ITEM	].remove( a_Plugin );
	m_pState->Hooks[ E_PLUGIN_BLOCK_DIG		].remove( a_Plugin );
	m_pState->Hooks[ E_PLUGIN_BLOCK_PLACE	].remove( a_Plugin );
	m_pState->Hooks[ E_PLUGIN_DISCONNECT	].remove( a_Plugin );
	m_pState->Hooks[ E_PLUGIN_HANDSHAKE		].remove( a_Plugin );
	m_pState->Hooks[ E_PLUGIN_LOGIN			].remove( a_Plugin );
	m_pState->Hooks[ E_PLUGIN_PLAYER_SPAWN	].remove( a_Plugin );
	m_pState->Hooks[ E_PLUGIN_PLAYER_JOIN	].remove( a_Plugin );
	m_pState->Hooks[ E_PLUGIN_PLAYER_MOVE	].remove( a_Plugin );
	m_pState->Hooks[ E_PLUGIN_TAKE_DAMAGE	].remove( a_Plugin );
	m_pState->Hooks[ E_PLUGIN_KILLED		].remove( a_Plugin );
}

void cPluginManager::RemovePlugin( cPlugin* a_Plugin, bool a_bDelete /* = false */ )
{
	if( a_bDelete )
	{
		m_LuaCommandBinder->RemoveBindingsForPlugin( a_Plugin );
		m_pState->Plugins.remove( a_Plugin );
		RemoveHooks( a_Plugin );
		a_Plugin->OnDisable();
		if( a_Plugin->GetLanguage() != cPlugin::E_SQUIRREL ) // Squirrel needs to clean it up himself!
			delete a_Plugin;
	}
	else
	{
		for( LuaPluginList::iterator itr = m_pState->LuaPlugins.begin(); itr != m_pState->LuaPlugins.end(); ++itr )
		{
			(*itr)->RemovePlugin( a_Plugin );
		}
	}
}

bool cPluginManager::AddPlugin( cPlugin* a_Plugin )
{
	if( a_Plugin->Initialize() )
	{
		m_pState->Plugins.remove( a_Plugin );
		m_pState->Plugins.push_back( a_Plugin );
		return true;
	}

	RemoveHooks( a_Plugin ); // Undo any damage the Initialize() might have done
	return false;
}

bool cPluginManager::AddPlugin( lua_State* a_LuaState, cPlugin* a_Plugin )
{
	a_Plugin->SetLanguage( cPlugin::E_LUA );
	cPlugin_Lua* LuaPlugin = GetLuaPlugin( a_LuaState );
	if( LuaPlugin && a_Plugin->Initialize() )
	{
		m_pState->Plugins.remove( a_Plugin );
		m_pState->Plugins.push_back( a_Plugin );
		LuaPlugin->AddPlugin( a_Plugin );
		return true;
	}
	return false;
}

bool cPluginManager::AddLuaPlugin( cPlugin_Lua* a_Plugin )
{
	m_pState->LuaPlugins.push_back( a_Plugin ); // It HAS to be in here before calling Initialize, so it can be found by AddPlugin()
	if(a_Plugin->Initialize() )
	{
		return true;
	}
	LOG(">>>>>>> Could not initialize a plugin! ");
	m_pState->LuaPlugins.remove( a_Plugin );
	return false;
}

void cPluginManager::RemoveLuaPlugin( std::string a_FileName )
{
	for( LuaPluginList::iterator itr = m_pState->LuaPlugins.begin(); itr != m_pState->LuaPlugins.end(); ++itr )
	{
		if( (*itr)->GetFileName() == a_FileName )
		{
			cPlugin_Lua* Plugin = *itr;
			m_pState->LuaPlugins.remove( Plugin );
			delete Plugin;
			return;
		}
	}
}

cPlugin_Lua* cPluginManager::GetLuaPlugin( lua_State* a_State )
{
	for( LuaPluginList::iterator itr = m_pState->LuaPlugins.begin(); itr != m_pState->LuaPlugins.end(); ++itr )
	{
		if( (*itr)->GetLuaState() == a_State )
		{
			return *itr;
		}
	}
	return 0;
}

void cPluginManager::AddHook( cPlugin* a_Plugin, PluginHook a_Hook )
{
	if( !a_Plugin )
	{
		LOGWARN("Called cPluginManager::AddHook while a_Plugin is NULL");
		return;
	}
	PluginList & Plugins = m_pState->Hooks[ a_Hook ];
	Plugins.remove( a_Plugin );
	Plugins.push_back( a_Plugin );
}

unsigned int cPluginManager::GetNumPlugins()
{
	return m_pState->Plugins.size(); 
}