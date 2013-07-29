#pragma once
#include "BlockHandler.h"
#include "../MersenneTwister.h"
#include "../World.h"
#include "../BlockArea.h"





// Leaves can be this many blocks that away (inclusive) from the log not to decay
#define LEAVES_CHECK_DISTANCE 6

#define PROCESS_NEIGHBOR(x,y,z) \
	switch (a_Area.GetBlockType(x, y, z)) \
	{ \
		case E_BLOCK_LEAVES: a_Area.SetBlockType(x, y, z, (BLOCKTYPE)(E_BLOCK_SPONGE + i + 1)); break; \
		case E_BLOCK_LOG: return true; \
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
		MTRand rand;

		// Only the first 2 bits contain the display information, the others are for growing
		if (rand.randInt(5) == 0)
		{
			a_Pickups.push_back(cItem(E_BLOCK_SAPLING, 1, a_BlockMeta & 3));
		}
		if ((a_BlockMeta & 3) == E_META_SAPLING_APPLE)
		{
			if (rand.rand(100) == 0)
			{
				a_Pickups.push_back(cItem(E_ITEM_RED_APPLE, 1, 0));
			}
		}
	}


	void OnDestroyed(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		cBlockHandler::OnDestroyed(a_World, a_BlockX, a_BlockY, a_BlockZ);
		
		//0.5% chance of dropping an apple
		NIBBLETYPE Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		//check if Oak (0x1 and 0x2 bit not set)
		MTRand rand;
		if(!(Meta & 3) && rand.randInt(200) == 100)
		{
			cItems Drops;
			Drops.push_back(cItem(E_ITEM_RED_APPLE, 1, 0));
			a_World->SpawnItemPickups(Drops, a_BlockX, a_BlockY, a_BlockZ);
		}
	}
	
	
	virtual void OnNeighborChanged(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		NIBBLETYPE Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		a_World->SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta & 0x7);	 // Unset 0x8 bit so it gets checked for decay
	}
	
	
	virtual void OnUpdate(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		NIBBLETYPE Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
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
		cBlockArea Area;
		if (!Area.Read(
			a_World, 
			a_BlockX - LEAVES_CHECK_DISTANCE, a_BlockX + LEAVES_CHECK_DISTANCE, 
			a_BlockY - LEAVES_CHECK_DISTANCE, a_BlockY + LEAVES_CHECK_DISTANCE, 
			a_BlockZ - LEAVES_CHECK_DISTANCE, a_BlockZ + LEAVES_CHECK_DISTANCE, 
			cBlockArea::baTypes)
		)
		{
			// Cannot check leaves, a chunk is missing too close
			return;
		}

		if (HasNearLog(Area, a_BlockX, a_BlockY, a_BlockZ))
		{
			// Wood found, the leaves stay; mark them as checked:
			a_World->SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta | 0x8);
			return;
		}
		// Decay the leaves:
		DropBlock(a_World, NULL, a_BlockX, a_BlockY, a_BlockZ);

		a_World->DigBlock(a_BlockX, a_BlockY, a_BlockZ);
		
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
	for (int i = a_Area.GetBlockCount() - 1; i > 0; i--)
	{
		switch (Types[i])
		{
			case E_BLOCK_LEAVES:
			case E_BLOCK_LOG:
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




