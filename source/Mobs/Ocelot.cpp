
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Ocelot.h"





cOcelot::cOcelot()
{
	m_MobType = 98;
	GetMonsterConfig("Ocelot");
}





bool cOcelot::IsA(const char * a_EntityType)
{
	return ((strcmp(a_EntityType, "cOcelot") == 0) || super::IsA(a_EntityType));
}





