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

#include "PassiveMonster.h"





class cSheep :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cSheep(int a_Color);
	
	CLASS_PROTODEF(cSheep);
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	virtual const cItem GetFollowedItem(void) const override { return cItem(E_ITEM_WHEAT); }

	bool IsSheared(void) const { return m_IsSheared; }
	int GetFurColor(void) const { return m_WoolColor; }

private:
	
	bool m_IsSheared;
	int m_WoolColor;
	int m_TimeToStopEating;

} ;




