#include "cChicken.h"

//TODO Drop egg every 5-10 minutes


cChicken::cChicken()
{
	m_MobType = 93;
	GetMonsterConfig("Chicken");
}

cChicken::~cChicken()
{
}

bool cChicken::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cChicken" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cChicken::KilledBy( cEntity* a_Killer )
{
	//Drops 0-2 Feathers
	cMonster::RandomDropItem(E_ITEM_FEATHER, 0, 2);

	// Raw Chicken 
	// TODO: (Check wheather it is burning to drop cooked Chicken)
	cMonster::DropItem(E_ITEM_RAW_CHICKEN, 1);

	cMonster::KilledBy( a_Killer );
}