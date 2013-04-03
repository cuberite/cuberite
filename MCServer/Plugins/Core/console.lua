
-- console.lua

-- Implements things related to console commands





function InitConsoleCommands()
	local PluginMgr = cPluginManager:Get();
	PluginMgr:BindConsoleCommand("help",                 HandleConsoleHelp,                 "Lists all commands");
	PluginMgr:BindConsoleCommand("numchunks",            HandleConsoleNumChunks,            "Shows number of chunks currently loaded");
	PluginMgr:BindConsoleCommand("players",              HandleConsolePlayers,              "Lists all connected players");
	PluginMgr:BindConsoleCommand("primaryserverversion", HandleConsolePrimaryServerVersion, "Gets or sets server version reported to 1.4+ clients");
	PluginMgr:BindConsoleCommand("reload",               HandleConsoleReload,               "Reloads all plugins");
	PluginMgr:BindConsoleCommand("save-all",             HandleConsoleSaveAll,              "Saves all chunks");
	PluginMgr:BindConsoleCommand("say",                  HandleConsoleSay,                  "Sends a chat message to all players");
	PluginMgr:BindConsoleCommand("unload",               HandleConsoleUnload,               "Unloads all unused chunks");
	PluginMgr:BindConsoleCommand("rank",                 HandleConsoleRank,                 " [Player] [Rank] - to add someone to a group");
	PluginMgr:BindConsoleCommand("listgroups",           HandleConsoleListGroups,           "Shows a list of all the groups");
end





function HandleConsoleHelp(Split)
	local Commands = {};   -- {index => {"Command", "HelpString"} }
	local MaxLength = 0;
	local AddToTable = function(Command, HelpString)
		table.insert(Commands, { Command, HelpString });
		local CmdLen = Command:len();
		if (CmdLen > MaxLength) then
			MaxLength = CmdLen;
		end
	end
	
	cPluginManager:Get():ForEachConsoleCommand(AddToTable);
	
	-- Sort the table:
	local CompareCommands = function(a, b)
		return a[1] < b[1];  -- compare command strings
	end
	table.sort(Commands, CompareCommands);
	
	for i, Command in ipairs(Commands) do
		local Cmd = Command[1] .. string.rep(" ", MaxLength - Command[1]:len());  -- Align to a table
		LOG(Cmd .. " - " .. Command[2]);
	end
	return true;
end





function HandleConsoleNumChunks(Split)
	local Output = {};
	local AddNumChunks = function(World)
		Output[World:GetName()] = World:GetNumChunks();
	end;
	
	cRoot:Get():ForEachWorld(AddNumChunks);
	
	local Total = 0;
	for name, num in pairs(Output) do
		LOG("  " .. name .. ": " .. num .. " chunks");
		Total = Total + num;
	end
	LOG("Total: " .. Total .. " chunks");
	
	return true;
end





function HandleConsolePlayers(Split)
	local PlayersInWorlds = {};    -- "WorldName" => [players array]
	local AddToTable = function(Player)
		local WorldName = Player:GetWorld():GetName();
		if (PlayersInWorlds[WorldName] == nil) then
			PlayersInWorlds[WorldName] = {};
		end
		table.insert(PlayersInWorlds[WorldName], Player:GetName() .. " @ " ..  Player:GetIP());
	end
	
	cRoot:Get():ForEachPlayer(AddToTable);
	
	for WorldName, Players in pairs(PlayersInWorlds) do
		LOG("World " .. WorldName .. ":");
		for i, PlayerName in ipairs(Players) do
			LOG("  " .. PlayerName);
		end
	end
	
	return true;
end





function HandleConsolePrimaryServerVersion(Split)
	if (#Split == 1) then
		-- Display current version:
		local Version = cRoot:Get():GetPrimaryServerVersion();
		LOG("Primary server version: #" .. Version .. ", " .. cRoot:GetProtocolVersionTextFromInt(Version));
		return true;
	end
	
	-- Set new value as the version:
	cRoot:Get():SetPrimaryServerVersion(tonumber(Split[2]));
	local Version = cRoot:Get():GetPrimaryServerVersion();
	LOG("Primary server version is now #" .. Version .. ", " .. cRoot:GetProtocolVersionTextFromInt(Version));
	return true;
end





function HandleConsoleReload(Split)
	Server = cRoot:Get():GetServer();
	Server:SendMessage(cChatColor.Green .. "Reloading all plugins.");
	cPluginManager:Get():ReloadPlugins();
	return true;
end





function HandleConsoleSaveAll(Split)
	cRoot:Get():SaveAllChunks();
	return true;
end





function HandleConsoleSay(Split)
	table.remove(Split, 1);
	local Message = "";
	for i, Text in ipairs(Split) do
		Message = Message .. " " .. Text;
	end
	Message = Message:sub(2);  -- Cut off the first space
	cRoot:Get():GetServer():BroadcastChat(cChatColor.Purple .. "[SERVER] " .. Message);
	return true;
end





function HandleConsoleUnload(Split)
	local UnloadChunks = function(World)
		World:UnloadUnusedChunks();
	end
	
	LOGINFO("Num loaded chunks before: " .. cRoot:Get():GetTotalChunkCount());
	cRoot:Get():ForEachWorld(UnloadChunks);
	LOGINFO("Num loaded chunks after: " .. cRoot:Get():GetTotalChunkCount());
	return true;
end






function HandleConsoleRank(Split)
	if Split[2] == nil or Split[3] == nil then
		LOG("Usage: /rank [Player] [Group]")
		return true
	end
	local GroupsIni = cIniFile("groups.ini")
	if( GroupsIni:ReadFile() == false ) then
		LOG("Could not read groups.ini!")
	end
	if GroupsIni:FindKey(Split[3]) == -1 then
		LOG("Group does not exist")
		return true
	end
	local UsersIni = cIniFile("users.ini")
	if( UsersIni:ReadFile() == false ) then
		LOG("Could not read users.ini!")
	end
	UsersIni:DeleteKey(Split[2])
	UsersIni:GetValueSet(Split[2], "Groups", Split[3])
	UsersIni:WriteFile()
	local loopPlayers = function( Player )
		if Player:GetName() == Split[2] then
			Player:SendMessage( cChatColor.Green .. "You were moved to group " .. Split[3] )
			Player:LoadPermissionsFromDisk()
		end
	end
	local loopWorlds = function ( World )
		World:ForEachPlayer( loopPlayers )
	end
	cRoot:Get():ForEachWorld( loopWorlds )
	LOG("Player " .. Split[2] .. " Was moved to " .. Split[3])
	return true
end






function HandleConsoleListGroups(Split)
	local GroupsIni = cIniFile("groups.ini")
	if GroupsIni:ReadFile() == false then
		LOG( "No groups found" )
	end
	Number = GroupsIni:NumKeys()
	Groups = {}
	for i=0, Number do
		table.insert( Groups, GroupsIni:KeyName(i) )
	end
	LOGINFO( "Groups:" )
	LOGINFO( table.concat( Groups, ", " ) )
	return true
end



function HandleConsole(Split)
	return true;
end





function HandleConsole(Split)
	return true;
end





