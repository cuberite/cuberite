
#pragma once

#include "BlockEntity.h"
#include "../Blocks/BlockPiston.h"
#include "MetaRotator.h"




class cBlockFurnaceHandler :
	public cMetaRotator<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>
{
public:
	cBlockFurnaceHandler(BLOCKTYPE a_BlockType)
		: cMetaRotator<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_BLOCK_FURNACE, 1, 0));
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;

		// FIXME: Do not use cPiston class for furnace placement!
		a_BlockMeta = cBlockPistonHandler::RotationPitchToMetaData(a_Player.GetYaw(), 0);

		return true;
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;




