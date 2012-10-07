
// BlockFarmland.h

// Declares the cBlcokFarmlandHandler representing the block handler for farmland





#pragma once

#include "BlockHandler.h"
#include "../BlockArea.h"





class cBlockFarmlandHandler :
	public cBlockHandler
{
	typedef cBlockHandler super;
	
public:
	cBlockFarmlandHandler(void) :
		super(E_BLOCK_FARMLAND)
	{
	}


	virtual void OnUpdate(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		// TODO: Rain hydrates farmland, too. Check world weather, don't search for water if raining.
		// NOTE: The desert biomes do not get precipitation, so another check needs to be made.
		
		// Search for water in a close proximity:
		// Ref.: http://www.minecraftwiki.net/wiki/Farmland#Hydrated_Farmland_Tiles
		cBlockArea Area;
		if (!Area.Read(a_World, a_BlockX - 4, a_BlockX + 4, a_BlockY, a_BlockY + 1, a_BlockZ - 4, a_BlockZ + 4))
		{
			// Too close to the world edge, cannot check surroudnings; don't tick at all
			return;
		}
		bool Found = false;
		int NumBlocks = Area.GetBlockCount();
		BLOCKTYPE * BlockTypes = Area.GetBlockTypes();
		for (int i = 0; i < NumBlocks; i++)
		{
			if (
				(BlockTypes[i] == E_BLOCK_WATER) ||
				(BlockTypes[i] == E_BLOCK_STATIONARY_WATER)
			)
			{
				Found = true;
				break;
			}
		}
		
		NIBBLETYPE BlockMeta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		
		if (Found)
		{
			// Water was found, hydrate the block until hydration reaches 7:
			if (BlockMeta < 7)
			{
				a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, ++BlockMeta);
			}
			return;
		}

		// Water wasn't found, de-hydrate block:
		if (BlockMeta > 0)
		{
			a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_FARMLAND, --BlockMeta);
			return;
		}
		
		// Farmland too dry. If nothing is growing on top, turn back to dirt:
		switch (a_World->GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ))
		{
			case E_BLOCK_CROPS:
			case E_BLOCK_MELON_STEM:
			case E_BLOCK_PUMPKIN_STEM:
			{
				// Produce on top, don't revert
				break;
			}
			default:
			{
				a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_DIRT, 0);
				break;
			}
		}
	}
} ;




