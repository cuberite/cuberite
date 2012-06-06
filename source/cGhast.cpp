
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cGhast.h"





cGhast::cGhast()
{
	m_MobType = 56;
	GetMonsterConfig("Ghast");
}





cGhast::~cGhast()
{
}





bool cGhast::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cGhast" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}





void cGhast::KilledBy( cEntity* a_Killer )
{
	cItems Drops;
	AddRandomDropItem(Drops, 0, 2, E_ITEM_GUNPOWDER);
	AddRandomDropItem(Drops, 0, 1, E_ITEM_GHAST_TEAR);

	cMonster::KilledBy( a_Killer );
}




