
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

	cItem & Drop = m_Contents.GetSlot(a_SlotNum);
	
	// Dispense the item:
	switch (Drop.m_ItemType)
	{
		case E_ITEM_BUCKET:
		{
			BLOCKTYPE DispBlock = m_World->GetBlock(DispX, DispY, DispZ);
			if (DispBlock == E_BLOCK_STATIONARY_WATER)
			{
				m_World->SetBlock(DispX, DispY, DispZ, E_BLOCK_AIR, 0);
				Drop.m_ItemType = E_ITEM_WATER_BUCKET;  // TODO: Duplication glitch - bucket stacking allows you to duplicate water
			}
			else if (DispBlock == E_BLOCK_STATIONARY_LAVA)
			{
				m_World->SetBlock(DispX, DispY, DispZ, E_BLOCK_AIR, 0);
				Drop.m_ItemType = E_ITEM_LAVA_BUCKET;  // TODO: Duplication glitch - bucket stacking allows you to duplicate lava
			}
			else
			{
				cItems Pickups;
				Pickups.push_back(Drop.CopyOne());
				m_World->SpawnItemPickups(Pickups, DispX, DispY, DispZ);
				Drop.m_ItemCount--;
			}
			break;
		}
		
		case E_ITEM_WATER_BUCKET:
		{
			BLOCKTYPE DispBlock = m_World->GetBlock(DispX, DispY, DispZ);
			if ((DispBlock == E_BLOCK_AIR) || IsBlockLiquid(DispBlock) || cFluidSimulator::CanWashAway(DispBlock))
			{
				m_World->SetBlock(DispX, DispY, DispZ, E_BLOCK_STATIONARY_WATER, 0);
				Drop.m_ItemType = E_ITEM_BUCKET;
			}
			else
			{
				cItems Pickups;
				Pickups.push_back(Drop.CopyOne());
				m_World->SpawnItemPickups(Pickups, DispX, DispY, DispZ);
				Drop.m_ItemCount--;
			}
			break;
		}
		
		case E_ITEM_LAVA_BUCKET:
		{
			BLOCKTYPE DispBlock = m_World->GetBlock( DispX, DispY, DispZ );
			if ((DispBlock == E_BLOCK_AIR) || IsBlockLiquid(DispBlock) || cFluidSimulator::CanWashAway(DispBlock))
			{
				m_World->SetBlock(DispX, DispY, DispZ, E_BLOCK_STATIONARY_LAVA, 0);
				Drop.m_ItemType = E_ITEM_BUCKET;
			}
			else
			{
				cItems Pickups;
				Pickups.push_back(Drop.CopyOne());
				m_World->SpawnItemPickups(Pickups, DispX, DispY, DispZ);
				Drop.m_ItemCount--;
			}
			break;
		}
		
		case E_ITEM_SPAWN_EGG:
		{
			if (m_World->SpawnMob(DispX + 0.5, DispY, DispZ + 0.5, Drop.m_ItemDamage) >= 0)
			{
				Drop.m_ItemCount--;
			}
			break;
		}
		
		default:
		{
			cItems Pickups;
			Pickups.push_back(Drop.CopyOne());
			m_World->SpawnItemPickups(Pickups, DispX, DispY, DispZ);
			Drop.m_ItemCount--;
			break;
		}
	}  // switch (ItemType)
}





