function HandleCoordsCommand( Split, Player )
	Player:SendMessage(cChatColor.Green .. string.format("[X:%0.2f] [Y:%0.2f] [Z:%0.2f]", Player:GetPosX(), Player:GetPosY(), Player:GetPosZ() ) )
	return true
end