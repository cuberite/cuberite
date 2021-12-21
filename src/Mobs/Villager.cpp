
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Villager.h"
#include "../World.h"
#include "../BlockArea.h"
#include "../Blocks/BlockHandler.h"
#include "../BlockInServerPluginInterface.h"





cVillager::cVillager(eVillagerType VillagerType) :
	Super("Villager", mtVillager, "entity.villager.hurt", "entity.villager.death", "entity.villager.ambient", 0.6f, 1.95f),
	m_ActionCountDown(-1),
	m_Type(VillagerType),
	m_VillagerAction(false),
	m_Harvesting(false),
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

	if (m_Harvesting)
	{
		// Forcing the villager to go to farm spots while harvesting

		MoveToPosition(Vector3d(m_CropsPos.x + 0.5, m_CropsPos.y + 0.0, m_CropsPos.z + 0.5));
	}

	if (m_ActionCountDown > -1)
	{
		m_ActionCountDown--;
		if (m_ActionCountDown == 0)
		{
			switch (m_Type)
			{
				case vtFarmer:
				{
					HandleFarmerPlaceCrops();
				}
			}
		}
		return;
	}

	if (m_VillagerAction)
	{
		switch (m_Type)
		{
			case vtFarmer:
			{
				m_VillagerAction = HandleFarmerTryHarvestCrops();
			}
		}
		return;
	}
	else
	{
		// Returning to idle state.
		m_Harvesting = false;
	}

	// Don't always try to do a special action. Each tick has 1% to do a special action.
	if (GetRandomProvider().RandBool(0.99))
	{
		return;
	}

	switch (m_Type)
	{
		case vtFarmer:
		{
			HandleFarmerPrepareFarmCrops();
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

void cVillager::HandleFarmerPrepareFarmCrops()
{
	if (!m_World->VillagersShouldHarvestCrops())
	{
		return;
	}

	cBlockArea Surrounding;

	// Read a 31x3x31 area:
	Surrounding.Read(
		*m_World,
		FloorC(GetPosX()) - 15,
		FloorC(GetPosX()) + 15,
		FloorC(GetPosY()) - 1,
		FloorC(GetPosY()) + 1,
		FloorC(GetPosZ()) - 15,
		FloorC(GetPosZ()) + 15
	);

	for (int I = 0; I < 5; I++)
	{
		for (int Y = 0; Y < 3; Y++)
		{
			// Pick random coordinates and check for crops.
			int X = m_World->GetTickRandomNumber(31 - 1);
			int Z = m_World->GetTickRandomNumber(31 - 1);

			// A villager can't farm this.
			if (!IsBlockFarmable(Surrounding.GetRelBlockType(X, Y, Z)))
			{
				continue;
			}
			if (Surrounding.GetRelBlockMeta(X, Y, Z) != 0x7)
			{
				continue;
			}


			m_Harvesting = true;
			m_VillagerAction = true;
			m_CropsPos = Vector3i(static_cast<int>(GetPosX()) + X - 15, static_cast<int>(GetPosY()) + Y - 1, static_cast<int>(GetPosZ()) + Z - 15);
			MoveToPosition(Vector3d(m_CropsPos.x + 0.5, m_CropsPos.y + 0.0, m_CropsPos.z + 0.5));
			return;
		}  // for Y loop.
	}  // Repeat the procces 5 times.
}





bool cVillager::HandleFarmerTryHarvestCrops()
{
	// Harvest the crops if it is closer than 1 block.
	if ((GetPosition() - m_CropsPos).Length() < 1)
	{
		// Check if the blocks didn't change while the villager was walking to the coordinates.
		BLOCKTYPE CropBlock = m_World->GetBlock(m_CropsPos);
		if (IsBlockFarmable(CropBlock) && m_World->GetBlockMeta(m_CropsPos) == 0x7)
		{
			m_World->DropBlockAsPickups(m_CropsPos, this, nullptr);
			m_ActionCountDown = 20;
			return false;  // The block no longer exist since he broke it.
		}
		return false;  // The block is no longer farmable.
	}
	return true;  // Keep walking.
}





void cVillager::CheckForNearbyCrops()
{

	// Search for adjacent crops

	Vector3i North = {0, 0, -1};
	Vector3i South = {0, 0, 1};
	Vector3i East = {1, 0, 0};
	Vector3i West = {-1, 0, 0};

	std::vector<Vector3i> Directions = {North, South, East, West};

	bool CropFound = false;

	for (Vector3i Direction : Directions)
	{
		if (IsBlockFarmable(m_World->GetBlock(m_CropsPos + Direction)) && m_World->GetBlockMeta(m_CropsPos + Direction) == 0x7)
		{
			m_CropsPos += Direction;
			CropFound = true;
			break;
		}
	}

	if (CropFound)
	{
		m_VillagerAction = true;
		MoveToPosition(Vector3d(m_CropsPos.x + 0.5, m_CropsPos.y + 0.0, m_CropsPos.z + 0.5));
	}
	else
	{
		// No more crops to harvest. Returning to idle state.

		m_Harvesting = false;
	}
}





void cVillager::HandleFarmerPlaceCrops()
{
	// Can't place crops if he does not have one (potato, carrot, seed) in his inventory.
	if (!CanPlantCrops())
	{
		return;
	}

	// Check if there is still farmland at the spot where the crops were.
	if (m_World->GetBlock(m_CropsPos.addedY(-1)) == E_BLOCK_FARMLAND)
	{
		// Finding the item to use to plant a crop
		int TargetSlot = -1;
		BLOCKTYPE CropBlockType;

		for (int I = 0; I < m_Inventory.GetWidth() && TargetSlot < 0; I++)
		{
			const cItem & Slot = m_Inventory.GetSlot(I);
			switch (Slot.m_ItemType)
			{
				case E_ITEM_SEEDS:
				{
					TargetSlot = I;
					CropBlockType = E_BLOCK_CROPS;
					break;
				}

				case E_ITEM_BEETROOT_SEEDS:
				{
					TargetSlot = I;
					CropBlockType = E_BLOCK_BEETROOTS;
					break;
				}

				case E_ITEM_POTATO:
				{
					TargetSlot = I;
					CropBlockType = E_BLOCK_POTATOES;
					break;
				}

				case E_ITEM_CARROT:
				{
					TargetSlot = I;
					CropBlockType = E_BLOCK_CARROTS;
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
		m_World->SetBlock(m_CropsPos, CropBlockType, 0);

		// Try to do the same with adjacent crops.
		CheckForNearbyCrops();
	}
}





bool cVillager::CanPlantCrops()
{
	return m_Inventory.HasItems(cItem(E_ITEM_SEEDS)) ||
		m_Inventory.HasItems(cItem(E_ITEM_BEETROOT_SEEDS)) ||
		m_Inventory.HasItems(cItem(E_ITEM_POTATO)) ||
		m_Inventory.HasItems(cItem(E_ITEM_CARROT));
}





bool cVillager::IsBlockFarmable(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_BEETROOTS:
		case E_BLOCK_CROPS:
		case E_BLOCK_POTATOES:
		case E_BLOCK_CARROTS:
		{
			return true;
		}
		default: return false;
	}
}





cVillager::eVillagerType cVillager::GetRandomProfession()
{
	int Profession = GetRandomProvider().RandInt(cVillager::eVillagerType::vtMax - 1);

	return static_cast<cVillager::eVillagerType>(Profession);
}
