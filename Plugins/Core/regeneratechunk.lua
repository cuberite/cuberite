function HandleRegenerateChunkCommand( Split, Player )
	if( (#Split == 2) or (#Split > 3) ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /regeneratechunk <X [Z]>" )
		return true
	end
	
	local X = Player:GetChunkX()
	local Z = Player:GetChunkZ()
	
	if( #Split == 3 ) then
		X = Split[2]
		Z = Split[3]
	end
	
	Player:SendMessage(cChatColor.Green .. "Regenerating chunk ["..X..", "..Z.."]")
	Player:GetWorld():RegenerateChunk(X, Z)
	return true
end