---- Some settings -----
SHOW_PLUGIN_NAMES = true	-- If true, plugin name will be shown before commands
							-- This is overwritten in the Initialize() function
------------------------

local BannedPlayersIni = {}
local WhiteListIni = {}

CorePlugin = {}
CorePlugin.__index = CorePlugin

function CorePlugin:new()
   local t = {}
   setmetatable(t, CorePlugin)
   local w = Lua__cPlugin:new()
   tolua.setpeer(w, t)
   w:tolua__set_instance(w)
   return w
end

function CorePlugin:OnDisable()
	LOG( self:GetName() .. " v." .. self:GetVersion() .. " is shutting down..." )
end

function CorePlugin:Initialize()
	self:SetName( "Core" )
	self:SetVersion( 6 )

	PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:AddHook( self, cPluginManager.E_PLUGIN_PLAYER_JOIN )
	PluginManager:AddHook( self, cPluginManager.E_PLUGIN_LOGIN )
	PluginManager:AddHook( self, cPluginManager.E_PLUGIN_BLOCK_PLACE )
	PluginManager:AddHook( self, cPluginManager.E_PLUGIN_KILLED )

	self:AddCommand("/help", " - [Page] Show this message", 					"core.help")
	self:AddCommand("/pluginlist", " - Show list of plugins", 					"core.pluginlist")
	self:AddCommand("/tp", " - [Player] - Teleport yourself to a player", 		"core.teleport")
	self:AddCommand("/item", " - [ItemID/Name] <Amount> - Spawn an item for yourself", "core.item")
	self:AddCommand("/list", " - Shows list of connected players", 				"core.playerlist")
	self:AddCommand("/motd", " - Show message of the day", 						"core.motd")
	self:AddCommand("/reload", " - Reload all plugins", 						"core.reload")
	self:AddCommand("/time", " - [Day/Night] - Sets the time of day", 			"core.time")
	self:AddCommand("/spawn", " - Return to the spawn", 						"core.spawn")
	self:AddCommand("/kick", " - [Player] - Kick a player", 					"core.kick")
	self:AddCommand("/ban", " - [Player] - Ban a player", 						"core.ban")
	self:AddCommand("/unban", " - [Player] - Unban a player", 					"core.unban")
	self:AddCommand("/top", " - Teleport yourself to the top most block",		"core.top")
	self:AddCommand("/gm", " - [Gamemode (0|1)] - Change your gamemode", 		"core.changegm")
	self:AddCommand("/gotoworld", " - Move to a different world!",				"core.gotoworld")

	self:BindCommand( "/help", 		"core.help",		HandleHelpCommand )
	self:BindCommand( "/pluginlist","core.pluginlist", 	HandlePluginListCommand )
	self:BindCommand( "/tp", 		"core.teleport",	HandleTPCommand )
	self:BindCommand( "/item", 		"core.item",		HandleItemCommand )
	self:BindCommand( "/i", 		"core.item",		HandleItemCommand )
	self:BindCommand( "/list", 		"core.playerlist",	HandlePlayerListCommand )
	self:BindCommand( "/who", 		"core.playerlist",	HandlePlayerListCommand )
	self:BindCommand( "/playerlist","core.playerlist", 	HandlePlayerListCommand )
	self:BindCommand( "/motd", 		"core.motd",		HandleMOTDCommand )
	self:BindCommand( "/reload", 	"core.reload",		HandleReloadCommand )
	self:BindCommand( "/time", 		"core.time", 		HandleTimeCommand )
	self:BindCommand( "/spawn", 	"core.spawn",		HandleSpawnCommand )
	self:BindCommand( "/home", 		"core.spawn",		HandleSpawnCommand )
	self:BindCommand( "/kick", 		"core.kick",		HandleKickCommand )
	self:BindCommand( "/ban", 		"core.ban",			HandleBanCommand )
	self:BindCommand( "/unban", 	"core.unban",		HandleUnbanCommand )
	self:BindCommand( "/top",		"core.top",			HandleTopCommand )
	self:BindCommand( "/gm", 		"core.changegm", 	HandleChangeGMCommand )
	self:BindCommand( "/gotoworld",	"core.gotoworld",	HandleGotoWorldCommand )

	local IniFile = cIniFile("settings.ini")
	if ( IniFile:ReadFile() == true ) then
		SHOW_PLUGIN_NAMES = IniFile:GetValueB("HelpPlugin", "ShowPluginNames", true )
	end

	itemsINI = cIniFile("items.ini")
	if ( itemsINI:ReadFile() == true ) then
		LOGINFO("Core: loaded "  .. itemsINI:GetNumValues('Items') .. " item names.")
		HAVE_ITEM_NAMES = true
	end
	
	-- Load whitelist, and add default values and stuff
	WhiteListIni = cIniFile("whitelist.ini")
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
	BannedPlayersIni = cIniFile("banned.ini")
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

	LOG( "Initialized " .. self:GetName() .. " v." .. self:GetVersion() )
	return true
end

function HandleTopCommand( Split, Player )
	local World = Player:GetWorld()
	
	local PlayerPos = Player:GetPosition()
	local Height = World:GetHeight( math.floor(PlayerPos.x), math.floor(PlayerPos.z) )
	
	Player:TeleportTo( PlayerPos.x, Height+1, PlayerPos.z )
	Player:SendMessage("Teleported to the top block")
	
	return true
end

function HandleKickCommand( Split, Player )
	if( #Split < 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /kick [Player] <Reason>" )
		return true
	end

	local World = Player:GetWorld()
	local OtherPlayer = World:GetPlayer( Split[2] )
	if( OtherPlayer == nil ) then
		Player:SendMessage( cChatColor.Green .. "Could not find player " .. Split[2] )
		return true
	end

	local Reason = "You have been kicked"
	if( #Split > 2 ) then
		Reason = table.concat(Split, " ", 3)
	end

	local Server = cRoot:Get():GetServer()
	LOGINFO( Player:GetName() .. " is kicking " .. OtherPlayer:GetName() .. " ( "..Reason..") " )
	Server:SendMessage( "Kicking " .. OtherPlayer:GetName() )

	local ClientHandle = OtherPlayer:GetClientHandle()
	ClientHandle:Kick( Reason )

	return true
end

function HandleBanCommand( Split, Player )
	if( #Split < 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /ban [Player] <Reason>" )
		return true
	end

	local World = Player:GetWorld()
	local OtherPlayer = World:GetPlayer( Split[2] )
	if( OtherPlayer == nil ) then
		Player:SendMessage( cChatColor.Green .. "Could not find player " .. Split[2] )
		return true
	end

	local Reason = "You have been banned"
	if( #Split > 2 ) then
		Reason = table.concat(Split, " ", 3)
	end

	local Server = cRoot:Get():GetServer()
	LOGINFO( Player:GetName() .. " is banning " .. OtherPlayer:GetName() .. " ( "..Reason..") " )
	Server:SendMessage( "Banning " .. OtherPlayer:GetName() )

	local ClientHandle = OtherPlayer:GetClientHandle()
	ClientHandle:Kick( Reason )
	
	BannedPlayersIni:SetValueB("Banned", OtherPlayer:GetName(), true)
	BannedPlayersIni:WriteFile()

	return true
end

function HandleUnbanCommand( Split, Player )
	if( #Split < 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /unban [Player]" )
		return true
	end
	
	if( BannedPlayersIni:GetValueB("Banned", Split[2], false) == false ) then
		Player:SendMessage( cChatColor.Green .. Split[2] .. " is not banned!" )
		return true
	end
	
	BannedPlayersIni:SetValueB("Banned", Split[2], false, false)
	BannedPlayersIni:WriteFile()

	local Server = cRoot:Get():GetServer()
	LOGINFO( Player:GetName() .. " is unbanning " .. Split[2] )
	Server:SendMessage( "Unbanning " .. Split[2] )

	return true
end

function HandleMOTDCommand( Split, Player )
	ShowMOTDTo( Player )
	return true
end

function ShowMOTDTo( Player )
	Player:SendMessage( cChatColor.Gold .. "Welcome to the MCServer test server!" );
	Player:SendMessage( cChatColor.Gold .. "http://mcserver.ae-c.net/" );
	Player:SendMessage( cChatColor.Gold .. "Type /help for all commands" );
end

function HandleSpawnCommand( Split, Player )
	World = Player:GetWorld()
	Player:TeleportTo( World:GetSpawnX(), World:GetSpawnY(), World:GetSpawnZ() )
	LOGINFO( Player:GetName() .. " returned to spawn." )
	return true
end

function HandleTimeCommand( Split, Player )
	if( #Split ~= 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /time [Day/Night]" )
		return true;
	end

	local Server = cRoot:Get():GetServer()
	if( string.upper( Split[2] ) == "DAY") then
		Player:GetWorld():SetWorldTime( 0 )
		Server:SendMessage( cChatColor.Green .. Player:GetName() .. " set the time to Day.")
	elseif( string.upper( Split[2] ) == "NIGHT") then
		Player:GetWorld():SetWorldTime( 12000 + 1000 )
		Server:SendMessage( cChatColor.Green .. Player:GetName() .. " set the time to Night.")
	else
		Player:SendMessage( cChatColor.Green .. "Usage: /time [Day/Night]" )
	end
	return true
end

function HandleReloadCommand( Split, Player )
	Server = cRoot:Get():GetServer()
	Server:SendMessage( cChatColor.Green .. "Reloading all plugins." )
	cRoot:Get():GetPluginManager():ReloadPlugins()
	return true
end

function HandlePlayerListCommand( Split, Player )
	local World = Player:GetWorld()
	local PlayerList = World:GetAllPlayers()

	local Message = cChatColor.Green .. "Connected players: (".. cChatColor.White.. #PlayerList .. cChatColor.Green .. ")"
	Player:SendMessage( Message )

	local PlayerTable = {}
	for i, TempPlayer in ipairs( PlayerList ) do
		local PlayerName = TempPlayer:GetName()
		table.insert(PlayerTable, PlayerName )
	end

	Player:SendMessage( table.concat(PlayerTable, " ") )
	return true
end

function HandleHelpCommand( Split, Player )
	local PluginManager = cRoot:Get():GetPluginManager()
	
	local LinesPerPage = 9
	local CurrentPage = 1
	local CurrentLine = 0
	
	if( #Split == 2 ) then
		CurrentPage = tonumber(Split[2])
	end
	
	local Pages = {}

	local PluginList = PluginManager:GetAllPlugins()
	for i, Plugin in ipairs( PluginList ) do
		local Commands = Plugin:GetCommands()
		for i, v in ipairs( Commands ) do
			if( Player:HasPermission( v.Permission ) ) then
				local PageNum = math.floor( CurrentLine/LinesPerPage )+1
				if( Pages[ PageNum ] == nil ) then Pages[ PageNum ] = {} end -- Create page
				
				if( Pages[ PageNum ].ShownName ~= Plugin:GetName() and SHOW_PLUGIN_NAMES == true ) then
					if( CurrentLine == LinesPerPage * PageNum -1 ) then	-- Don't add if it's the last line of the page, it looks silly
						-- Add it to the next page instead
						CurrentLine = CurrentLine+1
						PageNum = math.floor( CurrentLine/LinesPerPage )+1
						
						if( Pages[ PageNum ] == nil ) then Pages[ PageNum ] = {} end -- Create page
						table.insert( Pages[ PageNum ], cChatColor.Gold .. Plugin:GetName() )
					else
						Pages[ PageNum ].ShownName = Plugin:GetName()
						table.insert( Pages[ PageNum ], cChatColor.Gold .. Plugin:GetName() )
					end
					CurrentLine = CurrentLine+1
					PageNum = math.floor( CurrentLine/LinesPerPage )+1
					if( Pages[ PageNum ] == nil ) then Pages[ PageNum ] = {} end -- Create page
				end
				local Message = cChatColor.Blue .. v.Command .. v.Description;
				table.insert( Pages[ PageNum ], Message )
				CurrentLine = CurrentLine+1
			end
		end
	end
	
	Player:SendMessage( cChatColor.Purple .. "- All commands - " .. cChatColor.Gold .. "[Page " .. (CurrentPage) .."/"..#Pages.."]" )
	
	if( Pages[CurrentPage] ~= nil ) then
		for i, v in ipairs(Pages[CurrentPage]) do
			Player:SendMessage( v )
		end
	end
	
	return true
end

function HandlePluginListCommand( Split, Player )
	local PluginManager = cRoot:Get():GetPluginManager()
	local PluginList = PluginManager:GetAllPlugins()

	local PluginTable = {}
	for i, Plugin in ipairs( PluginList ) do
		table.insert(PluginTable, Plugin:GetName() )
	end

	Player:SendMessage( cChatColor.Green .. "Loaded plugins:" )
	Player:SendMessage( cChatColor.Gold .. table.concat(PluginTable, cChatColor.Gold.." ") )
	return true
end

function HandleItemCommand( Split, Player )
	if( #Split ~= 2 and #Split ~=3 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /item [ItemID/Name] <Amount>" )
		return true
	end

	foundItem = false

	ItemID = tonumber( Split[2] )
	if( ItemID == nil or not IsValidItem( ItemID ) ) then
		-- nothing
	else
		foundItem = true
	end

	if not foundItem then
		if ( HAVE_ITEM_NAMES == true ) then
			itemValue = itemsINI:GetValueI('Items', ''..Split[2]..'', 0)
			if itemValue ~= 0 then
				ItemID = itemValue
				if( ItemID == nil or not IsValidItem( tonumber(itemValue) ) ) then
					-- nothing
				else
					foundItem = true
				end
			end
		end
	end

	if not foundItem then
		Player:SendMessage( cChatColor.Green .. "Invalid Item ID / Name !" )
		return true
	end

	local Amount = 1
	if( #Split == 3 ) then
		Amount = tonumber( Split[3] )
		if( Amount == nil or Amount < 1 or Amount > 512 ) then
			Player:SendMessage( cChatColor.Green .. "Invalid Amount !" )
			return true
		end
	end

	local NewItem = cItem( ItemID, Amount )
	if( Player:GetInventory():AddItem( NewItem ) == true ) then
		Player:SendMessage( cChatColor.Green .. "There you go !" )
		LOG("Gave " .. Player:GetName() .. " " .. Amount .. " times " .. ItemID )
	else
		Player:SendMessage( cChatColor.Green .. "Not enough space in inventory !" )
	end
	return true
end

function HandleTPCommand( Split, Player )
	if( #Split ~= 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /tp [PlayerName]" )
		return true
	end
	local World = Player:GetWorld()
	local OtherPlayer = World:GetPlayer( Split[2] )
	if( OtherPlayer == nil ) then
		Player:SendMessage( cChatColor.Green .. "Can't find player " .. Split[2] )
	elseif( OtherPlayer == Player ) then
		Player:SendMessage( cChatColor.Green .. "Already there :)" )
	else
		Player:TeleportTo( OtherPlayer )
		Player:SendMessage( cChatColor.Green .. "You teleported to "..OtherPlayer:GetName().."!" )
		OtherPlayer:SendMessage( cChatColor.Green .. Player:GetName().." teleported to you!" )
	end
	return true
end

function HandleChangeGMCommand( Split, Player )
	if( #Split ~= 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /gm [GameMode (0|1)]" )
		return true
	end
		
	Player:SetGameMode(Split[2])
		
	return true
end

function HandleGotoWorldCommand( Split, Player )
	if( #Split ~= 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /gotoworld [WorldName]" )
		return true	
	end
	
	if( Player:MoveToWorld(Split[2]) == false ) then
		Player:SendMessage( cChatColor.Green .. "Could not move to world '" .. Split[2] .. "'!" )
		return true
	end
	
	
	Player:SendMessage( cChatColor.Green .. "Moved successfully to '" .. Split[2] .. "'! :D" )
	return true
end

function CorePlugin:OnPlayerJoin( Player )
	ShowMOTDTo( Player )
	return false
end

function CorePlugin:OnBlockPlace( Block, Player )

	-- dont check if the direction is in the air
	if Block.m_Direction ~= -1 then

		local X = Block.m_PosX
		local Y = Block.m_PosY
		local Z = Block.m_PosZ
		X, Y, Z = AddDirection( X, Y, Z, Block.m_Direction )
		if( Y >= 128 or Y < 0 ) then
			return true
		end

		local collision = false
		local World = Player:GetWorld()
		local PlayerList = World:GetAllPlayers()

		-- check if a player occupies the placement location
		for i, Player in ipairs( PlayerList ) do

			-- drop the decimals, we only care about the full block X,Y,Z
			local PlayerX = math.floor(Player:GetPosX(), 0)
			local PlayerY = math.floor(Player:GetPosY(), 0)
			local PlayerZ = math.floor(Player:GetPosZ(), 0)

			local BlockX = Block.m_PosX
			local BlockY = Block.m_PosY
			local BlockZ = Block.m_PosZ

			-- player height is 2 blocks, so we check the position and then offset it up one
			-- so they can't place a block on there face

			if Block.m_Direction == 0 then if PlayerY == BlockY-2 and PlayerX == BlockX and PlayerZ == BlockZ then collision = true end end
			if Block.m_Direction == 1 then if PlayerY == BlockY+1 and PlayerX == BlockX and PlayerZ == BlockZ then collision = true end end

			if Block.m_Direction == 2 then if PlayerY == BlockY and PlayerX == BlockX and PlayerZ == BlockZ-1 then collision = true end end
			if Block.m_Direction == 2 then if PlayerY+1 == BlockY and PlayerX == BlockX and PlayerZ == BlockZ-1 then collision = true end end

			if Block.m_Direction == 3 then if PlayerY == BlockY and PlayerX == BlockX and PlayerZ == BlockZ+1 then collision = true end end
			if Block.m_Direction == 3 then if PlayerY+1 == BlockY and PlayerX == BlockX and PlayerZ == BlockZ+1 then collision = true end end

			if Block.m_Direction == 4 then if PlayerY == BlockY and PlayerX == BlockX-1 and PlayerZ == BlockZ then collision = true end end
			if Block.m_Direction == 4 then if PlayerY+1 == BlockY and PlayerX == BlockX-1 and PlayerZ == BlockZ then collision = true end end

			if Block.m_Direction == 5 then if PlayerY == BlockY and PlayerX == BlockX+1 and PlayerZ == BlockZ then collision = true end end
			if Block.m_Direction == 5 then if PlayerY+1 == BlockY and PlayerX == BlockX+1 and PlayerZ == BlockZ then collision = true end end

		end

		if collision then
			return true
		else
			return false
		end

	end

	return false

end

function CorePlugin:OnKilled( Killed, Killer )
	if( Killer == nil ) then
		local KilledPlayer = tolua.cast( Killed, "cPlayer")
		if( not KilledPlayer:IsA("cPlayer") or KilledPlayer == nil ) then
			return false
		end

		local Server = cRoot:Get():GetServer()
		Server:SendMessage( cChatColor.Red .. KilledPlayer:GetName() .. " died" )
	else
		local KilledPlayer = tolua.cast( Killed, "cPlayer")
		if( not KilledPlayer:IsA("cPlayer") or KilledPlayer == nil ) then
			return false
		end
		local KillerPlayer = tolua.cast( Killer, "cPlayer")
		if( not KillerPlayer:IsA("cPlayer") or KillerPlayer == nil ) then
			return false
		end

		local Server = cRoot:Get():GetServer()
		Server:SendMessage( cChatColor.Red .. KilledPlayer:GetName() .. " was killed by " .. KillerPlayer:GetName() .. "!" )
	end
	return false
end

function CorePlugin:OnLogin( PacketData )
	if( PacketData.m_Username ~= "" ) then
		if( BannedPlayersIni:GetValueB("Banned", PacketData.m_Username, false) == true ) then
			local Server = cRoot:Get():GetServer()
			Server:SendMessage( PacketData.m_Username .. " tried to join, but is banned!" )
			LOGINFO( PacketData.m_Username .. " tried to join, but is banned!")
			return true	-- Player is banned, return true to deny access
		end
		if( WhiteListIni:GetValueB("WhiteListSettings", "WhiteListOn", false ) == true ) then
			if( WhiteListIni:GetValueB("WhiteList", PacketData.m_Username, false ) == false ) then -- not on whitelist
				local Server = cRoot:Get():GetServer()
				Server:SendMessage( PacketData.m_Username .. " tried to join, but is not on the whitelist." )
				LOGINFO( PacketData.m_Username .. " tried to join, but is not on the whitelist." )
				return true -- Deny access to the server
			end
		end
	end
	
	return false
end

Plugin = CorePlugin:new()
cRoot:Get():GetPluginManager():AddPlugin( Plugin )

------------------------------------------------
-- Now some webadmin stuffs!                  --
------------------------------------------------
local PlayerListWeb = {}
PlayerListWeb.__index = PlayerListWeb

function PlayerListWeb:new()
   local t = {}
   setmetatable(t, PlayerListWeb)
   local w = Lua__cWebPlugin:new()
   tolua.setpeer(w, t)
   w:tolua__set_instance(w)
   return w
end

function PlayerListWeb:HandleRequest( Request )
	local World = cRoot:Get():GetWorld()
	local Content = ""
	
	if( Request.Params:get("playerlist-kick") ~= "" ) then
		local KickPlayerName = Request.Params:get("playerlist-kick")
		local Player = World:GetPlayer( KickPlayerName )
		if( Player == nil ) then
			Content = Content .. "<p>Could not find player " .. KickPlayerName .. " !</p>"
		elseif( Player:GetName() == KickPlayerName ) then
			Player:GetClientHandle():Kick("You were kicked from the game!")
			Content = Content .. "<p>" .. KickPlayerName .. " has been kicked from the game!</p>"
		end
	end
	
	Content = Content .. "<p>Connected Players: <b>" .. World:GetNumPlayers() .. "</b></p>"
	Content = Content .. "<table>"
	

	local PlayerList = World:GetAllPlayers()
	for i, Player in ipairs( PlayerList ) do
		Content = Content .. "<tr>"
		Content = Content .. "<td style='width: 10px;'>" .. i .. ".</td>"
		Content = Content .. "<td>" .. Player:GetName() .. "</td>"
		Content = Content .. "<td><a href='?playerlist-kick=" .. Player:GetName() .. "'>Kick</a></td>"
		Content = Content .. "</tr>"
	end

	Content = Content .. "</table>"
	Content = Content .. "<br>"
	return Content
end

WebPlugin = PlayerListWeb:new()
WebPlugin:SetName("PlayerList")

------------------------------------------------
-- Reload web plugin                          --
------------------------------------------------
local ReloadWeb = {}
ReloadWeb.__index = ReloadWeb

function ReloadWeb:new()
   local t = {}
   setmetatable(t, ReloadWeb)
   local w = Lua__cWebPlugin:new()
   tolua.setpeer(w, t)
   w:tolua__set_instance(w)
   return w
end

function ReloadWeb:HandleRequest( Request )
	local Content = ""
	
	if( Request.Params:get("reload") ~= "" ) then
		Content = Content .. "<head><meta http-equiv=\"refresh\" content=\"1;././\"></head>"
		Content = Content .. "<p>Reloading plugins...</p>"
		cRoot:Get():GetPluginManager():ReloadPlugins()
	else
		Content = Content .. "<form method=GET>"
		Content = Content .. "<p>Click the reload button to reload all plugins!<br>"
		Content = Content .. "<input type=\"submit\" name=\"reload\" value=\"Reload!\"></p>"
		Content = Content .. "</form>"
	end
	return Content
end

WebPlugin = ReloadWeb:new()
WebPlugin:SetName("Reload")

------------------------------------------------
-- Whitelist web plugin                       --
------------------------------------------------
local WhiteListWeb = {}
WhiteListWeb.__index = WhiteListWeb

function WhiteListWeb:new()
   local t = {}
   setmetatable(t, WhiteListWeb)
   local w = Lua__cWebPlugin:new()
   tolua.setpeer(w, t)
   w:tolua__set_instance(w)
   return w
end

local function HTMLDeleteButton( name )
	return "<form method=GET><input type=\"hidden\" name=\"whitelist-delete\" value=\"".. name .."\"><input type=\"submit\" value=\"Remove from whitelist\"></form>"
end

function WhiteListWeb:HandleRequest( Request )
	local UpdateMessage = ""
	if( Request.Params:get("whitelist-add") ~= "" ) then
		local PlayerName = Request.Params:get("whitelist-add")
		
		if( WhiteListIni:GetValueB("WhiteList", PlayerName, false) == true ) then
			UpdateMessage = "<b>".. PlayerName.."</b> is already on the whitelist"
		else
			WhiteListIni:SetValueB("WhiteList", PlayerName, true )
			UpdateMessage = "Added <b>" .. PlayerName .. "</b> to whitelist."
			WhiteListIni:WriteFile()
		end
	elseif( Request.Params:get("whitelist-delete") ~= "" ) then
		local PlayerName = Request.Params:get("whitelist-delete")
		WhiteListIni:DeleteValue( "WhiteList", PlayerName )
		UpdateMessage = "Removed <b>" .. PlayerName .. "</b> from whitelist."
		WhiteListIni:WriteFile()
	elseif( Request.Params:get("whitelist-reload") ~= "" ) then
		WhiteListIni:Erase() -- Empty entire loaded ini first, otherwise weird shit goes down
		WhiteListIni:ReadFile()
		UpdateMessage = "Loaded from disk"
	elseif( Request.Params:get("whitelist-setenable") ~= "" ) then
		local Enabled = Request.Params:get("whitelist-setenable");
		local CreateNewValue = false;
		if( WhiteListIni:FindValue( WhiteListIni:FindKey("WhiteListSettings"), "WhiteListOn" ) == cIniFile.noID ) then -- Find out whether the value is in the ini
			CreateNewValue = true
		end
		
		if( Enabled == "1" ) then
			WhiteListIni:SetValueB("WhiteListSettings", "WhiteListOn", true, CreateNewValue )
		else
			WhiteListIni:SetValueB("WhiteListSettings", "WhiteListOn", false, CreateNewValue )
		end
		WhiteListIni:WriteFile()
	end
	
	
	local Content = ""
	
	local WhiteListEnabled = WhiteListIni:GetValueB("WhiteListSettings", "WhiteListOn", false)
	if( WhiteListEnabled == false ) then
		Content = Content .. "<p>Whitelist is currently disabled! Click <a href='?whitelist-setenable=1'>here</a> to enable.</p>"
	end
	
	
	Content = Content .. "<h4>Whitelisted players</h4>"
	Content = Content .. "<table>"
	local KeyNum = WhiteListIni:FindKey("WhiteList")
	local NumValues = WhiteListIni:GetNumValues(KeyNum)
	if( NumValues > 0 ) then
		for Num = 0, NumValues-1 do
			if( WhiteListIni:GetValue(KeyNum, Num, "0") == "1" ) then
				local PlayerName = WhiteListIni:GetValueName(KeyNum, Num )
				Content = Content .. "<tr><td>" .. PlayerName .. "</td><td>" .. HTMLDeleteButton( PlayerName ) .. "</td></tr>"
			end
		end
	else
		Content = Content .. "<tr><td>None</td></tr>"
	end
	Content = Content .. "</table>"
	Content = Content .. "<br><h4>Add player to whitelist</h4>"
	Content = Content .. "<form method=\"GET\">"
	Content = Content .. "<input type=\"text\" name=\"whitelist-add\"><input type=\"submit\" value=\"Add player\">"
	Content = Content .. "</form>"
	Content = Content .. "<form method=\"GET\">"
	Content = Content .. "<input type=\"submit\" name=\"whitelist-reload\" value=\"Reload from disk\">"
	Content = Content .. "</form>"
	Content = Content .. "<br>"..UpdateMessage
	
	if( WhiteListEnabled == true ) then
		Content = Content .. "<br><br><p>Whitelist is currently enabled, click <a href='?whitelist-setenable=0'>here</a> to disable.</p>"
	end
	
	return Content
end

WebPlugin = WhiteListWeb:new()
WebPlugin:SetName("WhiteList")