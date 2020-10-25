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





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Reset meta to zero:
		return cItem(E_BLOCK_LEVER, 1, 0);
	}





	virtual bool IsUseable(void) const override
	{
		return true;
	}





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
		a_BlockMeta = LeverDirectionToMetaData(a_ClickedBlockFace);
		return true;
	}





	/** Converts the block face of the neighbor to which the lever is attached to the lever block's meta. */
	inline static NIBBLETYPE LeverDirectionToMetaData(eBlockFace a_Dir)
	{
		// Determine lever direction:
		switch (a_Dir)
		{
			case BLOCK_FACE_YP:   return 0x6;
			case BLOCK_FACE_XP:   return 0x1;
			case BLOCK_FACE_XM:   return 0x2;
			case BLOCK_FACE_ZP:   return 0x3;
			case BLOCK_FACE_ZM:   return 0x4;
			case BLOCK_FACE_YM:   return 0x0;
			case BLOCK_FACE_NONE: return 0x6;
		}
		UNREACHABLE("Unsupported block face");
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





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		// Find the type of block the lever is attached to:
		auto Meta = a_Chunk.GetMeta(a_RelPos);
		auto NeighborFace = BlockMetaDataToBlockFace(Meta);
		auto NeighborPos = AddFaceDirection(a_RelPos, NeighborFace, true);
		if (!cChunkDef::IsValidHeight(NeighborPos.y))
		{
			return false;
		}
		BLOCKTYPE NeighborBlockType;
		if (!a_Chunk.UnboundedRelGetBlock(NeighborPos, NeighborBlockType, Meta))
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
				(((Meta & 0x08) == 0x08) && (NeighborFace == BLOCK_FACE_TOP)) ||
				(((Meta & 0x08) == 0)    && (NeighborFace == BLOCK_FACE_BOTTOM))
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




