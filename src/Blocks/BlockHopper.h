
// BlockHopper.h

// Declares the cBlockHopperHandler class representing the handler for the Hopper block

#include "MetaRotator.h"



class cBlockHopperHandler :
	public cMetaRotator<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>
{
public:
	cBlockHopperHandler(BLOCKTYPE a_BlockType)
		: cMetaRotator<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>(a_BlockType)
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

		// Convert the blockface into meta:
		switch (a_BlockFace)
		{
			case BLOCK_FACE_BOTTOM: a_BlockMeta = E_META_HOPPER_FACING_YM;  break;
			case BLOCK_FACE_TOP:    a_BlockMeta = E_META_HOPPER_FACING_YM;  break;
			case BLOCK_FACE_EAST:   a_BlockMeta = E_META_HOPPER_FACING_XM;  break;
			case BLOCK_FACE_NORTH:  a_BlockMeta = E_META_HOPPER_FACING_ZP;  break;
			case BLOCK_FACE_SOUTH:  a_BlockMeta = E_META_HOPPER_FACING_ZM;  break;
			case BLOCK_FACE_WEST:   a_BlockMeta = E_META_HOPPER_FACING_XP;  break;
			case BLOCK_FACE_NONE:   a_BlockMeta = E_META_HOPPER_UNATTACHED; break;
		}
		return true;
	}

	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) override
	{
		// Bit 0x08 is a flag.  Lowest three bits are position.  0x08 == 1000
		NIBBLETYPE OtherMeta = a_Meta & 0x08;
		// Mirrors defined by by a table. (Source, mincraft.gamepedia.com) 0x07 == 0111
		switch (a_Meta & 0x07)
		{
			case 0x00: return 0x01 + OtherMeta;  // Down -> Up
			case 0x01: return 0x00 + OtherMeta;  // Up   -> Down
		}
		// Not Facing Up or Down; No change.
		return a_Meta;
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(m_BlockType, 1, 0));
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;




