
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Zombie.h"





cZombie::cZombie()
{
	m_MobType = 54;
	GetMonsterConfig("Zombie");
}





void cZombie::Tick(float a_Dt)
{
	super::Tick(a_Dt);

	// TODO Same as in cSkeleton :D
	if ((GetWorld()->GetTimeOfDay() < (12000 + 1000)) && (GetMetaData() != BURNING))
	{
		SetMetaData(BURNING); // BURN, BABY, BURN!  >:D
	}
}





void cZombie::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_ROTTEN_FLESH);
	
	// TODO: Rare drops
}




