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

#include "Slime.h"





/// Creates a slime of the specified size; size is 1 .. 3, with 1 being the smallest
cSlime::cSlime(int a_Size) :
	super("Slime", mtSlime, "mob.slime.attack", "mob.slime.attack", 0.6 * a_Size, 0.6 * a_Size),
	m_Size(a_Size)
{
}





void cSlime::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	if (GetSize() == 1)
	{
		AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_SLIMEBALL);
	}
}




