
#pragma once





namespace RedstoneTorchHandler
{
	static bool IsOn(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::RedstoneTorch:     return Block::RedstoneTorch::Lit(a_Block);
			case BlockType::RedstoneWallTorch: return Block::RedstoneWallTorch::Lit(a_Block);
			default: return false;
		}
	}

	static Vector3i GetOffsetAttachedTo(const BlockState a_Block)
	{
		eBlockFace Face;
		switch (a_Block.Type())
		{
			case BlockType::RedstoneTorch:     Face = eBlockFace::BLOCK_FACE_TOP; break;
			case BlockType::RedstoneWallTorch: Face = Block::RedstoneWallTorch::Facing(a_Block); break;
			default: return Vector3i();
		}
		switch (Face)
		{
			case eBlockFace::BLOCK_FACE_TOP: return { 0, -1, 0 };
			case eBlockFace::BLOCK_FACE_EAST: return { -1, 0, 0 };
			case eBlockFace::BLOCK_FACE_WEST: return { 1, 0, 0 };
			case eBlockFace::BLOCK_FACE_NORTH: return { 0, 0, 1 };
			case eBlockFace::BLOCK_FACE_SOUTH : return { 0, 0, -1 };
			default:
			{
				ASSERT(!"Unhandled torch metadata");
				return { 0, 0, 0 };
			}
		}
	}

	static PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, Vector3i a_QueryPosition, BlockState a_QueryBlock, bool a_IsLinked)
	{
		const auto QueryOffset = a_QueryPosition - a_Position;

		if (
			!IsOn(a_Block) ||
			(QueryOffset == GetOffsetAttachedTo(a_Block)) ||
			(a_IsLinked && (QueryOffset != OffsetYP))
		)
		{
			return 0;
		}

		return 15;
	}

	static void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BlockState a_Block, const PowerLevel Power)
	{
		LOGREDSTONE("Evaluating torchy the redstone torch (%i %i %i)", a_Position.x, a_Position.y, a_Position.z);

		auto & Data = DataForChunk(a_Chunk);
		auto DelayInfo = Data.GetMechanismDelayInfo(a_Position);

		if (DelayInfo == nullptr)
		{
			const bool ShouldBeOn = (Power == 0);
			if (ShouldBeOn != IsOn(a_Block))
			{
				Data.m_MechanismDelays[a_Position] = std::make_pair(1, ShouldBeOn);
			}

			return;
		}

		int DelayTicks;
		bool ShouldPowerOn;
		std::tie(DelayTicks, ShouldPowerOn) = *DelayInfo;

		if (DelayTicks != 0)
		{
			return;
		}

		BlockState NewBlock = a_Block;

		// Determine the new block
		switch (a_Block.Type())
		{
			case BlockType::RedstoneTorch:     NewBlock = Block::RedstoneTorch::RedstoneTorch(ShouldPowerOn); break;
			case BlockType::RedstoneWallTorch: NewBlock = Block::RedstoneWallTorch::RedstoneWallTorch(Block::RedstoneWallTorch::Facing(a_Block), ShouldPowerOn); break;
			default: break;
		}

		a_Chunk.FastSetBlock(a_Position, NewBlock);
		Data.m_MechanismDelays.erase(a_Position);

		for (const auto & Adjacent : RelativeAdjacents)
		{
			// Update all adjacents (including linked power positions)
			// apart from our attachment, which can't possibly need an update:
			if (Adjacent != GetOffsetAttachedTo(a_Block))
			{
				UpdateAdjustedRelative(a_Chunk, CurrentlyTicking, a_Position, Adjacent);
			}
		}
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Block);
		Callback(a_Position + GetOffsetAttachedTo(a_Block));
	}
};
