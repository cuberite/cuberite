
#pragma once

#include "BlockHandler.h"
#include "BlockType.h"
#include "Mixins/Mixins.h"
#include "Mixins/SolidSurfaceUnderneath.h"
#include "ChunkInterface.h"
#include "../Chunk.h"





class cBlockRedstoneRepeaterHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	const unsigned char MaxDelay = 4;

	inline static Vector3i GetFrontCoordinateOffset(BlockState a_Block)
	{
		return -GetRearCoordinateOffset(a_Block);
	}

	inline static Vector3i GetLeftCoordinateOffset(BlockState a_Block)
	{
		auto Facing = Block::Repeater::Facing(a_Block);
		switch (Facing)
		{
			case eBlockFace::BLOCK_FACE_ZM: return { -1, 0, 0 };
			case eBlockFace::BLOCK_FACE_XP: return { 0, 0, -1 };
			case eBlockFace::BLOCK_FACE_ZP: return { 1, 0, 0 };
			case eBlockFace::BLOCK_FACE_XM: return { 0, 0, 1 };

			default:
			{
				LOGWARNING("%s: Unknown BlockFace: %d", __FUNCTION__, BlockFaceToString(Facing));
				ASSERT(!"Unknown BlockFace while determining orientation of repeater!");
				return { 0, 0, 0 };
			}
		}
	}

	inline static Vector3i GetRearCoordinateOffset(BlockState a_Block)
	{
		auto Facing = Block::Repeater::Facing(a_Block);
		switch (ReverseBlockFace(Facing))
		{
			case eBlockFace::BLOCK_FACE_ZM: return { 0, 0, 1 };
			case eBlockFace::BLOCK_FACE_XP: return { -1, 0, 0 };
			case eBlockFace::BLOCK_FACE_ZP: return { 0, 0, -1 };
			case eBlockFace::BLOCK_FACE_XM: return { 1, 0, 0 };
			default:
			{
				LOGWARNING("%s: Unknown BlockFace: %d", __FUNCTION__, BlockFaceToString(Facing));
				ASSERT(!"Unknown BlockFace while determining orientation of repeater!");
				return { 0, 0, 0 };
			}
		}
	}

	inline static BlockState IncreaseDelay(BlockState a_Block)
	{
		using namespace Block;
		return Repeater::Repeater
		(
			(Repeater::Delay(a_Block) + 1) % 4,
			Repeater::Facing(a_Block),
			Repeater::Locked(a_Block),
			Repeater::Powered(a_Block)
		);
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
		// Increment the delay setting:
		a_ChunkInterface.SetBlock(a_BlockPos, IncreaseDelay(a_ChunkInterface.GetBlock(a_BlockPos)));
		return true;
	}





	virtual void OnCancelRightClick(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace
	) const override
	{
		UNUSED(a_ChunkInterface);
		a_WorldInterface.SendBlockTo(a_BlockPos, a_Player);
	}





	virtual bool IsUseable(void) const override
	{
		return true;
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		if (a_Position.y <= 0)
		{
			return false;
		}

		auto BelowBlock = a_Chunk.GetBlock(a_Position.addedY(-1));

		if (cBlockInfo::FullyOccupiesVoxel(BelowBlock))
		{
			return true;
		}

		// upside down slabs
		if (cBlockSlabHandler::IsAnySlabType(BelowBlock))
		{
			return cBlockSlabHandler::IsSlabDown(BelowBlock);
		}

		// upside down stairs
		if (cBlockStairsHandler::IsAnyStairType(BelowBlock))
		{
			return cBlockSlabHandler::IsSlabTop(BelowBlock);
		}

		return false;
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		return cItem(Item::Repeater);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 11;
	}
} ;




