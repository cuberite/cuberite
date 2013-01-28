#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "PluginManager.h"
#include "Plugin.h"
#include "Plugin_NewLua.h"
#include "WebAdmin.h"
#include "Item.h"
#include "Root.h"
#include "LuaCommandBinder.h"

#ifdef USE_SQUIRREL
	#include "Plugin_Squirrel.h"
	#include "SquirrelCommandBinder.h"
#endif

#include "../iniFile/iniFile.h"
#include "tolua++.h"
#include "Player.h"

#ifdef USE_SQUIRREL
	#include "squirrelbindings/SquirrelBindings.h"
	#include "squirrelbindings/SquirrelFunctions.h"
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
#ifdef USE_SQUIRREL
	, m_SquirrelCommandBinder( new cSquirrelCommandBinder() )
#endif
	, m_bReloadPlugins(false)
{
}





cPluginManager::~cPluginManager()
{
	UnloadPluginsNow();
	
	delete m_LuaCommandBinder;
#ifdef USE_SQUIRREL
	delete m_SquirrelCommandBinder;
#endif
}





void cPluginManager::ReloadPlugins()
{
	m_bReloadPlugins = true;
}





void cPluginManager::FindPlugins()
{
	AString PluginsPath = FILE_IO_PREFIX + AString( "Plugins/" );

	// First get a clean list of only the currently running plugins, we don't want to mess those up
	for( PluginMap::iterator itr = m_Plugins.begin(); itr != m_Plugins.end(); )
	{
		if( itr->second == NULL )
		{
			PluginMap::iterator thiz = itr;
			++thiz;
			m_Plugins.erase( itr );
			itr = thiz;
			continue;
		}
		++itr;
	}

	AStringList Files = GetDirectoryContents(PluginsPath.c_str());
	for (AStringList::const_iterator itr = Files.begin(); itr != Files.end(); ++itr)
	{
		if (itr->rfind(".") != AString::npos)
		{
			// Ignore files, we only want directories
			continue;
		}

		// Add plugin name/directory to the list
		if( m_Plugins.find( *itr ) == m_Plugins.end() )
		{
			m_Plugins[ *itr ] = NULL;
		}
	}
}





void cPluginManager::ReloadPluginsNow()
{
	LOG("Loading plugins");
	m_bReloadPlugins = false;
	UnloadPluginsNow();

	#ifdef USE_SQUIRREL
	CloseSquirrelVM();
	OpenSquirrelVM();
	#endif  // USE_SQUIRREL

	FindPlugins();

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
			if( (ValueName.compare("NewPlugin") == 0) || (ValueName.compare("Plugin") == 0) ) // New plugin style
			{
				AString PluginFile = IniFile.GetValue(KeyNum, i );
				if( !PluginFile.empty() )
				{
					if( m_Plugins.find( PluginFile ) != m_Plugins.end() )
					{
						LoadPlugin( PluginFile );
					}
				}
			}
			
			#ifdef USE_SQUIRREL
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
	while( m_DisablePluginList.size() > 0 )
	{
		RemovePlugin( m_DisablePluginList.front(), true );
		m_DisablePluginList.pop_front();
	}

	if( m_bReloadPlugins )
	{
		ReloadPluginsNow();
	}

	HookMap::iterator Plugins = m_Hooks.find(HOOK_TICK);
	if( Plugins != m_Hooks.end() )
	{
		for( PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr )
		{
			(*itr)->Tick( a_Dt );
		}
	}
}





bool cPluginManager::CallHookBlockToPickups(
	cWorld * a_World, cEntity * a_Digger,
	int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, 
	cItems & a_Pickups
)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_BLOCK_TO_PICKUPS);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnBlockToPickups(a_World, a_Digger, a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, a_Pickups))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookChat(cPlayer * a_Player, const AString & a_Message)
{
	#ifdef USE_SQUIRREL
	if (m_SquirrelCommandBinder->HandleCommand(a_Message, a_Player))
	{
		return true;
	}
	#endif  // USE_SQUIRREL

	if (m_LuaCommandBinder->HandleCommand(a_Message, a_Player))
	{
		return true;
	}

	// Check if it was a standard command (starts with a slash)
	if (a_Message[0] == '/')
	{
		a_Player->SendMessage("Unknown Command");
		LOGINFO("Player \"%s\" issued an unknown command: \"%s\"", a_Player->GetName().c_str(), a_Message.c_str());
		return true;	// Cancel sending
	}

	HookMap::iterator Plugins = m_Hooks.find(HOOK_CHAT);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnChat(a_Player, a_Message))
		{
			return true;
		}
	}
	
	return false;
}





bool cPluginManager::CallHookChunkGenerated(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_CHUNK_GENERATED);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnChunkGenerated(a_World, a_ChunkX, a_ChunkZ))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookChunkGenerating(cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_LuaChunk)
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





bool cPluginManager::CallHookCollectingPickup(cPlayer * a_Player, cPickup & a_Pickup)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_COLLECTING_PICKUP);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnCollectingPickup(a_Player, &a_Pickup))
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





bool cPluginManager::CallHookDisconnect(cPlayer * a_Player, const AString & a_Reason)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_DISCONNECT);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnDisconnect(a_Player, a_Reason))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookHandshake(cClientHandle * a_ClientHandle, const AString & a_Username)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_HANDSHAKE);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnHandshake(a_ClientHandle, a_Username))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookKilling(cPawn & a_Victim, cEntity * a_Killer)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_KILLING);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnKilling(a_Victim, a_Killer))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookLogin(cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_LOGIN);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnLogin(a_Client, a_ProtocolVersion, a_Username))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerBreakingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_BREAKING_BLOCK);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerBreakingBlock(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_BlockType, a_BlockMeta))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerBrokenBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_BROKEN_BLOCK);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerBrokenBlock(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_BlockType, a_BlockMeta))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerEating(cPlayer & a_Player)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_EATING);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerEating(a_Player))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerJoined(cPlayer & a_Player)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_JOINED);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerJoined(a_Player))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerLeftClick(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_LEFT_CLICK);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerLeftClick(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_Status))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerMoving(cPlayer & a_Player)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_MOVING);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerMoved(a_Player))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerPlacedBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_PLACED_BLOCK);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerPlacedBlock(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, a_BlockType, a_BlockMeta))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerPlacingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_PLACING_BLOCK);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerPlacingBlock(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, a_BlockType, a_BlockMeta))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerRightClick(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_RIGHT_CLICK);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerRightClick(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerShooting(cPlayer & a_Player)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_SHOOTING);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerShooting(a_Player))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerSpawned(cPlayer & a_Player)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_SPAWNED);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerSpawned(a_Player))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerTossingItem(cPlayer & a_Player)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_TOSSING_ITEM);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerTossingItem(a_Player))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerUsedBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_USED_BLOCK);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerUsedBlock(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, a_BlockType, a_BlockMeta))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerUsedItem(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_USED_ITEM);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerUsedItem(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerUsingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_USING_BLOCK);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerUsingBlock(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, a_BlockType, a_BlockMeta))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerUsingItem(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_PLAYER_USING_ITEM);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerUsingItem(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ))
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





bool cPluginManager::CallHookTakeDamage(cPawn & a_Receiver, TakeDamageInfo & a_TDI)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_TAKE_DAMAGE);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnTakeDamage(a_Receiver, a_TDI))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookUpdatingSign(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4, cPlayer * a_Player)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_UPDATING_SIGN);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnUpdatingSign(a_World, a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4, a_Player))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookUpdatedSign(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_UPDATED_SIGN);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnUpdatedSign(a_World, a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4, a_Player))
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





cPlugin* cPluginManager::GetPlugin( const AString & a_Plugin ) const
{
	for( PluginMap::const_iterator itr = m_Plugins.begin(); itr != m_Plugins.end(); ++itr )
	{
		if (itr->second == NULL ) continue;
		if (itr->second->GetName().compare(a_Plugin) == 0)
		{
			return itr->second;
		}
	}
	return 0;
}





const cPluginManager::PluginMap & cPluginManager::GetAllPlugins() const
{
	return m_Plugins;
}





void cPluginManager::UnloadPluginsNow()
{
	m_Hooks.clear();

	while( m_Plugins.size() > 0 )
	{
		RemovePlugin( m_Plugins.begin()->second, true );
	}

	//SquirrelVM::Shutdown(); // This breaks shit
}





bool cPluginManager::DisablePlugin( AString & a_PluginName )
{
	PluginMap::iterator itr = m_Plugins.find( a_PluginName );
	if (itr != m_Plugins.end())
	{
		if (itr->first.compare( a_PluginName ) == 0)
		{
			m_DisablePluginList.push_back( itr->second );
			itr->second = NULL;	// Get rid of this thing right away
			return true;
		}
	}
	return false;
}





bool cPluginManager::LoadPlugin( AString & a_PluginName )
{
	cPlugin_NewLua* Plugin = new cPlugin_NewLua( a_PluginName.c_str() );
	if (!AddPlugin(Plugin))
	{
		delete Plugin;
		return false;
	}
	return true;
}





void cPluginManager::RemoveHooks(cPlugin * a_Plugin)
{
	for (HookMap::iterator itr = m_Hooks.begin(), end = m_Hooks.end(); itr != end; ++itr)
	{
		itr->second.remove(a_Plugin);
	}
}





void cPluginManager::RemovePlugin( cPlugin * a_Plugin, bool a_bDelete /* = false */ )
{
	if( a_bDelete )
	{
		m_LuaCommandBinder->RemoveBindingsForPlugin( a_Plugin );
#ifdef USE_SQUIRREL
		m_SquirrelCommandBinder->RemoveBindingsForPlugin( a_Plugin );
#endif

		for( PluginMap::iterator itr = m_Plugins.begin(); itr != m_Plugins.end(); ++itr )
		{
			if( itr->second == a_Plugin )
			{
				m_Plugins.erase( itr );
				break;
			}
		}
		if( a_Plugin != NULL )
		{
			RemoveHooks( a_Plugin );
			a_Plugin->OnDisable();
			delete a_Plugin;
		}
	}
}





bool cPluginManager::AddPlugin( cPlugin* a_Plugin )
{
	a_Plugin->m_bCanBindCommands = true;
	if( a_Plugin->Initialize() )
	{
		m_Plugins[ a_Plugin->GetDirectory() ] = a_Plugin;
		return true;
	}

	a_Plugin->m_bCanBindCommands = false;
	RemoveHooks( a_Plugin ); // Undo any damage the Initialize() might have done
	return false;
}





void cPluginManager::AddHook(cPlugin * a_Plugin, PluginHook a_Hook)
{
	if (!a_Plugin)
	{
		LOGWARN("Called cPluginManager::AddHook() with a_Plugin == NULL");
		return;
	}
	if (!a_Plugin->CanAddHook(a_Hook))
	{
		return;
	}
	PluginList & Plugins = m_Hooks[a_Hook];
	Plugins.remove(a_Plugin);
	Plugins.push_back(a_Plugin);
}





unsigned int cPluginManager::GetNumPlugins() const
{
	return m_Plugins.size(); 
}





bool cPluginManager::HasPlugin( cPlugin* a_Plugin ) const
{
	for( PluginMap::const_iterator itr = m_Plugins.begin(); itr != m_Plugins.end(); ++itr )
	{
		if( itr->second == a_Plugin )
			return true;
	}
	return false;
}




