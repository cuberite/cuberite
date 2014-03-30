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

#include "Monster.h"





class cPassiveMonster :
	public cMonster
{
	typedef cMonster super;
	
public:
	cPassiveMonster(const AString & a_ConfigName, eType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);

	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	/// When hit by someone, run away
	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	/** Returns the item that the animal of this class follows when a player holds it in hand
	Return an empty item not to follow (default). */
	virtual const cItem GetFollowedItem(void) const { return cItem(); }

} ;




