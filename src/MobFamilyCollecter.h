/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once

#include <map>
#include <set>
#include "BlockID.h"
#include "Mobs/Monster.h" //this is a side-effect of keeping Mobfamily inside Monster class. I'd prefer to keep both (Mobfamily and Monster) inside a "Monster" namespace MG TODO : do it




// fwd:
class cChunk;





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




