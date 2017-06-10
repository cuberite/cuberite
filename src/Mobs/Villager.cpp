
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Villager.h"
#include "../World.h"
#include "../BlockArea.h"
#include "../Blocks/BlockHandler.h"
#include "../BlockInServerPluginInterface.h"





cVillager::cVillager(eVillagerType VillagerType) :
	super("Villager", mtVillager, "entity.villager.hurt", "entity.villager.death", 0.6, 1.8),
	m_ActionCountDown(-1),
	m_Type(VillagerType),
	m_VillagerAction(false)
{
}





bool cVillager::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!super::DoTakeDamage(a_TDI))
	{
		return false;
	}

	if ((a_TDI.Attacker != nullptr) && a_TDI.Attacker->IsPlayer())
	{
		if (GetRandomProvider().RandBool(1.0 / 6.0))
		{
			m_World->BroadcastEntityStatus(*this, esVillagerAngry);
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
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
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
				HandleFarmerTryHarvestCrops();
			}
		}
		m_VillagerAction = false;
		return;
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




////////////////////////////////////////////////////////////////////////////////
// Farmer functions:

void cVillager::HandleFarmerPrepareFarmCrops()
{
	if (!m_World->VillagersShouldHarvestCrops())
	{
		return;
	}

	cBlockArea Surrounding;

	// Read a 11x7x11 area:
	Surrounding.Read(
		*m_World,
		FloorC(GetPosX()) - 5,
		FloorC(GetPosX()) + 6,
		FloorC(GetPosY()) - 3,
		FloorC(GetPosY()) + 4,
		FloorC(GetPosZ()) - 5,
		FloorC(GetPosZ()) + 6
	);

	for (int I = 0; I < 5; I++)
	{
		for (int Y = 0; Y < 6; Y++)
		{
			// Pick random coordinates and check for crops.
			int X = m_World->GetTickRandomNumber(11);
			int Z = m_World->GetTickRandomNumber(11);

			// A villager can't farm this.
			if (!IsBlockFarmable(Surrounding.GetRelBlockType(X, Y, Z)))
			{
				continue;
			}
			if (Surrounding.GetRelBlockMeta(X, Y, Z) != 0x7)
			{
				continue;
			}

			m_VillagerAction = true;
			m_CropsPos = Vector3i(static_cast<int>(GetPosX()) + X - 5, static_cast<int>(GetPosY()) + Y - 3, static_cast<int>(GetPosZ()) + Z - 5);
			MoveToPosition(Vector3d(m_CropsPos.x + 0.5, m_CropsPos.y + 0.0, m_CropsPos.z + 0.5));
			return;
		}  // for Y loop.
	}  // Repeat the procces 5 times.
}





void cVillager::HandleFarmerTryHarvestCrops()
{
	// Harvest the crops if the villager isn't moving and if the crops are closer then 2 blocks.
	if (!m_PathfinderActivated && (GetPosition() - m_CropsPos).Length() < 2)
	{
		// Check if the blocks didn't change while the villager was walking to the coordinates.
		BLOCKTYPE CropBlock = m_World->GetBlock(m_CropsPos.x, m_CropsPos.y, m_CropsPos.z);
		if (IsBlockFarmable(CropBlock) && m_World->GetBlockMeta(m_CropsPos.x, m_CropsPos.y, m_CropsPos.z) == 0x7)
		{
			cBlockHandler * Handler = cBlockInfo::GetHandler(CropBlock);
			cChunkInterface ChunkInterface(m_World->GetChunkMap());
			cBlockInServerPluginInterface PluginInterface(*m_World);
			Handler->DropBlock(ChunkInterface, *m_World, PluginInterface, this, m_CropsPos.x, m_CropsPos.y, m_CropsPos.z);
			m_World->SetBlock(m_CropsPos.x, m_CropsPos.y, m_CropsPos.z, E_BLOCK_AIR, 0);
			m_ActionCountDown = 20;
		}
	}
}




void cVillager::HandleFarmerPlaceCrops()
{
	// Check if there is still farmland at the spot where the crops were.
	if (m_World->GetBlock(m_CropsPos.x, m_CropsPos.y - 1, m_CropsPos.z) == E_BLOCK_FARMLAND)
	{
		m_World->SetBlock(m_CropsPos.x, m_CropsPos.y, m_CropsPos.z, E_BLOCK_CROPS, 0);
	}
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
	}
	return false;
}

