
#pragma once

#include "../BlockArea.h"
#include "../Entities/Player.h"
#include "Mixins/Mixins.h"





class cBlockChestHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

	/** This enum is used to unify the type of both chests for better readability. */
	enum class ChestType : char
	{
		Left,
		Single,
		Right
	};

	/** Sets the orientation of the chest. -1 = left, 0 = single, 1 = right. */
	static inline BlockState SetChestOrientation(BlockState a_Block, ChestType a_Orientation)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Chest:        return Chest::Chest              (Chest::Facing(a_Block),        a_Orientation == ChestType::Left ? Chest::Type::Left :        a_Orientation == ChestType::Right ? Chest::Type::Right        : Chest::Type::Single, Chest::Waterlogged(a_Block));
			case BlockType::TrappedChest: return TrappedChest::TrappedChest(TrappedChest::Facing(a_Block), a_Orientation == ChestType::Left ? TrappedChest::Type::Left : a_Orientation == ChestType::Right ? TrappedChest::Type::Right : TrappedChest::Type::Single, Chest::Waterlogged(a_Block));
			default: return a_Block;
		}
	}

	static inline ChestType GetChestOrientation(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Chest:
			{
				switch (Chest::Type(a_Block))
				{
					case Chest::Type::Single: return ChestType::Single;
					case Chest::Type::Left:   return ChestType::Left;
					case Chest::Type::Right:  return ChestType::Right;
				}
				break;
			}
			case BlockType::TrappedChest:
			{
				switch (TrappedChest::Type(a_Block))
				{
					case TrappedChest::Type::Single: return ChestType::Single;
					case TrappedChest::Type::Left:   return ChestType::Left;
					case TrappedChest::Type::Right:  return ChestType::Right;
				}
				break;
			}
			default: return ChestType::Single;
		}
		UNREACHABLE("Block is not a chest");
	}

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		auto BlockPos = a_Chunk.RelativeToAbsolute(a_Position);
		auto ChunkInterface = cChunkInterface(a_Chunk.GetWorld()->GetChunkMap());
		return CanBeAt(ChunkInterface, BlockPos);
	}





	bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_BlockPos) const
	{
		cBlockArea Area;
		if (!Area.Read(a_ChunkInterface, a_BlockPos - Vector3i(2, 0, 2), a_BlockPos + Vector3i(2, 0, 2)))
		{
			// Cannot read the surroundings, probably at the edge of loaded chunks. Disallow.
			return false;
		}

		int NumChestNeighbors = 0;
		if (Area.GetRelBlock({1, 0, 2}).Type() == m_BlockType)
		{
			if (
				(Area.GetRelBlock({0, 0, 2}).Type() == m_BlockType) ||
				(Area.GetRelBlock({1, 0, 1}).Type() == m_BlockType) ||
				(Area.GetRelBlock({1, 0, 3}).Type() == m_BlockType)
			)
			{
				// Already a doublechest neighbor, disallow:
				return false;
			}
			NumChestNeighbors += 1;
		}
		if (Area.GetRelBlock({3, 0, 2}).Type() == m_BlockType)
		{
			if (
				(Area.GetRelBlock({4, 0, 2}).Type() == m_BlockType) ||
				(Area.GetRelBlock({3, 0, 1}).Type() == m_BlockType) ||
				(Area.GetRelBlock({3, 0, 3}).Type() == m_BlockType)
			)
			{
				// Already a doublechest neighbor, disallow:
				return false;
			}
			NumChestNeighbors += 1;
		}
		if (Area.GetRelBlock({2, 0, 1}).Type() == m_BlockType)
		{
			if (
				(Area.GetRelBlock({2, 0, 0}).Type() == m_BlockType) ||
				(Area.GetRelBlock({1, 0, 1}).Type() == m_BlockType) ||
				(Area.GetRelBlock({3, 0, 1}).Type() == m_BlockType)
			)
			{
				// Already a doublechest neighbor, disallow:
				return false;
			}
			NumChestNeighbors += 1;
		}
		if (Area.GetRelBlock({2, 0, 3}).Type() == m_BlockType)
		{
			if (
				(Area.GetRelBlock({2, 0, 4}).Type() == m_BlockType) ||
				(Area.GetRelBlock({1, 0, 3}).Type() == m_BlockType) ||
				(Area.GetRelBlock({3, 0, 3}).Type() == m_BlockType)
			)
			{
				// Already a doublechest neighbor, disallow:
				return false;
			}
			NumChestNeighbors += 1;
		}
		return (NumChestNeighbors < 2);
	}



/*

	// If there's a chest in the a_Area in the specified coords, modifies its meta to a_NewMeta and returns true.
	bool CheckAndAdjustNeighbor(cChunkInterface & a_ChunkInterface, const cBlockArea & a_Area, int a_RelX, int a_RelZ, NIBBLETYPE a_NewMeta) const
	{
		if (a_Area.GetRelBlockType(a_RelX, 0, a_RelZ) != m_BlockType)
		{
			return false;
		}
		a_ChunkInterface.SetBlockMeta(a_Area.GetOriginX() + a_RelX, a_Area.GetOriginY(), a_Area.GetOriginZ() + a_RelZ, a_NewMeta);
		return true;
	}

*/
	virtual ColourID GetMapBaseColourID() const override
	{
		return 13;
	}
} ;
