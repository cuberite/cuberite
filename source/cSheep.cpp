#include "cSheep.h"

//Todo: Implement color

cSheep::cSheep()
{
	m_MobType = 91;
	GetMonsterConfig("Sheep");
}

cSheep::~cSheep()
{
}

bool cSheep::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cSheep" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cSheep::KilledBy( cEntity* a_Killer )
{
	//Todo: Check wheather it is sheared
	//Todo: Check color

	cMonster::DropItem(E_ITEM_WHITE_CLOTH, 1);

	cMonster::KilledBy( a_Killer );
}