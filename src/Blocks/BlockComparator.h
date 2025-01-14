
#pragma once

#include "BlockHandler.h"
#include "BlockRedstoneRepeater.h"
#include "BlockStairs.h"
#include "Mixins/Mixins.h"
#include "Mixins/SolidSurfaceUnderneath.h"





class cBlockComparatorHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	inline static bool IsInSubtractionMode(BlockState a_Block)
	{
		return (Block::Comparator::Mode(a_Block) == Block::Comparator::Mode::Subtract);
	}

	inline static Vector3i GetFrontCoordinate(Vector3i a_Position, BlockState a_Block)
	{
		using namespace Block;
		switch (Comparator::Facing(a_Block))
		{
			case BLOCK_FACE_NORTH: a_Position.z++; break;
			case BLOCK_FACE_WEST:  a_Position.x++; break;
			case BLOCK_FACE_EAST:  a_Position.x--; break;
			case BLOCK_FACE_SOUTH: a_Position.z--; break;
			default:
			{
				LOGWARNING("%s: Unknown facing: %s", __FUNCTION__, BlockFaceToString(Comparator::Facing(a_Block)));
				ASSERT(!"Unknown facing while determining orientation of comparator!");
				break;
			}
		}

		return a_Position;
	}

	inline static Vector3i GetSideCoordinate(Vector3i a_Position, BlockState a_Block, bool a_bInverse)
	{
		using namespace Block;
		if (!a_bInverse)
		{
			switch (Comparator::Facing(a_Block))
			{
				case BLOCK_FACE_NORTH: a_Position.x++; break;
				case BLOCK_FACE_WEST:  a_Position.z--; break;
				case BLOCK_FACE_EAST:  a_Position.z++; break;
				case BLOCK_FACE_SOUTH: a_Position.x--; break;
				default:
				{
					LOGWARNING("%s: Unknown facing: %s", __FUNCTION__, BlockFaceToString(Comparator::Facing(a_Block)));
					ASSERT(!"Unknown facing while determining orientation of comparator!");
					break;
				}
			}
		}
		else
		{
			switch (Comparator::Facing(a_Block))
			{
				case BLOCK_FACE_NORTH: a_Position.x--; break;
				case BLOCK_FACE_WEST:  a_Position.z++; break;
				case BLOCK_FACE_EAST:  a_Position.z--; break;
				case BLOCK_FACE_SOUTH: a_Position.x++; break;
				default:
				{
					LOGWARNING("%s: Unknown facing: %s", __FUNCTION__, BlockFaceToString(Comparator::Facing(a_Block)));
					ASSERT(!"Unknown facing while determining orientation of comparator!");
					break;
				}
			}
		}

		return a_Position;
	}

	inline static Vector3i GetRearCoordinate(Vector3i a_Position, BlockState a_Block)
	{
		using namespace Block;
		switch (Comparator::Facing(a_Block))
		{
			case BLOCK_FACE_NORTH: a_Position.z--; break;
			case BLOCK_FACE_WEST:  a_Position.x--; break;
			case BLOCK_FACE_EAST:  a_Position.x++; break;
			case BLOCK_FACE_SOUTH: a_Position.z++; break;
			default:
			{
				LOGWARNING("%s: Unknown metadata: %s", __FUNCTION__, BlockFaceToString(Comparator::Facing(a_Block)));
				ASSERT(!"Unknown metadata while determining orientation of comparator!");
				break;
			}
		}

		return a_Position;
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

		using namespace Block;
		switch (Comparator::Mode(Self))
		{
			case Comparator::Mode::Compare:  a_ChunkInterface.SetBlock(a_BlockPos, Comparator::Comparator(Comparator::Facing(Self), Comparator::Mode::Subtract, Comparator::Powered(Self))); break;
			case Comparator::Mode::Subtract: a_ChunkInterface.SetBlock(a_BlockPos, Comparator::Comparator(Comparator::Facing(Self), Comparator::Mode::Compare, Comparator::Powered(Self))); break;
		}
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
		UNUSED(a_BlockFace);

		a_WorldInterface.WakeUpSimulators(a_BlockPos);
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


		auto Below = a_Chunk.GetBlock(a_Position.addedY(-1));

		if (cBlockInfo::FullyOccupiesVoxel(Below))
		{
			return true;
		}

		// upside down slabs
		if (cBlockSlabHandler::IsAnySlabType(Below))
		{
			// Check if the slab is turned up side down or double slab
			if (cBlockSlabHandler::IsSlabFull(Below) || cBlockSlabHandler::IsSlabTop(Below))
			{
				return true;
			}
		}

		// upside down stairs
		else if (cBlockStairsHandler::IsBlockStairs(Below) && cBlockStairsHandler::IsStairsTopHalf(Below))
		{
			return true;
		}

		return false;
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		return cItem(Item::Comparator);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 11;
	}
} ;




