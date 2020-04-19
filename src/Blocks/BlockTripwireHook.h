#pragma once

#include "BlockHandler.h"
#include "Mixins.h"





class cBlockTripwireHookHandler :
	public cMetaRotator<cClearMetaOnDrop<cBlockHandler>, 0x03, 0x02, 0x03, 0x00, 0x01>
{
	using Super = cMetaRotator<cClearMetaOnDrop<cBlockHandler>, 0x03, 0x02, 0x03, 0x00, 0x01>;

public:

	cBlockTripwireHookHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = DirectionToMetadata(a_ClickedBlockFace);

		return true;
	}





	inline static NIBBLETYPE DirectionToMetadata(eBlockFace a_Direction)
	{
		switch (a_Direction)
		{
			case BLOCK_FACE_XM: return 0x1;
			case BLOCK_FACE_XP: return 0x3;
			case BLOCK_FACE_ZM: return 0x2;
			case BLOCK_FACE_ZP: return 0x0;
			case BLOCK_FACE_NONE:
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			{
				ASSERT(!"Unhandled tripwire hook direction!");
				return 0x0;
			}
		}
		UNREACHABLE("Unsupported block face");
	}





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





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) override
	{
		auto Meta = a_Chunk.GetMeta(a_RelPos);
		auto NeighborPos = AddFaceDirection(a_RelPos, MetadataToDirection(Meta), true);
		if (!cChunkDef::IsValidHeight(NeighborPos.y))
		{
			return false;
		}
		BLOCKTYPE NeighborBlockType;
		a_Chunk.UnboundedRelGetBlockType(a_RelPos, NeighborBlockType);
		return cBlockInfo::FullyOccupiesVoxel(NeighborBlockType);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
};




