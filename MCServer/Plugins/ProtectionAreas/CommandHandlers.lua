
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
	
	-- Get the cuboid that the player had selected
	local CmdState = GetCommandStateForPlayer(a_Player);
	if (CmdState == nil) then
		a_Player:SendMessage("Cannot add area, internal plugin error (CmdState == nil)");
		return true;
	end
	local Cuboid = CmdState:GetCurrentCuboid();
	if (Cuboid == nil) then
		a_Player:SendMessage("Cannot add area, internal plugin error (Cuboid == nil)");
		return true;
	end
	
	-- If the cuboid hasn't been assigned, give the player an error message and bail out
	if (
		(Cuboid.p1.x == 0) and (Cuboid.p1.y == 0) and (Cuboid.p1.z == 0) and
		(Cuboid.p1.x == 0) and (Cuboid.p1.y == 0) and (Cuboid.p1.z == 0)
	) then
		a_Player:SendMessage("Cannot add area, no area has been selected. Use a ProtWand lclk / rclk to select area first");
		return true;
	end
	
	-- Put all allowed players into a table:
	AllowedNames = {};
	for i = 2, #a_Split do
		table.insert(AllowedNames, a_Split[i]);
	end
	
	-- Add the area to the storage
	g_Storage:AddArea(Cuboid, a_Player:GetName(), AllowedNames);
	a_Player:SendMessage("Area added");
	
	-- TODO: Reload all currently logged in players
	
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





