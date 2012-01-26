function HandleTopCommand( Split, Player )
	local World = Player:GetWorld()
	
	local PlayerPos = Player:GetPosition()
	local Height = World:GetHeight( math.floor(PlayerPos.x), math.floor(PlayerPos.z) )
	
	Player:TeleportTo( PlayerPos.x, Height+1, PlayerPos.z )
	Player:SendMessage("Teleported to the top block")
	
	return true
end