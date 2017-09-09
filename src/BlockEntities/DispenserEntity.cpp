
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "DispenserEntity.h"
#include "../Simulator/FluidSimulator.h"
#include "../Entities/Boat.h"
#include "../Chunk.h"

#include "../Defines.h"
#include "../World.h"
#include "../Entities/ProjectileEntity.h"




cDispenserEntity::cDispenserEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, a_World)
{
	ASSERT(a_BlockType == E_BLOCK_DISPENSER);
}





void cDispenserEntity::DropSpenseFromSlot(cChunk & a_Chunk, int a_SlotNum)
{
	int DispX = m_RelX;
	int DispY = m_PosY;
	int DispZ = m_RelZ;
	NIBBLETYPE Meta = a_Chunk.GetMeta(m_RelX, m_PosY, m_RelZ);
	AddDropSpenserDir(DispX, DispY, DispZ, Meta);
	cChunk * DispChunk = a_Chunk.GetRelNeighborChunkAdjustCoords(DispX, DispZ);
	if (DispChunk == nullptr)
	{
		// Would dispense into / interact with a non-loaded chunk, ignore the tick
		return;
	}

	BLOCKTYPE DispBlock = DispChunk->GetBlock(DispX, DispY, DispZ);
	int BlockX = (DispX + DispChunk->GetPosX() * cChunkDef::Width);
	int BlockZ = (DispZ + DispChunk->GetPosZ() * cChunkDef::Width);

	// Dispense the item:
	const cItem & SlotItem = m_Contents.GetSlot(a_SlotNum);
	if (ItemCategory::IsMinecart(SlotItem.m_ItemType) && IsBlockRail(DispBlock))  // only actually place the minecart if there are rails!
	{
		if (m_World->SpawnMinecart(BlockX + 0.5, DispY + 0.5, BlockZ + 0.5, SlotItem.m_ItemType) != cEntity::INVALID_ID)
		{
			m_Contents.ChangeSlotCount(a_SlotNum, -1);
		}
		return;
	}
	switch (SlotItem.m_ItemType)
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
			if (m_World->SpawnMob(MobX, DispY, MobZ, static_cast<eMonsterType>(m_Contents.GetSlot(a_SlotNum).m_ItemDamage), false) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_BLOCK_TNT:
		{
			// Spawn a primed TNT entity, if space allows:
			if (!cBlockInfo::IsSolid(DispBlock))
			{
				double TNTX = 0.5 + (DispX + DispChunk->GetPosX() * cChunkDef::Width);
				double TNTZ = 0.5 + (DispZ + DispChunk->GetPosZ() * cChunkDef::Width);
				m_World->SpawnPrimedTNT({TNTX, DispY + 0.5, TNTZ}, 80, 0);  // 80 ticks fuse, no initial velocity
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_FLINT_AND_STEEL:
		{
			// Spawn fire if the block in front is air.
			if (DispBlock == E_BLOCK_AIR)
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
			if (SpawnProjectileFromDispenser(BlockX, DispY, BlockZ, cProjectileEntity::pkFireCharge, GetShootVector(Meta) * 20) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_ARROW:
		{
			if (SpawnProjectileFromDispenser(BlockX, DispY, BlockZ, cProjectileEntity::pkArrow, GetShootVector(Meta) * 30 + Vector3d(0, 1, 0)) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_SNOWBALL:
		{
			if (SpawnProjectileFromDispenser(BlockX, DispY, BlockZ, cProjectileEntity::pkSnowball, GetShootVector(Meta) * 20 + Vector3d(0, 1, 0)) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_EGG:
		{
			if (SpawnProjectileFromDispenser(BlockX, DispY, BlockZ, cProjectileEntity::pkEgg, GetShootVector(Meta) * 20 + Vector3d(0, 1, 0)) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_BOTTLE_O_ENCHANTING:
		{
			if (SpawnProjectileFromDispenser(BlockX, DispY, BlockZ, cProjectileEntity::pkExpBottle, GetShootVector(Meta) * 20 + Vector3d(0, 1, 0)) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_POTION:
		{
			if (SpawnProjectileFromDispenser(BlockX, DispY, BlockZ, cProjectileEntity::pkSplashPotion, GetShootVector(Meta) * 20 + Vector3d(0, 1, 0), &SlotItem) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_DYE:
		{
			if (SlotItem.m_ItemDamage != E_META_DYE_WHITE)
			{
				DropFromSlot(a_Chunk, a_SlotNum);
				break;
			}
			if (m_World->GrowRipePlant(BlockX, DispY, BlockZ, true))
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_BOAT:
		case E_ITEM_SPRUCE_BOAT:
		case E_ITEM_BIRCH_BOAT:
		case E_ITEM_JUNGLE_BOAT:
		case E_ITEM_ACACIA_BOAT:
		case E_ITEM_DARK_OAK_BOAT:
		{
			Vector3d SpawnPos;
			if (IsBlockWater(DispBlock))
			{
				// Water next to the dispenser, spawn a boat above the water block
				SpawnPos.Set(BlockX, DispY + 1, BlockZ);
			}
			else if (IsBlockWater(DispChunk->GetBlock(DispX, DispY - 1, DispZ)))
			{
				// Water one block below the dispenser, spawn a boat at the dispenser's Y level
				SpawnPos.Set(BlockX, DispY, BlockZ);
			}
			else
			{
				// There's no eligible water block, drop the boat as a pickup
				DropFromSlot(a_Chunk, a_SlotNum);
				break;
			}

			SpawnPos += GetShootVector(Meta) * 0.8;  // A boat is bigger than one block. Add the shoot vector to put it outside the dispenser.
			SpawnPos += Vector3d(0.5, 0.5, 0.5);

			if (m_World->SpawnBoat(SpawnPos, cBoat::ItemToMaterial(SlotItem)))
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_FIREWORK_ROCKET:
		{
			if (SpawnProjectileFromDispenser(BlockX, DispY, BlockZ, cProjectileEntity::pkFirework, GetShootVector(Meta) * 20 + Vector3d(0, 1, 0), &SlotItem) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		default:
		{
			DropFromSlot(a_Chunk, a_SlotNum);
			break;
		}
	}  // switch (SlotItem.m_ItemType)
}





UInt32 cDispenserEntity::SpawnProjectileFromDispenser(int a_BlockX, int a_BlockY, int a_BlockZ, cProjectileEntity::eKind a_Kind, const Vector3d & a_ShootVector, const cItem * a_Item)
{
	return m_World->CreateProjectile(
		static_cast<double>(a_BlockX + 0.5),
		static_cast<double>(a_BlockY + 0.5),
		static_cast<double>(a_BlockZ + 0.5),
		a_Kind, nullptr, a_Item, &a_ShootVector
	);
}



Vector3d cDispenserEntity::GetShootVector(NIBBLETYPE a_Meta)
{
	switch (a_Meta & E_META_DROPSPENSER_FACING_MASK)
	{
		case E_META_DROPSPENSER_FACING_YP: return Vector3d( 0,  1,  0);
		case E_META_DROPSPENSER_FACING_YM: return Vector3d( 0, -1,  0);
		case E_META_DROPSPENSER_FACING_XM: return Vector3d(-1,  0,  0);
		case E_META_DROPSPENSER_FACING_XP: return Vector3d( 1,  0,  0);
		case E_META_DROPSPENSER_FACING_ZM: return Vector3d( 0,  0, -1);
		case E_META_DROPSPENSER_FACING_ZP: return Vector3d( 0,  0,  1);
	}
	LOGWARNING("Unhandled dispenser meta: %d", a_Meta);
	ASSERT(!"Unhandled dispenser facing");
	return Vector3d(0, 1, 0);
}





bool cDispenserEntity::ScoopUpLiquid(int a_SlotNum, short a_ResultingBucketItemType)
{
	cItem LiquidBucket(a_ResultingBucketItemType, 1);
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
