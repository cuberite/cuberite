
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Villager.h"
#include "World.h"
#include "Chunk.h"
#include "BlockArea.h"
#include "Blocks/BlockHandler.h"
#include "Blocks/BlockCrops.h"
#include "BlockInServerPluginInterface.h"
#include "BlockInfo.h"





cVillager::cVillager(eVillagerType VillagerType) :
	Super("Villager", mtVillager, "entity.villager.hurt", "entity.villager.death", "entity.villager.ambient", 0.6f, 1.95f),
	m_ActionCountDown(-1),
	m_Type(VillagerType),
	m_FarmerAction(faIdling),
	m_Inventory(8, 1)
{
}





bool cVillager::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!Super::DoTakeDamage(a_TDI))
	{
		return false;
	}

	if ((a_TDI.Attacker != nullptr) && a_TDI.Attacker->IsPlayer())
	{
		if (GetRandomProvider().RandBool(1.0 / 6.0))
		{
			m_World->BroadcastEntityAnimation(*this, EntityAnimation::VillagerShowsAnger);
		}
	}

	if (a_TDI.DamageType == dtLightning)
	{
		Destroy();
		m_World->SpawnMob(GetPosX(), GetPosY(), GetPosZ(), mtWitch, false);
		return true;
	}
	return true;
}





void cVillager::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	switch (m_Type)
	{
		case vtFarmer:
		{
			TickFarmer();
			break;
		}
	}
}





void cVillager::KilledBy(TakeDamageInfo & a_TDI)
{
	Super::KilledBy(a_TDI);

	// TODO: 0% chance on Easy, 50% chance on Normal and 100% chance on Hard
	if (GetRandomProvider().RandBool(0.5) && (a_TDI.Attacker != nullptr) && (a_TDI.Attacker->IsMob()))
	{
		eMonsterType MonsterType = (static_cast<cMonster *>(a_TDI.Attacker)->GetMobType());
		if ((MonsterType == mtZombie) || (MonsterType == mtZombieVillager))
		{
			m_World->SpawnMob(GetPosX(), GetPosY(), GetPosZ(), mtZombieVillager, false);
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// Farmer functions:

void cVillager::TickFarmer()
{

	// Don't harvest crops if you must not
	if (!m_World->VillagersShouldHarvestCrops())
	{
		return;
	}

	// This is to prevent undefined behaviors
	if (m_FinalDestination.y <= 0)
	{
		return;
	}

	if (!IsIdling())
	{
		// Forcing the farmer to go to work spots.
		MoveToPosition(static_cast<Vector3d>(m_CropsPos) + Vector3d(0.5, 0, 0.5));

		// Forcing the farmer to look at the work spots.
		Vector3d Direction = (m_FinalDestination - (GetPosition() + Vector3d(0, 1.6, 0)));  // We get the direction from the eyes of the farmer to the work spot.
		Direction.Normalize();
		SetPitch(std::asin(-Direction.y) / M_PI * 180);
	}

	// Updating the timer
	if (m_ActionCountDown > -1)
	{
		m_ActionCountDown--;
	}

	// Searching for work in blocks where the farmer goes.
	if (IsHarvestable(m_FinalDestination.Floor()))
	{
		m_CropsPos = m_FinalDestination.Floor();
		m_FarmerAction = faHarvesting;
		HandleFarmerTryHarvestCrops();
		return;
	}
	else if (IsPlantable(m_FinalDestination.Floor()) && CanPlantCrops())
	{
		m_CropsPos = m_FinalDestination.Floor();
		m_FarmerAction = faPlanting;
		HandleFarmerTryPlaceCrops();
		return;
	}
	else
	{
		m_FarmerAction = faIdling;  // Returning to idling.
	}


	// Don't always try to do a special action. Each tick has 10% to do a special action.
	if (GetRandomProvider().RandBool(FARMER_SPECIAL_ACTION_CHANCE))
	{
		ScanAreaForWork();
	}

}





void cVillager::ScanAreaForWork()
{

	auto Pos = GetPosition().Floor();
	auto MinPos = Pos - FARMER_SCAN_CROPS_DIST;
	auto MaxPos = Pos + FARMER_SCAN_CROPS_DIST;

	// Read area to be checked for crops.
	cBlockArea Surrounding;
	Surrounding.Read(
		*m_World,
		MinPos, MaxPos
	);

	for (int I = 0; I < FARMER_RANDOM_TICK_SPEED; I++)
	{
		for (int Y = MinPos.y; Y <= MaxPos.y; Y++)
		{
			// Pick random coordinates and check for crops.
			Vector3i CandidatePos(MinPos.x + m_World->GetTickRandomNumber(MaxPos.x - MinPos.x - 1), Y, MinPos.z + m_World->GetTickRandomNumber(MaxPos.z - MinPos.z - 1));

			// A villager can harvest this.
			if (IsHarvestable(CandidatePos))
			{
				m_CropsPos = CandidatePos;
				m_FarmerAction = faHarvesting;
				MoveToPosition(static_cast<Vector3d>(m_CropsPos) + Vector3d(0.5, 0, 0.5));
				return;
			}
			// A villager can plant this.
			else if (IsPlantable(CandidatePos) && CanPlantCrops())
			{
				m_CropsPos = CandidatePos;
				m_FarmerAction = faHarvesting;
				MoveToPosition(static_cast<Vector3d>(m_CropsPos) + Vector3d(0.5, 0, 0.5));
				return;
			}

		}  // for Y
	}  // Repeat the proccess according to the random tick speed.
}





void cVillager::HandleFarmerTryHarvestCrops()
{
	if (m_ActionCountDown > 0)
	{
		// The farmer is still on cooldown
		return;
	}

	// Harvest the crops if it is closer than 1 block.
	if ((GetPosition() - m_CropsPos).Length() < 1)
	{
		// Check if the blocks didn't change while the villager was walking to the coordinates.
		if (IsHarvestable(m_CropsPos))
		{
			m_World->BroadcastSoundParticleEffect(EffectID::PARTICLE_BLOCK_BREAK, m_CropsPos, m_World->GetBlock(m_CropsPos).GetLegacyValues().first);
			m_World->DropBlockAsPickups(m_CropsPos, this, nullptr);
			// Applying 0.5 second cooldown.
			m_ActionCountDown = 10;
		}
	}
}





void cVillager::CheckForNearbyCrops()
{

	// Search for adjacent crops

	constexpr std::array<Vector3i, 4> Directions = { Vector3i{0, 0, -1}, {0, 0, 1}, {1, 0, 0}, {-1, 0, 0} };

	for (Vector3i Direction : Directions)
	{
		if (IsHarvestable(m_CropsPos + Direction))
		{
			m_CropsPos += Direction;
			m_FarmerAction = faHarvesting;
			MoveToPosition(static_cast<Vector3d>(m_CropsPos) + Vector3d(0.5, 0, 0.5));
			return;
		}
		else if (IsPlantable(m_CropsPos + Direction) && CanPlantCrops())
		{
			m_CropsPos += Direction;
			m_FarmerAction = faPlanting;
			MoveToPosition(static_cast<Vector3d>(m_CropsPos) + Vector3d(0.5, 0, 0.5));
			return;
		}

	}

	// There is no more work to do around the previous crops.
	m_FarmerAction = faIdling;
}





void cVillager::HandleFarmerTryPlaceCrops()
{

	if ((GetPosition() - m_CropsPos).Length() > 1)
	{
		// The farmer is still to far from the final destination
		return;
	}

	if (m_ActionCountDown > 0)
	{
		// The farmer is still on cooldown
		return;
	}

	// Check if there is still farmland at the spot where the crops were.
	if (IsPlantable(m_CropsPos))
	{
		// Finding the item to use to plant a crop
		int TargetSlot = -1;
		BlockState CropBlockType;

		for (int I = 0; I < m_Inventory.GetWidth() && TargetSlot < 0; I++)
		{
			const cItem & Slot = m_Inventory.GetSlot(I);
			switch (Slot.m_ItemType)
			{
				case Item::WheatSeeds:
				{
					TargetSlot = I;
					CropBlockType = Block::Wheat::Wheat();
					break;
				}

				case Item::BeetrootSeeds:
				{
					TargetSlot = I;
					CropBlockType = Block::Beetroots::Beetroots();
					break;
				}

				case Item::Potato:
				{
					TargetSlot = I;
					CropBlockType = Block::Potatoes::Potatoes();
					break;
				}

				case Item::Carrot:
				{
					TargetSlot = I;
					CropBlockType = Block::Carrots::Carrots();
					break;
				}

				default:
				{
					break;
				}
			}
		}

		// Removing item from villager inventory
		m_Inventory.RemoveOneItem(TargetSlot);

		// Placing crop block
		m_World->SetBlock(m_CropsPos, CropBlockType);

		// Applying 1 second cooldown
		m_ActionCountDown = 20;

		// Try to do the same with adjacent crops.
		CheckForNearbyCrops();
	}
}





bool cVillager::CanPlantCrops()
{
	return m_Inventory.HasItems(cItem(Item::WheatSeeds)) ||
		m_Inventory.HasItems(cItem(Item::BeetrootSeeds)) ||
		m_Inventory.HasItems(cItem(Item::Potato)) ||
		m_Inventory.HasItems(cItem(Item::Carrot));
}





bool cVillager::IsBlockFarmable(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::Beetroots:
		case BlockType::Carrots:
		{
			// The crop must have fully grown up.
			return cBlockCropsHandler::IsFullyGrown(a_Block);
		}
		case BlockType::Wheat:
		case BlockType::Potatoes:
			return true;
		default: return false;
	}
}





bool cVillager::IsHarvestable(Vector3i a_CropsPos)
{
	return IsBlockFarmable(m_World->GetBlock(a_CropsPos));
}





bool cVillager::IsPlantable(Vector3i a_CropsPos)
{
	return (m_World->GetBlock(a_CropsPos.addedY(-1)).Type() == BlockType::Farmland) && IsBlockAir(m_World->GetBlock(a_CropsPos));
}





cVillager::eVillagerType cVillager::GetRandomProfession()
{
	int Profession = GetRandomProvider().RandInt(cVillager::eVillagerType::vtMax - 1);

	return static_cast<cVillager::eVillagerType>(Profession);
}
