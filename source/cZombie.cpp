
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cZombie.h"





cZombie::cZombie()
{
	m_MobType = 54;
	GetMonsterConfig("Zombie");
}





cZombie::~cZombie()
{
}





bool cZombie::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cZombie" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}





void cZombie::Tick(float a_Dt)
{
	cMonster::Tick(a_Dt);

	//TODO Same as in cSkeleton :D
	if (GetWorld()->GetWorldTime() < (12000 + 1000) && GetMetaData() != BURNING) { //if daylight
		SetMetaData(BURNING); // BURN, BABY, BURN!  >:D
	}
}





void cZombie::KilledBy( cEntity* a_Killer )
{
	cItems Drops;
	AddRandomDropItem(Drops, 0, 2, E_ITEM_ROTTEN_FLESH);
	
	// TODO: Rare drops
	
	m_World->SpawnItemPickups(Drops, m_Pos.x, m_Pos.y, m_Pos.z);

	cMonster::KilledBy( a_Killer );
}




