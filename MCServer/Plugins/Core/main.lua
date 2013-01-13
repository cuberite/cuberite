---- Some settings -----
SHOW_PLUGIN_NAMES = true	-- If true, plugin name will be shown before commands
							-- This is overwritten in the Initialize() function
------------------------

-- Global variables
PLUGIN = {}	-- Reference to own plugin object
BannedPlayersIni = {}
WhiteListIni = {}

function Initialize( Plugin )
	PLUGIN = Plugin
	
	Plugin:SetName( "Core" )
	Plugin:SetVersion(9)
	
	PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_JOINED)
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_BREAKING_BLOCK)
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_PLACING_BLOCK)
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_LOGIN)
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_KILLED)
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_CRAFTING_NO_RECIPE)
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_CHAT) -- used in web_chat.lua
	
	Plugin:AddCommand("/help",         " - [Page] Show this message",                         "core.help")
	Plugin:AddCommand("/pluginlist",   " - Show list of plugins",                             "core.pluginlist")
	Plugin:AddCommand("/tp",           " - [Player] - Teleport yourself to a player",         "core.teleport")
	Plugin:AddCommand("/item",         " - [ItemType/Name] <Amount> - Give yourself an item", "core.item")
	Plugin:AddCommand("/list",         " - Shows list of connected players",                  "core.playerlist")
	Plugin:AddCommand("/motd",         " - Show message of the day",                          "core.motd")
	Plugin:AddCommand("/reload",       " - Reload all plugins",                               "core.reload")
	Plugin:AddCommand("/stop",         " - Stops the server",                                 "core.stop")
	Plugin:AddCommand("/time",         " - [Day/Night] - Sets the time of day",               "core.time")
	Plugin:AddCommand("/spawn",        " - Return to the spawn",                              "core.spawn")
	Plugin:AddCommand("/kick",         " - [Player] - Kick a player",                         "core.kick")
	Plugin:AddCommand("/ban",          " - [Player] - Ban a player",                          "core.ban")
	Plugin:AddCommand("/unban",        " - [Player] - Unban a player",                        "core.unban")
	Plugin:AddCommand("/top",          " - Teleport yourself to the top most block",          "core.top")
	Plugin:AddCommand("/gm",           " - [Gamemode (0|1)] - Change your gamemode",          "core.changegm")
	Plugin:AddCommand("/gotoworld",    " - Move to a different world!",                       "core.gotoworld")
	Plugin:AddCommand("/coords",       " - Show your current server coordinates",             "core.coords")
	Plugin:AddCommand("/viewdistance", " - [".. cClientHandle.MIN_VIEW_DISTANCE .."-".. cClientHandle.MAX_VIEW_DISTANCE .."] - Change your view distance", "core.viewdistance")
	Plugin:AddCommand("/regeneratechunk", " - <X [Z]> - Regenerates a chunk", "core.regeneratechunk")
	
	Plugin:BindCommand( "/help",            "core.help",            HandleHelpCommand )
	Plugin:BindCommand( "/pluginlist",      "core.pluginlist",      HandlePluginListCommand )
	Plugin:BindCommand( "/tp",              "core.teleport",        HandleTPCommand )
	Plugin:BindCommand( "/item",            "core.item",            HandleItemCommand )
	Plugin:BindCommand( "/i",               "core.item",            HandleItemCommand )
	Plugin:BindCommand( "/list",            "core.playerlist",      HandlePlayerListCommand )
	Plugin:BindCommand( "/who",             "core.playerlist",      HandlePlayerListCommand )
	Plugin:BindCommand( "/playerlist",      "core.playerlist",      HandlePlayerListCommand )
	Plugin:BindCommand( "/motd",            "core.motd",            HandleMOTDCommand )
	Plugin:BindCommand( "/reload",          "core.reload",          HandleReloadCommand )
	Plugin:BindCommand( "/stop",          	"core.stop",          	HandleStopCommand )
	Plugin:BindCommand( "/time",            "core.time",            HandleTimeCommand )
	Plugin:BindCommand( "/spawn",           "core.spawn",           HandleSpawnCommand )
	Plugin:BindCommand( "/kick",            "core.kick",            HandleKickCommand )
	Plugin:BindCommand( "/ban",             "core.ban",             HandleBanCommand )
	Plugin:BindCommand( "/unban",           "core.unban",           HandleUnbanCommand )
	Plugin:BindCommand( "/top",             "core.top",             HandleTopCommand )
	Plugin:BindCommand( "/gm",              "core.changegm",        HandleChangeGMCommand )
	Plugin:BindCommand( "/gotoworld",       "core.gotoworld",       HandleGotoWorldCommand )
	Plugin:BindCommand( "/coords",          "core.coords",          HandleCoordsCommand )
	Plugin:BindCommand( "/viewdistance",    "core.viewdistance",    HandleViewDistanceCommand )
	Plugin:BindCommand( "/regeneratechunk", "core.regeneratechunk", HandleRegenerateChunkCommand )

	local IniFile = cIniFile("settings.ini")
	if ( IniFile:ReadFile() == true ) then
		SHOW_PLUGIN_NAMES = IniFile:GetValueB("HelpPlugin", "ShowPluginNames", true )
	end
	
	-- Load whitelist, and add default values and stuff
	WhiteListIni = cIniFile( Plugin:GetLocalDirectory() .. "/whitelist.ini" )
	if ( WhiteListIni:ReadFile() == true ) then
		if( WhiteListIni:GetValueB("WhiteListSettings", "WhiteListOn", false) == true ) then
			if( WhiteListIni:GetNumValues("WhiteList") > 0 ) then
				LOGINFO("Core: loaded "  .. WhiteListIni:GetNumValues('WhiteList') .. " whitelisted players.")
			else
				LOGWARN("WARNING: WhiteList is on, but there are no people in the whitelist!")
			end
		end
	else
		WhiteListIni:SetValueB("WhiteListSettings", "WhiteListOn", false )
		WhiteListIni:SetValue("WhiteList", "", "")	-- So it adds an empty header
		WhiteListIni:DeleteValue("WhiteList", "") -- And remove the value
		WhiteListIni:KeyComment("WhiteList", "PlayerName=1")
		if( WhiteListIni:WriteFile() == false ) then
			LOGWARN("WARNING: Could not write to whitelist.ini")
		end
	end
	
	-- Load banned players, and add default values and stuff
	BannedPlayersIni = cIniFile( Plugin:GetLocalDirectory() .. "/banned.ini" )
	if ( BannedPlayersIni:ReadFile() == true ) then
		if( BannedPlayersIni:GetNumValues("Banned") > 0 ) then
			LOGINFO("Core: loaded "  .. BannedPlayersIni:GetNumValues("Banned") .. " banned players.")
		end
	else
		BannedPlayersIni:SetValue("Banned", "", "")	-- So it adds an empty header
		BannedPlayersIni:DeleteValue("Banned", "") -- And remove the value
		BannedPlayersIni:KeyComment("Banned", "PlayerName=1")
		if( BannedPlayersIni:WriteFile() == false ) then
			LOGWARN("WARNING: Could not write to banned.ini")
		end
	end
	
	Plugin:AddWebTab( "Server Settings", HandleRequest_ServerSettings )
	Plugin:AddWebTab( "Chat", HandleRequest_Chat )
	Plugin:AddWebTab( "Playerlist", HandleRequest_PlayerList )
	Plugin:AddWebTab( "Whitelist", HandleRequest_WhiteList )
	Plugin:AddWebTab( "Permissions", HandleRequest_Permissions )
	Plugin:AddWebTab( "Manage Plugins", HandleRequest_ManagePlugins )
	
	LOG( "Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion() )
	return true
end