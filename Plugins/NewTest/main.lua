LOGINFO("main.lua!!")
LOOLOL = "12345"
PLUGIN = {}

function Initialize( Plugin )
	LOGINFO("Initialize in main.lua")
	PLUGIN = Plugin
	
	Plugin:SetName("LOLOL new plugin!")
	
	PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:AddHook( Plugin, cPluginManager.E_PLUGIN_PLAYER_JOIN )
	PluginManager:AddHook( Plugin, cPluginManager.E_PLUGIN_LOGIN )
	PluginManager:AddHook( Plugin, cPluginManager.E_PLUGIN_BLOCK_PLACE )
	PluginManager:AddHook( Plugin, cPluginManager.E_PLUGIN_KILLED )
	
	Plugin:AddCommand("/help", " - [Page] Show this message", 					"core.help")
	Plugin:AddCommand("/pluginlist", " - Show list of plugins", 					"core.pluginlist")
	Plugin:AddCommand("/tp", " - [Player] - Teleport yourself to a player", 		"core.teleport")
	Plugin:AddCommand("/item", " - [ItemID/Name] <Amount> - Spawn an item for yourself", "core.item")
	Plugin:AddCommand("/list", " - Shows list of connected players", 				"core.playerlist")
	Plugin:AddCommand("/motd", " - Show message of the day", 						"core.motd")
	Plugin:AddCommand("/reload", " - Reload all plugins", 						"core.reload")
	Plugin:AddCommand("/time", " - [Day/Night] - Sets the time of day", 			"core.time")
	Plugin:AddCommand("/spawn", " - Return to the spawn", 						"core.spawn")
	Plugin:AddCommand("/kick", " - [Player] - Kick a player", 					"core.kick")
	Plugin:AddCommand("/ban", " - [Player] - Ban a player", 						"core.ban")
	Plugin:AddCommand("/unban", " - [Player] - Unban a player", 					"core.unban")
	Plugin:AddCommand("/top", " - Teleport yourself to the top most block",		"core.top")
	Plugin:AddCommand("/gm", " - [Gamemode (0|1)] - Change your gamemode", 		"core.changegm")
	Plugin:AddCommand("/gotoworld", " - Move to a different world!",				"core.gotoworld")

	Plugin:BindCommand( "/help", 		"core.help",		HandleHelpCommand )
	Plugin:BindCommand( "/pluginlist","core.pluginlist", 	HandlePluginListCommand )
	Plugin:BindCommand( "/tp", 		"core.teleport",	HandleTPCommand )
	Plugin:BindCommand( "/item", 		"core.item",		HandleItemCommand )
	Plugin:BindCommand( "/i", 		"core.item",		HandleItemCommand )
	Plugin:BindCommand( "/list", 		"core.playerlist",	HandlePlayerListCommand )
	Plugin:BindCommand( "/who", 		"core.playerlist",	HandlePlayerListCommand )
	Plugin:BindCommand( "/playerlist","core.playerlist", 	HandlePlayerListCommand )
	Plugin:BindCommand( "/motd", 		"core.motd",		HandleMOTDCommand )
	Plugin:BindCommand( "/reload", 	"core.reload",		HandleReloadCommand )
	Plugin:BindCommand( "/time", 		"core.time", 		HandleTimeCommand )
	Plugin:BindCommand( "/spawn", 	"core.spawn",		HandleSpawnCommand )
	Plugin:BindCommand( "/home", 		"core.spawn",		HandleSpawnCommand )
	Plugin:BindCommand( "/kick", 		"core.kick",		HandleKickCommand )
	Plugin:BindCommand( "/ban", 		"core.ban",			HandleBanCommand )
	Plugin:BindCommand( "/unban", 	"core.unban",		HandleUnbanCommand )
	Plugin:BindCommand( "/top",		"core.top",			HandleTopCommand )
	Plugin:BindCommand( "/gm", 		"core.changegm", 	HandleChangeGMCommand )
	Plugin:BindCommand( "/gotoworld",	"core.gotoworld",	HandleGotoWorldCommand )
	
	LOGINFO("Plugin name is: " .. Plugin:GetName() )
	
	return true
end