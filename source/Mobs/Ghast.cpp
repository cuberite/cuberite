
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Ghast.h"





cGhast::cGhast(void)
{
	m_MobType = 56;
	GetMonsterConfig("Ghast");
}





void cGhast::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_GUNPOWDER);
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_GHAST_TEAR);
}




