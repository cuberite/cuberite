
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Simulator/FluidSimulator.h"
#include "../Blocks/BlockHandler.h"





class cItemBucketHandler :
	public cItemHandler
{
public:
	cItemBucketHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{

	}

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		switch (m_ItemType)
		{
			case E_ITEM_BUCKET:       return ScoopUpFluid(a_World, a_Player, a_Item, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
			case E_ITEM_LAVA_BUCKET:  return PlaceFluid  (a_World, a_Player, a_Item, a_BlockX, a_BlockY, a_BlockZ, a_Dir, E_BLOCK_LAVA);
			case E_ITEM_WATER_BUCKET: return PlaceFluid  (a_World, a_Player, a_Item, a_BlockX, a_BlockY, a_BlockZ, a_Dir, E_BLOCK_WATER);
			default:
			{
				ASSERT(!"Unhandled ItemType");
				return false;
			}
		}
	}
	
	
	
	bool ScoopUpFluid(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace)
	{
		if (a_BlockFace < 0)
		{
			return false;
		}
		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
		BLOCKTYPE ClickedBlock;
		NIBBLETYPE ClickedMeta;
		a_World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, ClickedBlock, ClickedMeta);
		LOGD("Bucket Clicked BlockType %d, meta %d", ClickedBlock, ClickedMeta);
		if (ClickedMeta != 0)
		{
			// Not a source block
			return false;
		}
		
		if (a_Player->GetGameMode() == gmCreative)
		{
			// In creative mode don't modify the inventory, just remove the fluid:
			a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
			return true;
		}
		
		ENUM_ITEM_ID NewItem = E_ITEM_EMPTY;
		switch (ClickedBlock)
		{
			case E_BLOCK_WATER:
			case E_BLOCK_STATIONARY_WATER:
			{
				NewItem = E_ITEM_WATER_BUCKET;
				break;
			}
			case E_BLOCK_LAVA:
			case E_BLOCK_STATIONARY_LAVA:
			{
				NewItem = E_ITEM_LAVA_BUCKET;
				break;
			}
			
			default: return false;
		}
		
		// Remove the bucket from the inventory
		if (!a_Player->GetInventory().RemoveOneEquippedItem())
		{
			LOG("Clicked with an empty bucket, but cannot remove one from the inventory? WTF?");
			ASSERT(!"Inventory bucket mismatch");
			return true;
		}
		
		// Give new bucket, filled with fluid:
		cItem Item(NewItem, 1);
		a_Player->GetInventory().AddItem(Item, true, true);

		// Remove water / lava block
		a_Player->GetWorld()->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
		return true;
	}


	bool PlaceFluid(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_FluidBlock)
	{			
		if (a_BlockFace < 0)
		{
			return false;
		}
		
		BLOCKTYPE CurrentBlock = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		bool CanWashAway = cFluidSimulator::CanWashAway(CurrentBlock);
		if (!CanWashAway)
		{
			// The block pointed at cannot be washed away, so put fluid on top of it / on its sides
			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
			CurrentBlock = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		}
		if (
			!CanWashAway && 
			(CurrentBlock != E_BLOCK_AIR) && 
			(CurrentBlock != E_BLOCK_WATER) &&
			(CurrentBlock != E_BLOCK_STATIONARY_WATER) &&
			(CurrentBlock != E_BLOCK_LAVA) &&
			(CurrentBlock != E_BLOCK_STATIONARY_LAVA)
		)
		{
			// Cannot place water here
			return false;
		}
		
		if (a_Player->GetGameMode() != gmCreative)
		{
			// Remove fluid bucket, add empty bucket:
			if (!a_Player->GetInventory().RemoveOneEquippedItem())
			{
				LOG("Clicked with a full bucket, but cannot remove one from the inventory? WTF?");
				ASSERT(!"Inventory bucket mismatch");
				return false;
			}
			cItem Item(E_ITEM_BUCKET, 1);
			if (!a_Player->GetInventory().AddItem(Item,true,true))
			{
				return false;
			}
		}
		
		// Wash away anything that was there prior to placing:
		if (CanWashAway)
		{
			cBlockHandler * Handler = BlockHandler(CurrentBlock);
			if (Handler->DoesDropOnUnsuitable())
			{
				Handler->DropBlock(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ);
			}
		}

		a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, a_FluidBlock, 0);
		
		return true;
	}

};
