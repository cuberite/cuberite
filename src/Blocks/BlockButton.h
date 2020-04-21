#pragma once

#include "BlockHandler.h"
#include "../BlockInfo.h"
#include "../Chunk.h"
#include "Mixins.h"




class cBlockButtonHandler :
	public cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x04, 0x01, 0x03, 0x02, true>>
{
	using Super = cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x04, 0x01, 0x03, 0x02, true>>;

public:

	cBlockButtonHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) override
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockPos);

		// If button is already on, do nothing:
		if (Meta & 0x08)
		{
			return false;
		}

		// Set the ON bit to on
		Meta |= 0x08;

		a_ChunkInterface.SetBlockMeta(a_BlockPos, Meta, false);
		a_WorldInterface.WakeUpSimulators(a_BlockPos);
		a_WorldInterface.GetBroadcastManager().BroadcastSoundEffect("block.stone_button.click_on", a_BlockPos, 0.5f, 0.6f);

		// Queue a button reset (unpress)
		auto TickDelay = (m_BlockType == E_BLOCK_STONE_BUTTON) ? 20 : 30;
		a_Player.GetWorld()->ScheduleTask(TickDelay, [a_BlockPos, this](cWorld & a_World)
			{
				if (a_World.GetBlock(a_BlockPos) == m_BlockType)
				{
					// Block hasn't change in the meantime; set its meta
					a_World.SetBlockMeta(a_BlockPos, a_World.GetBlockMeta(a_BlockPos) & 0x07, false);
					a_World.WakeUpSimulators(a_BlockPos);
					a_World.BroadcastSoundEffect("block.stone_button.click_off", a_BlockPos, 0.5f, 0.5f);
				}
			}
		);

		return true;
	}





	virtual bool IsUseable(void) override
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
	) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = BlockFaceToMetaData(a_ClickedBlockFace);
		return true;
	}





	/** Converts the block face of the neighbor to which the button is attached, to the block meta for this button. */
	inline static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_BlockFace)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_YP: return 0x5;
			case BLOCK_FACE_ZM: return 0x4;
			case BLOCK_FACE_ZP: return 0x3;
			case BLOCK_FACE_XM: return 0x2;
			case BLOCK_FACE_XP: return 0x1;
			case BLOCK_FACE_YM: return 0x0;
			case BLOCK_FACE_NONE:
			{
				ASSERT(!"Unhandled block face!");
				return 0x0;
			}
		}
		UNREACHABLE("Unsupported block face");
	}





	/** Converts the block meta of this button into a block face of the neighbor to which the button is attached. */
	inline static eBlockFace BlockMetaDataToBlockFace(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x7)
		{
			case 0x0: return BLOCK_FACE_YM;
			case 0x1: return BLOCK_FACE_XP;
			case 0x2: return BLOCK_FACE_XM;
			case 0x3: return BLOCK_FACE_ZP;
			case 0x4: return BLOCK_FACE_ZM;
			case 0x5: return BLOCK_FACE_YP;
			default:
			{
				ASSERT(!"Unhandled block meta!");
				return BLOCK_FACE_NONE;
			}
		}
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) override
	{
		auto Meta = a_Chunk.GetMeta(a_RelPos);
		auto SupportRelPos = AddFaceDirection(a_RelPos, BlockMetaDataToBlockFace(Meta), true);
		if (!cChunkDef::IsValidHeight(SupportRelPos.y))
		{
			return false;
		}
		BLOCKTYPE SupportBlockType;
		a_Chunk.UnboundedRelGetBlockType(SupportRelPos, SupportBlockType);

		return cBlockInfo::FullyOccupiesVoxel(SupportBlockType);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}





	/** Extracts the ON bit from metadata. */
	static bool IsButtonOn(NIBBLETYPE a_BlockMeta)
	{
		return ((a_BlockMeta & 0x8) == 0x8);
	}
} ;




