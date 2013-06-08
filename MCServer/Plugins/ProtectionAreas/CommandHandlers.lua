
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
		a_Player:SendMessage("Cannot add area, no area has been selected. Use a ProtWand lclk / rclk to select area first");
		return true;
	end
	
	-- Put all allowed players into a table:
	AllowedNames = {};
	for i = 2, #a_Split do
		table.insert(AllowedNames, a_Split[i]);
	end
	
	-- Add the area to the storage
	local AreaID = g_Storage:AddArea(Cuboid, a_Player:GetWorld():GetName(), a_Player:GetName(), AllowedNames);
	a_Player:SendMessage("Area added, ID " .. AreaID);
	
	-- Reload all currently logged in players
	ReloadAllPlayersInWorld(a_Player:GetWorld():GetName());
	
	return true;
end





function HandleAddAreaCoords(a_Split, a_Player)
	-- Command syntax: ProtAddCoords x1 z1 x2 z2 username1 [username2] [username3] ...
	if (#a_Split < 6) then
		a_Player:SendMessage("Not enough parameters. Expected <x1> <z1> <x2> <z2> coords and a list of usernames.");
		return true;
	end
	
	-- Convert the coords to a cCuboid
	local x1, z1 = tonumber(a_Split[2]), tonumber(a_Split[3]);
	local x2, z2 = tonumber(a_Split[4]), tonumber(a_Split[5]);
	if ((x1 == nil) or (z1 == nil) or (x2 == nil) or (z2 == nil)) then
		a_Player:SendMessage("Cannot parse coords.");
		return true;
	end
	local Cuboid = cCuboid(x1, 0, z1, x2, 255, z1);
	Cuboid:Sort();
	
	-- Put all allowed players into a table:
	AllowedNames = {};
	for i = 6, #a_Split do
		table.insert(AllowedNames, a_Split[i]);
	end
	
	-- Add the area to the storage
	local AreaID = g_Storage:AddArea(Cuboid, a_Player:GetWorld():GetName(), a_Player:GetName(), AllowedNames);
	a_Player:SendMessage("Area added, ID = " .. AreaID);
	
	-- Reload all currently logged in players
	ReloadAllPlayersInWorld(a_Player:GetWorld():GetName());
	
	return true;
end





function HandleAddAreaUser(a_Split, a_Player)
	-- Command syntax: ProtAddUser AreaID username1 [username2] [username3] ...
	if (#a_Split < 3) then
		a_Player:SendMessage("Not enough parameters. Expected <AreaID> and a list of usernames.");
		return true;
	end
	
	-- Put all allowed players into a table:
	AllowedNames = {};
	for i = 3, #a_Split do
		table.insert(AllowedNames, a_Split[i]);
	end
	
	-- Add the area to the storage
	if (not(g_Storage:AddAreaUsers(
		tonumber(a_Split[2]), a_Player:GetWorld():GetName(), a_Player:GetName(), AllowedNames))
	) then
		LOGWARNING("g_Storage:AddAreaUsers failed");
		a_Player:SendMessage("Cannot add users, DB failure");
		return true;
	end
	if (#AllowedNames == 0) then
		a_Player:SendMessage("All the specified users were already allowed.");
	else
		a_Player:SendMessage("Users added: " .. table.concat(AllowedNames, ", "));
	end
	
	-- Reload all currently logged in players
	ReloadAllPlayersInWorld(a_Player:GetWorld():GetName());
	
	return true;
end





function HandleDelArea(a_Split, a_Player)
	-- Command syntax: ProtDelArea AreaID
	if (#a_Split ~= 2) then
		a_Player:SendMessage("Parameter mismatch. Expected <AreaID>.");
		return true;
	end
	
	-- Parse the AreaID
	local AreaID = tonumber(a_Split[2]);
	if (AreaID == nil) then
		a_Player:SendMessage("Cannot parse <AreaID>.");
		return true;
	end
	
	-- Delete the area
	g_Storage:DelArea(a_Player:GetWorld():GetName(), AreaID);

	-- Reload all currently logged in players
	ReloadAllPlayersInWorld(a_Player:GetWorld():GetName());
	
	return true;
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
	-- Command syntax: ProtListAreas [x, z]
	
	local x, z;
	if (#a_Split == 1) then
		-- Get the last "wanded" coord
		local CmdState = GetCommandStateForPlayer(a_Player);
		if (CmdState == nil) then
			a_Player:SendMessage("Cannot list area, internal plugin error (CmdState == nil)");
			return true;
		end
		x, z = CmdState:GetLastCoords();
		if ((x == nil) or (z == nil)) then
			a_Player:SendMessage("Cannot list areas, no query point has been selected. Use a ProtWand lclk / rclk to select a point first");
			return true;
		end
	elseif (#a_Split == 3) then
		-- Parse the coords from the command params
		x = tonumber(a_Split[2]);
		z = tonumber(a_Split[3]);
		if ((x == nil) or (z == nil)) then
			a_Player:SendMessage("Cannot list areas, cannot parse coords in params");
			return true;
		end
	else
		-- Wrong number of params, report back to the user
		a_Player:SendMessage("Cannot list areas, syntax error. Expected either no params or <x> <z>.");
		return true;
	end
	
	a_Player:SendMessage("Listing protection areas intersecting block column {" .. x .. ", " .. z .. "}:");
	
	-- List areas intersecting the coords
	local PlayerName = a_Player:GetName();
	local WorldName = a_Player:GetWorld():GetName();
	g_Storage:ForEachArea(x, z, WorldName,
		function(AreaID, MinX, MinZ, MaxX, MaxZ, CreatorName)
			local Coords = AreaID .. ": {" ..
				MinX .. ", " .. MinZ .. "} - {" ..
				MaxX .. ", " .. MaxZ .. "} ";
			local Allowance;
			if (g_Storage:IsAreaAllowed(AreaID, PlayerName, WorldName)) then
				Allowance = "Allowed";
			else
				Allowance = "NOT allowed";
			end
			a_Player:SendMessage("  " .. Coords .. Allowance .. ", Created by " .. CreatorName);
		end
	);
	
	a_Player:SendMessage("Area list finished");
	return true;
end




--- Lists all allowed users for a particular area
function HandleListUsers(a_Split, a_Player)
	-- Command syntax: ProtListUsers AreaID
	if (#a_Split ~= 2) then
		a_Player:SendMessage("Expected AreaID as a parameter");
	end
	
	-- Get the general info about the area
	local AreaID = a_Split[2];
	local WorldName = a_Player:GetWorld():GetName();
	local MinX, MinZ, MaxX, MaxZ, CreatorName = g_Storage:GetArea(AreaID, WorldName);
	if (MinX == nil) then
		a_Player:SendMessage("No such area: " .. AreaID);
		return true;
	end
	
	-- Send the header
	a_Player:SendMessage(
		"Area ID " .. AreaID .. ": {" .. 
		MinX .. ", " .. MinZ .. "} - {" .. 
		MaxX .. ", " .. MaxZ .. "} " ..
		"Created by " .. CreatorName .. "; allowed users:"
	);
	
	-- List and count the allowed users
	local NumUsers = 0;
	g_Storage:ForEachUserInArea(AreaID, WorldName, 
		function(UserName)
			a_Player:SendMessage("  " .. UserName);
			NumUsers = NumUsers + 1;
		end
	);
	
	-- Send the footer
	a_Player:SendMessage("End of area " .. AreaID .. " user list, total " .. NumUsers .. " users");
	
	return true;
end





function HandleRemoveUser(a_Split, a_Player)
	-- Command syntax: ProtRemUser AreaID UserName
	if (#a_Split ~= 3) then
		a_Player:SendMessage("Parameter mismatch. Expected <AreaID> <UserName>.");
		return true;
	end
	
	-- Parse the AreaID
	local AreaID = tonumber(a_Split[2]);
	if (AreaID == nil) then
		a_Player:SendMessage("Cannot parse <AreaID>.");
		return true;
	end
	
	-- Remove the user from the DB
	local UserName = a_Split[3];
	g_Storage:RemoveUser(AreaID, UserName, a_Player:GetWorld():GetName());
	
	-- Send confirmation
	a_Player:SendMessage("Removed " .. UserName .. " from area " .. AreaID);
	
	return true;
end





function HandleRemoveUserAll(a_Split, a_Player)
	-- Command syntax: ProtRemUserAll UserName
	if (#a_Split ~= 2) then
		a_Player:SendMessage("Parameter mismatch. Expected <UserName>.");
		return true;
	end
	
	-- Remove the user from the DB
	g_Storage:RemoveUserAll(a_Split[2], a_Player:GetWorld():GetName());
	return true;
end





