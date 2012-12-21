
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Magmacube.h"





cMagmacube::cMagmacube()
{
	m_MobType = 62;
	GetMonsterConfig("Magmacube");
}





bool cMagmacube::IsA(const char * a_EntityType)
{
	return ((strcmp(a_EntityType, "cMagmacube") == 0) || super::IsA(a_EntityType));
}





void cMagmacube::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_MAGMA_CREAM);
}




