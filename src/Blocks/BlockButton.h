#pragma once

#include "BlockHandler.h"
#include "../BlockInfo.h"
#include "../Chunk.h"
#include "Mixins.h"
#include "../Simulator/IncrementalRedstoneSimulator/IncrementalRedstoneSimulator.h"




class cBlockButtonHandler :
	public cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x04, 0x01, 0x03, 0x02, true>>
{
	using Super = cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x04, 0x01, 0x03, 0x02, true>>;

public:

	cBlockButtonHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
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

		auto soundToPlay = (m_BlockType == E_BLOCK_STONE_BUTTON ? "block.stone_button.click_on" : "block.wood_button.click_on");

		a_ChunkInterface.SetBlockMeta({a_BlockX, a_BlockY, a_BlockZ}, Meta, false);
		a_WorldInterface.WakeUpSimulators(Pos);
		a_WorldInterface.GetBroadcastManager().BroadcastSoundEffect(soundToPlay, SoundPos, 0.5f, 0.6f, a_Player.GetClientHandle());

		// Queue a button reset (unpress)
		if (dynamic_cast<cIncrementalRedstoneSimulator *>(a_Player.GetWorld()->GetRedstoneSimulator())->GetChunkData() != nullptr)
		{
			// Delay managed in Update-function of cRedstoneToggleHandler
			return true;
		}

		// noop-mode should show the buttons being pressed in at least
		auto TickDelay = (m_BlockType == E_BLOCK_STONE_BUTTON) ? 20 : 30;
		a_Player.GetWorld()->ScheduleTask(TickDelay, [SoundPos, Pos, this](cWorld & a_World)
			{
				if (a_World.GetBlock(Pos) == m_BlockType)
				{
					// Block hasn't change in the meantime; set its meta
					auto soundToPlay = (m_BlockType == E_BLOCK_STONE_BUTTON ? "block.stone_button.click_off" : "block.wood_button.click_off");

					a_World.SetBlockMeta(Pos.x, Pos.y, Pos.z, a_World.GetBlockMeta(Pos) & 0x07, false);
					a_World.WakeUpSimulators(Pos);
					a_World.BroadcastSoundEffect(soundToPlay, SoundPos, 0.5f, 0.5f);
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
	static bool IsButtonOn(NIBBLETYPE a_Meta)
	{
		if (a_Meta & 0x08)
		{
			return true;
		}
		return false;
	}

	/** Returns true if the button is on and handles arrows stuck in a wooden button */
	static bool IsButtonOn(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta)
	{
		if (IsButtonOn(a_Meta))
		{
			return true;
		}

		if ((a_BlockType == E_BLOCK_WOODEN_BUTTON) && (HandleArrowInIt(a_World, a_Position, a_Meta)))
		{
			a_World.SetBlockMeta(a_Position, a_World.GetBlockMeta(a_Position) | 0x08, false);
			a_World.WakeUpSimulators(a_Position);

			// sound name is ok to be wood, because only wood gets triggered by arrow
			a_World.GetBroadcastManager().BroadcastSoundEffect("block.wood_button.click_on", a_Position, 0.5f, 0.6f);
			return true;
		}

		return false;
	}

	/** Returns true if an arrow was found in the button */
	static bool HandleArrowInIt(cWorld & a_World, Vector3i a_Position, NIBBLETYPE a_Meta)
	{
		auto faceOffset = GetButtonOffsetOnBlock(a_Meta);

		bool FoundArrow = !a_World.ForEachEntityInBox(cBoundingBox(faceOffset + a_Position, 0.2, 0.2), [&](cEntity & a_Entity)
			{
				return a_Entity.IsArrow();
			}
		);
		if (FoundArrow)
		{
			auto ChunkData = static_cast<cIncrementalRedstoneSimulator *>(a_World.GetRedstoneSimulator())->GetChunkData();
			ChunkData->m_MechanismDelays[a_Position] = std::make_pair(1, false);
		}
		return FoundArrow;
	}

private:
	static Vector3d GetButtonOffsetOnBlock(NIBBLETYPE a_Meta)
	{
		switch (BlockMetaDataToBlockFace(a_Meta))
		{
		case BLOCK_FACE_YM:
			return Vector3d(0.5, 1, 0.5);
		case BLOCK_FACE_XP:
			return Vector3d(0, 0.5, 0.5);
		case BLOCK_FACE_XM:
			return Vector3d(1, 0.5, 0.5);
		case BLOCK_FACE_ZP:
			return Vector3d(0.5, 0.5, 0);
		case BLOCK_FACE_ZM:
			return Vector3d(0.5, 0.5, 1);
		case BLOCK_FACE_YP:
			return Vector3d(0.5, 0, 0.5);
		case BLOCK_FACE_NONE:
		{
			ASSERT(!"Unhandled block face!");
		}
		}
		return Vector3d(0, 0, 0);
	}
} ;



