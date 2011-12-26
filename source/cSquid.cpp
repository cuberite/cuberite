#include "cSquid.h"
#include "Vector3d.h"

cSquid::cSquid()
{
	m_MobType = 94;
	GetMonsterConfig("Squid");
	m_NoWater = 0.f;
}

cSquid::~cSquid()
{
	
}

bool cSquid::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cSquid" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cSquid::KilledBy( cEntity* a_Killer )
{
	//Drops 0-3 Ink Sacs
	cMonster::RandomDropItem(E_ITEM_DYE, 0, 3);

	cMonster::KilledBy( a_Killer );
}

void cSquid::Tick(float a_Dt)
{
	cPassiveMonster::Tick(a_Dt);
	
	Vector3d Pos = GetPosition();


	//TODO Not a real behavior, but cool :D
	if(!IsBlockWater(GetWorld()->GetBlock(Pos.x, Pos.y, Pos.z)))
	{
		//Die slowly Muhahaha :D (To prevent all this squids on the land :D)
		m_NoWater += a_Dt;

		if(m_NoWater > 1000.f)
		{
			m_NoWater -= 1000.f;
			TakeDamage(1, this);
		}
	}

}