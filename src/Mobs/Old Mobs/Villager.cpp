
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Villager.h"
#include "../World.h"
#include "../BlockArea.h"
#include "../Blocks/BlockHandler.h"
#include "../BlockInServerPluginInterface.h"





cVillager::cVillager(eVillagerType VillagerType) :
	super("Villager", mtVillager, "", "", 0.6, 1.8),
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

	if ((a_TDI.Attacker != NULL) && a_TDI.Attacker->IsPlayer())
	{
		if (m_World->GetTickRandomNumber(5) == 3)
		{
			m_World->BroadcastEntityStatus(*this, esVillagerAngry);
		}
	}
	return true;
}





void cVillager::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

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
	if (m_World->GetTickRandomNumber(99) != 0)
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
// Farmer functions.
void cVillager::HandleFarmerPrepareFarmCrops()
{
	if (!m_World->VillagersShouldHarvestCrops())
	{
		return;
	}

	cBlockArea Surrounding;
	/// Read a 11x7x11 area.
	Surrounding.Read(
		m_World,
		(int) GetPosX() - 5,
		(int) GetPosX() + 5,
		(int) GetPosY() - 3,
		(int) GetPosY() + 3,
		(int) GetPosZ() - 5,
		(int) GetPosZ() + 5
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
			m_CropsPos = Vector3i((int) GetPosX() + X - 5, (int) GetPosY() + Y - 3, (int) GetPosZ() + Z - 5);
			MoveToPosition(Vector3f((float) (m_CropsPos.x + 0.5), (float) m_CropsPos.y, (float) (m_CropsPos.z + 0.5)));
			return;
		}  // for Y loop.
	}  // Repeat the procces 5 times.
}





void cVillager::HandleFarmerTryHarvestCrops()
{
	// Harvest the crops if the villager isn't moving and if the crops are closer then 2 blocks.
	if (!m_bMovingToDestination && (GetPosition() - m_CropsPos).Length() < 2)
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
		case E_BLOCK_CROPS:
		case E_BLOCK_POTATOES:
		case E_BLOCK_CARROTS:
		{
			return true;
		}
	}
	return false;
}

