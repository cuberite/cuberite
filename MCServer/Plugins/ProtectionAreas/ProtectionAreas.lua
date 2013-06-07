
-- ProtectionAreas.lua
-- Defines the main plugin entrypoint, as well as some utility functions





--- Prefix for all messages logged to the server console
PluginPrefix = "ProtectionAreas: ";





--- Called by MCS when the plugin loads
-- Returns true if initialization successful, false otherwise
function Initialize(a_Plugin)
	a_Plugin:SetName("ProtectionAreas");
	a_Plugin:SetVersion(1);
	
	if (not(InitializeStorage())) then
		LOGWARNING(PluginPrefix .. "failed to initialize Storage, plugin is disabled");
		return false;
	end
	InitializeHooks(a_Plugin);
	InitializeCommandHandlers();
	
	-- We might be reloading, so there may be players already present in the server; reload all of them
	local function ReloadPlayers(a_World)
		ReloadAllPlayersInWorld(a_World:GetName());
	end
	cRoot:Get():ForEachWorld(ReloadPlayers);
	
	return true;
end





--- Loads a cPlayerAreas object from the DB for the player, and assigns it to the player map
function LoadPlayerAreas(a_Player)
	local PlayerID = a_Player:GetUniqueID();
	local PlayerX = math.floor(a_Player:GetPosX());
	local PlayerZ = math.floor(a_Player:GetPosZ());
	local WorldName = a_Player:GetWorld():GetName();
	g_PlayerAreas[PlayerID] = g_Storage:LoadPlayerAreas(a_Player:GetName(), PlayerX, PlayerZ, WorldName);
end





function ReloadAllPlayersInWorld(a_WorldName)
	local World = cRoot:Get():GetWorld(a_WorldName);
	World:ForEachPlayer(LoadPlayerAreas);
end





