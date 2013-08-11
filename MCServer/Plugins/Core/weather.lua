function HandleWeatherCommand(Split, Player)

	if #Split ~= 2 then
		SendMessage( Player, "Usage: /weather [clear/rain/thunder]" )
		return true
	end

	if (Split[2] == "clear") then
		Player:GetWorld():SetWeather(0)
		SendMessageSuccess( Player, "Downfall stopped" )
	elseif (Split[2] == "rain") then
		Player:GetWorld():SetWeather(1)
		SendMessageSuccess( Player, "Let it rain!" )
	elseif (Split[2] == "thunder") then
		Player:GetWorld():SetWeather(2)
		SendMessageSuccess( Player, "Thundery showers activate!")
	end
	
	return true

end
