#pragma once
#include "BlockHandler.h"
#include "../FastRandom.h"
#include "../BlockArea.h"

#include "../Registries/BlockItemConverter.h"




// Leaves can be this many blocks that away (inclusive) from the log not to decay
#define LEAVES_CHECK_DISTANCE 6





class cBlockLeavesHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsBlockLeaves(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaLeaves:
			case BlockType::BirchLeaves:
			case BlockType::DarkOakLeaves:
			case BlockType::JungleLeaves:
			case BlockType::OakLeaves:
			case BlockType::SpruceLeaves:
				return true;
			default: return false;
		}
	}

	/** Returns the distance to the next block. If the block is not a Leaf this function return 0. Else a value from 1 - 7. */
	static inline unsigned char GetLeafDistance(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaLeaves:  return AcaciaLeaves::Distance(a_Block);
			case BlockType::BirchLeaves:   return BirchLeaves::Distance(a_Block);
			case BlockType::DarkOakLeaves: return DarkOakLeaves::Distance(a_Block);
			case BlockType::JungleLeaves:  return JungleLeaves::Distance(a_Block);
			case BlockType::OakLeaves:     return OakLeaves::Distance(a_Block);
			case BlockType::SpruceLeaves:  return SpruceLeaves::Distance(a_Block);
			default: return 0;
		}
	}


	static inline BlockState SetLeaveDistance(BlockState a_Block, unsigned char a_Distance)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaLeaves:  return AcaciaLeaves::AcaciaLeaves   (a_Distance, AcaciaLeaves::Persistent(a_Block));
			case BlockType::BirchLeaves:   return BirchLeaves::BirchLeaves     (a_Distance, BirchLeaves::Persistent(a_Block));
			case BlockType::DarkOakLeaves: return DarkOakLeaves::DarkOakLeaves (a_Distance, DarkOakLeaves::Persistent(a_Block));
			case BlockType::JungleLeaves:  return JungleLeaves::JungleLeaves   (a_Distance, JungleLeaves::Persistent(a_Block));
			case BlockType::OakLeaves:     return OakLeaves::OakLeaves         (a_Distance, OakLeaves::Persistent(a_Block));
			case BlockType::SpruceLeaves:  return SpruceLeaves::SpruceLeaves   (a_Distance, SpruceLeaves::Persistent(a_Block));
			default: return a_Block;
		}
	}

	static inline bool IsLeafPersistent(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaLeaves:  return AcaciaLeaves::Persistent(a_Block);
			case BlockType::BirchLeaves:   return BirchLeaves::Persistent(a_Block);
			case BlockType::DarkOakLeaves: return DarkOakLeaves::Persistent(a_Block);
			case BlockType::JungleLeaves:  return JungleLeaves::Persistent(a_Block);
			case BlockType::OakLeaves:     return OakLeaves::Persistent(a_Block);
			case BlockType::SpruceLeaves:  return SpruceLeaves::Persistent(a_Block);
			default: return false;
		}
	}

	static inline BlockState setLeafPersistance(BlockState a_Block, bool a_IsPersistant)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaLeaves:  return AcaciaLeaves::AcaciaLeaves   (AcaciaLeaves::Distance(a_Block),  a_IsPersistant);
			case BlockType::BirchLeaves:   return BirchLeaves::BirchLeaves     (BirchLeaves::Distance(a_Block),   a_IsPersistant);
			case BlockType::DarkOakLeaves: return DarkOakLeaves::DarkOakLeaves (DarkOakLeaves::Distance(a_Block), a_IsPersistant);
			case BlockType::JungleLeaves:  return JungleLeaves::JungleLeaves   (JungleLeaves::Distance(a_Block),  a_IsPersistant);
			case BlockType::OakLeaves:     return OakLeaves::OakLeaves         (OakLeaves::Distance(a_Block),     a_IsPersistant);
			case BlockType::SpruceLeaves:  return SpruceLeaves::SpruceLeaves   (SpruceLeaves::Distance(a_Block),  a_IsPersistant);
			default: return a_Block;
		}
	}



private:

	static double FortuneDropProbability(unsigned char a_DefaultDenominator, unsigned char a_FirstDenominatorReduction, unsigned char a_FortuneLevel)
	{
		// Fortune 3 behaves like fortune 4 for some reason
		if (a_FortuneLevel == 3)
		{
			a_FortuneLevel++;
		}

		// Denominator, capped at minimum of 10.
		const auto Denominator = std::max<unsigned char>(10, a_DefaultDenominator - a_FortuneLevel * a_FirstDenominatorReduction);
		return 1.0 / Denominator;
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// If breaking with shears, drop self:
		if ((a_Tool != nullptr) && (a_Tool->m_ItemType == E_ITEM_SHEARS))
		{
			return cItem(BlockItemConverter::FromBlock(m_BlockType));
		}


		// There is a chance to drop a sapling that varies depending on the type of leaf broken.
		// Note: It is possible (though very rare) for a single leaves block to drop both a sapling and an apple
		double DropProbability;
		const auto FortuneLevel = ToolFortuneLevel(a_Tool);
		auto & Random = GetRandomProvider();
		cItems Res;

		if (m_BlockType == BlockType::JungleLeaves)
		{
			// Jungle leaves have a 2.5% default chance of dropping a sapling.
			DropProbability = FortuneDropProbability(40, 4, FortuneLevel);
		}
		else
		{
			// Other leaves have a 5% default chance of dropping a sapling.
			DropProbability = FortuneDropProbability(20, 4, FortuneLevel);
		}

		switch (m_BlockType)
		{
			case BlockType::AcaciaLeaves:
			{
				if (Random.RandBool(DropProbability))
				{
					Res.Add(Item::AcaciaSapling);
				}
				break;
			}
			case BlockType::BirchLeaves:
			{
				if (Random.RandBool(DropProbability))
				{
					Res.Add(Item::BirchSapling);
				}
				break;
			}
			case BlockType::DarkOakLeaves:
			{
				if (Random.RandBool(DropProbability))
				{
					Res.Add(Item::DarkOakSapling);
				}
				break;
			}
			case BlockType::JungleLeaves:
			{
				if (Random.RandBool(DropProbability))
				{
					Res.Add(Item::JungleSapling);
				}
				break;
			}
			case BlockType::OakLeaves:
			{
				if (Random.RandBool(DropProbability))
				{
					Res.Add(Item::OakSapling);
				}

				// 0.5 % chance of dropping an apple, increased by fortune, if the leaves' type is Apple Leaves
				DropProbability = FortuneDropProbability(200, 20, FortuneLevel);
				if (Random.RandBool(DropProbability))
				{
					Res.Add(Item::Apple);
				}
				break;
			}
			case BlockType::SpruceLeaves:
			{
				if (Random.RandBool(DropProbability))
				{
					Res.Add(Item::SpruceSapling);
				}
				break;
			}
			default: return {};
		}

		// 2% chance of dropping sticks (yuck) in 1.14
		DropProbability = FortuneDropProbability(50, 5, FortuneLevel);
		if (Random.RandBool(DropProbability))
		{
			// 1 or 2 sticks are dropped on success:
			Res.Add(Item::Stick, Random.RandInt<char>(1, 2));
		}

		return Res;
	}





	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const override
	{
		using namespace Block;
		auto Self = a_ChunkInterface.GetBlock(a_BlockPos);

		switch (Self.Type())
		{  // TODO (12xx12) NONONO
			case BlockType::AcaciaLeaves:  a_ChunkInterface.FastSetBlock(a_BlockPos, AcaciaLeaves::AcaciaLeaves  (AcaciaLeaves::Distance(Self), false)); break;
			case BlockType::BirchLeaves:   a_ChunkInterface.FastSetBlock(a_BlockPos, BirchLeaves::BirchLeaves    (BirchLeaves::Distance(Self), false)); break;
			case BlockType::DarkOakLeaves: a_ChunkInterface.FastSetBlock(a_BlockPos, DarkOakLeaves::DarkOakLeaves(DarkOakLeaves::Distance(Self), false)); break;
			case BlockType::JungleLeaves:  a_ChunkInterface.FastSetBlock(a_BlockPos, JungleLeaves::JungleLeaves  (JungleLeaves::Distance(Self), false)); break;
			case BlockType::OakLeaves:     a_ChunkInterface.FastSetBlock(a_BlockPos, OakLeaves::OakLeaves        (OakLeaves::Distance(Self), false)); break;
			case BlockType::SpruceLeaves:  a_ChunkInterface.FastSetBlock(a_BlockPos, SpruceLeaves::SpruceLeaves  (SpruceLeaves::Distance(Self), false)); break;
			default: return;
		}
	}





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const override
	{
		auto Self = a_Chunk.GetBlock(a_RelPos);

		if (IsLeafPersistent(Self))
		{
			return;
		}

		// Get the data around the leaves:
		auto AbsPos = a_Chunk.RelativeToAbsolute(a_RelPos);

		// Get new distance from Log
		SetNewLeafDistance(a_Chunk.GetWorld(), AbsPos, Self);

		// If distance > breaking distance - replace with air
		if (GetLeafDistance(Self) <= LEAVES_CHECK_DISTANCE)
		{
			return;
		}
		// Decay the leaves:
		a_ChunkInterface.DropBlockAsPickups(AbsPos);
		return;
	}





	void SetNewLeafDistance(cWorld * a_World, Vector3i a_BlockPos, BlockState & a_Block) const
	{
		cBlockArea Area;
		if (!Area.Read(
			*a_World,
			a_BlockPos - Vector3i(LEAVES_CHECK_DISTANCE, LEAVES_CHECK_DISTANCE, LEAVES_CHECK_DISTANCE),
			a_BlockPos + Vector3i(LEAVES_CHECK_DISTANCE, LEAVES_CHECK_DISTANCE, LEAVES_CHECK_DISTANCE),
			cBlockArea::baBlocks)
			)
		{
			// Cannot check leaves, a chunk is missing too close
			return;
		}
		// Filter the blocks into a {leaves, log, other (air)} set:
		auto Types = Area.GetBlocks();
		for (size_t I = 0; I < Area.GetBlockCount(); I++)
		{
			switch (Types[I].Type())
			{
				case BlockType::AcaciaLeaves:
				case BlockType::BirchLeaves:
				case BlockType::DarkOakLeaves:
				case BlockType::JungleLeaves:
				case BlockType::OakLeaves:
				case BlockType::SpruceLeaves:

				case BlockType::AcaciaLog:
				case BlockType::BirchLog:
				case BlockType::DarkOakLog:
				case BlockType::JungleLog:
				case BlockType::OakLog:
				case BlockType::SpruceLog:
				{
					break;
				}
				default:
				{
					Types[I] = Block::Air::Air();
					break;
				}
			}
		}  // for i - Types[]

#define OffsetValue 10000

		// Perform a breadth-first search to see if there's a log connected within 6 blocks of the leaves block:
		// Simply replace all reachable leaves blocks with a sponge block plus iteration (in the Area) and see if we can reach a log
		Area.SetBlock(a_BlockPos, OffsetValue);
		for (unsigned char i = 0; i < LEAVES_CHECK_DISTANCE; i++)
		{
			auto ProcessNeighbor = [&Area, i](int X, int Y, int Z) -> bool
			{
				switch (Area.GetBlock({X, Y, Z}).Type())
				{
					case BlockType::AcaciaLeaves:
					case BlockType::BirchLeaves:
					case BlockType::DarkOakLeaves:
					case BlockType::JungleLeaves:
					case BlockType::OakLeaves:
					case BlockType::SpruceLeaves:
						Area.SetBlock({X, Y, Z}, BlockState(OffsetValue + i + 1)); break;
					case BlockType::AcaciaLog:
					case BlockType::BirchLog:
					case BlockType::DarkOakLog:
					case BlockType::JungleLog:
					case BlockType::OakLog:
					case BlockType::SpruceLog:
						return true;
					default: break;
				}
				return false;
			};
			for (int y = std::max(a_BlockPos.y - i, 0); y <= std::min(a_BlockPos.y + i, cChunkDef::Height - 1); y++)
			{
				for (int z = a_BlockPos.z - i; z <= a_BlockPos.z + i; z++)
				{
					for (int x = a_BlockPos.x - i; x <= a_BlockPos.x + i; x++)
					{
						if (Area.GetBlock({x, y, z}).ID != OffsetValue + i)
						{
							continue;
						}
						if (
							ProcessNeighbor(x - 1, y,     z) ||
							ProcessNeighbor(x + 1, y,     z) ||
							ProcessNeighbor(x,     y,     z - 1) ||
							ProcessNeighbor(x,     y,     z + 1) ||
							ProcessNeighbor(x,     y + 1, z) ||
							ProcessNeighbor(x,     y - 1, z)
							)
						{
							a_Block = SetLeaveDistance(a_Block, i);
							return;
						}
					}  // for x
				}  // for z
			}  // for y
		}  // for i - BFS iterations

		// No wood nearby - destroy the leaf
		a_Block = SetLeaveDistance(a_Block, LEAVES_CHECK_DISTANCE + 1);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 7;
	}
} ;
