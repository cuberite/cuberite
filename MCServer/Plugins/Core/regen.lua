function HandleRegenCommand(Split, Player)

	if #Split == 2 or #Split > 3 then
		SendMessage( Player, "Usage: '/regeneratechunk' or '/regeneratechunk [X] [Z]'" )
		return true
	end

	local X = Player:GetChunkX()
	local Z = Player:GetChunkZ()

	if #Split == 3 then
		X = Split[2]
		Z = Split[3]
	end

	SendMessageSuccess( Player, "Regenerating chunk ["..X..", "..Z.."]")
	Player:GetWorld():RegenerateChunk(X, Z)
	return true

end
