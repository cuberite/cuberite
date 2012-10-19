
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Mooshroom.h"





// TODO: Milk Cow





cMooshroom::cMooshroom()
{
	m_MobType = 96;
	GetMonsterConfig("Mooshroom");
}





cMooshroom::~cMooshroom()
{
}





bool cMooshroom::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cMooshroom" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}





void cMooshroom::KilledBy( cEntity* a_Killer )
{
	cItems Drops;
	AddRandomDropItem(Drops, 0, 2, E_ITEM_LEATHER);
	AddRandomDropItem(Drops, 1, 3, (GetMetaData() == BURNING) ? E_ITEM_STEAK : E_ITEM_RAW_BEEF);
	m_World->SpawnItemPickups(Drops, m_Pos.x, m_Pos.y, m_Pos.z);

	cMonster::KilledBy( a_Killer );
}




