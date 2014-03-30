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

#include "AggressiveMonster.h"





class cWither :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cWither(void);

	CLASS_PROTODEF(cWither);

	unsigned int GetNumInvulnerableTicks(void) const { return m_InvulnerableTicks; }

	void SetNumInvulnerableTicks(unsigned int a_Ticks) { m_InvulnerableTicks = a_Ticks; }
	
	// cEntity overrides
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

private:

	/** The number of ticks of invulnerability left after being initially created. Zero once invulnerability has expired. */
	unsigned int m_InvulnerableTicks;

} ;




