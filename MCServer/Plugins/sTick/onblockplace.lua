function OnBlockPlace(Player, BlockX, BlockY, BlockZ, BlockFace, HeldItem)

	-- dont check if the direction is in the air
	if BlockFace == BLOCK_FACE_NONE then
		return false
	end

	if (HeldItem.m_ItemType ~= 280) then
		-- not a Stick of Ticking
		return false
	end

	LOG("Setting next block tick to {" .. BlockX .. ", " .. BlockY .. ", " .. BlockZ .. "}")

	Player:GetWorld():SetNextBlockTick(BlockX, BlockY, BlockZ);

	return true

end