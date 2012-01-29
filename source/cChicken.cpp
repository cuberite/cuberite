
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

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
	if(GetMetaData() == BURNING)
	{
		cMonster::DropItem(E_ITEM_COOKED_CHICKEN, 1);
	}else{
		cMonster::DropItem(E_ITEM_RAW_CHICKEN, 1);
	}

	cMonster::KilledBy( a_Killer );
}