#pragma once

#include "BlockHandler.h"
#include "MetaRotator.h"





class cBlockTripwireHookHandler :
	public cMetaRotator<cBlockHandler, 0x03, 0x02, 0x03, 0x00, 0x01>
{
public:
	cBlockTripwireHookHandler(BLOCKTYPE a_BlockType)
		: cMetaRotator<cBlockHandler, 0x03, 0x02, 0x03, 0x00, 0x01>(a_BlockType)
	{
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		if (!IsValidDirection(a_BlockFace))
		{
			return false;
		}

		a_BlockType = m_BlockType;
		a_BlockMeta = DirectionToMetadata(a_BlockFace);

		return true;
	}

	bool IsValidDirection(eBlockFace a_Direction)
	{
		switch (a_Direction)
		{
			case BLOCK_FACE_XM:
			case BLOCK_FACE_XP:
			case BLOCK_FACE_ZM:
			case BLOCK_FACE_ZP:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
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

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to zero
		a_Pickups.push_back(cItem(E_BLOCK_TRIPWIRE_HOOK, 1, 0));
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk, NIBBLETYPE a_BlockMeta) override
	{


		eBlockFace Face = MetadataToDirection(a_BlockMeta);
		AddFaceDirection(a_RelX, a_RelY, a_RelZ, Face, true);
		BLOCKTYPE BlockIsOn;
		a_Chunk.UnboundedRelGetBlockType(a_RelX, a_RelY, a_RelZ, BlockIsOn);

		if (a_RelY < 0)
		{
			return false;
		}

		// Can be place to side of any full solid opaque block
		if ((Face == BLOCK_FACE_BOTTOM) || (Face == BLOCK_FACE_TOP))
		{
			return false;
		}

		return cBlockInfo::IsFullSolidOpaqueBlock(BlockIsOn);
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
};




