
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Bat.h"





cBat::cBat(void)
{
	m_MobType = 65;
	GetMonsterConfig("Bat");
}





bool cBat::IsA(const char * a_EntityType)
{
	return ((strcmp(a_EntityType, "cBat") == 0) || super::IsA(a_EntityType));
}





