
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "PluginManager.h"
#include "Plugin.h"
#include "PluginLua.h"
#include "../WebAdmin.h"
#include "../Item.h"
#include "../Root.h"
#include "../Server.h"
#include "../CommandOutput.h"
#include "../ChatColor.h"

#include "inifile/iniFile.h"
#include "../Entities/Player.h"

#define FIND_HOOK(a_HookName) HookMap::iterator Plugins = m_Hooks.find(a_HookName);
#define VERIFY_HOOK \
	if (Plugins == m_Hooks.end()) \
	{ \
		return false; \
	}





cPluginManager * cPluginManager::Get(void)
{
	return cRoot::Get()->GetPluginManager();
}





cPluginManager::cPluginManager(void) :
	m_bReloadPlugins(false)
{
}





cPluginManager::~cPluginManager()
{
	UnloadPluginsNow();
}





void cPluginManager::ReloadPlugins(void)
{
	m_bReloadPlugins = true;
}





void cPluginManager::FindPlugins(void)
{
	AString PluginsPath = GetPluginsPath() + "/";

	// First get a clean list of only the currently running plugins, we don't want to mess those up
	for (PluginMap::iterator itr = m_Plugins.begin(); itr != m_Plugins.end();)
	{
		if (itr->second == NULL)
		{
			PluginMap::iterator thiz = itr;
			++thiz;
			m_Plugins.erase( itr);
			itr = thiz;
			continue;
		}
		++itr;
	}

	AStringVector Files = cFile::GetFolderContents(PluginsPath.c_str());
	for (AStringVector::const_iterator itr = Files.begin(); itr != Files.end(); ++itr)
	{
		if ((*itr == ".") || (*itr == "..") || (!cFile::IsFolder(PluginsPath + *itr)))
		{
			// We only want folders, and don't want "." or ".."
			continue;
		}

		// Add plugin name/directory to the list
		if (m_Plugins.find(*itr) == m_Plugins.end())
		{
			m_Plugins[*itr] = NULL;
		}
	}
}





void cPluginManager::ReloadPluginsNow(void)
{
	cIniFile a_SettingsIni;
	a_SettingsIni.ReadFile("settings.ini");
	ReloadPluginsNow(a_SettingsIni);
}





void cPluginManager::ReloadPluginsNow(cIniFile & a_SettingsIni)
{
	LOG("-- Loading Plugins --");
	m_bReloadPlugins = false;
	UnloadPluginsNow();

	FindPlugins();

	cServer::BindBuiltInConsoleCommands();

	// Check if the Plugins section exists.
	int KeyNum = a_SettingsIni.FindKey("Plugins");

	// If it does, how many plugins are there?
	int NumPlugins = ((KeyNum != -1) ? (a_SettingsIni.GetNumValues(KeyNum)) : 0);

	if (KeyNum == -1)
	{
		InsertDefaultPlugins(a_SettingsIni);
	}
	else if (NumPlugins > 0)
	{
		for (int i = 0; i < NumPlugins; i++)
		{
			AString ValueName = a_SettingsIni.GetValueName(KeyNum, i);
			if (ValueName.compare("Plugin") == 0)
			{
				AString PluginFile = a_SettingsIni.GetValue(KeyNum, i);
				if (!PluginFile.empty())
				{
					if (m_Plugins.find(PluginFile) != m_Plugins.end())
					{
						LoadPlugin(PluginFile);
					}
				}
			}
		}
	}

	size_t NumLoadedPlugins = GetNumPlugins();
	if (NumLoadedPlugins == 0)
	{
		LOG("-- No Plugins Loaded --");
	}
	else if (NumLoadedPlugins > 1)
	{
		LOG("-- Loaded %i Plugins --", (int)NumLoadedPlugins);
	}
	else
	{
		LOG("-- Loaded 1 Plugin --");
	}
	CallHookPluginsLoaded();
}





void cPluginManager::InsertDefaultPlugins(cIniFile & a_SettingsIni)
{
	a_SettingsIni.AddKeyName("Plugins");
	a_SettingsIni.AddKeyComment("Plugins", " Plugin=Debuggers");
	a_SettingsIni.AddKeyComment("Plugins", " Plugin=HookNotify");
	a_SettingsIni.AddKeyComment("Plugins", " Plugin=ChunkWorx");
	a_SettingsIni.AddKeyComment("Plugins", " Plugin=APIDump");
	a_SettingsIni.AddValue("Plugins", "Plugin", "Core");
	a_SettingsIni.AddValue("Plugins", "Plugin", "TransAPI");
	a_SettingsIni.AddValue("Plugins", "Plugin", "ChatLog");
}





void cPluginManager::Tick(float a_Dt)
{
	while (!m_DisablePluginList.empty())
	{
		RemovePlugin(m_DisablePluginList.front());
		m_DisablePluginList.pop_front();
	}

	if (m_bReloadPlugins)
	{
		ReloadPluginsNow();
	}

	FIND_HOOK(HOOK_TICK);
	if (Plugins != m_Hooks.end())
	{
		for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
		{
			(*itr)->Tick(a_Dt);
		}
	}
}





bool cPluginManager::CallHookBlockSpread(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, eSpreadSource a_Source)
{
	FIND_HOOK(HOOK_BLOCK_SPREAD);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnBlockSpread(a_World, a_BlockX, a_BlockY, a_BlockZ, a_Source))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookBlockToPickups(
	cWorld * a_World, cEntity * a_Digger,
	int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta,
	cItems & a_Pickups
)
{
	FIND_HOOK(HOOK_BLOCK_TO_PICKUPS);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnBlockToPickups(a_World, a_Digger, a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, a_Pickups))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookChat(cPlayer * a_Player, AString & a_Message)
{
	// Check if the message contains a command, execute it:
	switch (HandleCommand(a_Player, a_Message, true))
	{
		case crExecuted:
		{
			// The command has executed successfully
			return true;
		}
		
		case crBlocked:
		{
			// The command was blocked by a plugin using HOOK_EXECUTE_COMMAND
			// The plugin has most likely sent a message to the player already
			return true;
		}
		
		case crError:
		{
			// An error in the plugin has prevented the command from executing. Report the error to the player:
			a_Player->SendMessageFailure(Printf("Something went wrong while executing command \"%s\"", a_Message.c_str()));
			return true;
		}
		
		case crNoPermission:
		{
			// The player is not allowed to execute this command
			a_Player->SendMessageFailure(Printf("Forbidden command; insufficient privileges: \"%s\"", a_Message.c_str()));
			return true;
		}

		case crUnknownCommand:
		{
			// This was not a known command, keep processing as a message
			break;
		}
	}

	// Check if the message is a command (starts with a slash). If it is, we know that it wasn't recognised:
	if (!a_Message.empty() && (a_Message[0] == '/'))
	{
		AStringVector Split(StringSplit(a_Message, " "));
		ASSERT(!Split.empty());  // This should not happen - we know there's at least one char in the message so the split needs to be at least one item long
		a_Player->SendMessageInfo(Printf("Unknown command: \"%s\"", a_Message.c_str()));
		LOGINFO("Player %s issued an unknown command: \"%s\"", a_Player->GetName().c_str(), a_Message.c_str());
		return true;  // Cancel sending
	}

	FIND_HOOK(HOOK_CHAT);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnChat(a_Player, a_Message))
		{
			return true;
		}
	}

	return false;
}





bool cPluginManager::CallHookChunkAvailable(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	FIND_HOOK(HOOK_CHUNK_AVAILABLE);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnChunkAvailable(a_World, a_ChunkX, a_ChunkZ))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookChunkGenerated(cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc)
{
	FIND_HOOK(HOOK_CHUNK_GENERATED);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnChunkGenerated(a_World, a_ChunkX, a_ChunkZ, a_ChunkDesc))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookChunkGenerating(cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc)
{
	FIND_HOOK(HOOK_CHUNK_GENERATING);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnChunkGenerating(a_World, a_ChunkX, a_ChunkZ, a_ChunkDesc))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookChunkUnloaded(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	FIND_HOOK(HOOK_CHUNK_UNLOADED);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnChunkUnloaded(a_World, a_ChunkX, a_ChunkZ))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookChunkUnloading(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	FIND_HOOK(HOOK_CHUNK_UNLOADING);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnChunkUnloading(a_World, a_ChunkX, a_ChunkZ))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookCollectingPickup(cPlayer * a_Player, cPickup & a_Pickup)
{
	FIND_HOOK(HOOK_COLLECTING_PICKUP);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_CRAFTING_NO_RECIPE);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnCraftingNoRecipe(a_Player, a_Grid, a_Recipe))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookDisconnect(cClientHandle & a_Client, const AString & a_Reason)
{
	FIND_HOOK(HOOK_DISCONNECT);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnDisconnect(a_Client, a_Reason))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookEntityAddEffect(cEntity & a_Entity, int a_EffectType, int a_EffectDurationTicks, int a_EffectIntensity, double a_DistanceModifier)
{
	HookMap::iterator Plugins = m_Hooks.find(HOOK_ENTITY_ADD_EFFECT);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnEntityAddEffect(a_Entity, a_EffectType, a_EffectDurationTicks, a_EffectIntensity, a_DistanceModifier))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookExecuteCommand(cPlayer * a_Player, const AStringVector & a_Split)
{
	FIND_HOOK(HOOK_EXECUTE_COMMAND);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnExecuteCommand(a_Player, a_Split))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookExploded(cWorld & a_World, double a_ExplosionSize, bool a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData)
{
	FIND_HOOK(HOOK_EXPLODED);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnExploded(a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookExploding(cWorld & a_World, double & a_ExplosionSize, bool & a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData)
{
	FIND_HOOK(HOOK_EXPLODING);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnExploding(a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookHandshake(cClientHandle * a_ClientHandle, const AString & a_Username)
{
	FIND_HOOK(HOOK_HANDSHAKE);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnHandshake(a_ClientHandle, a_Username))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookHopperPullingItem(cWorld & a_World, cHopperEntity & a_Hopper, int a_DstSlotNum, cBlockEntityWithItems & a_SrcEntity, int a_SrcSlotNum)
{
	FIND_HOOK(HOOK_HOPPER_PULLING_ITEM);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnHopperPullingItem(a_World, a_Hopper, a_DstSlotNum, a_SrcEntity, a_SrcSlotNum))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookHopperPushingItem(cWorld & a_World, cHopperEntity & a_Hopper, int a_SrcSlotNum, cBlockEntityWithItems & a_DstEntity, int a_DstSlotNum)
{
	FIND_HOOK(HOOK_HOPPER_PUSHING_ITEM);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnHopperPushingItem(a_World, a_Hopper, a_SrcSlotNum, a_DstEntity, a_DstSlotNum))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookKilling(cEntity & a_Victim, cEntity * a_Killer, TakeDamageInfo & a_TDI)
{
	FIND_HOOK(HOOK_KILLING);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnKilling(a_Victim, a_Killer, a_TDI))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookLogin(cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username)
{
	FIND_HOOK(HOOK_LOGIN);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnLogin(a_Client, a_ProtocolVersion, a_Username))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerAnimation(cPlayer & a_Player, int a_Animation)
{
	FIND_HOOK(HOOK_PLAYER_ANIMATION);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerAnimation(a_Player, a_Animation))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerBreakingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	FIND_HOOK(HOOK_PLAYER_BREAKING_BLOCK);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_PLAYER_BROKEN_BLOCK);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerBrokenBlock(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_BlockType, a_BlockMeta))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerDestroyed(cPlayer & a_Player)
{
	FIND_HOOK(HOOK_PLAYER_DESTROYED);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerDestroyed(a_Player))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerEating(cPlayer & a_Player)
{
	FIND_HOOK(HOOK_PLAYER_EATING);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerEating(a_Player))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerFoodLevelChange(cPlayer & a_Player, int a_NewFoodLevel)
{
	FIND_HOOK(HOOK_PLAYER_FOOD_LEVEL_CHANGE);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerFoodLevelChange(a_Player, a_NewFoodLevel))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerFished(cPlayer & a_Player, const cItems a_Reward)
{
	FIND_HOOK(HOOK_PLAYER_FISHED);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerFished(a_Player, a_Reward))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerFishing(cPlayer & a_Player, cItems a_Reward)
{
	FIND_HOOK(HOOK_PLAYER_FISHING);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerFishing(a_Player, a_Reward))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerJoined(cPlayer & a_Player)
{
	FIND_HOOK(HOOK_PLAYER_JOINED);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_PLAYER_LEFT_CLICK);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_PLAYER_MOVING);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_PLAYER_PLACED_BLOCK);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_PLAYER_PLACING_BLOCK);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_PLAYER_RIGHT_CLICK);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerRightClick(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerRightClickingEntity(cPlayer & a_Player, cEntity & a_Entity)
{
	FIND_HOOK(HOOK_PLAYER_RIGHT_CLICKING_ENTITY);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerRightClickingEntity(a_Player, a_Entity))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPlayerShooting(cPlayer & a_Player)
{
	FIND_HOOK(HOOK_PLAYER_SHOOTING);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_PLAYER_SPAWNED);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_PLAYER_TOSSING_ITEM);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_PLAYER_USED_BLOCK);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_PLAYER_USED_ITEM);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_PLAYER_USING_BLOCK);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_PLAYER_USING_ITEM);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPlayerUsingItem(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPluginMessage(cClientHandle & a_Client, const AString & a_Channel, const AString & a_Message)
{
	FIND_HOOK(HOOK_PLUGIN_MESSAGE);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPluginMessage(a_Client, a_Channel, a_Message))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookPluginsLoaded(void)
{
	FIND_HOOK(HOOK_PLUGINS_LOADED);
	VERIFY_HOOK;

	bool res = false;
	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		res = !(*itr)->OnPluginsLoaded() || res;
	}
	return res;
}





bool cPluginManager::CallHookPostCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	FIND_HOOK(HOOK_POST_CRAFTING);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_PRE_CRAFTING);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnPreCrafting(a_Player, a_Grid, a_Recipe))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookProjectileHitBlock(cProjectileEntity & a_Projectile, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Face, const Vector3d & a_BlockHitPos)
{
	FIND_HOOK(HOOK_PROJECTILE_HIT_BLOCK);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnProjectileHitBlock(a_Projectile, a_BlockX, a_BlockY, a_BlockZ, a_Face, a_BlockHitPos))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookProjectileHitEntity(cProjectileEntity & a_Projectile, cEntity & a_HitEntity)
{
	FIND_HOOK(HOOK_PROJECTILE_HIT_ENTITY);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnProjectileHitEntity(a_Projectile, a_HitEntity))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookSpawnedEntity(cWorld & a_World, cEntity & a_Entity)
{
	FIND_HOOK(HOOK_SPAWNED_ENTITY);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnSpawnedEntity(a_World, a_Entity))
		{
			return true;
		}
	}
	return false;
}




bool cPluginManager::CallHookSpawnedMonster(cWorld & a_World, cMonster & a_Monster)
{
	FIND_HOOK(HOOK_SPAWNED_MONSTER);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnSpawnedMonster(a_World, a_Monster))
		{
			return true;
		}
	}
	return false;
}




bool cPluginManager::CallHookSpawningEntity(cWorld & a_World, cEntity & a_Entity)
{
	FIND_HOOK(HOOK_SPAWNING_ENTITY);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnSpawningEntity(a_World, a_Entity))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookSpawningMonster(cWorld & a_World, cMonster & a_Monster)
{
	FIND_HOOK(HOOK_SPAWNING_MONSTER);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnSpawningMonster(a_World, a_Monster))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookTakeDamage(cEntity & a_Receiver, TakeDamageInfo & a_TDI)
{
	FIND_HOOK(HOOK_TAKE_DAMAGE);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_UPDATING_SIGN);
	VERIFY_HOOK;

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
	FIND_HOOK(HOOK_UPDATED_SIGN);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnUpdatedSign(a_World, a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4, a_Player))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookWeatherChanged(cWorld & a_World)
{
	FIND_HOOK(HOOK_WEATHER_CHANGED);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnWeatherChanged(a_World))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookWeatherChanging(cWorld & a_World, eWeather & a_NewWeather)
{
	FIND_HOOK(HOOK_WEATHER_CHANGING);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnWeatherChanging(a_World, a_NewWeather))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookWorldStarted(cWorld & a_World)
{
	FIND_HOOK(HOOK_WORLD_STARTED);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnWorldStarted(a_World))
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::CallHookWorldTick(cWorld & a_World, float a_Dt, int a_LastTickDurationMSec)
{
	FIND_HOOK(HOOK_WORLD_TICK);
	VERIFY_HOOK;

	for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
	{
		if ((*itr)->OnWorldTick(a_World, a_Dt, a_LastTickDurationMSec))
		{
			return true;
		}
	}
	return false;
}





cPluginManager::CommandResult cPluginManager::HandleCommand(cPlayer * a_Player, const AString & a_Command, bool a_ShouldCheckPermissions)
{
	ASSERT(a_Player != NULL);

	AStringVector Split(StringSplit(a_Command, " "));
	if (Split.empty())
	{
		return crUnknownCommand;
	}

	CommandMap::iterator cmd = m_Commands.find(Split[0]);
	if (cmd == m_Commands.end())
	{
		// Command not found
		return crUnknownCommand;
	}

	// Ask plugins first if a command is okay to execute the command:
	if (CallHookExecuteCommand(a_Player, Split))
	{
		LOGINFO("Player %s tried executing command \"%s\" that was stopped by the HOOK_EXECUTE_COMMAND hook", a_Player->GetName().c_str(), Split[0].c_str());
		return crBlocked;
	}

	if (
		a_ShouldCheckPermissions &&
		!cmd->second.m_Permission.empty() &&
		!a_Player->HasPermission(cmd->second.m_Permission)
	)
	{
		LOGINFO("Player %s tried to execute forbidden command: \"%s\"", a_Player->GetName().c_str(), Split[0].c_str());
		return crNoPermission;
	}

	ASSERT(cmd->second.m_Plugin != NULL);

	if (!cmd->second.m_Plugin->HandleCommand(Split, a_Player))
	{
		return crError;
	}

	return crExecuted;
}





cPlugin * cPluginManager::GetPlugin( const AString & a_Plugin) const
{
	for (PluginMap::const_iterator itr = m_Plugins.begin(); itr != m_Plugins.end(); ++itr)
	{
		if (itr->second == NULL) continue;
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

	while (!m_Plugins.empty())
	{
		RemovePlugin(m_Plugins.begin()->second);
	}

	m_Commands.clear();
	m_ConsoleCommands.clear();
}





bool cPluginManager::DisablePlugin(const AString & a_PluginName)
{
	PluginMap::iterator itr = m_Plugins.find(a_PluginName);
	if (itr == m_Plugins.end())
	{
		return false;
	}

	if (itr->first.compare(a_PluginName) == 0)  // _X 2013_02_01: wtf? Isn't this supposed to be what find() does?
	{
		m_DisablePluginList.push_back(itr->second);
		itr->second = NULL;  // Get rid of this thing right away
		return true;
	}
	return false;
}





bool cPluginManager::LoadPlugin(const AString & a_PluginName)
{
	return AddPlugin(new cPluginLua(a_PluginName.c_str()));
}





void cPluginManager::RemoveHooks(cPlugin * a_Plugin)
{
	for (HookMap::iterator itr = m_Hooks.begin(), end = m_Hooks.end(); itr != end; ++itr)
	{
		itr->second.remove(a_Plugin);
	}
}





void cPluginManager::RemovePlugin(cPlugin * a_Plugin)
{
	for (PluginMap::iterator itr = m_Plugins.begin(); itr != m_Plugins.end(); ++itr)
	{
		if (itr->second == a_Plugin)
		{
			m_Plugins.erase(itr);
			break;
		}
	}

	RemovePluginCommands(a_Plugin);
	RemovePluginConsoleCommands(a_Plugin);
	RemoveHooks(a_Plugin);
	if (a_Plugin != NULL)
	{
		a_Plugin->OnDisable();
	}
	delete a_Plugin;
	a_Plugin = NULL;
}





void cPluginManager::RemovePluginCommands(cPlugin * a_Plugin)
{
	if (a_Plugin != NULL)
	{
		a_Plugin->ClearCommands();
	}

	for (CommandMap::iterator itr = m_Commands.begin(); itr != m_Commands.end();)
	{
		if (itr->second.m_Plugin == a_Plugin)
		{
			CommandMap::iterator EraseMe = itr;  // Stupid GCC doesn't have a std::map::erase() that would return the next iterator
			++itr;
			m_Commands.erase(EraseMe);
		}
		else
		{
			++itr;
		}
	}  // for itr - m_Commands[]
}





bool cPluginManager::BindCommand(const AString & a_Command, cPlugin * a_Plugin, const AString & a_Permission, const AString & a_HelpString)
{
	CommandMap::iterator cmd = m_Commands.find(a_Command);
	if (cmd != m_Commands.end())
	{
		LOGWARNING("Command \"%s\" is already bound to plugin \"%s\".", a_Command.c_str(), cmd->second.m_Plugin->GetName().c_str());
		return false;
	}

	m_Commands[a_Command].m_Plugin     = a_Plugin;
	m_Commands[a_Command].m_Permission = a_Permission;
	m_Commands[a_Command].m_HelpString = a_HelpString;
	return true;
}





bool cPluginManager::ForEachCommand(cCommandEnumCallback & a_Callback)
{
	for (CommandMap::iterator itr = m_Commands.begin(), end = m_Commands.end(); itr != end; ++itr)
	{
		if (a_Callback.Command(itr->first, itr->second.m_Plugin, itr->second.m_Permission, itr->second.m_HelpString))
		{
			return false;
		}
	}  // for itr - m_Commands[]
	return true;
}





bool cPluginManager::IsCommandBound(const AString & a_Command)
{
	return (m_Commands.find(a_Command) != m_Commands.end());
}





AString cPluginManager::GetCommandPermission(const AString & a_Command)
{
	CommandMap::iterator cmd = m_Commands.find(a_Command);
	return (cmd == m_Commands.end()) ? "" : cmd->second.m_Permission;
}





cPluginManager::CommandResult cPluginManager::ExecuteCommand(cPlayer * a_Player, const AString & a_Command)
{
	return HandleCommand(a_Player, a_Command, true);
}





cPluginManager::CommandResult cPluginManager::ForceExecuteCommand(cPlayer * a_Player, const AString & a_Command)
{
	return HandleCommand(a_Player, a_Command, false);
}





void cPluginManager::RemovePluginConsoleCommands(cPlugin * a_Plugin)
{
	if (a_Plugin != NULL)
	{
		a_Plugin->ClearConsoleCommands();
	}

	for (CommandMap::iterator itr = m_ConsoleCommands.begin(); itr != m_ConsoleCommands.end();)
	{
		if (itr->second.m_Plugin == a_Plugin)
		{
			CommandMap::iterator EraseMe = itr;  // Stupid GCC doesn't have a std::map::erase() that would return the next iterator
			++itr;
			m_ConsoleCommands.erase(EraseMe);
		}
		else
		{
			++itr;
		}
	}  // for itr - m_Commands[]
}





bool cPluginManager::BindConsoleCommand(const AString & a_Command, cPlugin * a_Plugin, const AString & a_HelpString)
{
	CommandMap::iterator cmd = m_ConsoleCommands.find(a_Command);
	if (cmd != m_ConsoleCommands.end())
	{
		if (cmd->second.m_Plugin == NULL)
		{
			LOGWARNING("Console command \"%s\" is already bound internally by MCServer, cannot bind in plugin \"%s\".", a_Command.c_str(), a_Plugin->GetName().c_str());
		}
		else
		{
			LOGWARNING("Console command \"%s\" is already bound to plugin \"%s\", cannot bind in plugin \"%s\".", a_Command.c_str(), cmd->second.m_Plugin->GetName().c_str(), a_Plugin->GetName().c_str());
		}
		return false;
	}

	m_ConsoleCommands[a_Command].m_Plugin     = a_Plugin;
	m_ConsoleCommands[a_Command].m_Permission = "";
	m_ConsoleCommands[a_Command].m_HelpString = a_HelpString;
	return true;
}





bool cPluginManager::ForEachConsoleCommand(cCommandEnumCallback & a_Callback)
{
	for (CommandMap::iterator itr = m_ConsoleCommands.begin(), end = m_ConsoleCommands.end(); itr != end; ++itr)
	{
		if (a_Callback.Command(itr->first, itr->second.m_Plugin, "", itr->second.m_HelpString))
		{
			return false;
		}
	}  // for itr - m_Commands[]
	return true;
}





bool cPluginManager::IsConsoleCommandBound(const AString & a_Command)
{
	return (m_ConsoleCommands.find(a_Command) != m_ConsoleCommands.end());
}





bool cPluginManager::ExecuteConsoleCommand(const AStringVector & a_Split, cCommandOutputCallback & a_Output)
{
	if (a_Split.empty())
	{
		return false;
	}

	CommandMap::iterator cmd = m_ConsoleCommands.find(a_Split[0]);
	if (cmd == m_ConsoleCommands.end())
	{
		// Command not found
		return false;
	}

	if (cmd->second.m_Plugin == NULL)
	{
		// This is a built-in command
		return false;
	}

	// Ask plugins first if a command is okay to execute the console command:
	if (CallHookExecuteCommand(NULL, a_Split))
	{
		a_Output.Out("Command \"%s\" was stopped by the HOOK_EXECUTE_COMMAND hook", a_Split[0].c_str());
		return false;
	}

	return cmd->second.m_Plugin->HandleConsoleCommand(a_Split, a_Output);
}





void cPluginManager::TabCompleteCommand(const AString & a_Text, AStringVector & a_Results, cPlayer * a_Player)
{
	for (CommandMap::iterator itr = m_Commands.begin(), end = m_Commands.end(); itr != end; ++itr)
	{
		if (NoCaseCompare(itr->first.substr(0, a_Text.length()), a_Text) != 0)
		{
			// Command name doesn't match
			continue;
		}
		if ((a_Player != NULL) && !a_Player->HasPermission(itr->second.m_Permission))
		{
			// Player doesn't have permission for the command
			continue;
		}
		a_Results.push_back(itr->first);
	}
}





bool cPluginManager::IsValidHookType(int a_HookType)
{
	return ((a_HookType >= 0) && (a_HookType <= HOOK_MAX));
}





bool cPluginManager::DoWithPlugin(const AString & a_PluginName, cPluginCallback & a_Callback)
{
	// TODO: Implement locking for plugins
	PluginMap::iterator itr = m_Plugins.find(a_PluginName);
	if ((itr == m_Plugins.end()) || (itr->second == NULL))
	{
		return false;
	}
	return a_Callback.Item(itr->second);
}





bool cPluginManager::AddPlugin(cPlugin * a_Plugin)
{
	m_Plugins[a_Plugin->GetDirectory()] = a_Plugin;
	if (a_Plugin->Initialize())
	{
		// Initialization OK
		return true;
	}

	// Initialization failed
	RemovePlugin(a_Plugin);  // Also undoes any registrations that Initialize() might have made
	return false;
}





void cPluginManager::AddHook(cPlugin * a_Plugin, int a_Hook)
{
	if (!a_Plugin)
	{
		LOGWARN("Called cPluginManager::AddHook() with a_Plugin == NULL");
		return;
	}
	PluginList & Plugins = m_Hooks[a_Hook];
	Plugins.remove(a_Plugin);
	Plugins.push_back(a_Plugin);
}





size_t cPluginManager::GetNumPlugins() const
{
	return m_Plugins.size();
}




