function OnPlayerPlacingBlock(Player, BlockX, BlockY, BlockZ, BlockFace, CursorX, CursorY, CursorZ, BlockType)

	-- dont check if the direction is in the air
	if (BlockFace == -1) then
		return false
	end

	if( Player:HasPermission("core.build") == false ) then
		return true
	end

	-- TODO: If the placed block is not a block (torch etc.), allow it without checking for collisions
	
	local X = BlockX
	local Y = BlockY
	local Z = BlockZ
	X, Y, Z = AddFaceDirection(X, Y, Z, BlockFace)
	if (Y >= 256 or Y < 0) then
		return true
	end
	
	local CheckCollision = function(Player)
		-- drop the decimals, we only care about the full block X,Y,Z
		local PlayerX = math.floor(Player:GetPosX(), 0)
		local PlayerY = math.floor(Player:GetPosY(), 0)
		local PlayerZ = math.floor(Player:GetPosZ(), 0)

		-- player height is 2 blocks, so we check the position and then offset it up one
		-- so they can't place a block in anyone's face

		local collision = false
		if ((BlockFace == BLOCK_FACE_TOP) and (PlayerY == BlockY - 2) and (PlayerX == BlockX) and (PlayerZ == BlockZ)) then
			collision = true
		end

		if ((BlockFace == BLOCK_FACE_BOTTOM) and (PlayerY == BlockY + 1) and (PlayerX == BlockX) and (PlayerZ == BlockZ)) then
			collision = true
		end

		if ((BlockFace == BLOCK_FACE_NORTH) and (PlayerX == BlockX) and (PlayerZ == BlockZ - 1)) then
			if ((PlayerY == BlockY) or (PlayerY + 1 == BlockY)) then collision = true end
		end

		if ((BlockFace == BLOCK_FACE_SOUTH) and (PlayerX == BlockX) and (PlayerZ == BlockZ + 1)) then
			if ((PlayerY == BlockY) or (PlayerY + 1 == BlockY)) then collision = true end
		end

		if ((BlockFace == BLOCK_FACE_WEST) and (PlayerX == BlockX - 1) and (PlayerZ == BlockZ)) then
			if ((PlayerY == BlockY) or (PlayerY + 1 == BlockY)) then collision = true end
		end

		if ((BlockFace == BLOCK_FACE_EAST) and (PlayerX == BlockX + 1) and (PlayerZ == BlockZ)) then
			if ((PlayerY == BlockY) or (PlayerY + 1 == BlockY)) then collision = true end
		end
		
		return collision
	end
	
	if (Player:GetWorld():ForEachPlayer(CheckCollision) == false) then
		return true
	end
	return false
end