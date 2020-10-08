
#pragma once





namespace RedstoneTorchHandler
{
	inline bool IsOn(BLOCKTYPE a_Block)
	{
		return (a_Block == E_BLOCK_REDSTONE_TORCH_ON);
	}

	inline Vector3i GetOffsetAttachedTo(const NIBBLETYPE a_Meta)
	{
		switch (a_Meta)
		{
			case E_META_TORCH_FLOOR: return { 0, -1, 0 };
			case E_META_TORCH_EAST: return { -1, 0, 0 };
			case E_META_TORCH_WEST: return { 1, 0, 0 };
			case E_META_TORCH_NORTH: return { 0, 0, 1 };
			case E_META_TORCH_SOUTH: return { 0, 0, -1 };
			default:
			{
				ASSERT(!"Unhandled torch metadata");
				return { 0, 0, 0 };
			}
		}
	}

	inline PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		const auto QueryOffset = a_QueryPosition - a_Position;

		if (
			!IsOn(a_BlockType) ||
			(QueryOffset == GetOffsetAttachedTo(a_Chunk.GetMeta(a_Position))) ||
			(IsLinked && (QueryOffset != OffsetYP))
		)
		{
			return 0;
		}

		return 15;
	}

	inline void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const PowerLevel Power)
	{
		// LOGD("Evaluating torchy the redstone torch (%i %i %i)", a_Position.x, a_Position.y, a_Position.z);

		auto & Data = DataForChunk(a_Chunk);
		auto DelayInfo = Data.GetMechanismDelayInfo(a_Position);

		if (DelayInfo == nullptr)
		{
			const bool ShouldBeOn = (Power == 0);
			if (ShouldBeOn != IsOn(a_BlockType))
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

		a_Chunk.FastSetBlock(a_Position, ShouldPowerOn ? E_BLOCK_REDSTONE_TORCH_ON : E_BLOCK_REDSTONE_TORCH_OFF, a_Meta);
		Data.m_MechanismDelays.erase(a_Position);

		for (const auto & Adjacent : RelativeAdjacents)
		{
			// Update all adjacents (including linked power positions)
			// apart from our attachment, which can't possibly need an update:
			if (Adjacent != GetOffsetAttachedTo(a_Meta))
			{
				UpdateAdjustedRelative(a_Chunk, CurrentlyTicking, a_Position, Adjacent);
			}
		}
	}

	inline void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);
		Callback(a_Position + GetOffsetAttachedTo(a_Meta));
	}
};
