
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "PluginManager.h"
#include "Plugin.h"
#include "PluginLua.h"
#include "../Item.h"
#include "../Root.h"
#include "../Server.h"
#include "../CommandOutput.h"

#include "../IniFile.h"
#include "../Entities/Player.h"





cPluginManager * cPluginManager::Get(void)
{
	return cRoot::Get()->GetPluginManager();
}





cPluginManager::cPluginManager(cDeadlockDetect & a_DeadlockDetect) :
	m_bReloadPlugins(false),
	m_DeadlockDetect(a_DeadlockDetect)
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





void cPluginManager::RefreshPluginList(void)
{
	// Get a list of currently available folders:
	AString PluginsPath = GetPluginsPath() + "/";
	AStringVector Contents = cFile::GetFolderContents(PluginsPath);
	AStringVector Folders;
	for (auto & item: Contents)
	{
		if (!cFile::IsFolder(PluginsPath + item))
		{
			// We only want folders, and don't want "." or ".."
			continue;
		}
		Folders.push_back(item);
	}  // for item - Contents[]

	// Set all plugins with invalid folders as psNotFound:
	for (auto & plugin: m_Plugins)
	{
		if (std::find(Folders.cbegin(), Folders.cend(), plugin->GetFolderName()) == Folders.end())
		{
			plugin->m_Status = psNotFound;
		}
	}  // for plugin - m_Plugins[]

	// Add all newly discovered plugins:
	for (auto & folder: Folders)
	{
		bool hasFound = false;
		for (auto & plugin: m_Plugins)
		{
			if (plugin->GetFolderName() == folder)
			{
				hasFound = true;
				break;
			}
		}  // for plugin - m_Plugins[]
		if (!hasFound)
		{
			m_Plugins.push_back(std::make_shared<cPluginLua>(folder, m_DeadlockDetect));
		}
	}  // for folder - Folders[]
}





void cPluginManager::ReloadPluginsNow(void)
{
	cIniFile a_SettingsIni;
	a_SettingsIni.ReadFile(cRoot::Get()->m_SettingsFilename);
	ReloadPluginsNow(a_SettingsIni);
}





void cPluginManager::ReloadPluginsNow(cSettingsRepositoryInterface & a_Settings)
{
	LOG("-- Loading Plugins --");

	// Unload any existing plugins:
	m_bReloadPlugins = false;
	UnloadPluginsNow();

	// Refresh the list of plugins to load new ones from disk / remove the deleted ones:
	RefreshPluginList();

	// Load the plugins:
	AStringVector ToLoad = GetFoldersToLoad(a_Settings);
	for (auto & pluginFolder: ToLoad)
	{
		LoadPlugin(pluginFolder);
	}  // for pluginFolder - ToLoad[]

	// Log a report of the loading process
	size_t NumLoadedPlugins = GetNumLoadedPlugins();
	if (NumLoadedPlugins == 0)
	{
		LOG("-- No Plugins Loaded --");
	}
	else if (NumLoadedPlugins == 1)
	{
		LOG("-- Loaded 1 Plugin --");
	}
	else
	{
		LOG("-- Loaded %u Plugins --", static_cast<unsigned>(NumLoadedPlugins));
	}
	CallHookPluginsLoaded();
}





void cPluginManager::InsertDefaultPlugins(cSettingsRepositoryInterface & a_Settings)
{
	a_Settings.AddKeyName("Plugins");
	a_Settings.AddValue("Plugins", "Core", "1");
	a_Settings.AddValue("Plugins", "ChatLog", "1");
	a_Settings.AddValue("Plugins", "ProtectionAreas", "0");
}





void cPluginManager::Tick(float a_Dt)
{
	decltype(m_PluginsNeedAction) PluginsNeedAction;
	{
		cCSLock Lock(m_CSPluginsNeedAction);
		std::swap(m_PluginsNeedAction, PluginsNeedAction);
	}

	// Process deferred actions:
	for (auto & CurrentPlugin : PluginsNeedAction)
	{
		auto & Action = CurrentPlugin.first;
		auto & Folder = CurrentPlugin.second;

		bool WasLoaded = false;
		bool WasFound = false;
		for (auto & Plugin: m_Plugins)
		{
			if (Plugin->GetFolderName() == Folder)
			{
				WasFound = true;
				if (Plugin->IsLoaded())
				{
					switch (Action)
					{
						case PluginAction::Reload :
						{
							// Reload plugins by unloading, then loading:
							Plugin->Unload();
							Plugin->Load();
							break;
						}
						case PluginAction::Unload :
						{
							// Unload plugins that have been scheduled for unloading:
							Plugin->Unload();
							break;
						}
					}
					WasLoaded = true;
				}
			}
		}
		if (!WasFound)
		{
			LOG("Cannot act on plugin in folder \"%s\", there's no such plugin folder", Folder.c_str());
		}
		else if (!WasLoaded)
		{
			LOG("Cannot act on plugin in folder \"%s\", it has not been loaded.", Folder.c_str());
		}
	}  // for plugin - m_Plugins[]

	// If a plugin reload has been scheduled, reload now:
	if (m_bReloadPlugins)
	{
		ReloadPluginsNow();
	}

	auto Plugins = m_Hooks.find(HOOK_TICK);
	if (Plugins == m_Hooks.end())
	{
		return;
	}

	for (auto * Plugin : Plugins->second)
	{
		Plugin->Tick(a_Dt);
	}
}





template <typename HookFunction>
bool cPluginManager::GenericCallHook(PluginHook a_HookName, HookFunction a_HookFunction)
{
	auto Plugins = m_Hooks.find(a_HookName);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}

	return std::any_of(Plugins->second.begin(), Plugins->second.end(), a_HookFunction);
}





bool cPluginManager::CallHookBlockSpread(cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, eSpreadSource a_Source)
{
	return GenericCallHook(HOOK_BLOCK_SPREAD, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnBlockSpread(a_World, a_BlockX, a_BlockY, a_BlockZ, a_Source);
		}
	);
}





bool cPluginManager::CallHookBlockToPickups(
	cWorld & a_World,
	Vector3i a_BlockPos,
	BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta,
	const cBlockEntity * a_BlockEntity,
	const cEntity * a_Digger,
	const cItem * a_Tool,
	cItems & a_Pickups
)
{
	return GenericCallHook(HOOK_BLOCK_TO_PICKUPS, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnBlockToPickups(a_World, a_BlockPos, a_BlockType, a_BlockMeta, a_BlockEntity, a_Digger, a_Tool, a_Pickups);
		}
	);
}





bool cPluginManager::CallHookBrewingCompleted(cWorld & a_World, cBrewingstandEntity & a_Brewingstand)
{
	return GenericCallHook(HOOK_BREWING_COMPLETED, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnBrewingCompleted(a_World, a_Brewingstand);
		}
	);
}





bool cPluginManager::CallHookBrewingCompleting(cWorld & a_World, cBrewingstandEntity & a_Brewingstand)
{
	return GenericCallHook(HOOK_BREWING_COMPLETING, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnBrewingCompleting(a_World, a_Brewingstand);
		}
	);
}





bool cPluginManager::CallHookChat(cPlayer & a_Player, AString & a_Message)
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
			a_Player.SendMessageFailure(Printf("Something went wrong while executing command \"%s\"", a_Message.c_str()));
			return true;
		}

		case crNoPermission:
		{
			// The player is not allowed to execute this command
			a_Player.SendMessageFailure(Printf("Forbidden command; insufficient privileges: \"%s\"", a_Message.c_str()));
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
		a_Player.SendMessageInfo(Printf("Unknown command: \"%s\"", a_Message.c_str()));
		LOGINFO("Player %s issued an unknown command: \"%s\"", a_Player.GetName().c_str(), a_Message.c_str());
		return true;  // Cancel sending
	}

	return GenericCallHook(HOOK_CHAT, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnChat(a_Player, a_Message);
		}
	);
}





bool cPluginManager::CallHookChunkAvailable(cWorld & a_World, int a_ChunkX, int a_ChunkZ)
{
	return GenericCallHook(HOOK_CHUNK_AVAILABLE, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnChunkAvailable(a_World, a_ChunkX, a_ChunkZ);
		}
	);
}





bool cPluginManager::CallHookChunkGenerated(cWorld & a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc)
{
	return GenericCallHook(HOOK_CHUNK_GENERATED, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnChunkGenerated(a_World, a_ChunkX, a_ChunkZ, a_ChunkDesc);
		}
	);
}





bool cPluginManager::CallHookChunkGenerating(cWorld & a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc)
{
	return GenericCallHook(HOOK_CHUNK_GENERATING, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnChunkGenerating(a_World, a_ChunkX, a_ChunkZ, a_ChunkDesc);
		}
	);
}





bool cPluginManager::CallHookChunkUnloaded(cWorld & a_World, int a_ChunkX, int a_ChunkZ)
{
	return GenericCallHook(HOOK_CHUNK_UNLOADED, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnChunkUnloaded(a_World, a_ChunkX, a_ChunkZ);
		}
	);
}





bool cPluginManager::CallHookChunkUnloading(cWorld & a_World, int a_ChunkX, int a_ChunkZ)
{
	return GenericCallHook(HOOK_CHUNK_UNLOADING, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnChunkUnloading(a_World, a_ChunkX, a_ChunkZ);
		}
	);
}





bool cPluginManager::CallHookCollectingPickup(cPlayer & a_Player, cPickup & a_Pickup)
{
	return GenericCallHook(HOOK_COLLECTING_PICKUP, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnCollectingPickup(a_Player, a_Pickup);
		}
	);
}





bool cPluginManager::CallHookCraftingNoRecipe(cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe)
{
	return GenericCallHook(HOOK_CRAFTING_NO_RECIPE, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnCraftingNoRecipe(a_Player, a_Grid, a_Recipe);
		}
	);
}





bool cPluginManager::CallHookDisconnect(cClientHandle & a_Client, const AString & a_Reason)
{
	return GenericCallHook(HOOK_DISCONNECT, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnDisconnect(a_Client, a_Reason);
		}
	);
}





bool cPluginManager::CallHookEntityAddEffect(cEntity & a_Entity, int a_EffectType, int a_EffectDurationTicks, int a_EffectIntensity, double a_DistanceModifier)
{
	return GenericCallHook(HOOK_ENTITY_ADD_EFFECT, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnEntityAddEffect(a_Entity, a_EffectType, a_EffectDurationTicks, a_EffectIntensity, a_DistanceModifier);
		}
	);
}





bool cPluginManager::CallHookEntityTeleport(cEntity & a_Entity, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition)
{
	return GenericCallHook(HOOK_ENTITY_TELEPORT, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnEntityTeleport(a_Entity, a_OldPosition, a_NewPosition);
		}
	);
}





bool cPluginManager::CallHookEntityChangingWorld(cEntity & a_Entity, cWorld & a_World)
{
	return GenericCallHook(HOOK_ENTITY_CHANGING_WORLD, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnEntityChangingWorld(a_Entity, a_World);
		}
	);
}





bool cPluginManager::CallHookEntityChangedWorld(cEntity & a_Entity, cWorld & a_World)
{
	return GenericCallHook(HOOK_ENTITY_CHANGED_WORLD, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnEntityChangedWorld(a_Entity, a_World);
		}
	);
}





bool cPluginManager::CallHookExecuteCommand(cPlayer * a_Player, const AStringVector & a_Split, const AString & a_EntireCommand, CommandResult & a_Result)
{
	// Output the command being executed to log (for troubleshooting deadlocks-in-commands):
	if (a_Player != nullptr)
	{
		auto world = a_Player->GetWorld();
		AString worldName;
		Int64 worldAge;
		if (world != nullptr)
		{
			worldName = world->GetName();
			worldAge = world->GetWorldAge();
		}
		else
		{
			worldName = "<no world>";
			worldAge = 0;
		}
		LOG("Player %s is executing command \"%s\" in world \"%s\" at world age %lld.",
			a_Player->GetName().c_str(),
			a_EntireCommand.c_str(),
			worldName.c_str(),
			worldAge
		);
	}

	return GenericCallHook(HOOK_EXECUTE_COMMAND, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnExecuteCommand(a_Player, a_Split, a_EntireCommand, a_Result);
		}
	);
}





bool cPluginManager::CallHookExploded(cWorld & a_World, double a_ExplosionSize, bool a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData)
{
	return GenericCallHook(HOOK_EXPLODED, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnExploded(a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData);
		}
	);
}





bool cPluginManager::CallHookExploding(cWorld & a_World, double & a_ExplosionSize, bool & a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData)
{
	return GenericCallHook(HOOK_EXPLODING, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnExploding(a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData);
		}
	);
}





bool cPluginManager::CallHookHandshake(cClientHandle & a_ClientHandle, const AString & a_Username)
{
	return GenericCallHook(HOOK_HANDSHAKE, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnHandshake(a_ClientHandle, a_Username);
		}
	);
}





bool cPluginManager::CallHookHopperPullingItem(cWorld & a_World, cHopperEntity & a_Hopper, int a_DstSlotNum, cBlockEntityWithItems & a_SrcEntity, int a_SrcSlotNum)
{
	return GenericCallHook(HOOK_HOPPER_PULLING_ITEM, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnHopperPullingItem(a_World, a_Hopper, a_DstSlotNum, a_SrcEntity, a_SrcSlotNum);
		}
	);
}





bool cPluginManager::CallHookHopperPushingItem(cWorld & a_World, cHopperEntity & a_Hopper, int a_SrcSlotNum, cBlockEntityWithItems & a_DstEntity, int a_DstSlotNum)
{
	return GenericCallHook(HOOK_HOPPER_PUSHING_ITEM, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnHopperPushingItem(a_World, a_Hopper, a_SrcSlotNum, a_DstEntity, a_DstSlotNum);
		}
	);
}





bool cPluginManager::CallHookDropSpense(cWorld & a_World, cDropSpenserEntity & a_DropSpenser, int a_SlotNum)
{
	return GenericCallHook(HOOK_DROPSPENSE, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnDropSpense(a_World, a_DropSpenser, a_SlotNum);
		}
	);
}





bool cPluginManager::CallHookKilled(cEntity & a_Victim, TakeDamageInfo & a_TDI, AString & a_DeathMessage)
{
	return GenericCallHook(HOOK_KILLED, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnKilled(a_Victim, a_TDI, a_DeathMessage);
		}
	);
}





bool cPluginManager::CallHookKilling(cEntity & a_Victim, cEntity * a_Killer, TakeDamageInfo & a_TDI)
{
	return GenericCallHook(HOOK_KILLING, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnKilling(a_Victim, a_Killer, a_TDI);
		}
	);
}





bool cPluginManager::CallHookLogin(cClientHandle & a_Client, UInt32 a_ProtocolVersion, const AString & a_Username)
{
	return GenericCallHook(HOOK_LOGIN, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnLogin(a_Client, a_ProtocolVersion, a_Username);
		}
	);
}





bool cPluginManager::CallHookLoginForge(cClientHandle & a_Client, AStringMap & a_Mods)
{
	return GenericCallHook(HOOK_LOGIN_FORGE, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnLoginForge(a_Client, a_Mods);
		}
	);
}





bool cPluginManager::CallHookPlayerAnimation(cPlayer & a_Player, int a_Animation)
{
	return GenericCallHook(HOOK_PLAYER_ANIMATION, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerAnimation(a_Player, a_Animation);
		}
	);
}





bool cPluginManager::CallHookPlayerBreakingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	return GenericCallHook(HOOK_PLAYER_BREAKING_BLOCK, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerBreakingBlock(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_BlockType, a_BlockMeta);
		}
	);
}





bool cPluginManager::CallHookPlayerBrokenBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	return GenericCallHook(HOOK_PLAYER_BROKEN_BLOCK, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerBrokenBlock(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_BlockType, a_BlockMeta);
		}
	);
}





bool cPluginManager::CallHookPlayerDestroyed(cPlayer & a_Player)
{
	return GenericCallHook(HOOK_PLAYER_DESTROYED, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerDestroyed(a_Player);
		}
	);
}





bool cPluginManager::CallHookPlayerEating(cPlayer & a_Player)
{
	return GenericCallHook(HOOK_PLAYER_EATING, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerEating(a_Player);
		}
	);
}





bool cPluginManager::CallHookPlayerFoodLevelChange(cPlayer & a_Player, int a_NewFoodLevel)
{
	return GenericCallHook(HOOK_PLAYER_FOOD_LEVEL_CHANGE, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerFoodLevelChange(a_Player, a_NewFoodLevel);
		}
	);
}





bool cPluginManager::CallHookPlayerFished(cPlayer & a_Player, const cItems & a_Reward)
{
	return GenericCallHook(HOOK_PLAYER_FISHED, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerFished(a_Player, a_Reward);
		}
	);
}





bool cPluginManager::CallHookPlayerFishing(cPlayer & a_Player, cItems a_Reward)
{
	return GenericCallHook(HOOK_PLAYER_FISHING, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerFishing(a_Player, a_Reward);
		}
	);
}





bool cPluginManager::CallHookPlayerJoined(cPlayer & a_Player)
{
	return GenericCallHook(HOOK_PLAYER_JOINED, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerJoined(a_Player);
		}
	);
}





bool cPluginManager::CallHookPlayerLeftClick(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status)
{
	return GenericCallHook(HOOK_PLAYER_LEFT_CLICK, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerLeftClick(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_Status);
		}
	);
}





bool cPluginManager::CallHookPlayerMoving(cPlayer & a_Player, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition, bool a_PreviousIsOnGround)
{
	return GenericCallHook(HOOK_PLAYER_MOVING, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerMoving(a_Player, a_OldPosition, a_NewPosition, a_PreviousIsOnGround);
		}
	);
}





bool cPluginManager::CallHookPlayerOpeningWindow(cPlayer & a_Player, cWindow & a_Window)
{
	return GenericCallHook(HOOK_PLAYER_OPENING_WINDOW, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerOpeningWindow(a_Player, a_Window);
		}
	);
}





bool cPluginManager::CallHookPlayerPlacedBlock(cPlayer & a_Player, const sSetBlock & a_BlockChange)
{
	return GenericCallHook(HOOK_PLAYER_PLACED_BLOCK, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerPlacedBlock(a_Player, a_BlockChange);
		}
	);
}





bool cPluginManager::CallHookPlayerPlacingBlock(cPlayer & a_Player, const sSetBlock & a_BlockChange)
{
	return GenericCallHook(HOOK_PLAYER_PLACING_BLOCK, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerPlacingBlock(a_Player, a_BlockChange);
		}
	);
}





bool cPluginManager::CallHookPlayerCrouched(cPlayer & a_Player)
{
	return GenericCallHook(HOOK_PLAYER_CROUCHED, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerCrouched(a_Player);
		}
	);
}





bool cPluginManager::CallHookPlayerRightClick(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	return GenericCallHook(HOOK_PLAYER_RIGHT_CLICK, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerRightClick(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ);
		}
	);
}





bool cPluginManager::CallHookPlayerRightClickingEntity(cPlayer & a_Player, cEntity & a_Entity)
{
	return GenericCallHook(HOOK_PLAYER_RIGHT_CLICKING_ENTITY, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerRightClickingEntity(a_Player, a_Entity);
		}
	);
}





bool cPluginManager::CallHookPlayerShooting(cPlayer & a_Player)
{
	return GenericCallHook(HOOK_PLAYER_SHOOTING, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerShooting(a_Player);
		}
	);
}





bool cPluginManager::CallHookPlayerSpawned(cPlayer & a_Player)
{
	return GenericCallHook(HOOK_PLAYER_SPAWNED, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerSpawned(a_Player);
		}
	);
}





bool cPluginManager::CallHookPlayerTossingItem(cPlayer & a_Player)
{
	return GenericCallHook(HOOK_PLAYER_TOSSING_ITEM, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerTossingItem(a_Player);
		}
	);
}





bool cPluginManager::CallHookPlayerUsedBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	return GenericCallHook(HOOK_PLAYER_USED_BLOCK, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerUsedBlock(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, a_BlockType, a_BlockMeta);
		}
	);
}





bool cPluginManager::CallHookPlayerUsedItem(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	return GenericCallHook(HOOK_PLAYER_USED_ITEM, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerUsedItem(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ);
		}
	);
}





bool cPluginManager::CallHookPlayerUsingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	return GenericCallHook(HOOK_PLAYER_USING_BLOCK, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerUsingBlock(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, a_BlockType, a_BlockMeta);
		}
	);
}





bool cPluginManager::CallHookPlayerUsingItem(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	return GenericCallHook(HOOK_PLAYER_USING_ITEM, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPlayerUsingItem(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ);
		}
	);
}





bool cPluginManager::CallHookPluginMessage(cClientHandle & a_Client, const AString & a_Channel, const AString & a_Message)
{
	return GenericCallHook(HOOK_PLUGIN_MESSAGE, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPluginMessage(a_Client, a_Channel, a_Message);
		}
	);
}





bool cPluginManager::CallHookPluginsLoaded(void)
{
	auto Plugins = m_Hooks.find(HOOK_PLUGINS_LOADED);
	if (Plugins == m_Hooks.end())
	{
		return false;
	}

	bool res = false;
	for (auto * Plugin : Plugins->second)
	{
		if (!Plugin->OnPluginsLoaded())
		{
			res = true;
		}
	}
	return res;
}





bool cPluginManager::CallHookPostCrafting(cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe)
{
	return GenericCallHook(HOOK_POST_CRAFTING, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPostCrafting(a_Player, a_Grid, a_Recipe);
		}
	);
}





bool cPluginManager::CallHookPreCrafting(cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe)
{
	return GenericCallHook(HOOK_PRE_CRAFTING, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnPreCrafting(a_Player, a_Grid, a_Recipe);
		}
	);
}





bool cPluginManager::CallHookProjectileHitBlock(cProjectileEntity & a_Projectile, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Face, const Vector3d & a_BlockHitPos)
{
	return GenericCallHook(HOOK_PROJECTILE_HIT_BLOCK, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnProjectileHitBlock(a_Projectile, a_BlockX, a_BlockY, a_BlockZ, a_Face, a_BlockHitPos);
		}
	);
}





bool cPluginManager::CallHookProjectileHitEntity(cProjectileEntity & a_Projectile, cEntity & a_HitEntity)
{
	return GenericCallHook(HOOK_PROJECTILE_HIT_ENTITY, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnProjectileHitEntity(a_Projectile, a_HitEntity);
		}
	);
}





bool cPluginManager::CallHookServerPing(cClientHandle & a_ClientHandle, AString & a_ServerDescription, int & a_OnlinePlayersCount, int & a_MaxPlayersCount, AString & a_Favicon)
{
	return GenericCallHook(HOOK_SERVER_PING, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnServerPing(a_ClientHandle, a_ServerDescription, a_OnlinePlayersCount, a_MaxPlayersCount, a_Favicon);
		}
	);
}





bool cPluginManager::CallHookSpawnedEntity(cWorld & a_World, cEntity & a_Entity)
{
	return GenericCallHook(HOOK_SPAWNED_ENTITY, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnSpawnedEntity(a_World, a_Entity);
		}
	);
}





bool cPluginManager::CallHookSpawnedMonster(cWorld & a_World, cMonster & a_Monster)
{
	return GenericCallHook(HOOK_SPAWNED_MONSTER, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnSpawnedMonster(a_World, a_Monster);
		}
	);
}





bool cPluginManager::CallHookSpawningEntity(cWorld & a_World, cEntity & a_Entity)
{
	return GenericCallHook(HOOK_SPAWNING_ENTITY, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnSpawningEntity(a_World, a_Entity);
		}
	);
}





bool cPluginManager::CallHookSpawningMonster(cWorld & a_World, cMonster & a_Monster)
{
	return GenericCallHook(HOOK_SPAWNING_MONSTER, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnSpawningMonster(a_World, a_Monster);
		}
	);
}





bool cPluginManager::CallHookTakeDamage(cEntity & a_Receiver, TakeDamageInfo & a_TDI)
{
	return GenericCallHook(HOOK_TAKE_DAMAGE, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnTakeDamage(a_Receiver, a_TDI);
		}
	);
}





bool cPluginManager::CallHookUpdatingSign(cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4, cPlayer * a_Player)
{
	return GenericCallHook(HOOK_UPDATING_SIGN, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnUpdatingSign(a_World, a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4, a_Player);
		}
	);
}





bool cPluginManager::CallHookUpdatedSign(cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player)
{
	return GenericCallHook(HOOK_UPDATED_SIGN, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnUpdatedSign(a_World, a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4, a_Player);
		}
	);
}





bool cPluginManager::CallHookWeatherChanged(cWorld & a_World)
{
	return GenericCallHook(HOOK_WEATHER_CHANGED, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnWeatherChanged(a_World);
		}
	);
}





bool cPluginManager::CallHookWeatherChanging(cWorld & a_World, eWeather & a_NewWeather)
{
	return GenericCallHook(HOOK_WEATHER_CHANGING, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnWeatherChanging(a_World, a_NewWeather);
		}
	);
}





bool cPluginManager::CallHookWorldStarted(cWorld & a_World)
{
	return GenericCallHook(HOOK_WORLD_STARTED, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnWorldStarted(a_World);
		}
	);
}





bool cPluginManager::CallHookWorldTick(cWorld & a_World, std::chrono::milliseconds a_Dt, std::chrono::milliseconds a_LastTickDurationMSec)
{
	return GenericCallHook(HOOK_WORLD_TICK, [&](cPlugin * a_Plugin)
		{
			return a_Plugin->OnWorldTick(a_World, a_Dt, a_LastTickDurationMSec);
		}
	);
}





cPluginManager::CommandResult cPluginManager::HandleCommand(cPlayer & a_Player, const AString & a_Command, bool a_ShouldCheckPermissions)
{
	AStringVector Split(StringSplit(a_Command, " "));
	if (Split.empty())
	{
		return crUnknownCommand;
	}

	CommandMap::iterator cmd = m_Commands.find(Split[0]);
	if (cmd == m_Commands.end())
	{
		// Command not found
		// If it started with a slash, ask the plugins if they still want to handle it:
		if (!a_Command.empty() && (a_Command[0] == '/'))
		{
			CommandResult Result = crUnknownCommand;
			CallHookExecuteCommand(&a_Player, Split, a_Command, Result);
			return Result;
		}
		return crUnknownCommand;
	}

	// Ask plugins first if a command is okay to execute the command:
	CommandResult Result = crBlocked;
	if (CallHookExecuteCommand(&a_Player, Split, a_Command, Result))
	{
		if (Result == crBlocked)
		{
			LOGINFO("Player %s tried executing command \"%s\" that was stopped by the HOOK_EXECUTE_COMMAND hook", a_Player.GetName().c_str(), Split[0].c_str());
		}
		return Result;
	}

	if (
		a_ShouldCheckPermissions &&
		!cmd->second.m_Permission.empty() &&
		!a_Player.HasPermission(cmd->second.m_Permission)
	)
	{
		LOGINFO("Player %s tried to execute forbidden command: \"%s\"", a_Player.GetName().c_str(), Split[0].c_str());
		return crNoPermission;
	}

	ASSERT(cmd->second.m_Handler != nullptr);

	if (!cmd->second.m_Handler->ExecuteCommand(Split, &a_Player, a_Command, nullptr))
	{
		return crError;
	}

	return crExecuted;
}





void cPluginManager::UnloadPluginsNow()
{
	// Remove all bindings:
	m_Hooks.clear();
	m_Commands.clear();
	m_ConsoleCommands.clear();

	// Re-bind built-in console commands:
	cServer::BindBuiltInConsoleCommands();

	// Unload all loaded plugins:
	for (auto & plugin: m_Plugins)
	{
		if (plugin->IsLoaded())
		{
			plugin->Unload();
		}
	}
}





void cPluginManager::UnloadPlugin(const AString & a_PluginFolder)
{
	cCSLock Lock(m_CSPluginsNeedAction);
	m_PluginsNeedAction.emplace_back(PluginAction::Unload, a_PluginFolder);
}





void cPluginManager::ReloadPlugin(const AString & a_PluginFolder)
{
	cCSLock Lock(m_CSPluginsNeedAction);
	m_PluginsNeedAction.emplace_back(PluginAction::Reload, a_PluginFolder);
}





bool cPluginManager::LoadPlugin(const AString & a_FolderName)
{
	for (auto & plugin: m_Plugins)
	{
		if (plugin->GetFolderName() == a_FolderName)
		{
			if (!plugin->IsLoaded())
			{
				return plugin->Load();
			}
			return true;
		}
	}  // for plugin - m_Plugins[]

	// Plugin not found
	LOG("Cannot load plugin, folder \"%s\" not found.", a_FolderName.c_str());
	return false;
}





void cPluginManager::RemoveHooks(cPlugin * a_Plugin)
{
	for (auto & Hook : m_Hooks)
	{
		Hook.second.remove(a_Plugin);
	}
}





void cPluginManager::RemovePluginCommands(cPlugin * a_Plugin)
{
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





bool cPluginManager::IsPluginLoaded(const AString & a_PluginName)
{
	for (auto & plugin: m_Plugins)
	{
		if (plugin->GetName() == a_PluginName)
		{
			return true;
		}
	}
	return false;
}





bool cPluginManager::BindCommand(
	const AString & a_Command,
	cPlugin * a_Plugin,
	cCommandHandlerPtr a_Handler,
	const AString & a_Permission,
	const AString & a_HelpString
)
{
	CommandMap::iterator cmd = m_Commands.find(a_Command);
	if (cmd != m_Commands.end())
	{
		LOGWARNING("Command \"%s\" is already bound to plugin \"%s\".", a_Command.c_str(), cmd->second.m_Plugin->GetName().c_str());
		return false;
	}

	auto & reg = m_Commands[a_Command];
	reg.m_Plugin     = a_Plugin;
	reg.m_Handler    = std::move(a_Handler);
	reg.m_Permission = a_Permission;
	reg.m_HelpString = a_HelpString;
	return true;
}





bool cPluginManager::ForEachCommand(cCommandEnumCallback & a_Callback)
{
	for (auto & itr : m_Commands)
	{
		if (a_Callback.Command(itr.first, itr.second.m_Plugin, itr.second.m_Permission, itr.second.m_HelpString))
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





cPluginManager::CommandResult cPluginManager::ExecuteCommand(cPlayer & a_Player, const AString & a_Command)
{
	return HandleCommand(a_Player, a_Command, true);
}





cPluginManager::CommandResult cPluginManager::ForceExecuteCommand(cPlayer & a_Player, const AString & a_Command)
{
	return HandleCommand(a_Player, a_Command, false);
}





void cPluginManager::RemovePluginConsoleCommands(cPlugin * a_Plugin)
{
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





bool cPluginManager::BindConsoleCommand(
	const AString & a_Command,
	cPlugin * a_Plugin,
	cCommandHandlerPtr a_Handler,
	const AString & a_HelpString
)
{
	CommandMap::iterator cmd = m_ConsoleCommands.find(a_Command);
	if (cmd != m_ConsoleCommands.end())
	{
		if (cmd->second.m_Plugin == nullptr)
		{
			LOGWARNING("Console command \"%s\" is already bound internally by Cuberite, cannot bind in plugin \"%s\".", a_Command.c_str(), a_Plugin->GetName().c_str());
		}
		else
		{
			LOGWARNING("Console command \"%s\" is already bound to plugin \"%s\", cannot bind in plugin \"%s\".", a_Command.c_str(), cmd->second.m_Plugin->GetName().c_str(), a_Plugin->GetName().c_str());
		}
		return false;
	}

	auto & reg = m_ConsoleCommands[a_Command];
	reg.m_Plugin     = a_Plugin;
	reg.m_Handler    = std::move(a_Handler);
	reg.m_Permission = "";
	reg.m_HelpString = a_HelpString;
	return true;
}





bool cPluginManager::ForEachConsoleCommand(cCommandEnumCallback & a_Callback)
{
	for (auto & itr : m_ConsoleCommands)
	{
		if (a_Callback.Command(itr.first, itr.second.m_Plugin, "", itr.second.m_HelpString))
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





bool cPluginManager::ExecuteConsoleCommand(const AStringVector & a_Split, cCommandOutputCallback & a_Output, const AString & a_Command)
{
	if (a_Split.empty())
	{
		return false;
	}

	CommandMap::iterator cmd = m_ConsoleCommands.find(a_Split[0]);
	if (cmd == m_ConsoleCommands.end())
	{
		// Command not found
		// Still notify the plugins (so that plugins such as Aliases can intercept unknown commands).
		CommandResult res = crBlocked;
		CallHookExecuteCommand(nullptr, a_Split, a_Command, res);
		return (res == crExecuted);
	}

	if (cmd->second.m_Plugin == nullptr)
	{
		// This is a built-in command
		return false;
	}

	// Ask plugins first if a command is okay to execute the console command:
	CommandResult res = crBlocked;
	if (CallHookExecuteCommand(nullptr, a_Split, a_Command, res))
	{
		return (res == crExecuted);
	}

	return cmd->second.m_Handler->ExecuteCommand(a_Split, nullptr, a_Command, &a_Output);
}





void cPluginManager::TabCompleteCommand(const AString & a_Text, AStringVector & a_Results, cPlayer * a_Player)
{
	for (auto & Command : m_Commands)
	{
		if (NoCaseCompare(Command.first.substr(0, a_Text.length()), a_Text) != 0)
		{
			// Command name doesn't match
			continue;
		}
		if ((a_Player != nullptr) && !a_Player->HasPermission(Command.second.m_Permission))
		{
			// Player doesn't have permission for the command
			continue;
		}

		/*  Client expects to only get back the last part of a space separated command.
		Find the position of the beginning of the last part:
		Position of last space + 1 for space separated commands
		string::npos + 1 = 0 for commands that are not separated

		Then skip all commands that have too many subcommands.
		When the client asks for suggestions for "/time s"
		the server must skip all commands that consist of more than 2 words just as
		"/time set day". Or in other words, the position of the last space (separator)
		in the strings must be equal or string::npos for both. */
		size_t LastSpaceInText = a_Text.find_last_of(' ') + 1;
		size_t LastSpaceInSuggestion = Command.first.find_last_of(' ') + 1;

		if (LastSpaceInText != LastSpaceInSuggestion)
		{
			// Suggestion has more subcommands than a_Text
			continue;
		}

		a_Results.push_back(Command.first.substr(LastSpaceInSuggestion));
	}
}





bool cPluginManager::IsValidHookType(int a_HookType)
{
	return ((a_HookType >= 0) && (a_HookType <= HOOK_MAX));
}





bool cPluginManager::DoWithPlugin(const AString & a_PluginName, cPluginCallback a_Callback)
{
	// TODO: Implement locking for plugins
	for (auto & plugin: m_Plugins)
	{
		if (plugin->GetName() == a_PluginName)
		{
			return a_Callback(*plugin);
		}
	}
	return false;
}





bool cPluginManager::ForEachPlugin(cPluginCallback a_Callback)
{
	// TODO: Implement locking for plugins
	for (auto & plugin: m_Plugins)
	{
		if (a_Callback(*plugin))
		{
			return false;
		}
	}
	return true;
}





AString cPluginManager::GetPluginFolderName(const AString & a_PluginName)
{
	// TODO: Implement locking for plugins
	for (auto & plugin: m_Plugins)
	{
		if (plugin->GetName() == a_PluginName)
		{
			return plugin->GetFolderName();
		}
	}
	return AString();
}





void cPluginManager::AddHook(cPlugin * a_Plugin, int a_Hook)
{
	if (a_Plugin == nullptr)
	{
		LOGWARN("Called cPluginManager::AddHook() with a_Plugin == nullptr");
		return;
	}
	PluginList & Plugins = m_Hooks[a_Hook];
	if (std::find(Plugins.cbegin(), Plugins.cend(), a_Plugin) == Plugins.cend())
	{
		Plugins.push_back(a_Plugin);
	}
}





size_t cPluginManager::GetNumPlugins(void) const
{
	return m_Plugins.size();
}





size_t cPluginManager::GetNumLoadedPlugins(void) const
{
	size_t res = 0;
	for (auto & plugin: m_Plugins)
	{
		if (plugin->IsLoaded())
		{
			res += 1;
		}
	}
	return res;
}





AStringVector cPluginManager::GetFoldersToLoad(cSettingsRepositoryInterface & a_Settings)
{
	// Check if the Plugins section exists.
	if (!a_Settings.KeyExists("Plugins"))
	{
		InsertDefaultPlugins(a_Settings);
	}

	AStringVector res;

	// Get the old format plugin list, and migrate it.
	// Upgrade path added on 2020-03-27
	auto OldValues = a_Settings.GetValues("Plugins");
	for (const auto & NameValue : OldValues)
	{
		AString ValueName = NameValue.first;
		if (ValueName.compare("Plugin") == 0)
		{
			AString PluginFile = NameValue.second;
			if (
				!PluginFile.empty() &&
				(PluginFile != "0") &&
				(PluginFile != "1")
			)
			{
				a_Settings.DeleteValue("Plugins", ValueName);
				a_Settings.SetValue("Plugins", PluginFile, "1");
			}
		}
	}  // for i - ini values

	// Get the list of plugins to load:
	auto Values = a_Settings.GetValues("Plugins");
	for (const auto & NameValue : Values)
	{
		AString Enabled = NameValue.second;
		if (Enabled == "1")
		{
			res.push_back(NameValue.first);
		}
	}  // for i - ini values

	return res;
}




