
-- CommandHandlers.lua
-- Defines the individual command handlers





function InitializeCommandHandlers()
	local PlgMgr = cRoot:Get():GetPluginManager();
	for idx, Cmd in ipairs(CommandReg()) do
		PlgMgr:BindCommand(Cmd[2], Cmd[3], Cmd[1], Cmd[4]);
	end
end





--- Handles the ProtAdd command
function HandleAddArea(a_Split, a_Player)
	-- Command syntax: ProtAdd username1 [username2] [username3] ...
	if (#a_Split < 2) then
		a_Player:SendMessage("Not enough parameters. Expected a list of usernames.");
		return true;
	end
	
	-- TODO: Add the area to the storage and reload all currently logged in players
	
	return true;
end





function HandleAddAreaCoords(a_Split, a_Player)
	-- TODO
end





function HandleAddAreaUser(a_Split, a_Player)
	-- TODO
end





function HandleDelArea(a_Split, a_Player)
	-- TODO
end





function HandleGiveWand(a_Split, a_Player)
	local NumGiven = a_Player:GetInventory():AddItem(cConfig:GetWandItem());
	if (NumGiven == 1) then
		a_Player:SendMessage("Wand given");
	else
		a_Player:SendMessage("Cannot give wand, no space in your inventory");
	end
	return true;
end





function HandleListAreas(a_Split, a_Player)
	-- TODO
end





function HandleRemoveUser(a_Split, a_Player)
	-- TODO
end





function HandleRemoveUserAll(a_Split, a_Player)
	-- TODO
end





