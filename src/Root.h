
#pragma once

#include "Protocol/Authenticator.h"
#include "Protocol/MojangAPI.h"
#include "HTTP/HTTPServer.h"
#include "Defines.h"
#include "FunctionRef.h"
#include "RankManager.h"





// fwd:
class cMonsterConfig;
class cBrewingRecipes;
class cCraftingRecipes;
class cFurnaceRecipe;
class cWebAdmin;
class cPluginManager;
class cServer;
class cWorld;
class cPlayer;
class cCommandOutputCallback;
class cCompositeChat;
class cSettingsRepositoryInterface;
class cDeadlockDetect;
class cUUID;

using cPlayerListCallback =  cFunctionRef<bool(cPlayer &)>;
using cWorldListCallback  =  cFunctionRef<bool(cWorld  &)>;

namespace Json
{
	class Value;
}





/** The root of the object hierarchy */
// tolua_begin
class cRoot
{
public:
	static cRoot * Get() { return s_Root; }
	// tolua_end

	static bool m_TerminateEventRaised;
	static bool m_RunAsService;

	/** which ini file to load settings from, default is settings.ini */
	AString m_SettingsFilename;

	cRoot(void);
	~cRoot();

	void Start(std::unique_ptr<cSettingsRepositoryInterface> a_OverridesRepo);

	/** Stops the server, as if "/stop" was typed in the console. */
	void StopServer();

	// tolua_begin
	cServer * GetServer(void) { return m_Server; }
	cWorld *  GetDefaultWorld(void);

	/** Returns a pointer to the world specified. If no world of that name exists, returns a nullptr. */
	cWorld * GetWorld(const AString & a_WorldName);

	/** Returns the up time of the server in seconds */
	int GetServerUpTime(void)
	{
		return static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_StartTime).count());
	}
	// tolua_end

	/** Calls the callback for each world; returns true if the callback didn't abort (return true) */
	bool ForEachWorld(cWorldListCallback a_Callback);  // >> Exported in ManualBindings <<

	/** Writes chunkstats, for each world and totals, to the output callback */
	void LogChunkStats(cCommandOutputCallback & a_Output);

	cMonsterConfig * GetMonsterConfig(void) { return m_MonsterConfig; }

	cCraftingRecipes * GetCraftingRecipes(void) { return m_CraftingRecipes; }  // tolua_export
	cFurnaceRecipe *   GetFurnaceRecipe  (void) { return m_FurnaceRecipe; }    // Exported in ManualBindings.cpp with quite a different signature
	cBrewingRecipes *  GetBrewingRecipes (void) { return m_BrewingRecipes.get(); }    // Exported in ManualBindings.cpp

	/** Returns the number of ticks for how long the item would fuel a furnace. Returns zero if not a fuel */
	static int GetFurnaceFuelBurnTime(const cItem & a_Fuel);  // tolua_export

	/** Returns the completions for a player name across all worlds. Returns an
	empty vector if none are found. */
	AStringVector GetPlayerTabCompletionMultiWorld(const AString & a_Text);

	/** The current time where the startup of the server has been completed */
	std::chrono::steady_clock::time_point m_StartTime;

	cWebAdmin *        GetWebAdmin       (void) { return m_WebAdmin; }         // tolua_export
	cPluginManager *   GetPluginManager  (void) { return m_PluginManager; }    // tolua_export
	cAuthenticator &   GetAuthenticator  (void) { return m_Authenticator; }
	cMojangAPI &       GetMojangAPI      (void) { return *m_MojangAPI; }
	cRankManager *     GetRankManager    (void) { return m_RankManager.get(); }

	/** Queues a console command for execution through the cServer class.
	The command will be executed in the tick thread
	The command's output will be written to the a_Output callback
	"stop" and "restart" commands have special handling.
	*/
	void QueueExecuteConsoleCommand(const AString & a_Cmd, cCommandOutputCallback & a_Output);

	/** Queues a console command for execution through the cServer class.
	The command will be executed in the tick thread
	The command's output will be sent to console
	"stop" and "restart" commands have special handling.
	*/
	void QueueExecuteConsoleCommand(const AString & a_Cmd);  // tolua_export

	/** Executes a console command through the cServer class; does special handling for "stop" and "restart". */
	void ExecuteConsoleCommand(const AString & a_Cmd, cCommandOutputCallback & a_Output);

	/** Kicks the user, no matter in what world they are. Used from cAuthenticator */
	void KickUser(int a_ClientID, const AString & a_Reason);

	/** Called by cAuthenticator to auth the specified user */
	void AuthenticateUser(int a_ClientID, const AString & a_Name, const cUUID & a_UUID, const Json::Value & a_Properties);

	/** Executes commands queued in the command queue */
	void TickCommands(void);

	/** Returns the number of chunks loaded */
	int GetTotalChunkCount(void);  // tolua_export

	/** Saves all chunks in all worlds */
	void SaveAllChunks(void);  // tolua_export

	/** Sets whether saving chunks is enabled in all worlds (overrides however the worlds were already set) */
	void SetSavingEnabled(bool a_SavingEnabled);  // tolua_export

	/** Calls the callback for each player in all worlds */
	bool ForEachPlayer(cPlayerListCallback a_Callback);  // >> EXPORTED IN MANUALBINDINGS <<

	/** Finds a player from a partial or complete player name and calls the callback - case-insensitive */
	bool FindAndDoWithPlayer(const AString & a_PlayerName, cPlayerListCallback a_Callback);  // >> EXPORTED IN MANUALBINDINGS <<

	/** Finds the player over his uuid and calls the callback */
	bool DoWithPlayerByUUID(const cUUID & a_PlayerUUID, cPlayerListCallback a_Callback);  // >> EXPORTED IN MANUALBINDINGS <<

	/** Finds the player using it's complete username and calls the callback */
	bool DoWithPlayer(const AString & a_PlayerName, cPlayerListCallback a_Callback);

	/** Send playerlist of all worlds to player */
	void SendPlayerLists(cPlayer * a_DestPlayer);

	/** Broadcast Player through all worlds */
	void BroadcastPlayerListsAddPlayer(const cPlayer & a_Player, const cClientHandle * a_Exclude = nullptr);

	// tolua_begin

	/** Sends a chat message to all connected clients (in all worlds) */
	void BroadcastChat       (const AString & a_Message, eMessageType a_ChatPrefix = mtCustom);
	void BroadcastChat       (const cCompositeChat & a_Message);
	void BroadcastChatDeath  (const AString & a_Message) { BroadcastChat(a_Message, mtDeath); }
	void BroadcastChatFailure(const AString & a_Message) { BroadcastChat(a_Message, mtFailure); }
	void BroadcastChatFatal  (const AString & a_Message) { BroadcastChat(a_Message, mtFailure); }
	void BroadcastChatInfo   (const AString & a_Message) { BroadcastChat(a_Message, mtInformation); }
	void BroadcastChatJoin   (const AString & a_Message) { BroadcastChat(a_Message, mtJoin); }
	void BroadcastChatLeave  (const AString & a_Message) { BroadcastChat(a_Message, mtLeave); }
	void BroadcastChatSuccess(const AString & a_Message) { BroadcastChat(a_Message, mtSuccess); }
	void BroadcastChatWarning(const AString & a_Message) { BroadcastChat(a_Message, mtWarning); }

	/** Returns the textual description of the protocol version: 49 -> "1.4.4". Provided specifically for Lua API */
	static AString GetProtocolVersionTextFromInt(int a_ProtocolVersionNum);

	/** Returns the amount of virtual RAM used, in KiB. Returns a negative number on error */
	static int GetVirtualRAMUsage(void);

	/** Returns the amount of virtual RAM used, in KiB. Returns a negative number on error */
	static int GetPhysicalRAMUsage(void);

	// tolua_end

private:
	class cCommand
	{
	public:
		cCommand(const AString & a_Command, cCommandOutputCallback * a_Output) :
			m_Command(a_Command),
			m_Output(a_Output)
		{
		}

		AString m_Command;
		cCommandOutputCallback * m_Output;
	} ;

	typedef std::map<AString, cWorld *> WorldMap;
	typedef std::vector<cCommand> cCommandQueue;

	cWorld * m_pDefaultWorld;
	WorldMap m_WorldsByName;

	cCriticalSection m_CSPendingCommands;
	cCommandQueue    m_PendingCommands;

	std::thread m_InputThread;
	cEvent m_StopEvent;
	std::atomic_flag m_InputThreadRunFlag;

	cServer *        m_Server;
	cMonsterConfig * m_MonsterConfig;

	cCraftingRecipes * m_CraftingRecipes;
	cFurnaceRecipe *   m_FurnaceRecipe;
	std::unique_ptr<cBrewingRecipes> m_BrewingRecipes;
	cWebAdmin *        m_WebAdmin;
	cPluginManager *   m_PluginManager;
	cAuthenticator     m_Authenticator;
	cMojangAPI *       m_MojangAPI;

	std::unique_ptr<cRankManager> m_RankManager;

	cHTTPServer m_HTTPServer;


	void LoadGlobalSettings();

	/** Loads the worlds from settings.ini, creates the worldmap */
	void LoadWorlds(cDeadlockDetect & a_dd, cSettingsRepositoryInterface & a_Settings, bool a_IsNewIniFile);

	/** Starts each world's life */
	void StartWorlds(cDeadlockDetect & a_DeadlockDetect);

	/** Stops each world's threads, so that it's safe to unload them */
	void StopWorlds(cDeadlockDetect & a_DeadlockDetect);

	/** Unloads all worlds from memory */
	void UnloadWorlds(void);

	/** Does the actual work of executing a command */
	void DoExecuteConsoleCommand(const AString & a_Cmd);

	static cRoot * s_Root;

	static void InputThread(cRoot & a_Params);
};  // tolua_export





