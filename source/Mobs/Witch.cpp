
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Witch.h"





cWitch::cWitch()
{
	m_MobType = 66;
	GetMonsterConfig("Witch");
}





cWitch::~cWitch()
{
}





bool cWitch::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cWitch" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}





void cWitch::KilledBy( cEntity* a_Killer )
{
	cItems Drops;
	AddRandomDropItem(Drops, 0, 6, E_ITEM_GLASS_BOTTLE);
	AddRandomDropItem(Drops, 0, 6, E_ITEM_GLOWSTONE_DUST);
	AddRandomDropItem(Drops, 0, 6, E_ITEM_GUNPOWDER);
	AddRandomDropItem(Drops, 0, 6, E_ITEM_REDSTONE_DUST);
	AddRandomDropItem(Drops, 0, 6, E_ITEM_SPIDER_EYE);
	AddRandomDropItem(Drops, 0, 6, E_ITEM_STICK);
	AddRandomDropItem(Drops, 0, 6, E_ITEM_SUGAR);
	m_World->SpawnItemPickups(Drops, m_Pos.x, m_Pos.y, m_Pos.z);

	cMonster::KilledBy( a_Killer );
}




