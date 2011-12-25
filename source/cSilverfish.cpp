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

