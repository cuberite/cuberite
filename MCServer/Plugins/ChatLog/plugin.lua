function Initialize( Plugin )
	Plugin:SetName( "ChatLog" )
	Plugin:SetVersion( 2 )

	PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:AddHook( Plugin, cPluginManager.E_PLUGIN_CHAT )

	LOG( "Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion() )
	return true
end

function OnChat( Player, Message )
	-- Lets get loggin'
	LOGINFO("[" .. Player:GetName() .. "]: " .. Message);

	return false
end