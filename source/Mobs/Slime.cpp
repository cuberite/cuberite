
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Slime.h"

// TODO: Implement sized slimes





cSlime::cSlime(void)
{
	m_MobType = 55;
	GetMonsterConfig("Slime");
}





bool cSlime::IsA(const char * a_EntityType)
{
	return ((strcmp(a_EntityType, "cSlime") == 0) || super::IsA(a_EntityType));
}





void cSlime::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	// TODO: only when tiny
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_SLIMEBALL);
}




