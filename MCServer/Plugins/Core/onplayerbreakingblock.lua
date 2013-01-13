function OnPlayerBreakingBlock(Player, BlockX, BlockY, BlockZ, BlockFace, Status, OldBlockType, OldBlockMeta)
	-- dont check if the direction is in the air
	if (BlockFace ~= -1) then

		if (Player:HasPermission("core.build") == false) then
			return true
		end
	end
	return false
end