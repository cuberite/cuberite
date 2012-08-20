function HandleBanCommand( Split, Player )
	if( #Split < 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /ban [Player] <Reason>" )
		return true
	end

	local World = Player:GetWorld()
	local OtherPlayer = World:GetPlayer( Split[2] )
	if( OtherPlayer == nil ) then
		Player:SendMessage( cChatColor.Green .. "Could not find player " .. Split[2] )
		return true
	end

	local Reason = "You have been banned"
	if( #Split > 2 ) then
		Reason = table.concat(Split, " ", 3)
	end

	local Server = cRoot:Get():GetServer()
	LOGINFO( Player:GetName() .. " is banning " .. OtherPlayer:GetName() .. " ( "..Reason..") " )
	Server:SendMessage( "Banning " .. OtherPlayer:GetName() )

	local ClientHandle = OtherPlayer:GetClientHandle()
	ClientHandle:Kick( Reason )
	
	BannedPlayersIni:SetValueB("Banned", OtherPlayer:GetName(), true)
	BannedPlayersIni:WriteFile()

	return true
end