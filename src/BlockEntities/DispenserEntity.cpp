
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "DispenserEntity.h"
#include "../Chunk.h"
#include "../BlockInfo.h"
#include "../Defines.h"
#include "../World.h"
#include "../Entities/Boat.h"
#include "../Entities/ProjectileEntity.h"
#include "../Simulator/FluidSimulator.h"
#include "../Items/ItemSpawnEgg.h"



cDispenserEntity::cDispenserEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_Pos, a_World)
{
	ASSERT(a_BlockType == E_BLOCK_DISPENSER);
}





void cDispenserEntity::DropSpenseFromSlot(cChunk & a_Chunk, int a_SlotNum)
{
	Vector3i dispRelCoord(GetRelPos());
	auto meta = a_Chunk.GetMeta(dispRelCoord);
	AddDropSpenserDir(dispRelCoord, meta);
	auto dispChunk = a_Chunk.GetRelNeighborChunkAdjustCoords(dispRelCoord);
	if (dispChunk == nullptr)
	{
		// Would dispense into / interact with a non-loaded chunk, ignore the tick
		return;
	}

	BLOCKTYPE dispBlock = dispChunk->GetBlock(dispRelCoord);
	auto dispAbsCoord = dispChunk->RelativeToAbsolute(dispRelCoord);

	// Dispense the item:
	const cItem & SlotItem = m_Contents.GetSlot(a_SlotNum);
	if (ItemCategory::IsMinecart(SlotItem.m_ItemType) && IsBlockRail(dispBlock))  // only actually place the minecart if there are rails!
	{
		if (m_World->SpawnMinecart(dispAbsCoord.x + 0.5, dispAbsCoord.y + 0.5, dispAbsCoord.z + 0.5, SlotItem.m_ItemType) != cEntity::INVALID_ID)
		{
			m_Contents.ChangeSlotCount(a_SlotNum, -1);
		}
		return;
	}
	switch (SlotItem.m_ItemType)
	{
		case E_ITEM_BUCKET:
		{
			LOGD("Dispensing empty bucket in slot %d; dispBlock is \"%s\" (%d).", a_SlotNum, ItemTypeToString(dispBlock).c_str(), dispBlock);
			switch (dispBlock)
			{
				case E_BLOCK_STATIONARY_WATER:
				case E_BLOCK_WATER:
				{
					if (ScoopUpLiquid(a_SlotNum, E_ITEM_WATER_BUCKET))
					{
						dispChunk->SetBlock(dispRelCoord, E_BLOCK_AIR, 0);
					}
					break;
				}
				case E_BLOCK_STATIONARY_LAVA:
				case E_BLOCK_LAVA:
				{
					if (ScoopUpLiquid(a_SlotNum, E_ITEM_LAVA_BUCKET))
					{
						dispChunk->SetBlock(dispRelCoord, E_BLOCK_AIR, 0);
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
			LOGD("Dispensing water bucket in slot %d; dispBlock is \"%s\" (%d).", a_SlotNum, ItemTypeToString(dispBlock).c_str(), dispBlock);
			if (EmptyLiquidBucket(dispBlock, a_SlotNum))
			{
				dispChunk->SetBlock(dispRelCoord, E_BLOCK_WATER, 0);
			}
			else
			{
				DropFromSlot(a_Chunk, a_SlotNum);
			}
			break;
		}

		case E_ITEM_LAVA_BUCKET:
		{
			LOGD("Dispensing lava bucket in slot %d; dispBlock is \"%s\" (%d).", a_SlotNum, ItemTypeToString(dispBlock).c_str(), dispBlock);
			if (EmptyLiquidBucket(dispBlock, a_SlotNum))
			{
				dispChunk->SetBlock(dispRelCoord, E_BLOCK_LAVA, 0);
			}
			else
			{
				DropFromSlot(a_Chunk, a_SlotNum);
			}
			break;
		}

		case E_ITEM_SPAWN_EGG:
		{
			double MobX = 0.5 + dispAbsCoord.x;
			double MobZ = 0.5 + dispAbsCoord.z;
			auto MonsterType = cItemSpawnEggHandler::ItemDamageToMonsterType(m_Contents.GetSlot(a_SlotNum).m_ItemDamage);
			if (m_World->SpawnMob(MobX, dispAbsCoord.y, MobZ, MonsterType, false) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_BLOCK_TNT:
		{
			// Spawn a primed TNT entity, if space allows:
			if (!cBlockInfo::IsSolid(dispBlock))
			{
				m_World->SpawnPrimedTNT(Vector3d(0.5, 0.5, 0.5) + dispAbsCoord, 80, 0);  // 80 ticks fuse, no initial velocity
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_FLINT_AND_STEEL:
		{
			// Spawn fire if the block in front is air.
			if (dispBlock == E_BLOCK_AIR)
			{
				dispChunk->SetBlock(dispRelCoord, E_BLOCK_FIRE, 0);

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
			if (SpawnProjectileFromDispenser(dispAbsCoord, cProjectileEntity::pkFireCharge, GetShootVector(meta) * 20) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_ARROW:
		{
			if (SpawnProjectileFromDispenser(dispAbsCoord, cProjectileEntity::pkArrow, GetShootVector(meta) * 30 + Vector3d(0, 1, 0)) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_SNOWBALL:
		{
			if (SpawnProjectileFromDispenser(dispAbsCoord, cProjectileEntity::pkSnowball, GetShootVector(meta) * 20 + Vector3d(0, 1, 0)) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_EGG:
		{
			if (SpawnProjectileFromDispenser(dispAbsCoord, cProjectileEntity::pkEgg, GetShootVector(meta) * 20 + Vector3d(0, 1, 0)) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_BOTTLE_O_ENCHANTING:
		{
			if (SpawnProjectileFromDispenser(dispAbsCoord, cProjectileEntity::pkExpBottle, GetShootVector(meta) * 20 + Vector3d(0, 1, 0)) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_POTION:
		{
			if (SpawnProjectileFromDispenser(dispAbsCoord, cProjectileEntity::pkSplashPotion, GetShootVector(meta) * 20 + Vector3d(0, 1, 0), &SlotItem) != cEntity::INVALID_ID)
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
			if (m_World->GrowRipePlant(dispAbsCoord.x, dispAbsCoord.y, dispAbsCoord.z, true))
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
			Vector3d spawnPos = dispAbsCoord;
			if (IsBlockWater(dispBlock))
			{
				// Water next to the dispenser, spawn a boat above the water block
				spawnPos.y += 1;
			}
			else if (IsBlockWater(dispChunk->GetBlock(dispRelCoord.addedY(-1))))
			{
				// Water one block below the dispenser, spawn a boat at the dispenser's Y level
				// No adjustment needed
			}
			else
			{
				// There's no eligible water block, drop the boat as a pickup
				DropFromSlot(a_Chunk, a_SlotNum);
				break;
			}

			spawnPos += GetShootVector(meta) * 0.8;  // A boat is bigger than one block. Add the shoot vector to put it outside the dispenser.
			spawnPos += Vector3d(0.5, 0.5, 0.5);

			if (m_World->SpawnBoat(spawnPos, cBoat::ItemToMaterial(SlotItem)))
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case E_ITEM_FIREWORK_ROCKET:
		{
			if (SpawnProjectileFromDispenser(dispAbsCoord, cProjectileEntity::pkFirework, GetShootVector(meta) * 20 + Vector3d(0, 1, 0), &SlotItem) != cEntity::INVALID_ID)
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





UInt32 cDispenserEntity::SpawnProjectileFromDispenser(Vector3i a_BlockPos, cProjectileEntity::eKind a_Kind, const Vector3d & a_ShootVector, const cItem * a_Item)
{
	return m_World->CreateProjectile(Vector3d(0.5, 0.5, 0.5) + a_BlockPos,
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
	cItem LiquidBucket(a_ResultingBucketItemType);
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
