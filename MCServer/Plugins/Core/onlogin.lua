function OnLogin(Client, ProtocolVersion, Username)
	if( Username ~= "" ) then
		if( BannedPlayersIni:GetValueB("Banned", Username, false) == true ) then
			local Server = cRoot:Get():GetServer()
			Server:SendMessage( Username .. " tried to join, but is banned!" )
			LOGINFO( Username .. " tried to join, but is banned!")
			return true -- Player is banned, return true to deny access
		end
		if( WhiteListIni:GetValueB("WhiteListSettings", "WhiteListOn", false ) == true ) then
			if( WhiteListIni:GetValueB("WhiteList", Username, false ) == false ) then -- not on whitelist
				local Server = cRoot:Get():GetServer()
				Server:SendMessage( Username .. " tried to join, but is not on the whitelist." )
				LOGINFO( Username .. " tried to join, but is not on the whitelist." )
				return true -- Deny access to the server
			end
		end
	end	
	return false
end