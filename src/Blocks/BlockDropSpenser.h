
// BlockDropSpenser.h

// Declares the cBlockDropSpenserHandler class representing the BlockHandler for Dropper and Dispenser blocks

#pragma once

#include "../Blocks/BlockPiston.h"
#include "../BlockEntities/DropSpenserEntity.h"
#include "Mixins.h"





class cBlockDropSpenserHandler :
	public cMetaRotator<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>
{
	using super = cMetaRotator<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>;

public:

	cBlockDropSpenserHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;

		// FIXME: Do not use cPiston class for dispenser placement!
		a_BlockMeta = cBlockPistonHandler::RotationPitchToMetaData(a_Player.GetYaw(), a_Player.GetPitch());
		return true;
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		cItems res(cItem(m_BlockType, 1));
		if (a_BlockEntity != nullptr)
		{
			auto be = static_cast<cDropSpenserEntity *>(a_BlockEntity);
			res.AddItemGrid(be->GetContents());
		}
		return res;
	}





	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) override
	{
		// Bit 0x08 is a flag. Lowest three bits are position.
		NIBBLETYPE OtherMeta = a_Meta & 0x08;
		// Mirrors defined by a table. (Source, minecraft.gamepedia.com)
		switch (a_Meta & 0x07)
		{
			case 0x00: return 0x01 + OtherMeta;  // Down -> Up
			case 0x01: return 0x00 + OtherMeta;  // Up -> Down
		}
		// Not Facing Up or Down; No change.
		return a_Meta;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;




