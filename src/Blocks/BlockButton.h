#pragma once

#include "BlockHandler.h"
#include "Chunk.h"
#include "MetaRotator.h"




class cBlockButtonHandler :
	public cMetaRotator<cBlockHandler, 0x07, 0x04, 0x01, 0x03, 0x02, true>
{
public:
	cBlockButtonHandler(BLOCKTYPE a_BlockType)
		: cMetaRotator<cBlockHandler, 0x07, 0x04, 0x01, 0x03, 0x02, true>(a_BlockType)
	{
	}

	virtual bool OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		Vector3i Pos(a_BlockX, a_BlockY, a_BlockZ);
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(Pos);

		Vector3d SoundPos(Pos);

		// If button is already on do nothing
		if (Meta & 0x08)
		{
			return false;
		}

		// Set p the ON bit to on
		Meta |= 0x08;

		a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta, false);
		a_WorldInterface.WakeUpSimulators(Pos);
		a_WorldInterface.GetBroadcastManager().BroadcastSoundEffect("block.stone_button.click_on", SoundPos, 0.5f, 0.6f);

		// Queue a button reset (unpress)
		auto TickDelay = (m_BlockType == E_BLOCK_STONE_BUTTON) ? 20 : 30;
		a_Player.GetWorld()->ScheduleTask(TickDelay, [SoundPos, Pos, this](cWorld & a_World)
			{
				if (a_World.GetBlock(Pos) == m_BlockType)
				{
					// Block hasn't change in the meantime; set its meta
					a_World.SetBlockMeta(Pos.x, Pos.y, Pos.z, a_World.GetBlockMeta(Pos) & 0x07, false);
					a_World.WakeUpSimulators(Pos);
					a_World.BroadcastSoundEffect("block.stone_button.click_off", SoundPos, 0.5f, 0.5f);
				}
			}
		);

		return true;
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(m_BlockType, 1, 0));
	}

	virtual bool IsUseable(void) override
	{
		return true;
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = BlockFaceToMetaData(a_BlockFace);
		return true;
	}

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

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		NIBBLETYPE Meta;
		a_Chunk.UnboundedRelGetBlockMeta(a_RelX, a_RelY, a_RelZ, Meta);

		AddFaceDirection(a_RelX, a_RelY, a_RelZ, BlockMetaDataToBlockFace(Meta), true);
		BLOCKTYPE BlockIsOn; a_Chunk.UnboundedRelGetBlockType(a_RelX, a_RelY, a_RelZ, BlockIsOn);

		return (a_RelY > 0) && (cBlockInfo::FullyOccupiesVoxel(BlockIsOn));
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}

	/** Extracts the ON bit from metadata and returns if true if it is set */
	static bool IsButtonOn(NIBBLETYPE a_BlockMeta)
	{
		return ((a_BlockMeta & 0x8) == 0x8);
	}
} ;




