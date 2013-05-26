
-- ProtectionAreas.lua
-- Defines the main plugin entrypoint





function Initialize(a_Plugin)
	a_Plugin:SetName("ProtectionAreas");
	a_Plugin:SetVersion(1);
	
	InitializeHooks(a_Plugin);
	InitializeCommandHandlers();
	
	-- TODO: We might be reloading, so there may be players already present in the server
	-- Reload areas for all present players
	
	return true;
end
