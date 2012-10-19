
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Ocelot.h"





cOcelot::cOcelot()
{
	m_MobType = 98;
	GetMonsterConfig("Ocelot");
}





cOcelot::~cOcelot()
{
}





bool cOcelot::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cOcelot" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}





void cOcelot::KilledBy( cEntity* a_Killer )
{
	cMonster::KilledBy( a_Killer );
}




