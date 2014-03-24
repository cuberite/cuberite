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

#include "Zombiepigman.h"
#include "../World.h"





cZombiePigman::cZombiePigman(void) :
	super("ZombiePigman", mtZombiePigman, "mob.zombiepig.zpighurt", "mob.zombiepig.zpigdeath", 0.6, 1.8)
{
}





void cZombiePigman::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_ROTTEN_FLESH);
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_GOLD_NUGGET);
	
	cItems RareDrops;
	RareDrops.Add(cItem(E_ITEM_GOLD));
	AddRandomRareDropItem(a_Drops, RareDrops, LootingLevel);
	AddRandomArmorDropItem(a_Drops, LootingLevel);
	AddRandomWeaponDropItem(a_Drops, LootingLevel);
}





void cZombiePigman::KilledBy(cEntity * a_Killer)
{
	super::KilledBy(a_Killer);

	if ((a_Killer != NULL) && (a_Killer->IsPlayer()))
	{
		// TODO: Anger all nearby zombie pigmen
		// TODO: In vanilla, if one player angers ZPs, do they attack any nearby player, or only that one attacker?
	}
}




