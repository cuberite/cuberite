
-- plugin.lua

-- Implements the main entrypoint for the plugin, as well as all the handling needed

-- ChatLog plugin logs all chat messages into the server log





function Initialize(Plugin)
	Plugin:SetName("ChatLog")
	Plugin:SetVersion(3)

	PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_CHAT)

	LOG("Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion())
	return true
end





function OnChat(Player, Message)
	-- Lets get loggin'
	LOGINFO("[" .. Player:GetName() .. "]: " .. Message);

	return false
end