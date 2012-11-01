
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Enderman.h"





cEnderman::cEnderman()
{
	m_MobType = 58;
	GetMonsterConfig("Enderman");
}





cEnderman::~cEnderman()
{
}





bool cEnderman::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cEnderman" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}





void cEnderman::Tick(float a_Dt)
{
	cMonster::Tick(a_Dt);

	// TODO Same as stated in cSkeleton
	if ((GetWorld()->GetTimeOfDay() < (12000 + 1000)) && (GetMetaData() != BURNING))
	{
		SetMetaData(BURNING); // BURN, BABY, BURN!  >:D
	}
}





void cEnderman::KilledBy( cEntity* a_Killer )
{
	cItems Drops;
	AddRandomDropItem(Drops, 0, 1, E_ITEM_ENDER_PEARL);
	m_World->SpawnItemPickups(Drops, m_Pos.x, m_Pos.y, m_Pos.z);

	cMonster::KilledBy( a_Killer );
}




