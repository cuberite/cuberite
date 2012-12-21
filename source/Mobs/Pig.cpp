
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pig.h"





cPig::cPig(void)
{
	m_MobType = 90;
	GetMonsterConfig("Pig");
}





void cPig::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 1, 3, (GetMetaData() == BURNING) ? E_ITEM_COOKED_PORKCHOP : E_ITEM_RAW_MEAT);
}




