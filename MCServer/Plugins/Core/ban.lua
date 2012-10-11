function HandleBanCommand( Split, Player )
	if( #Split < 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /ban [Player] <Reason>" )
		return true
	end

	local Reason = "You have been banned"
	if( #Split > 2 ) then
		Reason = table.concat(Split, " ", 3)
	end
	
	
	if( BanPlayer(Split[2], Reason) == false ) then
		Player:SendMessage( cChatColor.Green .. "Could not find player " .. Split[2] )
		return true
	end

	return true
end

function BanPlayer( PlayerName, Reason )
	if( Reason == nil ) then
		Reason = "You have been banned"
	end
		
	local Success, RealName = KickPlayer( PlayerName, Reason )
	if( Success == false ) then
		return false
	end
	
	LOGINFO( "'" .. RealName .. "' is being banned for ( "..Reason..") " )
	
	local Server = cRoot:Get():GetServer()
	Server:SendMessage( "Banning " .. RealName )
	
	BannedPlayersIni:SetValueB("Banned", RealName, true)
	BannedPlayersIni:WriteFile()
	
	return true
end