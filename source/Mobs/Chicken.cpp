
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Chicken.h"
#include "../World.h"








cChicken::cChicken(void) :
	super("Chicken", mtChicken, "mob.chicken.hurt", "mob.chicken.hurt", 0.3, 0.4),
	m_DropEggCount(0)
{
}




void cChicken::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if (m_DropEggCount == 6000 && m_World->GetTickRandomNumber(1) == 0)
	{
		cItems Drops;
		m_DropEggCount = 0;
		Drops.push_back(cItem(E_ITEM_EGG, 1));
		m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ(), 10);
	}
	else if (m_DropEggCount == 12000)
	{
		cItems Drops;
		m_DropEggCount = 0;
		Drops.push_back(cItem(E_ITEM_EGG, 1));
		m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ(), 10);
	}
	else
	{
		m_DropEggCount++;
	}
}





void cChicken::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_FEATHER);
	a_Drops.push_back(cItem(IsOnFire() ? E_ITEM_COOKED_CHICKEN : E_ITEM_RAW_CHICKEN, 1));
}








