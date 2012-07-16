#pragma once
#include "Block.h"
#include "../MersenneTwister.h"
#include "../cWorld.h"
#include "../BlockArea.h"



// Leaves can be this many blocks that away (inclusive) from the log not to decay
#define LEAVES_CHECK_DISTANCE 6

#define PROCESS_NEIGHBOR(x,y,z) \
	switch (a_Area.GetBlockType(x, y, z)) \
	{ \
		case E_BLOCK_LEAVES: a_Area.SetBlockType(x, y, z, E_BLOCK_SPONGE + i + 1); break; \
		case E_BLOCK_LOG: return true; \
	}

bool HasNearLog(cBlockArea &a_Area, int a_BlockX, int a_BlockY, int a_BlockZ);



class cBlockLeavesHandler : public cBlockHandler
{
public:
	cBlockLeavesHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual int GetDropID() override
	{
		MTRand rand;

		if(rand.randInt(5) == 0)
		{
			return E_ITEM_SAPLING;
		}
		
		return E_ITEM_EMPTY;
	}

	void OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z) override
	{
		cBlockHandler::OnDestroyed(a_World, a_X, a_Y, a_Z);
		
		//0.5% chance of dropping an apple
		NIBBLETYPE Meta = a_World->GetBlockMeta(a_X, a_Y, a_Z);
		//check if Oak (0x1 and 0x2 bit not set)
		MTRand rand;
		if(!(Meta & 3) && rand.randInt(200) == 100)
		{
			cItems Drops;
			Drops.push_back(cItem(E_ITEM_RED_APPLE, 1, 0));
			a_World->SpawnItemPickups(Drops, a_X, a_Y, a_Z);
		}
	}
	
	virtual void OnNeighborChanged(cWorld *a_World, int a_X, int a_Y, int a_Z) override
	{
		NIBBLETYPE Meta = a_World->GetBlockMeta(a_X, a_Y, a_Z);
		a_World->SetBlockMeta(a_X, a_Y, a_Z, Meta & 0x7);	//Unset 0x8 bit so it gets checked for decay
	}
	
	virtual bool NeedsRandomTicks() override
	{
		return true;
	}
	
	virtual void OnUpdate(cWorld *a_World, int a_X, int a_Y, int a_Z) override
	{
		NIBBLETYPE Meta = a_World->GetBlockMeta(a_X, a_Y, a_Z);
		if ((Meta & 0x04) != 0)
		{
			// Player-placed leaves, don't decay
			return;
		}

		if (Meta & 0x8)
		{
			// These leaves have been checked for decay lately and nothing around them changed
			return;
		}

		// Get the data around the leaves:
		cBlockArea Area;
		if (!Area.Read(
			a_World, 
			a_X - LEAVES_CHECK_DISTANCE, a_X + LEAVES_CHECK_DISTANCE, 
			a_Y - LEAVES_CHECK_DISTANCE, a_Y + LEAVES_CHECK_DISTANCE, 
			a_Z - LEAVES_CHECK_DISTANCE, a_Z + LEAVES_CHECK_DISTANCE, 
			cBlockArea::baTypes)
		)
		{
			// Cannot check leaves, a chunk is missing too close
			return;
		}

		if (HasNearLog(Area, a_X, a_Y, a_Z))
		{
			// Wood found, the leaves stay; mark them as checked:
			a_World->SetBlockMeta(a_X, a_Y, a_Z, Meta | 0x8);
			return;
		}
		// Decay the leaves:
		DropBlock(a_World, a_X, a_Y, a_Z);

		a_World->DigBlock(a_X, a_Y, a_Z);
		
	}
};


bool HasNearLog(cBlockArea &a_Area, int a_BlockX, int a_BlockY, int a_BlockZ)
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

