function HandleToggleDownfallCommand( Split, Player )
	World = Player:GetWorld()
	if World:GetWeather() == 0 then
		World:SetWeather(1)
	else
		World:SetWeather(0)
	end
	Player:SendMessage( cChatColor.Green .. "Weather toggled")
	return true
end