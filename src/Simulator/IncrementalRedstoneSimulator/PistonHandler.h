
#pragma once

#include "RedstoneHandler.h"
#include "../../Blocks/BlockPiston.h"





class cPistonHandler final: public cRedstoneHandler
{
	virtual unsigned char GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		UNUSED(IsLinked);
		return 0;
	}

	virtual void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// LOGD("Evaluating pisty the piston (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		const bool ShouldBeExtended = a_PoweringData.PowerLevel != 0;
		if (ShouldBeExtended == cBlockPistonHandler::IsExtended(a_Meta))
		{
			return;
		}

		a_Position = cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos());

		if (ShouldBeExtended)
		{
			cBlockPistonHandler::ExtendPiston(a_Position, *a_Chunk.GetWorld());
		}
		else
		{
			cBlockPistonHandler::RetractPiston(a_Position, *a_Chunk.GetWorld());
		}

		// It is necessary to delay after a signal to prevent an infinite loop (#3168)
		// However, this delay is already present: as a side effect of the implementation of piston animation in Blocks\BlockPiston.cpp
	}

	virtual void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, SourceCallback Callback) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);

		const auto Face = cBlockPistonHandler::MetaDataToDirection(a_Meta);
		const auto FrontOffset = AddFaceDirection(Vector3i(), Face);

		for (const auto Offset : RelativeAdjacents)
		{
			if (Offset != FrontOffset)
			{
				Callback(a_Position + Offset);
			}
		}
	}
};
