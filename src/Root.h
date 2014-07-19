
#pragma once

#include "Protocol/Authenticator.h"
#include "HTTPServer/HTTPServer.h"
#include "Defines.h"





// fwd:
class cThread;
class cMonsterConfig;
class cGroupManager;
class cCraftingRecipes;
class cFurnaceRecipe;
class cWebAdmin;
class cPluginManager;
class cServer;
class cWorld;
class cPlayer;
class cCommandOutputCallback;
class cCompositeChat;

typedef cItemCallback<cPlayer> cPlayerListCallback;
typedef cItemCallback<cWorld>  cWorldListCallback;

namespace Json
{
	class Value;
}





/// The root of the object hierarchy
// tolua_begin
class cRoot
{
public:
	static bool m_TerminateEventRaised;
	
	static cRoot * Get() { return s_Root; }
	// tolua_end

	cRoot(void);
	~cRoot();

	void Start(void);

	// tolua_begin
	cServer * GetServer(void) { return m_Server; }
	cWorld *  GetDefaultWorld(void);
	cWorld *  GetWorld(const AString & a_WorldName);
	cWorld *  CreateAndInitializeWorld(const AString & a_WorldName);
	// tolua_end
	
	/// Calls the callback for each world; returns true if the callback didn't abort (return true)
	bool ForEachWorld(cWorldListCallback & a_Callback);  // >> Exported in ManualBindings <<
	
	/// Writes chunkstats, for each world and totals, to the output callback
	void LogChunkStats(cCommandOutputCallback & a_Output);
	
	int GetPrimaryServerVersion(void) const { return m_PrimaryServerVersion; }  // tolua_export
	void SetPrimaryServerVersion(int a_Version) { m_PrimaryServerVersion = a_Version; }  // tolua_export
	
	cMonsterConfig * GetMonsterConfig(void) { return m_MonsterConfig; }

	cGroupManager *    GetGroupManager   (void) { return m_GroupManager; }     // tolua_export
	cCraftingRecipes * GetCraftingRecipes(void) { return m_CraftingRecipes; }  // tolua_export
	cFurnaceRecipe *   GetFurnaceRecipe  (void) { return m_FurnaceRecipe; }    // Exported in ManualBindings.cpp with quite a different signature
	
	/// Returns the number of ticks for how long the item would fuel a furnace. Returns zero if not a fuel
	static int GetFurnaceFuelBurnTime(const cItem & a_Fuel);  // tolua_export
	
	cWebAdmin *        GetWebAdmin       (void) { return m_WebAdmin; }         // tolua_export
	cPluginManager *   GetPluginManager  (void) { return m_PluginManager; }    // tolua_export
	cAuthenticator &   GetAuthenticator  (void) { return m_Authenticator; }

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
	
	/// Executes a console command through the cServer class; does special handling for "stop" and "restart".
	void ExecuteConsoleCommand(const AString & a_Cmd, cCommandOutputCallback & a_Output);
	
	/// Kicks the user, no matter in what world they are. Used from cAuthenticator
	void KickUser(int a_ClientID, const AString & a_Reason);
	
	/// Called by cAuthenticator to auth the specified user
	void AuthenticateUser(int a_ClientID, const AString & a_Name, const AString & a_UUID, const Json::Value & a_Properties);
	
	/// Executes commands queued in the command queue
	void TickCommands(void);

	/// Returns the number of chunks loaded
	int GetTotalChunkCount(void);  // tolua_export
	
	/// Saves all chunks in all worlds
	void SaveAllChunks(void);  // tolua_export
	
	/// Reloads all the groups
	void ReloadGroups(void);  // tolua_export
	
	/// Calls the callback for each player in all worlds
	bool ForEachPlayer(cPlayerListCallback & a_Callback);  // >> EXPORTED IN MANUALBINDINGS <<

	/// Finds a player from a partial or complete player name and calls the callback - case-insensitive
	bool FindAndDoWithPlayer(const AString & a_PlayerName, cPlayerListCallback & a_Callback);  // >> EXPORTED IN MANUALBINDINGS <<

	// tolua_begin
	
	/// Sends a chat message to all connected clients (in all worlds)
	void BroadcastChat       (const AString & a_Message, eMessageType a_ChatPrefix = mtCustom);
	void BroadcastChatInfo   (const AString & a_Message) { BroadcastChat(a_Message, mtInformation); }
	void BroadcastChatFailure(const AString & a_Message) { BroadcastChat(a_Message, mtFailure); }
	void BroadcastChatSuccess(const AString & a_Message) { BroadcastChat(a_Message, mtSuccess); }
	void BroadcastChatWarning(const AString & a_Message) { BroadcastChat(a_Message, mtWarning); }
	void BroadcastChatFatal  (const AString & a_Message) { BroadcastChat(a_Message, mtFailure); }
	void BroadcastChatJoin   (const AString & a_Message) { BroadcastChat(a_Message, mtJoin); }
	void BroadcastChatLeave  (const AString & a_Message) { BroadcastChat(a_Message, mtLeave); }
	void BroadcastChatDeath  (const AString & a_Message) { BroadcastChat(a_Message, mtDeath); }
	void BroadcastChat       (const cCompositeChat & a_Message);
	
	/// Returns the textual description of the protocol version: 49 -> "1.4.4". Provided specifically for Lua API
	static AString GetProtocolVersionTextFromInt(int a_ProtocolVersionNum);
	
	/// Returns the amount of virtual RAM used, in KiB. Returns a negative number on error
	static int GetVirtualRAMUsage(void);
	
	/// Returns the amount of virtual RAM used, in KiB. Returns a negative number on error
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
	
	/// The version of the protocol that is primary for the server (reported in the server list). All versions are still supported.
	int m_PrimaryServerVersion;

	cWorld * m_pDefaultWorld;
	WorldMap m_WorldsByName;
	
	cCriticalSection m_CSPendingCommands;
	cCommandQueue    m_PendingCommands;

	cThread * m_InputThread;

	cServer *        m_Server;
	cMonsterConfig * m_MonsterConfig;

	cGroupManager *    m_GroupManager;
	cCraftingRecipes * m_CraftingRecipes;
	cFurnaceRecipe *   m_FurnaceRecipe;
	cWebAdmin *        m_WebAdmin;
	cPluginManager *   m_PluginManager;
	cAuthenticator     m_Authenticator;
	cHTTPServer        m_HTTPServer;

	cMCLogger *      m_Log;

	bool m_bStop;
	bool m_bRestart;

	void LoadGlobalSettings();

	/// Loads the worlds from settings.ini, creates the worldmap
	void LoadWorlds(cIniFile & IniFile);
	
	/// Starts each world's life
	void StartWorlds(void);
	
	/// Stops each world's threads, so that it's safe to unload them
	void StopWorlds(void);
	
	/// Unloads all worlds from memory
	void UnloadWorlds(void);
	
	/// Does the actual work of executing a command
	void DoExecuteConsoleCommand(const AString & a_Cmd);

	static void InputThread(void* a_Params);
	
	static cRoot* s_Root;
};  // tolua_export




