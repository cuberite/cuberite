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

--- Kicks a player by name, with the specified reason; returns bool whether found and player's real name
function KickPlayer( PlayerName, Reason )

	local RealName = ""
	if (Reason == nil) then
		Reason = "You have been kicked"
	end

	local FoundPlayerCallback = function( a_Player )
		RealName = a_Player:GetName()

		local Server = cRoot:Get():GetServer()
		LOGINFO( "'" .. RealName .. "' is being kicked for ( "..Reason..") " )
		Server:SendMessage("Kicking " .. RealName)

		a_Player:GetClientHandle():Kick(Reason)
	end

	if not cRoot:Get():FindAndDoWithPlayer( PlayerName, FoundPlayerCallback ) then
		-- Could not find player
		return false
	end

	return true, RealName  -- Player has been kicked

end
