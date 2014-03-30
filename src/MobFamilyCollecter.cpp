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

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobFamilyCollecter.h"
#include "Mobs/Monster.h"



void cMobFamilyCollecter::CollectMob(cMonster & a_Monster)
{
	cMonster::eFamily MobFamily = a_Monster.GetMobFamily();
	m_Mobs[MobFamily].insert(&a_Monster);
}





int cMobFamilyCollecter::GetNumberOfCollectedMobs(cMonster::eFamily a_Family)
{
	return m_Mobs[a_Family].size();
}




