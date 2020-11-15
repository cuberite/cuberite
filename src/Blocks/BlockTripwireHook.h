#pragma once

#include "BlockHandler.h"
#include "Mixins.h"





class cBlockTripwireHookHandler final :
	public cMetaRotator<cClearMetaOnDrop<cBlockHandler>, 0x03, 0x02, 0x03, 0x00, 0x01>
{
	using Super = cMetaRotator<cClearMetaOnDrop<cBlockHandler>, 0x03, 0x02, 0x03, 0x00, 0x01>;

public:

	using Super::Super;

	inline static eBlockFace MetadataToDirection(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x03)
		{
			case 0x1: return BLOCK_FACE_XM;
			case 0x3: return BLOCK_FACE_XP;
			case 0x2: return BLOCK_FACE_ZM;
			case 0x0: return BLOCK_FACE_ZP;
			default: ASSERT(!"Unhandled tripwire hook metadata!"); return BLOCK_FACE_NONE;
		}
	}

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

		switch (a_ClickedBlockFace)
		{
			case BLOCK_FACE_XM:
			{
				a_BlockMeta = 0x1;
				return true;
			}
			case BLOCK_FACE_XP:
			{
				a_BlockMeta = 0x3;
				return true;
			}
			case BLOCK_FACE_ZM:
			{
				a_BlockMeta = 0x2;
				return true;
			}
			case BLOCK_FACE_ZP:
			{
				a_BlockMeta = 0x0;
				return true;
			}
			case BLOCK_FACE_NONE:
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			{
				return false;
			}
		}

		UNREACHABLE("Unsupported block face");
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		const auto Meta = a_Chunk.GetMeta(a_RelPos);
		const auto RearPosition = AddFaceDirection(a_RelPos, MetadataToDirection(Meta), true);

		BLOCKTYPE NeighborBlockType;
		if (!a_Chunk.UnboundedRelGetBlockType(RearPosition, NeighborBlockType))
		{
			return false;
		}

		return cBlockInfo::FullyOccupiesVoxel(NeighborBlockType);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
};




