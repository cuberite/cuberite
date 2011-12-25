#include "cCavespider.h"

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
	cMonster::RandomDropItem(E_ITEM_STRING, 0, 2);

	cMonster::RandomDropItem(E_ITEM_SPIDER_EYE, 0, 1);

	cMonster::KilledBy( a_Killer );
}
