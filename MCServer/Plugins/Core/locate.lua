function HandleLocateCommand( Split, Player )
	Player:SendMessage(cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. string.format("You are at [X:%0.2f Y:%0.2f Z:%0.2f] in world %s", Player:GetPosX(), Player:GetPosY(), Player:GetPosZ(), Player:GetWorld():GetName()) )
	return true
end