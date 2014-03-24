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





class cZombie :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cZombie(bool a_IsVillagerZombie);

	CLASS_PROTODEF(cZombie);
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void MoveToPosition(const Vector3f & a_Position) override;

	bool IsVillagerZombie(void) const {return m_IsVillagerZombie; }
	bool IsConverting    (void) const {return m_IsConverting; }

private:

	bool m_IsVillagerZombie;
	bool m_IsConverting;

} ;




