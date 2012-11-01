
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Bat.h"





cBat::cBat()
{
	m_MobType = 65;
	GetMonsterConfig("Bat");
}





cBat::~cBat()
{
}





bool cBat::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cBat" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}





void cBat::KilledBy( cEntity* a_Killer )
{
	cMonster::KilledBy( a_Killer );
}




