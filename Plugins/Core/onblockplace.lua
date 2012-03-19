function OnBlockPlace( Block, Player )

	-- dont check if the direction is in the air
	if Block.m_Direction ~= -1 then

		if( Player:HasPermission("core.build") == false ) then
			return true
		end
	
		local X = Block.m_PosX
		local Y = Block.m_PosY
		local Z = Block.m_PosZ
		X, Y, Z = AddDirection( X, Y, Z, Block.m_Direction )
		if( Y >= 256 or Y < 0 ) then
			return true
		end
		
		local CheckCollision = function( Player )
			-- drop the decimals, we only care about the full block X,Y,Z
			local PlayerX = math.floor(Player:GetPosX(), 0)
			local PlayerY = math.floor(Player:GetPosY(), 0)
			local PlayerZ = math.floor(Player:GetPosZ(), 0)

			local BlockX = Block.m_PosX
			local BlockY = Block.m_PosY
			local BlockZ = Block.m_PosZ

			-- player height is 2 blocks, so we check the position and then offset it up one
			-- so they can't place a block on there face

			local collision = false
			if Block.m_Direction == 0 then if PlayerY == BlockY-2 and PlayerX == BlockX and PlayerZ == BlockZ then collision = true end end
			if Block.m_Direction == 1 then if PlayerY == BlockY+1 and PlayerX == BlockX and PlayerZ == BlockZ then collision = true end end

			if Block.m_Direction == 2 then if PlayerY == BlockY and PlayerX == BlockX and PlayerZ == BlockZ-1 then collision = true end end
			if Block.m_Direction == 2 then if PlayerY+1 == BlockY and PlayerX == BlockX and PlayerZ == BlockZ-1 then collision = true end end

			if Block.m_Direction == 3 then if PlayerY == BlockY and PlayerX == BlockX and PlayerZ == BlockZ+1 then collision = true end end
			if Block.m_Direction == 3 then if PlayerY+1 == BlockY and PlayerX == BlockX and PlayerZ == BlockZ+1 then collision = true end end

			if Block.m_Direction == 4 then if PlayerY == BlockY and PlayerX == BlockX-1 and PlayerZ == BlockZ then collision = true end end
			if Block.m_Direction == 4 then if PlayerY+1 == BlockY and PlayerX == BlockX-1 and PlayerZ == BlockZ then collision = true end end

			if Block.m_Direction == 5 then if PlayerY == BlockY and PlayerX == BlockX+1 and PlayerZ == BlockZ then collision = true end end
			if Block.m_Direction == 5 then if PlayerY+1 == BlockY and PlayerX == BlockX+1 and PlayerZ == BlockZ then collision = true end end
			
			return collision
		end
		
		if( Player:GetWorld():ForEachPlayer( CheckCollision ) == false ) then
			return true
		else
			return false
		end

	end

	return false

end