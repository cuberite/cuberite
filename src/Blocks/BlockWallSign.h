
#pragma once

#include "BlockHandler.h"
#include "../Chunk.h"
#include "../BlockInfo.h"
#include "../Registries/BlockItemConverter.h"


class cBlockWallSignHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsBlockWallSign(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaWallSign:
			case BlockType::BirchWallSign:
			case BlockType::CrimsonWallSign:
			case BlockType::DarkOakWallSign:
			case BlockType::OakWallSign:
			case BlockType::JungleWallSign:
			case BlockType::WarpedWallSign:
			case BlockType::SpruceWallSign:
			case BlockType::BambooWallSign:
			case BlockType::CherryWallSign:
			case BlockType::MangroveWallSign:
			case BlockType::PaleOakWallSign:
				return true;
			default: return false;
		}
	}

	static inline bool IsBlockSignPost(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSign:
			case BlockType::BirchSign:
			case BlockType::CrimsonSign:
			case BlockType::DarkOakSign:
			case BlockType::JungleSign:
			case BlockType::OakSign:
			case BlockType::SpruceSign:
			case BlockType::WarpedSign:
			case BlockType::BambooWallSign:
			case BlockType::CherryWallSign:
			case BlockType::MangroveWallSign:
			case BlockType::PaleOakWallSign:
				return true;
			default: return false;
		}
	}

	static inline eBlockFace GetSignFacing(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaWallSign:  return AcaciaWallSign::Facing(a_Block);
			case BlockType::BirchWallSign:   return BirchWallSign::Facing(a_Block);
			case BlockType::CrimsonWallSign: return CrimsonWallSign::Facing(a_Block);
			case BlockType::DarkOakWallSign: return DarkOakWallSign::Facing(a_Block);
			case BlockType::OakWallSign:     return OakWallSign::Facing(a_Block);
			case BlockType::JungleWallSign:  return JungleWallSign::Facing(a_Block);
			case BlockType::WarpedWallSign:  return WarpedWallSign::Facing(a_Block);
			case BlockType::SpruceWallSign:  return SpruceWallSign::Facing(a_Block);
			case BlockType::BambooWallSign:  return BambooWallSign::Facing(a_Block);
			case BlockType::CherryWallSign:  return CherryWallSign::Facing(a_Block);
			case BlockType::MangroveWallSign:  return MangroveWallSign::Facing(a_Block);
			case BlockType::PaleOakWallSign: return PaleOakWallSign::Facing(a_Block);
			default: return eBlockFace::BLOCK_FACE_NONE;
		}
	}

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		return cItem(BlockItemConverter::FromBlock(m_BlockType));
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		auto NeighborPos = a_Position + GetOffsetBehindTheSign(a_Self);
		BlockState Neighbor;
		if (!a_Chunk.UnboundedRelGetBlock(NeighborPos, Neighbor))
		{
			// The neighbor is not accessible (unloaded chunk), bail out without changing this
			return true;
		}
		return (IsBlockWallSign(Neighbor) || IsBlockSignPost(Neighbor) || cBlockInfo::IsSolid(Neighbor));
	}





	/** Returns the offset from the sign coords to the block to which the wallsign is attached, based on the wallsign's block meta.
	Asserts / returns a zero vector on wrong meta. */
	static Vector3i GetOffsetBehindTheSign(BlockState a_Block)
	{
		switch (GetSignFacing(a_Block))
		{
			case BLOCK_FACE_XM: return Vector3i( 1, 0,  0);
			case BLOCK_FACE_XP: return Vector3i(-1, 0,  0);
			case BLOCK_FACE_ZM: return Vector3i( 0, 0,  1);
			case BLOCK_FACE_ZP: return Vector3i( 0, 0, -1);
			default:
			{
				ASSERT(!"Invalid wallsign block facing");
				return Vector3i();
			}
		}
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 13;
	}
} ;




