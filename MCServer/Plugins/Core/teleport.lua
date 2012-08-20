function HandleTPCommand( Split, Player )
	if( #Split ~= 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /tp [PlayerName]" )
		return true
	end
	
	World = Player:GetWorld()
	
	local TeleportDestination = function(OtherPlayer)
		if( OtherPlayer == Player ) then
			Player:SendMessage( cChatColor.Green .. "Already there :)" )
		else
			Player:TeleportToEntity( OtherPlayer )
			Player:SendMessage( cChatColor.Green .. "You teleported to "..OtherPlayer:GetName().."!" )
			OtherPlayer:SendMessage( cChatColor.Green .. Player:GetName().." teleported to you!" )
		end
    end
	
    if (not(World:DoWithPlayer(Split[2], TeleportDestination))) then
		Player:SendMessage( cChatColor.Green .. "Can't find player " .. Split[2] )
    end
	return true
end