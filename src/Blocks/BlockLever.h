#pragma once

#include "BlockHandler.h"
#include "../Chunk.h"
#include "Blocks/BlockStairs.h"
#include "ChunkDef.h"
#include "Defines.h"
#include "Mixins/Mixins.h"
#include "BlockSlab.h"


class cBlockLeverHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	/** Extracts the ON bit from metadata and returns if true if it is set */
	static bool IsLeverOn(BlockState a_Block)
	{
		return Block::Lever::Powered(a_Block);
	}

	static inline void Flip(cChunkInterface & a_ChunkInterface, Vector3i a_Position)
	{
		auto Self = a_ChunkInterface.GetBlock(a_Position);
		Self = Block::Lever::Lever(Block::Lever::Face(Self), Block::Lever::Facing(Self), !Block::Lever::Powered(Self));
		a_ChunkInterface.FastSetBlock(a_Position, Self);
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
		Flip(a_ChunkInterface, a_BlockPos);
		a_WorldInterface.WakeUpSimulators(a_BlockPos);
		a_WorldInterface.GetBroadcastManager().BroadcastSoundEffect("block.lever.click", a_BlockPos, 0.5f, IsLeverOn(a_ChunkInterface.GetBlock(a_BlockPos)) ? 0.6f : 0.5f);
		return true;
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// Reset meta to zero:
		return cItem(Item::Lever, 1, 0);
	}





	virtual bool IsUseable(void) const override
	{
		return true;
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		// Find the type of block the lever is attached to:
		eBlockFace NeighbourFace = eBlockFace::BLOCK_FACE_NONE;
		switch (Block::Lever::Face(a_Self))
		{
			case Block::Lever::Face::Floor:   NeighbourFace = BLOCK_FACE_YP; break;
			case Block::Lever::Face::Ceiling: NeighbourFace = BLOCK_FACE_YM; break;
			case Block::Lever::Face::Wall:    NeighbourFace = Block::Lever::Facing(a_Self); break;
		}
		auto NeighborPos = AddFaceDirection(a_Position, NeighbourFace, true);
		if (!cChunkDef::IsValidHeight(NeighborPos))
		{
			return false;
		}

		BlockState Neighbour;
		if (!a_Chunk.UnboundedRelGetBlock(NeighborPos, Neighbour))
		{
			return false;
		}

		// Allow any full block or the "good" side of a half-slab:
		if (cBlockInfo::FullyOccupiesVoxel(Neighbour))
		{
			return true;
		}

		else if (cBlockSlabHandler::IsAnySlabType(Neighbour))
		{
			switch (Block::Lever::Face(a_Self))
			{
				case Block::Lever::Face::Floor:   return cBlockSlabHandler::IsSlabTop(Neighbour);
				case Block::Lever::Face::Wall:    return false;
				case Block::Lever::Face::Ceiling: return !cBlockSlabHandler::IsSlabTop(Neighbour);
			}
		}
		/* else if (cBlockStairsHandler::IsAnyStairType(Neighbour))
		{
			switch (NeighbourFace)
			{
				UNREACHABLE("unimplemented");
				case eBlockFace::BLOCK_FACE_YM:
					return !(NeighborMeta & E_BLOCK_STAIRS_UPSIDE_DOWN);
				case eBlockFace::BLOCK_FACE_YP:
					return (NeighborMeta & E_BLOCK_STAIRS_UPSIDE_DOWN);
				case eBlockFace::BLOCK_FACE_XP:
					return ((NeighborMeta & 0b11) == E_BLOCK_STAIRS_XP);
				case eBlockFace::BLOCK_FACE_XM:
					return ((NeighborMeta & 0b11) == E_BLOCK_STAIRS_XM);
				case eBlockFace::BLOCK_FACE_ZP:
					return ((NeighborMeta & 0b11) == E_BLOCK_STAIRS_ZP);
				case eBlockFace::BLOCK_FACE_ZM:
					return ((NeighborMeta & 0b11) == E_BLOCK_STAIRS_ZM);
				default:
				{
					return false;
				}
			}
		} */
		return false;
	}





	virtual BlockState RotateCCW(BlockState a_Block) const override
	{
		using namespace Block;
		return Lever::Lever(Lever::Face(a_Block), RotateBlockFaceCCW(Lever::Facing(a_Block)), Lever::Powered(a_Block));
	}





	virtual BlockState RotateCW(BlockState a_Block) const override
	{
		using namespace Block;
		return Lever::Lever(Lever::Face(a_Block), RotateBlockFaceCW(Lever::Facing(a_Block)), Lever::Powered(a_Block));
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;




