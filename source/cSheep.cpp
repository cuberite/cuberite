
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cSheep.h"





//Todo: Implement color





cSheep::cSheep(void) :
	m_IsSheared(false),
	m_WoolColor(0)  // TODO: E_META_WOOL_WHITE
{
	m_MobType = 91;
	GetMonsterConfig("Sheep");
}





cSheep::~cSheep()
{
}





bool cSheep::IsA( const char* a_EntityType )
{
	if (strcmp( a_EntityType, "cSheep" ) == 0)
	{
		return true;
	}
	return cMonster::IsA( a_EntityType );
}





void cSheep::KilledBy( cEntity* a_Killer )
{
	// TODO: Check whether it is sheared
	// TODO: Check color

	if (!m_IsSheared)
	{
		cItems Drops;
		Drops.push_back(cItem(E_ITEM_WHITE_CLOTH, 1, m_WoolColor));
		m_World->SpawnItemPickups(Drops, m_Pos.x, m_Pos.y, m_Pos.z);
	}

	cMonster::KilledBy( a_Killer );
}



