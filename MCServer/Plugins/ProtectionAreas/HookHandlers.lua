
-- HookHandlers.lua
-- Implements the handlers for individual hooks





function InitializeHooks(a_Plugin)
	local PlgMgr = cRoot:Get():GetPluginManager();
	PlgMgr:AddHook(a_Plugin, cPluginManager.HOOK_DISCONNECT);
	PlgMgr:AddHook(a_Plugin, cPluginManager.HOOK_PLAYER_JOINED);
	PlgMgr:AddHook(a_Plugin, cPluginManager.HOOK_PLAYER_LEFT_CLICK);
	PlgMgr:AddHook(a_Plugin, cPluginManager.HOOK_PLAYER_RIGHT_CLICK);
end





function OnDisconnect(a_Player, a_Reason)
	-- Remove the player's cProtectionArea object
	-- TODO: What if there are two players with the same name? need to check
	g_PlayerAreas[a_Player:GetUniqueID()] = nil;
	
	-- If the player is a VIP, they had a command state, remove that as well
	g_CommandStates[a_Player:GetUniqueID()] = nil;
	
	return false;
end;





function OnPlayerJoined(a_Player)
	-- Create a new cPlayerAreas object for this player
	local PlayerName = a_Player:GetName();
	local PlayerID = a_Player:GetUniqueID();
	if (g_PlayerAreas[PlayerID] == nil) then
		g_PlayerAreas[PlayerID] = g_Storage:LoadPlayerAreas(PlayerName);
	end;

	return false;
end





function OnPlayerLeftClick(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_Status)
	-- If the player has lclked with the wand; regardless of their permissions, let's set the coords:
	if (cConfig:IsWand(a_Player:GetEquippedItem())) then
		-- BlockFace < 0 means "use item", for which the coords are not given by the client
		if (a_BlockFace < 0) then
			return true;
		end

		-- Convert the clicked coords into the block space
		a_BlockX, a_BlockY, a_BlockZ = AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);

		-- Set the coords in the CommandState
		GetCommandStateForPlayer(a_Player):SetCoords1(a_BlockX, a_BlockY, a_BlockZ);
		a_Player:SendMessage("Coords1 set as {" .. a_BlockX .. ", " .. a_BlockY .. ", " .. a_BlockZ .."}.");
		return true;
	end;
	
	-- Check the player areas to see whether to disable this action
	local Areas = g_PlayerAreas[a_Player:GetUniqueID()];
	if not(Areas:CanInteractWithBlock(a_BlockX, a_BlockY, a_BlockZ)) then
		return true;
	end
	
	-- Allow interaction
	return false;
end





function OnPlayerRightClick(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, a_Status)
	-- If the player has rclked with the wand; regardless of their permissions, let's set the coords
	if (cConfig:IsWand(a_Player:GetEquippedItem())) then
		-- BlockFace < 0 means "use item", for which the coords are not given by the client
		if (a_BlockFace < 0) then
			return true;
		end

		-- Convert the clicked coords into the block space
		a_BlockX, a_BlockY, a_BlockZ = AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
		
		-- Set the coords in the CommandState
		GetCommandStateForPlayer(a_Player):SetCoords2(a_BlockX, a_BlockY, a_BlockZ);
		a_Player:SendMessage("Coords2 set as {" .. a_BlockX .. ", " .. a_BlockY .. ", " .. a_BlockZ .."}.");
		return true;
	end;
	
	-- Check the player areas to see whether to disable this action
	local Areas = g_PlayerAreas[a_Player:GetUniqueID()];
	if not(Areas:CanInteractWithBlock(a_BlockX, a_BlockY, a_BlockZ)) then
		return true;
	end

	-- Allow interaction
	return false;
end




