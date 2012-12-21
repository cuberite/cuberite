
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Enderman.h"





cEnderman::cEnderman(void)
{
	m_MobType = 58;
	GetMonsterConfig("Enderman");
}





void cEnderman::Tick(float a_Dt)
{
	cMonster::Tick(a_Dt);

	// TODO Same as stated in cSkeleton
	if ((GetWorld()->GetTimeOfDay() < (12000 + 1000)) && (GetMetaData() != BURNING))
	{
		SetMetaData(BURNING); // BURN, BABY, BURN!  >:D
	}
}





void cEnderman::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_ENDER_PEARL);
}




