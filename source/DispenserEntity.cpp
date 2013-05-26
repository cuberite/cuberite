
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "DispenserEntity.h"
#include "Player.h"
#include "Simulator/FluidSimulator.h"





cDispenserEntity::cDispenserEntity(int a_BlockX, int a_BlockY, int a_BlockZ) :
	super(E_BLOCK_DISPENSER, a_BlockX, a_BlockY, a_BlockZ, NULL)
{
	SetBlockEntity(this);  // cBlockEntityWindowOwner
}





cDispenserEntity::cDispenserEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_DISPENSER, a_BlockX, a_BlockY, a_BlockZ, a_World)
{
	SetBlockEntity(this);  // cBlockEntityWindowOwner
}





void cDispenserEntity::DropSpenseFromSlot(int a_SlotNum)
{
	int DispX = m_PosX;
	int DispY = m_PosY;
	int DispZ = m_PosZ;
	NIBBLETYPE Meta = m_World->GetBlockMeta(m_PosX, m_PosY, m_PosZ);
	AddDropSpenserDir(DispX, DispY, DispZ, Meta);

	// Dispense the item:
	switch (m_Contents.GetSlot(a_SlotNum).m_ItemType)
	{
		case E_ITEM_BUCKET:
		{
			BLOCKTYPE DispBlock = m_World->GetBlock(DispX, DispY, DispZ);
			switch (DispBlock)
			{
				case E_BLOCK_STATIONARY_WATER:
				case E_BLOCK_WATER:
				{
					if (ScoopUpLiquid(a_SlotNum, E_ITEM_WATER_BUCKET))
					{
						m_World->SetBlock(DispX, DispY, DispZ, E_BLOCK_AIR, 0);
					}
					break;
				}
				case E_BLOCK_STATIONARY_LAVA:
				case E_BLOCK_LAVA:
				{
					if (ScoopUpLiquid(a_SlotNum, E_ITEM_LAVA_BUCKET))
					{
						m_World->SetBlock(DispX, DispY, DispZ, E_BLOCK_AIR, 0);
					}
					break;
				}
				default:
				{
					DropFromSlot(a_SlotNum);
					break;
				}
			}
			break;
		}  // E_ITEM_BUCKET
		
		case E_ITEM_WATER_BUCKET:
		{
			BLOCKTYPE DispBlock = m_World->GetBlock(DispX, DispY, DispZ);
			if (PlaceLiquid(DispBlock, a_SlotNum))
			{
				m_World->SetBlock(DispX, DispY, DispZ, E_BLOCK_WATER, 0);
			}
			else
			{
				DropFromSlot(a_SlotNum);
			}
			break;
		}
		
		case E_ITEM_LAVA_BUCKET:
		{
			BLOCKTYPE DispBlock = m_World->GetBlock(DispX, DispY, DispZ);
			if (PlaceLiquid(DispBlock, a_SlotNum))
			{
				m_World->SetBlock(DispX, DispY, DispZ, E_BLOCK_LAVA, 0);
			}
			else
			{
				DropFromSlot(a_SlotNum);
			}
			break;
		}
		
		case E_ITEM_SPAWN_EGG:
		{
			if (m_World->SpawnMob(DispX + 0.5, DispY, DispZ + 0.5, m_Contents.GetSlot(a_SlotNum).m_ItemDamage) >= 0)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}
		
		default:
		{
			DropFromSlot(a_SlotNum);
			break;
		}
	}  // switch (ItemType)
}






bool cDispenserEntity::ScoopUpLiquid(int a_SlotNum, short a_BucketItemType)
{
	cItem LiquidBucket(a_BucketItemType, 1);
	if (m_Contents.GetSlot(a_SlotNum).m_ItemCount == 1)
	{
		// Special case: replacing one empty bucket with one full bucket
		m_Contents.SetSlot(a_SlotNum, LiquidBucket);
		return true;
	}
	
	// There are stacked buckets at the selected slot, see if a full bucket will fit somewhere else
	if (m_Contents.HowManyCanFit(LiquidBucket) < 1)
	{
		// Cannot fit into m_Contents
		return false;
	}
	
	m_Contents.ChangeSlotCount(a_SlotNum, -1);
	m_Contents.AddItem(LiquidBucket);
	return true;
}





bool cDispenserEntity::PlaceLiquid(BLOCKTYPE a_BlockInFront, int a_SlotNum)
{
	if (
		(a_BlockInFront != E_BLOCK_AIR) &&
		!IsBlockLiquid(a_BlockInFront) &&
		!cFluidSimulator::CanWashAway(a_BlockInFront)
	)
	{
		// Not a suitable block in front
		return false;
	}
	
	cItem EmptyBucket(E_ITEM_BUCKET, 1);
	if (m_Contents.GetSlot(a_SlotNum).m_ItemCount == 1)
	{
		// Change the single full bucket present into a single empty bucket
		m_Contents.SetSlot(a_SlotNum, EmptyBucket);
		return true;
	}
	
	// There are full buckets stacked at this slot, check if we can fit in the empty bucket
	if (m_Contents.HowManyCanFit(EmptyBucket) < 1)
	{
		// The empty bucket wouldn't fit into m_Contents
		return false;
	}
	
	// The empty bucket fits in, remove one full bucket and add the empty one
	m_Contents.ChangeSlotCount(a_SlotNum, -1);
	m_Contents.AddItem(EmptyBucket);
	return true;
}




