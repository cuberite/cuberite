
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Cow.h"





// TODO: Milk Cow





cCow::cCow(void)
{
	m_MobType = 92;
	m_SoundHurt = "mob.cow.hurt";
	m_SoundDeath = "mob.cow.hurt";
	GetMonsterConfig("Cow");
}





void cCow::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_LEATHER);
	AddRandomDropItem(a_Drops, 1, 3, (GetMetaData() == BURNING) ? E_ITEM_STEAK : E_ITEM_RAW_BEEF);
}




