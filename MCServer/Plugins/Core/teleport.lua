function HandleTPCommand( Split, Player )
	if( Split[2] == nil )  then
		Player:SendMessage( cChatColor.Green .. "Usage: /tp [PlayerName] (-h)" )
		return true
	end
	
	local TeleportDestination = function(OtherPlayer)
		if( OtherPlayer == Player ) then
			Player:SendMessage( cChatColor.Green .. "Already there :)" )
		else
			X[Player:GetName()] = Player:GetPosX()
			Y[Player:GetName()] = Player:GetPosY()
			Z[Player:GetName()] = Player:GetPosZ()
			Player:TeleportToEntity( OtherPlayer )
			Player:SendMessage( cChatColor.Green .. "You teleported to "..OtherPlayer:GetName().."!" )
			if Split[3] ~= "-h" then
				OtherPlayer:SendMessage( cChatColor.Green .. Player:GetName().." teleported to you!" )
			end
		end
	end
	World = Player:GetWorld()
	if (not(World:DoWithPlayer(Split[2], TeleportDestination))) then
		Player:SendMessage( cChatColor.Green .. "Can't find player " .. Split[2] )
	end
	return true
end
