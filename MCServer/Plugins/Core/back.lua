function HandleBackCommand( Split, Player )
	if BackCoords[Player:GetName()] == nil then
		Player:SendMessage(cChatColor.Green .. "There is no last position known")
	else
		Player:TeleportToCoords(BackCoords[Player:GetName()].x, BackCoords[Player:GetName()].y, BackCoords[Player:GetName()].z)
		Player:SendMessage(cChatColor.Green .. "You teleported back to your last known position")
	end
	return true
end