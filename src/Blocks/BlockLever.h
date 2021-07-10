#pragma once

#include "BlockHandler.h"
#include "../Chunk.h"
#include "Mixins.h"
#include "BlockSlab.h"


class cBlockLeverHandler final :
	public cMetaRotator<cBlockHandler, 0x07, 0x04, 0x01, 0x03, 0x02, false>
{
	using Super = cMetaRotator<cBlockHandler, 0x07, 0x04, 0x01, 0x03, 0x02, false>;

public:

	using Super::Super;

	/** Extracts the ON bit from metadata and returns if true if it is set */
	static bool IsLeverOn(NIBBLETYPE a_BlockMeta)
	{
		return ((a_BlockMeta & 0x8) == 0x8);
	}

private:

	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) const override
	{
		// Flip the ON bit on / off using the XOR bitwise operation
		NIBBLETYPE Meta = (a_ChunkInterface.GetBlockMeta(a_BlockPos) ^ 0x08);

		a_ChunkInterface.SetBlockMeta(a_BlockPos, Meta);
		a_WorldInterface.WakeUpSimulators(a_BlockPos);
		a_WorldInterface.GetBroadcastManager().BroadcastSoundEffect("block.lever.click", a_BlockPos, 0.5f, (Meta & 0x08) ? 0.6f : 0.5f);
		return true;
	}





	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Reset meta to zero:
		return cItem(E_BLOCK_LEVER, 1, 0);
	}





	virtual bool IsUseable(void) const override
	{
		return true;
	}





	/** Converts the leve block's meta to the block face of the neighbor to which the lever is attached. */
	inline static eBlockFace BlockMetaDataToBlockFace(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x7)
		{
			case 0x1: return BLOCK_FACE_XP;
			case 0x2: return BLOCK_FACE_XM;
			case 0x3: return BLOCK_FACE_ZP;
			case 0x4: return BLOCK_FACE_ZM;
			case 0x5:
			case 0x6: return BLOCK_FACE_YP;
			case 0x7:
			case 0x0: return BLOCK_FACE_YM;
			default:
			{
				ASSERT(!"Unhandled block meta!");
				return BLOCK_FACE_NONE;
			}
		}
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		// Find the type of block the lever is attached to:
		auto NeighborFace = BlockMetaDataToBlockFace(a_Meta);
		auto NeighborPos = AddFaceDirection(a_Position, NeighborFace, true);
		if (!cChunkDef::IsValidHeight(NeighborPos.y))
		{
			return false;
		}
		BLOCKTYPE NeighborBlockType;
		NIBBLETYPE NeighborMeta;
		if (!a_Chunk.UnboundedRelGetBlock(NeighborPos, NeighborBlockType, NeighborMeta))
		{
			return false;
		}

		// Allow any full block or the "good" side of a half-slab:
		if (cBlockInfo::FullyOccupiesVoxel(NeighborBlockType))
		{
			return true;
		}
		else if (cBlockSlabHandler::IsAnySlabType(NeighborBlockType))
		{
			return (
				(((NeighborMeta & 0x08) == 0x08) && (NeighborFace == BLOCK_FACE_TOP)) ||
				(((NeighborMeta & 0x08) == 0)    && (NeighborFace == BLOCK_FACE_BOTTOM))
			);
		}

		return false;
	}





	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) const override
	{
		switch (a_Meta)
		{
			case 0x00: return 0x07;  // Ceiling rotation
			case 0x07: return 0x00;

			case 0x05: return 0x06;  // Ground rotation
			case 0x06: return 0x05;

			default:  return Super::MetaRotateCCW(a_Meta);  // Wall Rotation
		}
	}





	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) const override
	{
		switch (a_Meta)
		{
			case 0x00: return 0x07;  // Ceiling rotation
			case 0x07: return 0x00;

			case 0x05: return 0x06;  // Ground rotation
			case 0x06: return 0x05;

			default:  return Super::MetaRotateCW(a_Meta);  // Wall Rotation
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




