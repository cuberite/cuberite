function HandleBackCommand( Split, Player )
	if BackCoords[Player:GetName()] == nil then
		Player:SendMessage(cChatColor.Rose .. "[INFO] " .. cChatColor.White .. "No known last position")
	else
		Player:TeleportToCoords(BackCoords[Player:GetName()].x, BackCoords[Player:GetName()].y, BackCoords[Player:GetName()].z)
		Player:SendMessage(cChatColor.Green .. "[INFO] " .. cChatColor.White .. "Teleported back to your last known position")
	end
	return true
end