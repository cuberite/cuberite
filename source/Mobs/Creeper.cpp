
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Creeper.h"





cCreeper::cCreeper(void)
{
	m_MobType = 50;
	m_SoundHurt = "mob.creeper.say";
	m_SoundDeath = "mob.creeper.say";
	GetMonsterConfig("Creeper");
}





void cCreeper::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_GUNPOWDER);

	// TODO Check if killed by a skeleton, then drop random music disk
}




