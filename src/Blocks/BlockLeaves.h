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
			case BlockType::AzaleaLeaves:
			case BlockType::CherryLeaves:
			case BlockType::MangroveLeaves:
			case BlockType::FloweringAzaleaLeaves:
			case BlockType::PaleOakLeaves:
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
			case BlockType::AzaleaLeaves:  return AzaleaLeaves::Distance(a_Block);
			case BlockType::CherryLeaves:  return CherryLeaves::Distance(a_Block);
			case BlockType::MangroveLeaves:return MangroveLeaves::Distance(a_Block);
			case BlockType::FloweringAzaleaLeaves:return FloweringAzaleaLeaves::Distance(a_Block);
			case BlockType::PaleOakLeaves: return PaleOakLeaves::Distance(a_Block);
			default: return 0;
		}
	}


	static inline BlockState SetLeaveDistance(BlockState a_Block, unsigned char a_Distance)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaLeaves:  return AcaciaLeaves::AcaciaLeaves   (a_Distance, AcaciaLeaves::Persistent(a_Block), AcaciaLeaves::Waterlogged(a_Block));
			case BlockType::BirchLeaves:   return BirchLeaves::BirchLeaves     (a_Distance, BirchLeaves::Persistent(a_Block), BirchLeaves::Waterlogged(a_Block));
			case BlockType::DarkOakLeaves: return DarkOakLeaves::DarkOakLeaves (a_Distance, DarkOakLeaves::Persistent(a_Block), DarkOakLeaves::Waterlogged(a_Block));
			case BlockType::JungleLeaves:  return JungleLeaves::JungleLeaves   (a_Distance, JungleLeaves::Persistent(a_Block), JungleLeaves::Waterlogged(a_Block));
			case BlockType::OakLeaves:     return OakLeaves::OakLeaves         (a_Distance, OakLeaves::Persistent(a_Block), OakLeaves::Waterlogged(a_Block));
			case BlockType::SpruceLeaves:  return SpruceLeaves::SpruceLeaves   (a_Distance, SpruceLeaves::Persistent(a_Block), SpruceLeaves::Waterlogged(a_Block));
			case BlockType::AzaleaLeaves:  return AzaleaLeaves::AzaleaLeaves   (a_Distance, AzaleaLeaves::Persistent(a_Block), AzaleaLeaves::Waterlogged(a_Block));
			case BlockType::CherryLeaves:  return CherryLeaves::CherryLeaves   (a_Distance, CherryLeaves::Persistent(a_Block), CherryLeaves::Waterlogged(a_Block));
			case BlockType::MangroveLeaves:return MangroveLeaves::MangroveLeaves(a_Distance, MangroveLeaves::Persistent(a_Block), MangroveLeaves::Waterlogged(a_Block));
			case BlockType::FloweringAzaleaLeaves:return FloweringAzaleaLeaves::FloweringAzaleaLeaves(a_Distance, FloweringAzaleaLeaves::Persistent(a_Block), FloweringAzaleaLeaves::Waterlogged(a_Block));
			case BlockType::PaleOakLeaves:  return PaleOakLeaves::PaleOakLeaves   (a_Distance, PaleOakLeaves::Persistent(a_Block), PaleOakLeaves::Waterlogged(a_Block));
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
			case BlockType::AzaleaLeaves:  return AzaleaLeaves::Persistent(a_Block);
			case BlockType::CherryLeaves:  return CherryLeaves::Persistent(a_Block);
			case BlockType::MangroveLeaves:return MangroveLeaves::Persistent(a_Block);
			case BlockType::FloweringAzaleaLeaves:return FloweringAzaleaLeaves::Persistent(a_Block);
			case BlockType::PaleOakLeaves: return PaleOakLeaves::Persistent(a_Block);
			default: return false;
		}
	}

	static inline BlockState setLeafPersistance(BlockState a_Block, bool a_IsPersistant)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaLeaves:  return AcaciaLeaves::AcaciaLeaves   (AcaciaLeaves::Distance(a_Block),  a_IsPersistant, AcaciaLeaves::Waterlogged(a_Block));
			case BlockType::BirchLeaves:   return BirchLeaves::BirchLeaves     (BirchLeaves::Distance(a_Block),   a_IsPersistant, BirchLeaves::Waterlogged(a_Block));
			case BlockType::DarkOakLeaves: return DarkOakLeaves::DarkOakLeaves (DarkOakLeaves::Distance(a_Block), a_IsPersistant, DarkOakLeaves::Waterlogged(a_Block));
			case BlockType::JungleLeaves:  return JungleLeaves::JungleLeaves   (JungleLeaves::Distance(a_Block),  a_IsPersistant, JungleLeaves::Waterlogged(a_Block));
			case BlockType::OakLeaves:     return OakLeaves::OakLeaves         (OakLeaves::Distance(a_Block),     a_IsPersistant, OakLeaves::Waterlogged(a_Block));
			case BlockType::SpruceLeaves:  return SpruceLeaves::SpruceLeaves   (SpruceLeaves::Distance(a_Block),  a_IsPersistant, SpruceLeaves::Waterlogged(a_Block));
			case BlockType::AzaleaLeaves:  return AzaleaLeaves::AzaleaLeaves   (AzaleaLeaves::Distance(a_Block),  a_IsPersistant, AzaleaLeaves::Waterlogged(a_Block));
			case BlockType::CherryLeaves:  return CherryLeaves::CherryLeaves   (CherryLeaves::Distance(a_Block),  a_IsPersistant, CherryLeaves::Waterlogged(a_Block));
			case BlockType::MangroveLeaves:return MangroveLeaves::MangroveLeaves(MangroveLeaves::Distance(a_Block), a_IsPersistant, MangroveLeaves::Waterlogged(a_Block));
			case BlockType::FloweringAzaleaLeaves:return FloweringAzaleaLeaves::FloweringAzaleaLeaves(FloweringAzaleaLeaves::Distance(a_Block), a_IsPersistant, FloweringAzaleaLeaves::Waterlogged(a_Block));
			case BlockType::PaleOakLeaves: return PaleOakLeaves::PaleOakLeaves   (PaleOakLeaves::Distance(a_Block),  a_IsPersistant, PaleOakLeaves::Waterlogged(a_Block));
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
		if ((a_Tool != nullptr) && (a_Tool->m_ItemType == Item::Shears))
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
			case BlockType::CherryLeaves:
			{
				if (Random.RandBool(DropProbability))
				{
					Res.Add(Item::CherrySapling);
				}
				break;
			}
			case BlockType::PaleOakLeaves:
			{
				if (Random.RandBool(DropProbability))
				{
					Res.Add(Item::PaleOakSapling);
				}
				break;
			}
			// todo azlea trees
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



	/** Returns true if the area contains a continous path from the specified block to a log block entirely made out of leaves blocks. */
	static bool HasNearLog(cBlockArea & a_Area, const Vector3i a_BlockPos)
	{
		// Filter the blocks into a {leaves, log, other (air)} set:
		auto * Types = a_Area.GetBlocks();
		for (size_t i = a_Area.GetBlockCount() - 1; i > 0; i--)
		{
			switch (Types[i].Type())
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
				case BlockType::CherryLog:
				case BlockType::MangroveLog:
				case BlockType::AzaleaLeaves:
				case BlockType::FloweringAzaleaLeaves:
				case BlockType::PaleOakLeaves:
				{
					break;
				}
				default:
				{
					Types[i] = Block::Air::Air();
					break;
				}
			}
		}  // for i - Types[]

		// Perform a breadth-first search to see if there's a log connected within 4 blocks of the leaves block:
		// Simply replace all reachable leaves blocks with a sponge block plus iteration (in the Area) and see if we can reach a log
		a_Area.SetBlock(a_BlockPos, Block::Sponge::Sponge());
		for (int i = 0; i < LEAVES_CHECK_DISTANCE; i++)
		{
			auto ProcessNeighbor = [&a_Area, i](int cbx, int cby, int cbz) -> bool
			{
				switch (a_Area.GetBlock({cbx, cby, cbz}).Type())
				{
					case BlockType::AcaciaLeaves:
					case BlockType::BirchLeaves:
					case BlockType::DarkOakLeaves:
					case BlockType::JungleLeaves:
					case BlockType::OakLeaves:
					case BlockType::SpruceLeaves:
					case BlockType::CherryLeaves:
					case BlockType::MangroveLeaves:
					case BlockType::FloweringAzaleaLeaves:
					case BlockType::PaleOakLeaves:
						a_Area.SetBlock({cbx, cby, cbz}, BlockState(static_cast<uint_least16_t>(Block::Sponge::Sponge().ID + i + 1))); break;
					case BlockType::AcaciaLog:
					case BlockType::BirchLog:
					case BlockType::DarkOakLog:
					case BlockType::JungleLog:
					case BlockType::OakLog:
					case BlockType::SpruceLog:
					case BlockType::CherryLog:
					case BlockType::MangroveLog:
					case BlockType::PaleOakLog: return true;
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
						if (a_Area.GetBlock({x, y, z}).ID != Block::Sponge::Sponge().ID + i)
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
							return true;
						}
					}  // for x
				}  // for z
			}  // for y
		}  // for i - BFS iterations
		return false;
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
		auto WorldPos = a_Chunk.RelativeToAbsolute(a_RelPos);
		cBlockArea Area;
		if (!Area.Read(
			*a_Chunk.GetWorld(),
			WorldPos - Vector3i(LEAVES_CHECK_DISTANCE, LEAVES_CHECK_DISTANCE, LEAVES_CHECK_DISTANCE),
			WorldPos + Vector3i(LEAVES_CHECK_DISTANCE, LEAVES_CHECK_DISTANCE, LEAVES_CHECK_DISTANCE),
			cBlockArea::baBlocks)
		)
		{
			// Cannot check leaves, a chunk is missing too close
			return;
		}

		if (HasNearLog(Area, WorldPos))
		{
			return;
		}

		// Decay the leaves:
		a_ChunkInterface.DropBlockAsPickups(WorldPos);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 7;
	}
} ;
