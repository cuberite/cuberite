
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cCreeper.h"





cCreeper::cCreeper()
{
	m_MobType = 50;
	GetMonsterConfig("Creeper");
}





cCreeper::~cCreeper()
{
}





bool cCreeper::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cCreeper" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}





void cCreeper::KilledBy( cEntity* a_Killer )
{
	cItems Drops;
	AddRandomDropItem(Drops, 0, 2, E_ITEM_GUNPOWDER);

	// TODO Check if killed by a skeleton, then drop random music disk

	m_World->SpawnItemPickups(Drops, m_Pos.x, m_Pos.y, m_Pos.z);

	cMonster::KilledBy( a_Killer );
}




