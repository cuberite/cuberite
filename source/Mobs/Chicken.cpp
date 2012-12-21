
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Chicken.h"





// TODO: Drop egg every 5-10 minutes





cChicken::cChicken(void)
{
	m_MobType = 93;
	GetMonsterConfig("Chicken");
}





bool cChicken::IsA(const char * a_EntityType)
{
	return ((strcmp(a_EntityType, "cChicken") == 0) || super::IsA(a_EntityType));
}





void cChicken::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_FEATHER);
	a_Drops.push_back(cItem((GetMetaData() == BURNING) ? E_ITEM_COOKED_CHICKEN : E_ITEM_RAW_CHICKEN, 1));
}




