function OnBlockPlace( Block, Player )

	-- dont check if the direction is in the air
	if Block.m_Direction == -1 then
		return false
	end

	if (Block.m_ItemType ~= 280) then -- not a Stick of Ticking
		return false
	end

	LOG("Setting next block tick to {" .. Block.m_PosX .. ", " .. Block.m_PosY .. ", " .. Block.m_PosZ .. "}")

	Player:GetWorld():SetNextBlockTick(Block.m_PosX, Block.m_PosY, Block.m_PosZ);

	return true

end