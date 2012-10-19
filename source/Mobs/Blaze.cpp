
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Blaze.h"





cBlaze::cBlaze()
{
	m_MobType = 61;
	GetMonsterConfig("Blaze");
}





cBlaze::~cBlaze()
{
}





bool cBlaze::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cBlaze" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}





void cBlaze::KilledBy( cEntity* a_Killer )
{
	cItems Drops;
	AddRandomDropItem(Drops, 0, 1, E_ITEM_BLAZE_ROD);
	m_World->SpawnItemPickups(Drops, m_Pos.x, m_Pos.y, m_Pos.z);

	cMonster::KilledBy( a_Killer );
}




