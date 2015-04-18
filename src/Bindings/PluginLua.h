
// PluginLua.h

// Declares the cPluginLua class representing a plugin written in Lua





#pragma once

#include "WebPlugin.h"
#include "LuaState.h"


class cCommandOutputCallback;
class cBlockEntityWithItems;

// Names for the global variables through which the plugin is identified in its LuaState
#define LUA_PLUGIN_NAME_VAR_NAME     "_MCServerInternal_PluginName"
#define LUA_PLUGIN_INSTANCE_VAR_NAME "_MCServerInternal_PluginInstance"





// fwd: UI/Window.h
class cWindow;





// tolua_begin
class cPluginLua :
	public cWebPlugin
{
public:
	// tolua_end
	
	/** A RAII-style mutex lock for accessing the internal LuaState.
	This will be the only way to retrieve the plugin's LuaState;
	therefore it directly supports accessing the LuaState of the locked plugin.
	Usage:
		cPluginLua::cOperation Op(SomePlugin);
		Op().Call(...)  // Call a function in the plugin's LuaState
	*/
	class cOperation
	{
	public:
		cOperation(cPluginLua & a_Plugin) :
			m_Plugin(a_Plugin),
			m_Lock(a_Plugin.m_CriticalSection)
		{
		}

		cLuaState & operator ()(void) { return m_Plugin.m_LuaState; }
		
	protected:
		cPluginLua & m_Plugin;
		
		/** RAII lock for m_Plugin.m_CriticalSection */
		cCSLock m_Lock;
	} ;



	/** A base class that represents something related to a plugin
	The plugin can reset this class so that the instance can continue to exist but will not engage the (possibly non-existent) plugin anymore.
	This is used for scheduled tasks etc., so that they can be queued and reset when the plugin is terminated, without removing them from the queue. */
	class cResettable
	{
	public:
		/** Creates a new instance bound to the specified plugin. */
		cResettable(cPluginLua & a_Plugin);

		// Force a virtual destructor in descendants:
		virtual ~cResettable() {}

		/** Resets the plugin instance stored within.
		The instance will continue to exist, but should not call into the plugin anymore. */
		virtual void Reset(void);

	protected:
		/** The plugin that this instance references.
		If nullptr, the plugin has already unloaded and the instance should bail out any processing.
		Protected against multithreaded access by m_CSPlugin. */
		cPluginLua * m_Plugin;

		/** The mutex protecting m_Plugin against multithreaded access. */
		cCriticalSection m_CSPlugin;
	};

	typedef SharedPtr<cResettable> cResettablePtr;
	typedef std::vector<cResettablePtr> cResettablePtrs;
	
	
	cPluginLua(const AString & a_PluginDirectory);
	~cPluginLua();

	void OnDisable(void);
	bool Initialize(void);

	void Tick(float a_Dt);
	
	bool HandleCommand(const AStringVector & a_Split, cPlayer & a_Player, const AString & a_FullCommand);
	
	bool HandleConsoleCommand(const AStringVector & a_Split, cCommandOutputCallback & a_Output, const AString & a_FullCommand);

	void ClearCommands(void);
	
	void ClearConsoleCommands(void);



	// tolua_begin
	const AString & GetName(void) const  { return m_Name; }
	void SetName(const AString & a_Name) { m_Name = a_Name; }

	int GetVersion(void) const     { return m_Version; }
	void SetVersion(int a_Version) { m_Version = a_Version; }

	const AString & GetDirectory(void) const { return m_Directory; }
	AString GetLocalDirectory(void) const { return GetLocalFolder(); }  // OBSOLETE, use GetLocalFolder() instead
	AString GetLocalFolder(void) const {
		return std::string("Plugins/") + m_Directory;
	}
	// tolua_end

	/** Returns true if the plugin contains the function for the specified hook type, using the old-style registration (#121) */
	bool CanAddOldStyleHook(int a_HookType);
	
	// cWebPlugin
	const AString GetWebTitle(void) const {return GetName(); }

	// cWebPlugin and WebAdmin stuff
	AString HandleWebRequest(const HTTPRequest * a_Request);
	bool AddWebTab(const AString & a_Title, lua_State * a_LuaState, int a_FunctionReference);  // >> EXPORTED IN MANUALBINDINGS <<
	
	/** Binds the command to call the function specified by a Lua function reference. Simply adds to CommandMap. */
	void BindCommand(const AString & a_Command, int a_FnRef);

	/** Binds the console command to call the function specified by a Lua function reference. Simply adds to CommandMap. */
	void BindConsoleCommand(const AString & a_Command, int a_FnRef);

	cLuaState & GetLuaState(void) { return m_LuaState; }

	cCriticalSection & GetCriticalSection(void) { return m_CriticalSection; }
	
	/** Removes a previously referenced object (luaL_unref()) */
	void Unreference(int a_LuaRef);
	
	/** Calls the plugin-specified "cLuaWindow closing" callback. Returns true only if the callback returned true */
	bool CallbackWindowClosing(int a_FnRef, cWindow & a_Window, cPlayer & a_Player, bool a_CanRefuse);
	
	/** Calls the plugin-specified "cLuaWindow slot changed" callback. */
	void CallbackWindowSlotChanged(int a_FnRef, cWindow & a_Window, int a_SlotNum);
	
	/** Returns the name of Lua function that should handle the specified hook type in the older (#121) API */
	static const char * GetHookFnName(int a_HookType);
	
	/** Adds a Lua function to be called for the specified hook.
	The function has to be on the Lua stack at the specified index a_FnRefIdx
	Returns true if the hook was added successfully.
	*/
	bool AddHookRef(int a_HookType, int a_FnRefIdx);
	
	/** Calls a function in this plugin's LuaState with parameters copied over from a_ForeignState.
	The values that the function returns are placed onto a_ForeignState.
	Returns the number of values returned, if successful, or negative number on failure. */
	int CallFunctionFromForeignState(
		const AString & a_FunctionName,
		cLuaState & a_ForeignState,
		int a_ParamStart,
		int a_ParamEnd
	);
	
	/** Call a Lua function residing in the plugin. */
	template <typename FnT, typename... Args>
	bool Call(FnT a_Fn, Args && ... a_Args)
	{
		cCSLock Lock(m_CriticalSection);
		return m_LuaState.Call(a_Fn, a_Args...);
	}

	template <typename... Args>
	bool CallHook(int a_Hook, Args &&... args)
	{
		cCSLock Lock(m_CriticalSection);
		if (!m_LuaState.IsValid())
		{
			return false;
		}
		bool res = false;
		cLuaRefs & Refs = m_HookMap[a_Hook];
		for (auto itr = Refs.begin(), end = Refs.end(); itr != end; ++itr)
		{
			m_LuaState.Call((int)(**itr), args..., cLuaState::Return, res);
			if (res)
			{
				return true;
			}
		}
		return false;
	}

	/** Adds the specified cResettable instance to m_Resettables, so that it is notified when the plugin is being closed. */
	void AddResettable(cResettablePtr a_Resettable);

protected:
	/** Maps command name into Lua function reference */
	typedef std::map<AString, int> CommandMap;
	
	/** Provides an array of Lua function references */
	typedef std::vector<cLuaState::cRef *> cLuaRefs;
	
	/** Maps hook types into arrays of Lua function references to call for each hook type */
	typedef std::map<int, cLuaRefs> cHookMap;
	

	/** The mutex protecting m_LuaState and each of the m_Resettables[] against multithreaded use. */
	cCriticalSection m_CriticalSection;

	/** The plugin's Lua state. */
	cLuaState m_LuaState;
	
	/** Objects that need notification when the plugin is about to be unloaded. */
	cResettablePtrs m_Resettables;

	/** In-game commands that the plugin has registered. */
	CommandMap m_Commands;

	/** Console commands that the plugin has registered. */
	CommandMap m_ConsoleCommands;
	
	/** Hooks that the plugin has registered. */
	cHookMap m_HookMap;


	AString m_Name;
	int m_Version;

	AString m_Directory;
	

	/** Releases all Lua references, notifies and removes all m_Resettables[] and closes the m_LuaState. */
	void Close(void);
} ;  // tolua_export




