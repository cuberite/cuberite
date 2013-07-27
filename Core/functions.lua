function SetBackCoordinates( Player )
	BackCoords[Player:GetName()] = Vector3i( Player:GetPosX(), Player:GetPosY(), Player:GetPosZ() )
end