function HandleKickCommand( Split, Player )
	if( #Split < 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /kick [Player] <Reason>" )
		return true
	end

	local World = Player:GetWorld()
	local OtherPlayer = World:GetPlayer( Split[2] )
	if( OtherPlayer == nil ) then
		Player:SendMessage( cChatColor.Green .. "Could not find player " .. Split[2] )
		return true
	end

	local Reason = "You have been kicked"
	if( #Split > 2 ) then
		Reason = table.concat(Split, " ", 3)
	end

	local Server = cRoot:Get():GetServer()
	LOGINFO( Player:GetName() .. " is kicking " .. OtherPlayer:GetName() .. " ( "..Reason..") " )
	Server:SendMessage( "Kicking " .. OtherPlayer:GetName() )

	local ClientHandle = OtherPlayer:GetClientHandle()
	ClientHandle:Kick( Reason )

	return true
end