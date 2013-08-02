function HandleHelpCommand(Split, Player)
	local PluginManager = cRoot:Get():GetPluginManager()
	
	local LinesPerPage = 8;
	local CurrentPage = 1;
	local CurrentLine = 0;
	local PageRequested = 1;
	local Output = {};
	
	if (#Split == 2) then
		PageRequested = tonumber(Split[2]);
	end
	
	local Process = function(Command, Permission, HelpString)
		if not(Player:HasPermission(Permission)) then
			return false;
		end;
		if (HelpString == "") then
			return false;
		end;

		CurrentLine = CurrentLine + 1;
		CurrentPage = math.floor(CurrentLine / LinesPerPage) + 1;
		if (CurrentPage ~= PageRequested) then
			return false;
		end;
		table.insert(Output, cChatColor.Blue .. Command .. HelpString);
	end

	PluginManager:ForEachCommand(Process);

	-- CurrentPage now contains the total number of pages, and Output has the individual help lines to be sent

	Player:SendMessage(cChatColor.Purple .. "---------- [COMMANDS HELP " .. cChatColor.Gold .. "(Page " .. PageRequested .. " / " .. CurrentPage .. ")" .. cChatColor.Purple .. "] -----------");
	Player:SendMessage(cChatColor.Purple .. "'-' means no prefix, '~' means a value is required.");
	for idx, msg in ipairs(Output) do
		Player:SendMessage(msg);
	end;
	
	return true
end