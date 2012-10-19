
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Villager.h"





cVillager::cVillager()
{
	m_MobType = 120;
	GetMonsterConfig("Villager");
}





cVillager::~cVillager()
{
}





bool cVillager::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cVillager" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}





void cVillager::KilledBy( cEntity* a_Killer )
{
	cMonster::KilledBy( a_Killer );
}




