
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Chicken.h"





// TODO Drop egg every 5-10 minutes





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
	cItems Drops;
	AddRandomDropItem(Drops, 0, 2, E_ITEM_FEATHER);
	Drops.push_back(cItem((GetMetaData() == BURNING) ? E_ITEM_COOKED_CHICKEN : E_ITEM_RAW_CHICKEN, 1));
	m_World->SpawnItemPickups(Drops, m_Pos.x, m_Pos.y, m_Pos.z);

	cMonster::KilledBy( a_Killer );
}




