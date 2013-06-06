
-- ProtectionAreas.lua
-- Defines the main plugin entrypoint





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
	
	-- TODO: We might be reloading, so there may be players already present in the server
	-- Reload areas for all present players
	
	return true;
end
