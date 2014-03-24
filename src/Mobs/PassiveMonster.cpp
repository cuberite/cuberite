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

#include "PassiveMonster.h"
#include "../World.h"
#include "../Entities/Player.h"




cPassiveMonster::cPassiveMonster(const AString & a_ConfigName, eType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height) :
	super(a_ConfigName, a_MobType, a_SoundHurt, a_SoundDeath, a_Width, a_Height)
{
	m_EMPersonality = PASSIVE;
}





void cPassiveMonster::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	super::DoTakeDamage(a_TDI);
	if ((a_TDI.Attacker != this) && (a_TDI.Attacker != NULL))
	{
		m_EMState = ESCAPING;
	}
}





void cPassiveMonster::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if (m_EMState == ESCAPING)
	{
		CheckEventLostPlayer();
	}
	cItem FollowedItem = GetFollowedItem();
	if (FollowedItem.IsEmpty())
	{
		return;
	}
	cPlayer * a_Closest_Player = m_World->FindClosestPlayer(GetPosition(), (float)m_SightDistance);
	if (a_Closest_Player != NULL)
	{
		if (a_Closest_Player->GetEquippedItem().IsEqual(FollowedItem))
		{
			Vector3d PlayerPos = a_Closest_Player->GetPosition();
			MoveToPosition(PlayerPos);
		}
	}
}





