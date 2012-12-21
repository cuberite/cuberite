
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Blaze.h"





cBlaze::cBlaze(void)
{
	m_MobType = 61;
	GetMonsterConfig("Blaze");
}





void cBlaze::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_BLAZE_ROD);
}




