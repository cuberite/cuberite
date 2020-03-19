#pragma once
#include "BlockHandler.h"
#include "../FastRandom.h"
#include "../BlockArea.h"





// Leaves can be this many blocks that away (inclusive) from the log not to decay
#define LEAVES_CHECK_DISTANCE 6

#define PROCESS_NEIGHBOR(x, y, z) \
	do { \
		switch (a_Area.GetBlockType(x, y, z)) \
		{ \
			case E_BLOCK_LEAVES: a_Area.SetBlockType(x, y, z, static_cast<BLOCKTYPE>(E_BLOCK_SPONGE + i + 1)); break; \
			case E_BLOCK_LOG: return true; \
			case E_BLOCK_NEW_LEAVES: a_Area.SetBlockType(x, y, z, static_cast<BLOCKTYPE>(E_BLOCK_SPONGE + i + 1)); break; \
			case E_BLOCK_NEW_LOG: return true; \
		} \
	} while (false)

bool HasNearLog(cBlockArea &a_Area, int a_BlockX, int a_BlockY, int a_BlockZ);





class cBlockLeavesHandler :
	public cBlockHandler
{
public:
	cBlockLeavesHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
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





	virtual void OnUpdate(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		NIBBLETYPE Meta = a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ);
		if ((Meta & 0x04) != 0)
		{
			// Player-placed leaves, don't decay
			return;
		}

		if ((Meta & 0x8) == 0)
		{
			// These leaves have been checked for decay lately and nothing around them changed
			return;
		}

		// Get the data around the leaves:
		int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
		int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
		cBlockArea Area;
		if (!Area.Read(
			*a_Chunk.GetWorld(),
			BlockX - LEAVES_CHECK_DISTANCE, BlockX + LEAVES_CHECK_DISTANCE,
			a_RelY - LEAVES_CHECK_DISTANCE, a_RelY + LEAVES_CHECK_DISTANCE,
			BlockZ - LEAVES_CHECK_DISTANCE, BlockZ + LEAVES_CHECK_DISTANCE,
			cBlockArea::baTypes)
		)
		{
			// Cannot check leaves, a chunk is missing too close
			return;
		}

		if (HasNearLog(Area, BlockX, a_RelY, BlockZ))
		{
			// Wood found, the leaves stay; unset the check bit
			a_Chunk.SetMeta(a_RelX, a_RelY, a_RelZ, Meta ^ 0x08, true, false);
			return;
		}

		// Decay the leaves:
		a_ChunkInterface.DropBlockAsPickups({BlockX, a_RelY, BlockZ});
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;





bool HasNearLog(cBlockArea & a_Area, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Filter the blocks into a {leaves, log, other (air)} set:
	BLOCKTYPE * Types = a_Area.GetBlockTypes();
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
	// Simply replace all reachable leaves blocks with a sponge block plus iteration (in the Area) and see if we can reach a log in 4 iterations
	a_Area.SetBlockType(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_SPONGE);
	for (int i = 0; i < LEAVES_CHECK_DISTANCE; i++)
	{
		for (int y = std::max(a_BlockY - i, 0); y <= std::min(a_BlockY + i, cChunkDef::Height - 1); y++)
		{
			for (int z = a_BlockZ - i; z <= a_BlockZ + i; z++)
			{
				for (int x = a_BlockX - i; x <= a_BlockX + i; x++)
				{
					if (a_Area.GetBlockType(x, y, z) != E_BLOCK_SPONGE + i)
					{
						continue;
					}
					PROCESS_NEIGHBOR(x - 1, y,     z);
					PROCESS_NEIGHBOR(x + 1, y,     z);
					PROCESS_NEIGHBOR(x,     y,     z - 1);
					PROCESS_NEIGHBOR(x,     y,     z + 1);
					PROCESS_NEIGHBOR(x,     y + 1, z);
					PROCESS_NEIGHBOR(x,     y - 1, z);
				}  // for x
			}  // for z
		}  // for y
	}  // for i - BFS iterations
	return false;
}




