function HandleBackCommand( Split, Player )
	if BackCoords[Player:GetName()] == nil then
		SendMessageFailure(Player, "No known last position")
		return true
	else
		Player:TeleportToCoords(BackCoords[Player:GetName()].x, BackCoords[Player:GetName()].y, BackCoords[Player:GetName()].z)
		SendMessageSuccess(Player, "Teleported back to your last known position")
	end
	return true
end
