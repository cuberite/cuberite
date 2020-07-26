
#pragma once

#include "RedstoneHandler.h"
#include "../../Blocks/BlockDoor.h"





class cDoorHandler final : public cRedstoneHandler
{
public:

	virtual unsigned char GetPowerDeliveredToPosition(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		return 0;
	}

	virtual void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// LOGD("Evaluating dori the door (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		cChunkInterface ChunkInterface(a_Chunk.GetWorld()->GetChunkMap());
		const bool ShouldBeOpen = a_PoweringData.PowerLevel != 0;
		const auto AbsolutePosition = cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos());
		const bool IsOpen = cBlockDoorHandler::IsOpen(ChunkInterface, AbsolutePosition);

		if (ShouldBeOpen != IsOpen)
		{
			cBlockDoorHandler::SetOpen(ChunkInterface, AbsolutePosition, ShouldBeOpen);
			a_Chunk.GetWorld()->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_WOODEN_DOOR_OPEN, AbsolutePosition, 0);
		}
	}

	virtual void ForValidSourcePositions(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, SourceCallback Callback) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
