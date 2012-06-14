
#pragma once




#include "cAuthenticator.h"





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
typedef cItemCallback<cPlayer> cPlayerListCallback;
typedef cItemCallback<cWorld>  cWorldListCallback;





class cRoot	//tolua_export
{			//tolua_export
public:
	static cRoot* Get() { return s_Root; }							//tolua_export

	cRoot(void);
	~cRoot();

	void Start(void);

	cServer * GetServer(void) { return m_Server; }						//tolua_export
	cWorld *  GetDefaultWorld(void);										//tolua_export
	cWorld *  GetWorld(const AString & a_WorldName);				//tolua_export
	
	/// Calls the callback for each world; returns true if the callback didn't abort (return true)
	bool ForEachWorld(cWorldListCallback & a_Callback);  // >> Exported in ManualBindings <<
	
	cMonsterConfig * GetMonsterConfig() { return m_MonsterConfig; }

	cGroupManager *    GetGroupManager   (void) { return m_GroupManager; }     // tolua_export
	cCraftingRecipes * GetCraftingRecipes(void) { return m_CraftingRecipes; }  // tolua_export
	cFurnaceRecipe *   GetFurnaceRecipe  (void) { return m_FurnaceRecipe; }    // tolua_export
	cWebAdmin *        GetWebAdmin       (void) { return m_WebAdmin; }         // tolua_export
	cPluginManager *   GetPluginManager  (void) { return m_PluginManager; }    // tolua_export
	cAuthenticator &   GetAuthenticator  (void) { return m_Authenticator; }

	void ServerCommand(const char* a_Cmd );						//tolua_export
	
	void KickUser(int a_ClientID, const AString & a_Reason);  // Kicks the user, no matter in what world they are. Used from cAuthenticator
	void AuthenticateUser(int a_ClientID);  // Called by cAuthenticator to auth the specified user

	void TickWorlds( float a_Dt );
	
	/// Returns the number of chunks loaded
	int GetTotalChunkCount(void);  // tolua_export
	
	/// Saves all chunks in all worlds
	void SaveAllChunks(void);
	
	/// Calls the callback for each player in all worlds
	bool ForEachPlayer(cPlayerListCallback & a_Callback);
	
private:
	void LoadGlobalSettings();

	/// Loads the worlds from settings.ini, creates the worldmap
	void LoadWorlds(void);
	
	/// Starts each world's life
	void StartWorlds(void);
	
	void UnloadWorlds(void);

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

	struct sRootState;
	sRootState* m_pState;

	cThread* m_InputThread;
	static void InputThread(void* a_Params);

	static cRoot*	s_Root;
};	//tolua_export




