
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Villager.h"





cVillager::cVillager()
{
	m_MobType = 120;
	GetMonsterConfig("Villager");
}





bool cVillager::IsA(const char * a_EntityType)
{
	return ((strcmp(a_EntityType, "cVillager") == 0) || super::IsA(a_EntityType));
}




