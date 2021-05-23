
#pragma once

#include "../BlockArea.h"
#include "../Entities/Player.h"
#include "Mixins.h"





class cBlockChestHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

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
			case BlockType::Chest:        return Chest::Chest              (Chest::Facing(a_Block),        a_Orientation == ChestType::Left ? Chest::Type::Left :        a_Orientation == ChestType::Right ? Chest::Type::Right        : Chest::Type::Single);
			case BlockType::TrappedChest: return TrappedChest::TrappedChest(TrappedChest::Facing(a_Block), a_Orientation == ChestType::Left ? TrappedChest::Type::Left : a_Orientation == ChestType::Right ? TrappedChest::Type::Right : TrappedChest::Type::Single);
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
			}
			case BlockType::TrappedChest:
			{

			}
			default: return ChestType::Single;
		}
	}

private:

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) const override
	{
		// Cannot place right next to double-chest:
		if (!CanBeAt(a_ChunkInterface, a_PlacedBlockPos))
		{
			// Yup, cannot form a triple-chest, refuse:
			return false;
		}

		// Try to read double-chest information:
		cBlockArea Area;
		if (!Area.Read(a_ChunkInterface, a_PlacedBlockPos - Vector3i(1, 0, 1), a_PlacedBlockPos + Vector3i(1, 0, 1)))
		{
			return false;
		}

		auto Facing = RotationToBlockFace(a_Player.GetYaw());
		/*
		x is the placed chest
			| 0 | 1 | 2 |
		---------------
		0   |   |   |   |
		---------------
		1   |   | x |   |
		---------------
		2   |   |   |   |
		*/

		using namespace Block;
		if (Area.GetRelBlock({0, 0, 1}).Type() == m_BlockType)
		{
			switch (m_BlockType)
			{
				case BlockType::Chest:        a_Block = Chest::Chest              (Facing, Chest::Type::Right); break;
				case BlockType::TrappedChest: a_Block = TrappedChest::TrappedChest(Facing, TrappedChest::Type::Right); break;
				default: return false;
			}
			a_ChunkInterface.FastSetBlock(a_PlacedBlockPos.addedZ(1), SetChestOrientation(a_ChunkInterface.GetBlock(a_PlacedBlockPos.addedZ(1)), ChestType::Left));
		}
		else if (Area.GetRelBlock({2, 0, 1}).Type() == m_BlockType)
		{
			switch (m_BlockType)
			{
				case BlockType::Chest:        a_Block = Chest::Chest              (Facing, Chest::Type::Left); break;
				case BlockType::TrappedChest: a_Block = TrappedChest::TrappedChest(Facing, TrappedChest::Type::Left); break;
				default: return false;
			}
			a_ChunkInterface.FastSetBlock(a_PlacedBlockPos.addedXZ(2, 1), SetChestOrientation(a_ChunkInterface.GetBlock(a_PlacedBlockPos.addedXZ(2, 1)), ChestType::Right));
		}
		else if (Area.GetRelBlock({1, 0, 0}).Type() == m_BlockType)
		{
			switch (m_BlockType)
			{
				case BlockType::Chest:        a_Block = Chest::Chest              (Facing, Chest::Type::Right); break;
				case BlockType::TrappedChest: a_Block = TrappedChest::TrappedChest(Facing, TrappedChest::Type::Right); break;
				default: return false;
			}
			a_ChunkInterface.FastSetBlock(a_PlacedBlockPos.addedX(1), SetChestOrientation(a_ChunkInterface.GetBlock(a_PlacedBlockPos.addedX(1)), ChestType::Left));
		}
		else if (Area.GetRelBlock({1, 0, 2}).Type() == m_BlockType)
		{
			switch (m_BlockType)
			{
				case BlockType::Chest:        a_Block = Chest::Chest              (Facing, Chest::Type::Left); break;
				case BlockType::TrappedChest: a_Block = TrappedChest::TrappedChest(Facing, TrappedChest::Type::Left); break;
				default: return false;
			}
			a_ChunkInterface.FastSetBlock(a_PlacedBlockPos.addedXZ(1, 2), SetChestOrientation(a_ChunkInterface.GetBlock(a_PlacedBlockPos.addedXZ(1, 2)), ChestType::Right));
		}
		else
		{
			switch (m_BlockType)
			{
				case BlockType::Chest:        a_Block = Chest::Chest              (Facing, Chest::Type::Single); break;
				case BlockType::TrappedChest: a_Block = TrappedChest::TrappedChest(Facing, TrappedChest::Type::Single); break;
				default: return false;
			}
		}
		return true;
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		auto BlockPos = a_Chunk.RelativeToAbsolute(a_RelPos);
		return CanBeAt(a_ChunkInterface, BlockPos);
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
