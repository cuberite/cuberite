
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Creeper.h"





cCreeper::cCreeper(void)
{
	m_MobType = 50;
	GetMonsterConfig("Creeper");
}





bool cCreeper::IsA(const char * a_EntityType)
{
	return ((strcmp(a_EntityType, "cCreeper") == 0) || super::IsA(a_EntityType));
}





void cCreeper::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_GUNPOWDER);

	// TODO Check if killed by a skeleton, then drop random music disk
}




