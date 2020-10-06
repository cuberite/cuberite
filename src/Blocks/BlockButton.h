#pragma once

#include "BlockHandler.h"
#include "../BlockInfo.h"
#include "../Chunk.h"
#include "Mixins.h"
#include "ChunkInterface.h"




class cBlockButtonHandler final :
	public cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x04, 0x01, 0x03, 0x02, true>>
{
	using Super = cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x04, 0x01, 0x03, 0x02, true>>;

public:

	using Super::Super;

	/** Extracts the ON bit from metadata and returns if true if it is set */
	static bool IsButtonOn(NIBBLETYPE a_Meta)
	{
		return (a_Meta & 0x08) == 0x08;
	}

	/** Event handler for an arrow striking a block.
	Performs appropriate handling if the arrow intersected a wooden button. */
	static void OnArrowHit(cWorld & a_World, const Vector3i a_Position, const eBlockFace a_HitFace)
	{
		BLOCKTYPE Type;
		NIBBLETYPE Meta;
		const auto Pos = AddFaceDirection(a_Position, a_HitFace);

		if (
			!a_World.GetBlockTypeMeta(Pos, Type, Meta) ||
			IsButtonOn(Meta) ||
			!IsButtonPressedByArrow(a_World, Pos, Type, Meta)
		)
		{
			// Bail if we're not specifically a wooden button, or it's already on
			// or if the arrow didn't intersect. It is very important that nothing is
			// done if the button is depressed, since the release task will already be queued
			return;
		}

		a_World.SetBlockMeta(Pos, Meta | 0x08);
		a_World.WakeUpSimulators(Pos);

		// sound name is ok to be wood, because only wood gets triggered by arrow
		a_World.GetBroadcastManager().BroadcastSoundEffect("block.wood_button.click_on", Pos, 0.5f, 0.6f);

		// Queue a button reset
		QueueButtonRelease(a_World, Pos, Type);
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
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockPos);

		// If button is already on, do nothing:
		if (IsButtonOn(Meta))
		{
			return false;
		}

		// Set the ON bit to on
		Meta |= 0x08;

		const auto SoundToPlay = (m_BlockType == E_BLOCK_STONE_BUTTON) ? "block.stone_button.click_on" : "block.wood_button.click_on";

		a_ChunkInterface.SetBlockMeta(a_BlockPos, Meta);
		a_WorldInterface.WakeUpSimulators(a_BlockPos);
		a_WorldInterface.GetBroadcastManager().BroadcastSoundEffect(SoundToPlay, a_BlockPos, 0.5f, 0.6f, a_Player.GetClientHandle());

		// Queue a button reset (unpress)
		QueueButtonRelease(*a_Player.GetWorld(), a_BlockPos, m_BlockType);

		return true;
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





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
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





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}

	/** Schedules a recurring event at appropriate intervals to release a button at a given position.
	The given block type is checked when the task is executed to ensure the position still contains a button. */
	static void QueueButtonRelease(cWorld & a_ButtonWorld, const Vector3i a_Position, const BLOCKTYPE a_BlockType)
	{
		const auto TickDelay = (a_BlockType == E_BLOCK_STONE_BUTTON) ? 20 : 30;
		a_ButtonWorld.ScheduleTask(
			TickDelay,
			[a_Position, a_BlockType](cWorld & a_World)
			{
				BLOCKTYPE Type;
				NIBBLETYPE Meta;

				if (
					!a_World.GetBlockTypeMeta(a_Position, Type, Meta) ||
					(Type != a_BlockType) || !IsButtonOn(Meta)
				)
				{
					// Total failure or block changed, bail
					return;
				}

				if (IsButtonPressedByArrow(a_World, a_Position, Type, Meta))
				{
					// Try again in a little while
					QueueButtonRelease(a_World, a_Position, a_BlockType);
					return;
				}

				// Block hasn't change in the meantime; release it
				const auto SoundToPlayOnRelease = (Type == E_BLOCK_STONE_BUTTON) ? "block.stone_button.click_off" : "block.wood_button.click_off";

				a_World.SetBlockMeta(a_Position, Meta & 0x07);
				a_World.WakeUpSimulators(a_Position);
				a_World.BroadcastSoundEffect(SoundToPlayOnRelease, a_Position, 0.5f, 0.5f);
			}
		);
	}

	/** Returns true if an arrow was found in the wooden button */
	static bool IsButtonPressedByArrow(cWorld & a_World, const Vector3i a_ButtonPosition, const BLOCKTYPE a_BlockType, const NIBBLETYPE a_Meta)
	{
		if (a_BlockType != E_BLOCK_WOODEN_BUTTON)
		{
			return false;
		}

		const auto FaceOffset = GetButtonOffsetOnBlock(a_Meta);
		const bool FoundArrow = !a_World.ForEachEntityInBox(
			cBoundingBox(FaceOffset + a_ButtonPosition, 0.2, 0.2),
			[](cEntity & a_Entity)
			{
				return a_Entity.IsArrow();
			}
		);

		return FoundArrow;
	}

	/** Returns an offset to the integer world coordinates of a button.
	Applying this offset yields the centre of the button's bounding box,
	in terms of the position within the block the button with given meta occupies.

	TODO: this is only approximate, return the exact bbox instead. */
	static Vector3d GetButtonOffsetOnBlock(NIBBLETYPE a_Meta)
	{
		switch (BlockMetaDataToBlockFace(a_Meta))
		{
			case BLOCK_FACE_YM: return { 0.5, 1, 0.5 };
			case BLOCK_FACE_XP: return { 0, 0.5, 0.5 };
			case BLOCK_FACE_XM: return { 1, 0.5, 0.5 };
			case BLOCK_FACE_ZP: return { 0.5, 0.5, 0 };
			case BLOCK_FACE_ZM: return { 0.5, 0.5, 1 };
			case BLOCK_FACE_YP: return { 0.5, 0, 0.5 };
			case BLOCK_FACE_NONE:
			{
				ASSERT(!"Unhandled block face!");
				return { 0, 0, 0 };
			}
		}
		UNREACHABLE(!"Unhandled block face!");
	}
} ;
