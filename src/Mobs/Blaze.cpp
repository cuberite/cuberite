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

#include "Blaze.h"
#include "../World.h"




cBlaze::cBlaze(void) :
	// TODO: The size is only a guesstimate, measure in vanilla and fix the size values here
	super("Blaze", mtBlaze, "mob.blaze.hit", "mob.blaze.death", 0.7, 1.8)
{
}





void cBlaze::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if ((a_Killer != NULL) && (a_Killer->IsPlayer() || a_Killer->IsA("cWolf")))
	{
		int LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
		AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_BLAZE_ROD);
	}
}





void cBlaze::Attack(float a_Dt)
{
	m_AttackInterval += a_Dt * m_AttackRate;

	if (m_Target != NULL && m_AttackInterval > 3.0)
	{
		// Setting this higher gives us more wiggle room for attackrate
		Vector3d Speed = GetLookVector() * 20;
		Speed.y = Speed.y + 1;
		cFireChargeEntity * FireCharge = new cFireChargeEntity(this, GetPosX(), GetPosY() + 1, GetPosZ(), Speed);
		if (FireCharge == NULL)
		{
			return;
		}
		if (!FireCharge->Initialize(m_World))
		{
			delete FireCharge;
			return;
		}
		m_World->BroadcastSpawnEntity(*FireCharge);
		m_AttackInterval = 0.0;
		// ToDo: Shoot 3 fireballs instead of 1.
	}
}