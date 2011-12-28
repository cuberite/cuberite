#include "cCow.h"

//TODO: Milk Cow

cCow::cCow()
{
	m_MobType = 92;
	GetMonsterConfig("Cow");
}

cCow::~cCow()
{
}

bool cCow::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cCow" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cCow::KilledBy( cEntity* a_Killer )
{
	//Drops 0-2 Lether
	cMonster::RandomDropItem(E_ITEM_LEATHER, 0, 2);

	if(GetMetaData() == BURNING)
	{
		cMonster::RandomDropItem(E_ITEM_STEAK, 1, 3);
	}else{
		cMonster::RandomDropItem(E_ITEM_RAW_BEEF, 1, 3);
	}
	

	cMonster::KilledBy( a_Killer );
}
