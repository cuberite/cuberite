function OnLogin( PacketData )
	if( PacketData.m_Username ~= "" ) then
		if( BannedPlayersIni:GetValueB("Banned", PacketData.m_Username, false) == true ) then
			local Server = cRoot:Get():GetServer()
			Server:SendMessage( PacketData.m_Username .. " tried to join, but is banned!" )
			LOGINFO( PacketData.m_Username .. " tried to join, but is banned!")
			return true	-- Player is banned, return true to deny access
		end
		if( WhiteListIni:GetValueB("WhiteListSettings", "WhiteListOn", false ) == true ) then
			if( WhiteListIni:GetValueB("WhiteList", PacketData.m_Username, false ) == false ) then -- not on whitelist
				local Server = cRoot:Get():GetServer()
				Server:SendMessage( PacketData.m_Username .. " tried to join, but is not on the whitelist." )
				LOGINFO( PacketData.m_Username .. " tried to join, but is not on the whitelist." )
				return true -- Deny access to the server
			end
		end
	end
	
	return false
end