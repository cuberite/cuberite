
// BlockHopper.h

// Declares the cBlockHopperHandler class representing the handler for the Hopper block

#include "Mixins.h"



class cBlockHopperHandler final :
	public cPitchYawRotator<cClearMetaOnDrop<cBlockEntityHandler>>
{
	using Super = cPitchYawRotator<cClearMetaOnDrop<cBlockEntityHandler>>;

public:

	using Super::Super;

private:

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) const override
	{
		a_BlockType = m_BlockType;

		// Convert the blockface into meta:
		switch (a_ClickedBlockFace)
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





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;




