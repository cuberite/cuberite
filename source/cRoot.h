#pragma once

class cMonsterConfig;
class cMCLogger;
class cGroupManager;
class cRecipeChecker;
class cFurnaceRecipe;
class cWebAdmin;
class cPluginManager;
class cServer;
class cWorld;
class cRoot	//tolua_export
{			//tolua_export
public:
	static cRoot* Get() { return s_Root; }							//tolua_export

	cRoot();
	~cRoot();

	void Start();

	cServer* GetServer() { return m_Server; }						//tolua_export
	cWorld* GetWorld() { return m_World; }							//tolua_export
	cMonsterConfig *GetMonsterConfig() { return m_MonsterConfig;}

	cGroupManager* GetGroupManager() { return m_GroupManager; }		//tolua_export
	cRecipeChecker* GetRecipeChecker() { return m_RecipeChecker; }	//tolua_export
	cFurnaceRecipe* GetFurnaceRecipe() { return m_FurnaceRecipe; }	//tolua_export
	cWebAdmin*		GetWebAdmin() { return m_WebAdmin; }			//tolua_export
	cPluginManager* GetPluginManager() { return m_PluginManager; }	//tolua_export

	void ServerCommand( const char* a_Cmd );						//tolua_export
private:
	cServer*		m_Server;
	cWorld*			m_World;
	cMonsterConfig *m_MonsterConfig;

	cGroupManager*	m_GroupManager;
	cRecipeChecker* m_RecipeChecker;
	cFurnaceRecipe* m_FurnaceRecipe;
	cWebAdmin*		m_WebAdmin;
	cPluginManager* m_PluginManager;

	cMCLogger*			m_Log;

	bool m_bStop;
	bool m_bRestart;

	void* m_hInputThread;

	static cRoot*	s_Root;
};	//tolua_export