
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "DispenserEntity.h"
#include "../Chunk.h"
#include "../BlockInfo.h"
#include "../Defines.h"
#include "../World.h"
#include "../Blocks/ChunkInterface.h"
#include "../Blocks/BlockAir.h"
#include "../Blocks/BlockRail.h"
#include "../Entities/Boat.h"
#include "../Entities/ProjectileEntity.h"
#include "../Simulator/FluidSimulator.h"
#include "../Items/ItemSpawnEgg.h"
#include "../Items/ItemDye.h"



cDispenserEntity::cDispenserEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World):
	Super(a_Block, a_Pos, a_World)
{
	ASSERT(a_Block.Type() == BlockType::Dispenser);
}





void cDispenserEntity::DropSpenseFromSlot(cChunk & a_Chunk, int a_SlotNum)
{
	Vector3i DispRelCoord(GetRelPos());
	auto Self = a_Chunk.GetBlock(DispRelCoord);
	DispRelCoord = AddFaceDirection(DispRelCoord, Block::Dispenser::Facing(Self));
	auto DispChunk = a_Chunk.GetRelNeighborChunkAdjustCoords(DispRelCoord);
	if (DispChunk == nullptr)
	{
		// Would dispense into / interact with a non-loaded chunk, ignore the tick
		return;
	}

	auto DispBlock = DispChunk->GetBlock(DispRelCoord);
	auto DispAbsCoord = DispChunk->RelativeToAbsolute(DispRelCoord);

	// Dispense the item:
	const cItem & SlotItem = m_Contents.GetSlot(a_SlotNum);
	if (ItemCategory::IsMinecart(SlotItem.m_ItemType) && cBlockRailHandler::IsBlockRail(DispBlock))  // only actually place the minecart if there are rails!
	{
		if (m_World->SpawnMinecart(DispAbsCoord.x + 0.5, DispAbsCoord.y + 0.5, DispAbsCoord.z + 0.5, SlotItem.m_ItemType) != cEntity::INVALID_ID)
		{
			m_Contents.ChangeSlotCount(a_SlotNum, -1);
		}
		return;
	}
	switch (SlotItem.m_ItemType)
	{
		case Item::Bucket:
		{
			FLOGD("Dispensing empty bucket in slot {}; DispBlock is \"{}\".", a_SlotNum, DispBlock);
			switch (DispBlock.Type())
			{
				case BlockType::Water:
				{
					if (ScoopUpLiquid(a_SlotNum, Item::WaterBucket))
					{
						DispChunk->SetBlock(DispRelCoord, Block::Air::Air());
					}
					break;
				}
				case BlockType::Lava:
				{
					if (ScoopUpLiquid(a_SlotNum, Item::LavaBucket))
					{
						DispChunk->SetBlock(DispRelCoord, Block::Air::Air());
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
		}

		case Item::WaterBucket:
		{
			FLOGD("Dispensing water bucket in slot {}; DispBlock is \"{}\".", a_SlotNum, DispBlock);
			if (EmptyLiquidBucket(DispBlock, a_SlotNum))
			{
				DispChunk->SetBlock(DispRelCoord, Block::Water::Water());
			}
			else
			{
				DropFromSlot(a_Chunk, a_SlotNum);
			}
			break;
		}

		case Item::LavaBucket:
		{
			FLOGD("Dispensing lava bucket in slot {}; DispBlock is \"{}\".", a_SlotNum, DispBlock);
			if (EmptyLiquidBucket(DispBlock, a_SlotNum))
			{
				DispChunk->SetBlock(DispRelCoord, Block::Lava::Lava());
			}
			else
			{
				DropFromSlot(a_Chunk, a_SlotNum);
			}
			break;
		}

		case Item::BatSpawnEgg:
		case Item::BlazeSpawnEgg:
		case Item::CaveSpiderSpawnEgg:
		case Item::ChickenSpawnEgg:
		case Item::CowSpawnEgg:
		case Item::CreeperSpawnEgg:
		case Item::EndermanSpawnEgg:
		case Item::GhastSpawnEgg:
		case Item::GuardianSpawnEgg:
		case Item::HorseSpawnEgg:
		case Item::MagmaCubeSpawnEgg:
		case Item::MooshroomSpawnEgg:
		case Item::OcelotSpawnEgg:
		case Item::PigSpawnEgg:
		case Item::RabbitSpawnEgg:
		case Item::SheepSpawnEgg:
		case Item::SilverfishSpawnEgg:
		case Item::SkeletonSpawnEgg:
		case Item::SlimeSpawnEgg:
		case Item::SpiderSpawnEgg:
		case Item::SquidSpawnEgg:
		case Item::VillagerSpawnEgg:
		case Item::WitchSpawnEgg:
		case Item::WitherSkeletonSpawnEgg:
		case Item::WolfSpawnEgg:
		case Item::ZombieSpawnEgg:
		case Item::ZombiePigmanSpawnEgg:
		case Item::ZombieVillagerSpawnEgg:
		{
			double MobX = 0.5 + DispAbsCoord.x;
			double MobZ = 0.5 + DispAbsCoord.z;
			auto MonsterType = cItemSpawnEggHandler::ItemToMonsterType(m_Contents.GetSlot(a_SlotNum));
			if (m_World->SpawnMob(MobX, DispAbsCoord.y, MobZ, MonsterType, false) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case Item::TNT:
		{
			// Spawn a primed TNT entity, if space allows:
			if (!cBlockInfo::IsSolid(DispBlock))
			{
				m_World->SpawnPrimedTNT(Vector3d(0.5, 0.5, 0.5) + DispAbsCoord, 80, 0);  // 80 ticks fuse, no initial velocity
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case Item::FlintAndSteel:
		{
			// Spawn fire if the block in front is air.
			if (cBlockAirHandler::IsBlockAir(DispBlock))
			{
				DispChunk->SetBlock(DispRelCoord, Block::Fire::Fire());

				bool ItemBroke = m_Contents.DamageItem(a_SlotNum, 1);

				if (ItemBroke)
				{
					m_Contents.ChangeSlotCount(a_SlotNum, -1);
				}
			}
			break;
		}

		case Item::FireCharge:
		{
			if (SpawnProjectileFromDispenser(DispAbsCoord, cProjectileEntity::pkFireCharge, GetShootVector(Self) * 20) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case Item::Arrow:
		{
			if (SpawnProjectileFromDispenser(DispAbsCoord, cProjectileEntity::pkArrow, GetShootVector(Self) * 30 + Vector3d(0, 1, 0)) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case Item::Snowball:
		{
			if (SpawnProjectileFromDispenser(DispAbsCoord, cProjectileEntity::pkSnowball, GetShootVector(Self) * 20 + Vector3d(0, 1, 0)) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case Item::Egg:
		{
			if (SpawnProjectileFromDispenser(DispAbsCoord, cProjectileEntity::pkEgg, GetShootVector(Self) * 20 + Vector3d(0, 1, 0)) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case Item::ExperienceBottle:
		{
			if (SpawnProjectileFromDispenser(DispAbsCoord, cProjectileEntity::pkExpBottle, GetShootVector(Self) * 20 + Vector3d(0, 1, 0)) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case Item::Potion:
		{
			if (SpawnProjectileFromDispenser(DispAbsCoord, cProjectileEntity::pkSplashPotion, GetShootVector(Self) * 20 + Vector3d(0, 1, 0), &SlotItem) != cEntity::INVALID_ID)
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case Item::BoneMeal:
		{
			// Simulate a right-click with bonemeal:
			if (cItemDyeHandler::FertilizePlant(*m_World, DispAbsCoord))
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case Item::OakBoat:
		case Item::SpruceBoat:
		case Item::BirchBoat:
		case Item::JungleBoat:
		case Item::AcaciaBoat:
		case Item::DarkOakBoat:
		{
			Vector3d spawnPos = DispAbsCoord;
			if (DispBlock.Type() == BlockType::Water)
			{
				// Water next to the dispenser, spawn a boat above the water block
				spawnPos.y += 1;
			}
			else if (DispChunk->GetBlock(DispRelCoord.addedY(-1)).Type() == BlockType::Water)
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

			spawnPos += GetShootVector(Self) * 0.8;  // A boat is bigger than one block. Add the shoot vector to put it outside the dispenser.
			spawnPos += Vector3d(0.5, 0.5, 0.5);

			if (m_World->SpawnBoat(spawnPos, cBoat::ItemToMaterial(SlotItem)))
			{
				m_Contents.ChangeSlotCount(a_SlotNum, -1);
			}
			break;
		}

		case Item::FireworkRocket:
		{
			if (SpawnProjectileFromDispenser(DispAbsCoord, cProjectileEntity::pkFirework, GetShootVector(Self) * 20 + Vector3d(0, 1, 0), &SlotItem) != cEntity::INVALID_ID)
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





Vector3d cDispenserEntity::GetShootVector(BlockState a_Self)
{
	switch (Block::Dispenser::Facing(a_Self))
	{
		case BLOCK_FACE_YP: return Vector3d( 0,  1,  0);
		case BLOCK_FACE_YM: return Vector3d( 0, -1,  0);
		case BLOCK_FACE_XM: return Vector3d(-1,  0,  0);
		case BLOCK_FACE_XP: return Vector3d( 1,  0,  0);
		case BLOCK_FACE_ZM: return Vector3d( 0,  0, -1);
		case BLOCK_FACE_ZP: return Vector3d( 0,  0,  1);
		default:
		{
			FLOGWARNING("Unhandled dispenser facing: {}", Block::Dispenser::Facing(a_Self));
			ASSERT(!"Unhandled dispenser facing");
			return Vector3d(0, 1, 0);
		}
	}
}





bool cDispenserEntity::ScoopUpLiquid(int a_SlotNum, Item a_ResultingBucketItemType)
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





bool cDispenserEntity::EmptyLiquidBucket(BlockState a_BlockInFront, int a_SlotNum)
{
	if (
		!cBlockAirHandler::IsBlockAir(a_BlockInFront) &&
		!IsBlockLiquid(a_BlockInFront) &&
		!cFluidSimulator::CanWashAway(a_BlockInFront)
	)
	{
		// Not a suitable block in front
		return false;
	}

	cItem EmptyBucket(Item::Bucket);
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
