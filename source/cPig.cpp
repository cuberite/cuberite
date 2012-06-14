
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPig.h"





cPig::cPig()
{
	m_MobType = 90;
	GetMonsterConfig("Pig");
}





cPig::~cPig()
{
}





bool cPig::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cPig" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}





void cPig::KilledBy( cEntity* a_Killer )
{
	cItems Drops;
	AddRandomDropItem(Drops, 0, 2, E_ITEM_RAW_MEAT);
	m_World->SpawnItemPickups(Drops, m_Pos.x, m_Pos.y, m_Pos.z);

	// TODO: Check for burning state

	cMonster::KilledBy( a_Killer );
}




