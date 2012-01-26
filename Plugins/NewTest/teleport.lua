function HandleTPCommand( Split, Player )
	if( #Split ~= 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /tp [PlayerName]" )
		return true
	end
	local World = Player:GetWorld()
	local OtherPlayer = World:GetPlayer( Split[2] )
	if( OtherPlayer == nil ) then
		Player:SendMessage( cChatColor.Green .. "Can't find player " .. Split[2] )
	elseif( OtherPlayer == Player ) then
		Player:SendMessage( cChatColor.Green .. "Already there :)" )
	else
		Player:TeleportTo( OtherPlayer )
		Player:SendMessage( cChatColor.Green .. "You teleported to "..OtherPlayer:GetName().."!" )
		OtherPlayer:SendMessage( cChatColor.Green .. Player:GetName().." teleported to you!" )
	end
	return true
end