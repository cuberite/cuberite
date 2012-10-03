
-- Global variables
PLUGIN = {}	-- Reference to own plugin object





function Initialize(Plugin)
	PLUGIN = Plugin
	
	Plugin:SetName("sTick")
	Plugin:SetVersion(2)
	
	PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:AddHook(Plugin, cPluginManager.E_PLUGIN_BLOCK_PLACE)
	
	LOG("Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion())
	return true
end