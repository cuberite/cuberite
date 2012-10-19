
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Magmacube.h"





cMagmacube::cMagmacube()
{
	m_MobType = 62;
	GetMonsterConfig("Magmacube");
}





cMagmacube::~cMagmacube()
{
}





bool cMagmacube::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cMagmacube" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}





void cMagmacube::KilledBy( cEntity* a_Killer )
{
	cItems Drops;
	AddRandomDropItem(Drops, 0, 1, E_ITEM_MAGMA_CREAM);
	m_World->SpawnItemPickups(Drops, m_Pos.x, m_Pos.y, m_Pos.z);

	cMonster::KilledBy( a_Killer );
}




