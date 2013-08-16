
#pragma once

#include "Authenticator.h"





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
class cCommandOutputCallback ;

typedef cItemCallback<cPlayer> cPlayerListCallback;
typedef cItemCallback<cWorld>  cWorldListCallback;





/// The root of the object hierarchy
class cRoot	// tolua_export
{			// tolua_export
public:
	/// The version of the protocol that is primary for the server (reported in the server list). All versions are still supported.
	int m_PrimaryServerVersion;  // tolua_export
	
	static cRoot* Get() { return s_Root; }							// tolua_export

	cRoot(void);
	~cRoot();

	void Start(void);

	cServer * GetServer(void) { return m_Server; }						// tolua_export
	cWorld *  GetDefaultWorld(void);										// tolua_export
	cWorld *  GetWorld(const AString & a_WorldName);				// tolua_export
	
	/// Calls the callback for each world; returns true if the callback didn't abort (return true)
	bool ForEachWorld(cWorldListCallback & a_Callback);  // >> Exported in ManualBindings <<
	
	/// Writes chunkstats, for each world and totals, to the output callback
	void LogChunkStats(cCommandOutputCallback & a_Output);
	
	int GetPrimaryServerVersion(void) const { return m_PrimaryServerVersion; }  // tolua_export
	void SetPrimaryServerVersion(int a_Version) { m_PrimaryServerVersion = a_Version; }  // tolua_export
	
	cMonsterConfig * GetMonsterConfig() { return m_MonsterConfig; }

	cGroupManager *    GetGroupManager   (void) { return m_GroupManager; }     // tolua_export
	cCraftingRecipes * GetCraftingRecipes(void) { return m_CraftingRecipes; }  // tolua_export
	cFurnaceRecipe *   GetFurnaceRecipe  (void) { return m_FurnaceRecipe; }    // tolua_export
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
	void AuthenticateUser(int a_ClientID);
	
	/// Executes commands queued in the command queue
	void TickCommands(void);

	/// Returns the number of chunks loaded
	int GetTotalChunkCount(void);  // tolua_export
	
	/// Saves all chunks in all worlds
	void SaveAllChunks(void);  // tolua_export
	
	/// Calls the callback for each player in all worlds
	bool ForEachPlayer(cPlayerListCallback & a_Callback);	// >> EXPORTED IN MANUALBINDINGS <<

	/// Finds a player from a partial or complete player name and calls the callback - case-insensitive
	bool FindAndDoWithPlayer(const AString & a_PlayerName, cPlayerListCallback & a_Callback);	// >> EXPORTED IN MANUALBINDINGS <<
	
	/// Returns the textual description of the protocol version: 49 -> "1.4.4". Provided specifically for Lua API
	static AString GetProtocolVersionTextFromInt(int a_ProtocolVersionNum);  // tolua_export
	
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

	cThread * m_InputThread;

	cServer *        m_Server;
	cMonsterConfig * m_MonsterConfig;

	cGroupManager *    m_GroupManager;
	cCraftingRecipes * m_CraftingRecipes;
	cFurnaceRecipe *   m_FurnaceRecipe;
	cWebAdmin *        m_WebAdmin;
	cPluginManager *   m_PluginManager;
	cAuthenticator     m_Authenticator;

	cMCLogger *      m_Log;

	bool m_bStop;
	bool m_bRestart;

	void LoadGlobalSettings();

	/// Loads the worlds from settings.ini, creates the worldmap
	void LoadWorlds(void);
	
	/// Starts each world's life
	void StartWorlds(void);
	
	/// Stops each world's threads, so that it's safe to unload them
	void StopWorlds(void);
	
	/// Unloads all worlds from memory
	void UnloadWorlds(void);
	
	/// Does the actual work of executing a command
	void DoExecuteConsoleCommand(const AString & a_Cmd);

	static void InputThread(void* a_Params);
	
	static cRoot*	s_Root;
};	// tolua_export




