
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cSilverfish.h"





cSilverfish::cSilverfish()
{
	m_MobType = 60;
	GetMonsterConfig("Silverfish");
}

cSilverfish::~cSilverfish()
{
}

bool cSilverfish::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cSilverfish" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

