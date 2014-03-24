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

#include "Ghast.h"
#include "../World.h"




cGhast::cGhast(void) :
	super("Ghast", mtGhast, "mob.ghast.scream", "mob.ghast.death", 4, 4)
{
}





void cGhast::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_GUNPOWDER);
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_GHAST_TEAR);
}





void cGhast::Attack(float a_Dt)
{
	m_AttackInterval += a_Dt * m_AttackRate;
	
	if (m_Target != NULL && m_AttackInterval > 3.0)
	{
		// Setting this higher gives us more wiggle room for attackrate
		Vector3d Speed = GetLookVector() * 20;
		Speed.y = Speed.y + 1;
		cGhastFireballEntity * GhastBall = new cGhastFireballEntity(this, GetPosX(), GetPosY() + 1, GetPosZ(), Speed);
		if (GhastBall == NULL)
		{
			return;
		}
		if (!GhastBall->Initialize(m_World))
		{
			delete GhastBall;
			return;
		}
		m_World->BroadcastSpawnEntity(*GhastBall);
		m_AttackInterval = 0.0;
	}
}



