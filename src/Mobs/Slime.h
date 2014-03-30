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





class cSlime :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	/// Creates a slime of the specified size; size is 1 .. 3, with 1 being the smallest
	cSlime(int a_Size);

	CLASS_PROTODEF(cSlime);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	int GetSize(void) const { return m_Size; }
	
protected:

	/// Size of the slime, 1 .. 3, with 1 being the smallest
	int m_Size;
} ;




