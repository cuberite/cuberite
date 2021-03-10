
// BlockDropSpenser.h

// Declares the cBlockDropSpenserHandler class representing the BlockHandler for Dropper and Dispenser blocks

#pragma once

#include "Mixins.h"





class cBlockDropSpenserHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

private:


	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) const
	{
		switch (m_BlockType)
		{
			case BlockType::Dispenser: a_Block = Block::Dispenser::Dispenser(RotationToBlockFace(a_Player.GetYaw()), false); break;
			case BlockType::Dropper:   a_Block = Block::Dropper::Dropper(RotationToBlockFace(a_Player.GetYaw()), false); break;
			default: return false;
		}
		return true;
	}

	virtual ColourID GetMapBaseColourID() const override
	{
		return 11;
	}
} ;
