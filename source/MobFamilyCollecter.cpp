
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobFamilyCollecter.h"
#include "Mobs/Monster.h"



cMobFamilyCollecter::tMobFamilyList cMobFamilyCollecter::InitMobFamilyBeforeCx11(void)
{
	std::set<cMonster::eFamily> toReturn;
	toReturn.insert(cMonster::mfHostile);
	toReturn.insert(cMonster::mfPassive);
	toReturn.insert(cMonster::mfAmbient);
	toReturn.insert(cMonster::mfWater);
	return toReturn;
}





cMobFamilyCollecter::tMobFamilyList & cMobFamilyCollecter::m_AllFamilies(void)
{
	// TODO: This memory is leaked:
	static tMobFamilyList * AllFamilies = new tMobFamilyList(InitMobFamilyBeforeCx11());
	return *AllFamilies;
}





void cMobFamilyCollecter::CollectMob(cMonster& a_Monster)
{
	cMonster::eFamily MobFamily = a_Monster.GetMobFamily();
	m_Mobs[MobFamily].insert(&a_Monster);
}





int cMobFamilyCollecter::GetNumberOfCollectedMobs(cMonster::eFamily a_Family)
{
	return m_Mobs[a_Family].size();
}




