
#pragma once

#include "Mobs/Monster.h"  // This is a side-effect of keeping Mobfamily inside Monster class. I'd prefer to keep both (Mobfamily and Monster) inside a "Monster" namespace MG TODO : do it





/** This class is used to collect the list of mobs for each family
*/
class cMobFamilyCollecter
{
public :
	typedef const std::set<cMonster::eFamily> tMobFamilyList;

	// collect a mob
	void CollectMob(cMonster & a_Monster);

	// return the number of mobs for this family
	int GetNumberOfCollectedMobs(cMonster::eFamily a_Family);

protected :
	std::map<cMonster::eFamily, std::set<cMonster *> > m_Mobs;

} ;




