LOGINFO("main.lua!!")
LOOLOL = "12345"
PLUGIN = {}

function Initialize( Plugin )
	LOGINFO("Initialize in main.lua")
	PLUGIN = Plugin
	
	Plugin:SetName("LOLOL new plugin!")
	
	PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:AddHook( Plugin, cPluginManager.E_PLUGIN_TICK )
	
	LOGINFO("Plugin name is: " .. Plugin:GetName() )
	
	return true
end