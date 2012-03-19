function OnBlockDig( Block, Player )

	-- dont check if the direction is in the air
	if Block.m_Direction ~= -1 then

		if( Player:HasPermission("core.build") == false ) then
			return true
		end
	end
	return false
end