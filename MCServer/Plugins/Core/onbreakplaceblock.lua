function OnPlayerPlacingBlock(Player, BlockX, BlockY, BlockZ, BlockFace, CursorX, CursorY, CursorZ, BlockType)
	-- Direction is air check
	if (BlockFace == -1) then
		return false
	end

	if( Player:HasPermission("core.build") == false ) then
		return true
	else
        if Player:HasPermission("core.spawnprotect.bypass") == false and SPAWNPROTECT == true then
    	    local World = Player:GetWorld()
            local xcoord = World:GetSpawnX()
            local ycoord = World:GetSpawnY()
	        local zcoord = World:GetSpawnZ()

	        if not ((BlockX <= (xcoord + PROTECTRADIUS)) and (BlockX >= (xcoord - PROTECTRADIUS))) then
		        return false -- Not in spawn area.
	        end
	        if not ((BlockY <= (ycoord + PROTECTRADIUS)) and (BlockY >= (ycoord - PROTECTRADIUS))) then 
                return false -- Not in spawn area.
            end
	        if not ((BlockZ <= (zcoord + PROTECTRADIUS)) and (BlockZ >= (zcoord - PROTECTRADIUS))) then 
                return false -- Not in spawn area.
            end
		
	        --WriteLog(1, BlockX, BlockY, BlockZ, Player:GetName(), id, meta)

	        WarnPlayer(Player)

	        return true
        else
            if BlockType == "50" or BlockType == "76" then
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
            end
	    end
    end
    return false
end

function OnPlayerBreakingBlock(Player, BlockX, BlockY, BlockZ, BlockFace, Status, OldBlockType, OldBlockMeta)
    -- dont check if the direction is in the air
	if (BlockFace ~= -1) then

		if (Player:HasPermission("core.build") == false) then
			return true
		else
            if Player:HasPermission("core.spawnprotect.bypass") == false and SPAWNPROTECT == true then
    	        local World = Player:GetWorld()
	            local xcoord = World:GetSpawnX()
	            local ycoord = World:GetSpawnY()
	            local zcoord = World:GetSpawnZ()
    
        	    if not ((BlockX <= (xcoord + PROTECTRADIUS)) and (BlockX >= (xcoord - PROTECTRADIUS))) then
            		return false -- Not in spawn area.
	            end
	            if not ((BlockY <= (ycoord + PROTECTRADIUS)) and (BlockY >= (ycoord - PROTECTRADIUS))) then 
                    return false -- Not in spawn area.
                end
	            if not ((BlockZ <= (zcoord + PROTECTRADIUS)) and (BlockZ >= (zcoord - PROTECTRADIUS))) then 
                    return false -- Not in spawn area.
                end
    		
            	--WriteLog(0, BlockX, BlockY, BlockZ, Player:GetName(), id, meta)
                
	            WarnPlayer(Player)

            	return true
	        end
        end
    end

	return false
end