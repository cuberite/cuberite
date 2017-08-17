
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobFamilyCollecter.h"





void cMobFamilyCollecter::CollectMob(cMonster & a_Monster)
{
	cMonster::eFamily MobFamily = a_Monster.GetMobFamily();
	m_Mobs[MobFamily].insert(&a_Monster);
}





int cMobFamilyCollecter::GetNumberOfCollectedMobs(cMonster::eFamily a_Family)
{
	return static_cast<int>(m_Mobs[a_Family].size());
}




