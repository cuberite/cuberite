function HandleBackCommand( Split, Player )
	if (X[Player:GetName()] == nil) or (Y[Player:GetName()] == nil) or (Z[Player:GetName()] == nil) then
		Player:SendMessage(cChatColor.Green .. "There is no last position known")
	else
		Player:TeleportToCoords(X[Player:GetName()], Y[Player:GetName()], Z[Player:GetName()])
		Player:SendMessage(cChatColor.Green .. "You teleported back to your last known position")
	end
	return true
end