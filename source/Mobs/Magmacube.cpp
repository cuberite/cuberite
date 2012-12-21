
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Magmacube.h"





cMagmacube::cMagmacube()
{
	m_MobType = 62;
	m_SoundHurt = "mob.magmacube.big";
	m_SoundDeath = "mob.magmacube.big";
	GetMonsterConfig("Magmacube");
}





void cMagmacube::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_MAGMA_CREAM);
}




