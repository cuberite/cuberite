
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Skeleton.h"





cSkeleton::cSkeleton(void)
{
	m_MobType = 51;
	m_SoundHurt = "mob.skeleton.hurt";
	m_SoundDeath = "mob.skeleton.death";
	GetMonsterConfig("Skeleton");
}





void cSkeleton::Tick(float a_Dt)
{
	cMonster::Tick(a_Dt);

	// TODO Outsource
	// TODO should do SkyLight check, mobs in the dark don´t burn 
	if ((GetWorld()->GetTimeOfDay() < (12000 + 1000)) && (GetMetaData() != BURNING))
	{
		SetMetaData(BURNING); // BURN, BABY, BURN!  >:D
	}
}





void cSkeleton::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_ARROW);
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_BONE);
}




