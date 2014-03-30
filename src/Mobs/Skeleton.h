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





class cSkeleton :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cSkeleton(bool IsWither);

	CLASS_PROTODEF(cSkeleton);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void MoveToPosition(const Vector3f & a_Position) override;
	virtual void Attack(float a_Dt) override;
	bool IsWither(void) const { return m_bIsWither; };

private:

	bool m_bIsWither;

} ;




