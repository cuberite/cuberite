
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Cavespider.h"





cCavespider::cCavespider()
{
	m_MobType = 59;
	GetMonsterConfig("Cavespider");
}





cCavespider::~cCavespider()
{
}





bool cCavespider::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cCavespider" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}





void cCavespider::Tick(float a_Dt)
{
	cMonster::Tick(a_Dt);
	m_EMPersonality = (GetWorld()->GetWorldTime() < (12000 + 1000) ) ? PASSIVE : AGGRESSIVE;
}





void cCavespider::KilledBy( cEntity* a_Killer )
{
	cItems Drops;
	AddRandomDropItem(Drops, 0, 2, E_ITEM_STRING);
	AddRandomDropItem(Drops, 0, 1, E_ITEM_SPIDER_EYE);

	cMonster::KilledBy( a_Killer );
}




