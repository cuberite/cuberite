
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
		table.insert(PlayersInWorlds[WorldName], Player:GetName());
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





function HandleConsole(Split)
	return true;
end





function HandleConsole(Split)
	return true;
end





