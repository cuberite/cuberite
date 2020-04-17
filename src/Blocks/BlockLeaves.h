#pragma once
#include "BlockHandler.h"
#include "../FastRandom.h"
#include "../BlockArea.h"





// Leaves can be this many blocks that away (inclusive) from the log not to decay
#define LEAVES_CHECK_DISTANCE 6





class cBlockLeavesHandler:
	public cBlockHandler
{
	using Super = cBlockHandler;

	/** Returns true if the area contains a continous path from the specified block to a log block entirely made out of leaves blocks. */
	static bool HasNearLog(cBlockArea & a_Area, const Vector3i a_BlockPos)
	{
		// Filter the blocks into a {leaves, log, other (air)} set:
		auto * Types = a_Area.GetBlockTypes();
		for (size_t i = a_Area.GetBlockCount() - 1; i > 0; i--)
		{
			switch (Types[i])
			{
				case E_BLOCK_LEAVES:
				case E_BLOCK_LOG:
				case E_BLOCK_NEW_LEAVES:
				case E_BLOCK_NEW_LOG:
				{
					break;
				}
				default:
				{
					Types[i] = E_BLOCK_AIR;
					break;
				}
			}
		}  // for i - Types[]

		// Perform a breadth-first search to see if there's a log connected within 4 blocks of the leaves block:
		// Simply replace all reachable leaves blocks with a sponge block plus iteration (in the Area) and see if we can reach a log
		a_Area.SetBlockType(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, E_BLOCK_SPONGE);
		for (int i = 0; i < LEAVES_CHECK_DISTANCE; i++)
		{
			auto ProcessNeighbor = [&a_Area, i](int cbx, int cby, int cbz) -> bool
			{
				switch (a_Area.GetBlockType(cbx, cby, cbz))
				{
					case E_BLOCK_LEAVES: a_Area.SetBlockType(cbx, cby, cbz, static_cast<BLOCKTYPE>(E_BLOCK_SPONGE + i + 1)); break;
					case E_BLOCK_LOG: return true;
					case E_BLOCK_NEW_LEAVES: a_Area.SetBlockType(cbx, cby, cbz, static_cast<BLOCKTYPE>(E_BLOCK_SPONGE + i + 1)); break;
					case E_BLOCK_NEW_LOG: return true;
				}
				return false;
			};
			for (int y = std::max(a_BlockPos.y - i, 0); y <= std::min(a_BlockPos.y + i, cChunkDef::Height - 1); y++)
			{
				for (int z = a_BlockPos.z - i; z <= a_BlockPos.z + i; z++)
				{
					for (int x = a_BlockPos.x - i; x <= a_BlockPos.x + i; x++)
					{
						if (a_Area.GetBlockType(x, y, z) != E_BLOCK_SPONGE + i)
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


public:

	cBlockLeavesHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// If breaking with shears, drop self:
		if ((a_Tool != nullptr) && (a_Tool->m_ItemType == E_ITEM_SHEARS))
		{
			return cItem(m_BlockType, a_BlockMeta & 0x03);
		}


		// There is a chance to drop a sapling that varies depending on the type of leaf broken.
		// Note: It is possible (though very rare) for a single leaves block to drop both a sapling and an apple
		// TODO: Take into account fortune for sapling drops.
		double chance = 0.0;
		auto & rand = GetRandomProvider();
		cItems res;
		if ((m_BlockType == E_BLOCK_LEAVES) && ((a_BlockMeta & 0x03) == E_META_LEAVES_JUNGLE))
		{
			// Jungle leaves have a 2.5% chance of dropping a sapling.
			chance = 0.025;
		}
		else
		{
			// Other leaves have a 5% chance of dropping a sapling.
			chance = 0.05;
		}
		if (rand.RandBool(chance))
		{
			res.Add(
				E_BLOCK_SAPLING,
				1,
				(m_BlockType == E_BLOCK_LEAVES) ? (a_BlockMeta & 0x03) : static_cast<short>(4 + (a_BlockMeta & 0x01))
			);
		}

		// 0.5 % chance of dropping an apple, if the leaves' type is Apple Leaves
		if ((m_BlockType == E_BLOCK_LEAVES) && ((a_BlockMeta & 0x03) == E_META_LEAVES_APPLE))
		{
			if (rand.RandBool(0.005))
			{
				res.Add(E_ITEM_RED_APPLE, 1, 0);
			}
		}
		return res;
	}





	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) override
	{
		auto meta = a_ChunkInterface.GetBlockMeta(a_BlockPos);

		// Set bit 0x08, so this block gets checked for decay:
		if ((meta & 0x08) == 0)
		{
			a_ChunkInterface.SetBlockMeta(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, meta | 0x8, true, false);
		}
	}





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) override
	{
		auto Meta = a_Chunk.GetMeta(a_RelPos);
		if ((Meta & 0x04) != 0)
		{
			// Player-placed leaves, don't decay
			return;
		}

		if ((Meta & 0x08) == 0)
		{
			// These leaves have been checked for decay lately and nothing around them changed
			return;
		}

		// Get the data around the leaves:
		auto worldPos = a_Chunk.RelativeToAbsolute(a_RelPos);
		cBlockArea Area;
		if (!Area.Read(
			*a_Chunk.GetWorld(),
			worldPos - Vector3i(LEAVES_CHECK_DISTANCE, LEAVES_CHECK_DISTANCE, LEAVES_CHECK_DISTANCE),
			worldPos + Vector3i(LEAVES_CHECK_DISTANCE, LEAVES_CHECK_DISTANCE, LEAVES_CHECK_DISTANCE),
			cBlockArea::baTypes)
		)
		{
			// Cannot check leaves, a chunk is missing too close
			return;
		}

		if (HasNearLog(Area, worldPos))
		{
			// Wood found, the leaves stay; unset the check bit
			a_Chunk.SetMeta(a_RelPos, Meta ^ 0x08, true, false);
			return;
		}

		// Decay the leaves:
		a_ChunkInterface.DropBlockAsPickups(worldPos);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;
