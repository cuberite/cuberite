#pragma once
#include "BlockHandler.h"
#include "../FastRandom.h"
#include "../World.h"
#include "../BlockArea.h"





// Leaves can be this many blocks that away (inclusive) from the log not to decay
#define LEAVES_CHECK_DISTANCE 6

#define PROCESS_NEIGHBOR(x, y, z) \
	switch (a_Area.GetBlockType(x, y, z)) \
	{ \
		case E_BLOCK_LEAVES: a_Area.SetBlockType(x, y, z, (BLOCKTYPE)(E_BLOCK_SPONGE + i + 1)); break; \
		case E_BLOCK_LOG: return true; \
		case E_BLOCK_NEW_LEAVES: a_Area.SetBlockType(x, y, z, (BLOCKTYPE)(E_BLOCK_SPONGE + i + 1)); break; \
		case E_BLOCK_NEW_LOG: return true; \
	}

bool HasNearLog(cBlockArea &a_Area, int a_BlockX, int a_BlockY, int a_BlockZ);





class cBlockLeavesHandler :
	public cBlockHandler
{
public:
	cBlockLeavesHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		cFastRandom rand;

		// Old leaves - 3 bits contain display; new leaves - 1st bit, shifted left two for saplings to understand
		if (rand.NextInt(6) == 0)
		{
			a_Pickups.push_back(
				cItem(
					E_BLOCK_SAPLING,
					1,
					(m_BlockType == E_BLOCK_LEAVES) ? (a_BlockMeta & 0x03) : (2 << (a_BlockMeta & 0x01))
				)
			);
		}
		
		// 1 % chance of dropping an apple, if the leaves' type is Apple Leaves
		if ((m_BlockType == E_BLOCK_LEAVES) && ((a_BlockMeta & 0x03) == E_META_LEAVES_APPLE))
		{
			if (rand.NextInt(101) == 0)
			{
				a_Pickups.push_back(cItem(E_ITEM_RED_APPLE, 1, 0));
			}
		}
	}


	void OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		cBlockHandler::OnDestroyed(a_ChunkInterface, a_WorldInterface, a_BlockX, a_BlockY, a_BlockZ);
		
		// 0.5% chance of dropping an apple, if the leaves' type is Apple Leaves:
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		cFastRandom rand;
		if (((Meta & 3) == E_META_LEAVES_APPLE) && (rand.NextInt(201) == 100))
		{
			cItems Drops;
			Drops.push_back(cItem(E_ITEM_RED_APPLE, 1, 0));
			a_WorldInterface.SpawnItemPickups(Drops, a_BlockX, a_BlockY, a_BlockZ);
		}
	}
	
	
	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta & 0x7);  // Unset 0x8 bit so it gets checked for decay
	}
	
	
	virtual void OnUpdate(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		NIBBLETYPE Meta = a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ);
		if ((Meta & 0x04) != 0)
		{
			// Player-placed leaves, don't decay
			return;
		}

		if ((Meta & 0x8) != 0)
		{
			// These leaves have been checked for decay lately and nothing around them changed
			return;
		}

		// Get the data around the leaves:
		int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
		int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
		cBlockArea Area;
		if (!Area.Read(
			a_Chunk.GetWorld(),
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
			// Wood found, the leaves stay; mark them as checked:
			a_Chunk.SetMeta(a_RelX, a_RelY, a_RelZ, Meta | 0x8);
			return;
		}

		// Decay the leaves:
		DropBlock(a_ChunkInterface, a_WorldInterface, a_PluginInterface, NULL, BlockX, a_RelY, BlockZ);
		a_ChunkInterface.DigBlock(a_WorldInterface, BlockX, a_RelY, BlockZ);
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
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
		for (int y = a_BlockY - i; y <= a_BlockY + i; y++)
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




