function HandleKickCommand( Split, Player )

	if( #Split < 2 ) then
		SendMessage( Player, "Usage: /kick [Player] <Reason>" )
		return true
	end

	local Reason = "You have been kicked"
	if ( #Split > 2 ) then
		Reason = table.concat( Split, " ", 3 )
	end

	if( KickPlayer( Split[2], Reason ) == false ) then
		SendMessageFailure( Player, "Could not find player " .. Split[2] )
	end

	return true

end
