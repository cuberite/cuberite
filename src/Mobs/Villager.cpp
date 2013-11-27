
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Villager.h"
#include "../World.h"





cVillager::cVillager(eVillagerType VillagerType) :
	super("Villager", mtVillager, "", "", 0.6, 1.8),
	m_Type(VillagerType)
{
}





void cVillager::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	super::DoTakeDamage(a_TDI);
	if (a_TDI.Attacker->IsPlayer())
	{
		if (m_World->GetTickRandomNumber(5) == 3)
		{
			m_World->BroadcastEntityStatus(*this, ENTITY_STATUS_VILLAGER_ANGRY);
		}
	}
}




