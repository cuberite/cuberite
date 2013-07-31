function HandleDownfallCommand( Split, Player )
	World = Player:GetWorld()
	if World:GetWeather() == 0 then
		World:SetWeather(1)
	else
		World:SetWeather(0)
	end
	Player:SendMessage(cChatColor.Green .. "[INFO] " .. cChatColor.White .. "Downfall toggled")
	return true
end