function HandleKickCommand( Split, Player )
	if( #Split < 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /kick [Player] <Reason>" )
		return true
	end
	
	local Reason = "You have been kicked"
	if( #Split > 2 ) then
		Reason = table.concat(Split, " ", 3)
	end
	
	if( KickPlayer( Split[2], Reason ) == false ) then
		Player:SendMessage( cChatColor.Green .. "Could not find player " .. Split[2] )
	end

	return true
end


function KickPlayer( PlayerName, Reason )
	local RealName = ""
	local FoundPlayerCallback = function( OtherPlayer )
		if( Reason == nil ) then
			Reason = "You have been kicked"
		end
		
		RealName = OtherPlayer:GetName()

		local Server = cRoot:Get():GetServer()
		LOGINFO( "'" .. RealName .. "' is being kicked for ( "..Reason..") " )
		Server:SendMessage( "Kicking " .. RealName )

		local ClientHandle = OtherPlayer:GetClientHandle()
		ClientHandle:Kick( Reason )
	end

	if( cRoot:Get():FindAndDoWithPlayer( PlayerName, FoundPlayerCallback ) == false ) then
		return false -- could not find player
	end
	
	return true, RealName -- player should be kicked now
end