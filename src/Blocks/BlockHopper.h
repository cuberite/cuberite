
// BlockHopper.h

// Declares the cBlockHopperHandler class representing the handler for the Hopper block

#include "Mixins.h"



class cBlockHopperHandler :
	public cPitchYawRotator<cContainerEntityHandler<cBlockEntityHandler>>
{
	using super = cPitchYawRotator<cContainerEntityHandler<cBlockEntityHandler>>;

public:

	cBlockHopperHandler(BLOCKTYPE a_BlockType):
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





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;




