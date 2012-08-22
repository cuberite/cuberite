function HandleKickCommand( Split, Player )
	if( #Split < 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /kick [Player] <Reason>" )
		return true
	end
	
	local FoundPlayerCallback = function( OtherPlayer )
		local Reason = "You have been kicked"
		if( #Split > 2 ) then
			Reason = table.concat(Split, " ", 3)
		end

		local Server = cRoot:Get():GetServer()
		LOGINFO( Player:GetName() .. " is kicking " .. OtherPlayer:GetName() .. " ( "..Reason..") " )
		Server:SendMessage( "Kicking " .. OtherPlayer:GetName() )

		local ClientHandle = OtherPlayer:GetClientHandle()
		ClientHandle:Kick( Reason )
	end

	if( cRoot:Get():FindAndDoWithPlayer( Split[2], FoundPlayerCallback ) == false ) then
		Player:SendMessage( cChatColor.Green .. "Could not find player " .. Split[2] )
		return true
	end



	return true
end