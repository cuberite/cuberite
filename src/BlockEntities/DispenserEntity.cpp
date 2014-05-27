
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "DispenserEntity.h"
#include "../Entities/Player.h"
#include "../Simulator/FluidSimulator.h"
#include "../Chunk.h"

#include "../World.h"
#include "../Entities/ArrowEntity.h"
#include "../Entities/FireChargeEntity.h"
#include "../Entities/ProjectileEntity.h"
#include "../Matrix4.h"




cDispenserEntity::cDispenserEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_DISPENSER, a_BlockX, a_BlockY, a_BlockZ, a_World)
{
	SetBlockEntity(this);  // cBlockEntityWindowOwner
}





void cDispenserEntity::DropSpenseFromSlot(cChunk & a_Chunk, int a_SlotNum)
{
	int DispX = m_RelX;
	int DispY = m_PosY;
	int DispZ = m_RelZ;
	NIBBLETYPE Meta = a_Chunk.GetMeta(m_RelX, m_PosY, m_RelZ);
	AddDropSpenserDir(DispX, DispY, DispZ, Meta);
	cChunk * DispChunk = a_Chunk.GetRelNeighborChunkAdjustCoords(DispX, DispZ);
	if (DispChunk == NULL)
	{
		// Would dispense into / interact with a non-loaded chunk, ignore the tick
		return;
	}
	BLOCKTYPE DispBlock = DispChunk->GetBlock(DispX, DispY, DispZ);

	// Dispense the item:
	switch (m_Contents.GetSlot(a_SlotNum).m_ItemType)
	{
		case E_ITEM_BUCKET:
		{
			LOGD("Dispensing empty bucket in slot %d; DispBlock is \"%s\" (%d).", a_SlotNum, ItemTypeToString(DispBlock).c_str(), DispBlock);
			switch (DispBlock)
			{
				case E_BLOCK_STATIONARY_WATER:
				case E_BLOCK_WATER:
				{
					if (ScoopUpLiquid(a_SlotNum, E_ITEM_WATER_BUCKET))
					{
						DispChunk->SetBlock(DispX, DispY, DispZ, E_BLOCK_AIR, 0);
					}
					break;
				}
				case E_BLOCK_STATIONARY_LAVA:
				case E_BLOCK_LAVA:
				{
					if (ScoopUpLiquid(a_SlotNum, E_ITEM_LAVA_BUCKET))
					{
						DispChunk->SetBlock(DispX, DispY, DispZ, E_BLOCK_AIR, 0);
					}
					break;
				}
				default:
				{
					DropFromSlot(a_Chunk, a_SlotNum);
					break;
				}
			}
			break;
		}  // E_ITEM_BUCKET

		case E_ITEM_WATER_BUCKET:
		{
			LOGD("Dispensing water bucket in slot %d; DispBlock is \"%s\" (%d).", a_SlotNum, ItemTypeToString(DispBlock).c_str(), DispBlock);
			if (EmptyLiquidBucket(DispBlock, a_SlotNum))
			{
				DispChunk->SetBlock(DispX, DispY, DispZ, E_BLOCK_WATER, 0);
			}
			else
			{
				DropFromSlot(a_Chunk, a_SlotNum);
			}
			break;
		}

		case E_ITEM_LAVA_BUCKET:
		{
			LOGD("Dispensing lava bucket in slot %d; DispBlock is \"%s\" (%d).", a_SlotNum, ItemTypeToString(DispBlock).c_str(), DispBlock);
			if (EmptyLiquidBucket(DispBlock, a_SlotNum))
			{
				DispChunk->SetBlock(DispX, DispY, DispZ, E_BLOCK_LAVA, 0);
			}
			else
			{
				DropFromSlot(a_Chunk, a_SlotNum);
			}
			break;
		}

		case E_ITEM_SPAWN_EGG:
		{
			double MobX = 0.5 + (DispX + DispChunk->GetPosX() * cChunkDef::Width);
			double MobZ = 0.5 + (DispZ + DispChunk->GetPosZ() * cChunkDef::Width);
			if (m_World->SpawnMob(MobX, DispY, MobZ, (cMonster::eType)m_Contents.GetSlot(a_SlotNum).m_ItemDamage) >= 0)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_BLOCK_TNT:
		{
			// Spawn a primed TNT entity, if space allows:
			if (DispChunk->GetBlock(DispX, DispY, DispZ) == E_BLOCK_AIR)
			{
				double TNTX = 0.5 + (DispX + DispChunk->GetPosX() * cChunkDef::Width);
				double TNTZ = 0.5 + (DispZ + DispChunk->GetPosZ() * cChunkDef::Width);
				m_World->SpawnPrimedTNT(TNTX, DispY + 0.5, TNTZ, 80, 0);  // 80 ticks fuse, no initial velocity
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_FLINT_AND_STEEL:
		{
			// Spawn fire if the block in front is air.
			if (DispChunk->GetBlock(DispX, DispY, DispZ) == E_BLOCK_AIR)
			{
				DispChunk->SetBlock(DispX, DispY, DispZ, E_BLOCK_FIRE, 0);

				bool ItemBroke = m_Contents.DamageItem(a_SlotNum, 1);

				if (ItemBroke)
				{
					m_Contents.ChangeSlotCount(a_SlotNum, -1);
				}
			}
			break;
		}

		case E_ITEM_FIRE_CHARGE:
		{
			SpawnProjectileFromDispenser(a_Chunk, DispX, DispY, DispZ, cProjectileEntity::pkFireCharge);

			break;
		}

		case E_ITEM_ARROW:
		{
			SpawnProjectileFromDispenser(a_Chunk, DispX, DispY, DispZ, cProjectileEntity::pkArrow);

			break;
		}

		case E_ITEM_SNOWBALL:
		{
			// Not working as there is no such entity yet?
			SpawnProjectileFromDispenser(a_Chunk, DispX, DispY, DispZ, cProjectileEntity::pkSnowball);

			break;
		}

		case E_ITEM_EGG:
		{
			// Not working as there is no such entity yet?
			SpawnProjectileFromDispenser(a_Chunk, DispX, DispY, DispZ, cProjectileEntity::pkEgg);

			break;
		}

		case E_ITEM_FIREWORK_ROCKET:
		{
			SpawnProjectileFromDispenser(a_Chunk, DispX, DispY, DispZ, cProjectileEntity::pkFirework);

			break;
		}

		default:
		{
			DropFromSlot(a_Chunk, a_SlotNum);
			break;
		}
	}  // switch (ItemType)
}



void cDispenserEntity::SpawnProjectileFromDispenser(cChunk & a_Chunk, int & DispX, int & DispY, int & DispZ, cProjectileEntity::eKind kind)
{
	Vector3d Speed = GetProjectileLookVector(a_Chunk);
	cChunk * DispChunk = a_Chunk.GetRelNeighborChunkAdjustCoords(DispX, DispZ);

	double EntityX = 0.5 + (DispX + DispChunk->GetPosX() * cChunkDef::Width);
	double EntityZ = 0.5 + (DispZ + DispChunk->GetPosZ() * cChunkDef::Width);

	m_World->CreateProjectile((double) EntityX, (double) DispY, (double) EntityZ, cProjectileEntity::pkArrow, NULL, NULL, & Speed);
}



Vector3d cDispenserEntity::GetProjectileLookVector(cChunk & a_Chunk)
{
	NIBBLETYPE Meta = a_Chunk.GetMeta(m_RelX, m_PosY, m_RelZ);
	int Direction = 0;
	switch (Meta)
	{
		case E_META_DROPSPENSER_FACING_YP: Direction = 0; break; // YP & YM don't have associated smoke dirs, just do 4 (centre of block)
		case E_META_DROPSPENSER_FACING_YM: Direction = 0; break;
		case E_META_DROPSPENSER_FACING_XM: Direction = 90; break; // WEST
		case E_META_DROPSPENSER_FACING_XP: Direction = 270; break; // EAST
		case E_META_DROPSPENSER_FACING_ZM: Direction = 180; break;
		case E_META_DROPSPENSER_FACING_ZP: Direction = 0; break;
	}

	Matrix4d m;
	m.Init(Vector3d(), 0, Direction, 0);
	Vector3d Look = m.Transform(Vector3d(0, 0, 1));

	Vector3d Speed = Look * 20;
	Speed.y = Speed.y + 1;

	return Speed;
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





bool cDispenserEntity::EmptyLiquidBucket(BLOCKTYPE a_BlockInFront, int a_SlotNum)
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




