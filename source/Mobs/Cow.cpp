
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Cow.h"





// TODO: Milk Cow





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
	cItems Drops;
	AddRandomDropItem(Drops, 0, 2, E_ITEM_LEATHER);
	AddRandomDropItem(Drops, 1, 3, (GetMetaData() == BURNING) ? E_ITEM_STEAK : E_ITEM_RAW_BEEF);
	m_World->SpawnItemPickups(Drops, m_Pos.x, m_Pos.y, m_Pos.z);

	cMonster::KilledBy( a_Killer );
}




