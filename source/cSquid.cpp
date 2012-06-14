
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

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
	// Drops 0-3 Ink Sacs
	cItems Drops;
	AddRandomDropItem(Drops, 0, 3, E_ITEM_DYE, E_META_DYE_BLACK);
	m_World->SpawnItemPickups(Drops, m_Pos.x, m_Pos.y, m_Pos.z);

	cMonster::KilledBy( a_Killer );
}





void cSquid::Tick(float a_Dt)
{
	cPassiveMonster::Tick(a_Dt);
	
	Vector3d Pos = GetPosition();


	//TODO Not a real behavior, but cool :D
	if(!IsBlockWater(GetWorld()->GetBlock((int) Pos.x, (int) Pos.y, (int) Pos.z)) && GetMetaData() != BURNING)
	{
		SetMetaData(BURNING);
	}

}