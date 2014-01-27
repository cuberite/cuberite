
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Villager.h"
#include "../World.h"
#include "../Blockarea.h"
#include "../Blocks/BlockHandler.h"





cVillager::cVillager(eVillagerType VillagerType) :
	super("Villager", mtVillager, "", "", 0.6, 1.8),
	m_Type(VillagerType),
	m_DidFindCrops(false)
{
}





void cVillager::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	super::DoTakeDamage(a_TDI);
	if ((a_TDI.Attacker != NULL) && a_TDI.Attacker->IsPlayer())
	{
		if (m_World->GetTickRandomNumber(5) == 3)
		{
			m_World->BroadcastEntityStatus(*this, ENTITY_STATUS_VILLAGER_ANGRY);
		}
	}
}





void cVillager::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (m_DidFindCrops)
	{
		Vector3i Pos = Vector3i(GetPosition());
		std::cout << Pos.Equals(m_CropsPos) << "\n";
		if (Pos.Equals(m_CropsPos))
		{
			cBlockHandler Handler(E_BLOCK_CROPS);
			Handler.DropBlock(m_World, this, m_CropsPos.x, m_CropsPos.y, m_CropsPos.z);
			m_DidFindCrops = false;
		}
	}

	if (m_World->GetTickRandomNumber(50) != 0)
	{
		return;
	}

	switch (m_Type)
	{
		case vtFarmer:
		{
			HandleFarmer();
		}
	}

}




void cVillager::HandleFarmer()
{
	cBlockArea Surrounding;
	Surrounding.Read(m_World,
	(int) GetPosX() - 5,
	(int) GetPosX() + 5,
	(int) GetPosY() - 3,
	(int) GetPosY() + 3,
	(int) GetPosZ() - 5,
	(int) GetPosZ() + 5);

	// Check for crops in a 10x6x10 area.
	for (int X = 0; X < 10; X++)
	{
		for (int Y = 0; Y < 6; Y++)
		{
			for (int Z = 0; Z < 10; Z++)
			{
				if (Surrounding.GetRelBlockType(X, Y, Z) == E_BLOCK_CROPS && Surrounding.GetRelBlockMeta(X, Y, Z) == 0x7)
				{
					m_DidFindCrops = true;
					m_CropsPos = Vector3i((int) GetPosX() + X - 5, (int) GetPosY() + Y - 3, (int) GetPosZ() + Z - 5);
					MoveToPosition(Vector3f((float) GetPosX() + X - 5, (float) GetPosY() + Y - 3, (float) GetPosZ() + Z - 5));
					return;
				}
			}
		}
	}
}




