function HandleUnbanCommand( Split, Player )
	if( #Split < 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /unban [Player]" )
		return true
	end
	
	if( BannedPlayersIni:GetValueB("Banned", Split[2], false) == false ) then
		Player:SendMessage( cChatColor.Green .. Split[2] .. " is not banned!" )
		return true
	end
	
	BannedPlayersIni:SetValueB("Banned", Split[2], false, false)
	BannedPlayersIni:WriteFile()

	local Server = cRoot:Get():GetServer()
	LOGINFO( Player:GetName() .. " is unbanning " .. Split[2] )
	Server:SendMessage( "Unbanning " .. Split[2] )

	return true
end