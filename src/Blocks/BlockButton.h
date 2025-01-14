#pragma once

#include "BlockHandler.h"
#include "BlockSlab.h"
#include "BlockStairs.h"
#include "../BlockInfo.h"
#include "../Chunk.h"
#include "Defines.h"
#include "Entities/Player.h"
#include "Mixins/Mixins.h"
#include "ChunkInterface.h"
#include "World.h"




class cBlockButtonHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static bool IsButton(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaButton:
			case BlockType::BirchButton:
			case BlockType::CrimsonButton:
			case BlockType::DarkOakButton:
			case BlockType::JungleButton:
			case BlockType::OakButton:
			case BlockType::PolishedBlackstoneButton:
			case BlockType::SpruceButton:
			case BlockType::StoneButton:
			case BlockType::WarpedButton:
			case BlockType::BambooButton:
			case BlockType::CherryButton:
			case BlockType::MangroveButton:
			case BlockType::PaleOakButton:
				return true;
			default: return false;
		}
	}

	static bool IsButtonOn(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaButton:             return AcaciaButton::Powered(a_Block);
			case BlockType::BirchButton:              return BirchButton::Powered(a_Block);
			case BlockType::CrimsonButton:            return CrimsonButton::Powered(a_Block);
			case BlockType::DarkOakButton:            return DarkOakButton::Powered(a_Block);
			case BlockType::JungleButton:             return JungleButton::Powered(a_Block);
			case BlockType::OakButton:                return OakButton::Powered(a_Block);
			case BlockType::PolishedBlackstoneButton: return PolishedBlackstoneButton::Powered(a_Block);
			case BlockType::SpruceButton:             return SpruceButton::Powered(a_Block);
			case BlockType::StoneButton:              return StoneButton::Powered(a_Block);
			case BlockType::WarpedButton:             return WarpedButton::Powered(a_Block);
			case BlockType::BambooButton:             return BambooButton::Powered(a_Block);
			case BlockType::CherryButton:             return CherryButton::Powered(a_Block);
			case BlockType::MangroveButton:           return MangroveButton::Powered(a_Block);
			case BlockType::PaleOakButton:            return PaleOakButton::Powered(a_Block);
			default: return false;
		}
	}

#define TOGGLEBUTTON(ButtonType) \
	a_ChunkInterface.SetBlock(a_Position, ButtonType::ButtonType(ButtonType::Face(Self), ButtonType::Facing(Self), !ButtonType::Powered(Self))); break \

	static inline void ToggleButton(cChunkInterface & a_ChunkInterface, Vector3i a_Position)
	{
		using namespace Block;
		BlockState Self = a_ChunkInterface.GetBlock(a_Position);

		switch (Self.Type())
		{
			case BlockType::AcaciaButton:             TOGGLEBUTTON(AcaciaButton);
			case BlockType::BirchButton:              TOGGLEBUTTON(BirchButton);
			case BlockType::CrimsonButton:            TOGGLEBUTTON(CrimsonButton);
			case BlockType::DarkOakButton:            TOGGLEBUTTON(DarkOakButton);
			case BlockType::JungleButton:             TOGGLEBUTTON(JungleButton);
			case BlockType::OakButton:                TOGGLEBUTTON(OakButton);
			case BlockType::PolishedBlackstoneButton: TOGGLEBUTTON(PolishedBlackstoneButton);
			case BlockType::SpruceButton:             TOGGLEBUTTON(SpruceButton);
			case BlockType::StoneButton:              TOGGLEBUTTON(StoneButton);
			case BlockType::WarpedButton:             TOGGLEBUTTON(WarpedButton);
			case BlockType::BambooButton:             TOGGLEBUTTON(BambooButton);
			case BlockType::CherryButton:             TOGGLEBUTTON(CherryButton);
			case BlockType::MangroveButton:           TOGGLEBUTTON(MangroveButton);
			case BlockType::PaleOakButton:            TOGGLEBUTTON(PaleOakButton);
			default: break;
		}
	}

	/** Event handler for an arrow striking a block.
	Performs appropriate handling if the arrow intersected a wooden button. */
	static void OnArrowHit(cWorld & a_World, const Vector3i a_Position, const eBlockFace a_HitFace)
	{
		BlockState Self = 0;
		const auto Pos = AddFaceDirection(a_Position, a_HitFace);

		if (
			!a_World.GetBlock(Pos, Self) ||
			IsButtonOn(Self) ||
			!IsButtonPressedByArrow(a_World, Pos, Self)
		)
		{
			// Bail if we're not specifically a wooden button, or it's already on
			// or if the arrow didn't intersect. It is very important that nothing is
			// done if the button is depressed, since the release task will already be queued
			return;
		}

		auto ChunkInterface = cChunkInterface(a_World.GetChunkMap());

		ToggleButton(ChunkInterface, a_Position);
		a_World.WakeUpSimulators(Pos);

		// sound name is ok to be wood, because only wood gets triggered by arrow
		a_World.GetBroadcastManager().BroadcastSoundEffect("block.wood_button.click_on", Pos, 0.5f, 0.6f);

		// Queue a button reset
		QueueButtonRelease(a_World, Pos, Self);
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
		auto Self = a_ChunkInterface.GetBlock(a_BlockPos);

		// If button is already on, do nothing:
		if (IsButtonOn(Self))
		{
			return false;
		}

		ToggleButton(a_ChunkInterface, a_BlockPos);

		const auto SoundToPlay = (m_BlockType == BlockType::StoneButton) ? "block.stone_button.click_on" : "block.wood_button.click_on";

		a_WorldInterface.WakeUpSimulators(a_BlockPos);
		a_WorldInterface.GetBroadcastManager().BroadcastSoundEffect(SoundToPlay, a_BlockPos, 0.5f, 0.6f, a_Player.GetClientHandle());

		// Queue a button reset (unpress)
		QueueButtonRelease(*a_Player.GetWorld(), a_BlockPos, Self);

		return true;
	}





	virtual bool IsUseable(void) const override
	{
		return true;
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		auto SupportRelPos = AddFaceDirection(a_Position, GetBlockFace(a_Self), true);
		if (!cChunkDef::IsValidHeight(SupportRelPos.y))
		{
			return false;
		}
		BlockState SupportBlock = 0;
		a_Chunk.UnboundedRelGetBlock(SupportRelPos, SupportBlock);
		eBlockFace Face = GetBlockFace(SupportBlock);
		// upside down slabs
		if (cBlockSlabHandler::IsAnySlabType(SupportBlock))
		{
			return ((Face == BLOCK_FACE_YP) && cBlockSlabHandler::IsSlabTop(SupportBlock));
		}

		// stairs (top and sides)
		if (cBlockStairsHandler::IsAnyStairType(SupportBlock))
		{
			switch (Face)
			{
				case eBlockFace::BLOCK_FACE_YP:
					return cBlockStairsHandler::IsStairsTopHalf(SupportBlock);
				case eBlockFace::BLOCK_FACE_XP:
					return (cBlockStairsHandler::GetRotation(SupportBlock) == E_BLOCK_STAIRS_XP);
				case eBlockFace::BLOCK_FACE_XM:
					return (cBlockStairsHandler::GetRotation(SupportBlock) == E_BLOCK_STAIRS_XM);
				case eBlockFace::BLOCK_FACE_ZP:
					return (cBlockStairsHandler::GetRotation(SupportBlock) == E_BLOCK_STAIRS_ZP);
				case eBlockFace::BLOCK_FACE_ZM:
					return (cBlockStairsHandler::GetRotation(SupportBlock) == E_BLOCK_STAIRS_ZM);
				default:
				{
					return false;
				}
			}
		}

		return cBlockInfo::FullyOccupiesVoxel(SupportBlock);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}

	/** Schedules a recurring event at appropriate intervals to release a button at a given position.
	The given block type is checked when the task is executed to ensure the position still contains a button. */
	static void QueueButtonRelease(cWorld & a_ButtonWorld, const Vector3i a_Position, const BlockState a_Block)
	{
		const auto TickDelay = (a_Block.Type() == BlockType::StoneButton) ? 20_tick : 30_tick;
		a_ButtonWorld.ScheduleTask(
			TickDelay,
			[a_Position, a_Block](cWorld & a_World)
			{
				BlockState Self = 0;

				if (
					!a_World.GetBlock(a_Position, Self) ||
					(Self != a_Block) || !IsButtonOn(Self)
				)
				{
					// Total failure or block changed, bail
					return;
				}

				if (IsButtonPressedByArrow(a_World, a_Position, Self))
				{
					// Try again in a little while
					QueueButtonRelease(a_World, a_Position, a_Block);
					return;
				}

				// Block hasn't change in the meantime; release it
				const auto SoundToPlayOnRelease = (a_Block.Type() == BlockType::StoneButton) ? "block.stone_button.click_off" : "block.wood_button.click_off";

				cChunkInterface ChunkInterface(a_World.GetChunkMap());

				ToggleButton(ChunkInterface, a_Position);
				a_World.WakeUpSimulators(a_Position);
				a_World.BroadcastSoundEffect(SoundToPlayOnRelease, a_Position, 0.5f, 0.5f);
			}
		);
	}

	/** Returns true if an arrow was found in the wooden button */
	static bool IsButtonPressedByArrow(cWorld & a_World, const Vector3i a_ButtonPosition, const BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::StoneButton: return false;
			case BlockType::AcaciaButton:
			case BlockType::BirchButton:
			case BlockType::CrimsonButton:
			case BlockType::DarkOakButton:
			case BlockType::JungleButton:
			case BlockType::OakButton:
			case BlockType::PolishedBlackstoneButton:
			case BlockType::SpruceButton:
			case BlockType::WarpedButton:
			case BlockType::BambooButton:
			case BlockType::CherryButton:
			case BlockType::MangroveButton: break;
			default: return false;
		}

		const auto FaceOffset = GetButtonOffsetOnBlock(a_Block);
		const bool FoundArrow = !a_World.ForEachEntityInBox(
			cBoundingBox(FaceOffset + a_ButtonPosition, 0.2, 0.2),
			[](cEntity & a_Entity)
			{
				return a_Entity.IsArrow();
			}
		);

		return FoundArrow;
	}

#define GETFACE(ButtonType) \
	Block::ButtonType::Facing(a_Block)

	static inline eBlockFace GetBlockFace(const BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::StoneButton:              return GETFACE(StoneButton);
			case BlockType::AcaciaButton:             return GETFACE(AcaciaButton);
			case BlockType::BirchButton:              return GETFACE(BirchButton);
			case BlockType::CrimsonButton:            return GETFACE(CrimsonButton);
			case BlockType::DarkOakButton:            return GETFACE(DarkOakButton);
			case BlockType::JungleButton:             return GETFACE(JungleButton);
			case BlockType::OakButton:                return GETFACE(OakButton);
			case BlockType::PolishedBlackstoneButton: return GETFACE(PolishedBlackstoneButton);
			case BlockType::SpruceButton:             return GETFACE(SpruceButton);
			case BlockType::WarpedButton:             return GETFACE(WarpedButton);
			case BlockType::BambooButton:             return GETFACE(BambooButton);
			case BlockType::CherryButton:             return GETFACE(CherryButton);
			case BlockType::MangroveButton:           return GETFACE(MangroveButton);
			case BlockType::PaleOakButton:            return GETFACE(PaleOakButton);
			default: return eBlockFace::BLOCK_FACE_NONE;
		}
	}

	/** Returns an offset to the integer world coordinates of a button.
	Applying this offset yields the centre of the button's bounding box,
	in terms of the position within the block the button with given meta occupies.

	TODO: this is only approximate, return the exact bbox instead. */
	static Vector3d GetButtonOffsetOnBlock(BlockState a_Block)
	{
		auto Face = GetBlockFace(a_Block);

		switch (Face)
		{
			case BLOCK_FACE_YM: return { 0.5, 1, 0.5 };
			case BLOCK_FACE_XP: return { 0, 0.5, 0.5 };
			case BLOCK_FACE_XM: return { 1, 0.5, 0.5 };
			case BLOCK_FACE_ZP: return { 0.5, 0.5, 0 };
			case BLOCK_FACE_ZM: return { 0.5, 0.5, 1 };
			case BLOCK_FACE_YP: return { 0.5, 0, 0.5 };
			case BLOCK_FACE_NONE:
			{
				break;
			}
		}
		UNREACHABLE("Unhandled block face!");
	}
} ;
