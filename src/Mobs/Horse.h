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





class cHorse :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cHorse(int Type, int Color, int Style, int TameTimes);

	CLASS_PROTODEF(cHorse);
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

	bool IsSaddled      (void) const  {return m_bIsSaddled; }
	bool IsChested      (void) const  {return m_bHasChest; }
	bool IsEating       (void) const  {return m_bIsEating; }
	bool IsRearing      (void) const  {return m_bIsRearing; }
	bool IsMthOpen      (void) const  {return m_bIsMouthOpen; }
	bool IsTame         (void) const  {return m_bIsTame; }
	int  GetHorseType   (void) const  {return m_Type; }
	int  GetHorseColor  (void) const  {return m_Color; }
	int  GetHorseStyle  (void) const  {return m_Style; }
	int  GetHorseArmour (void) const  {return m_Armour;}

private:

	bool m_bHasChest, m_bIsEating, m_bIsRearing, m_bIsMouthOpen, m_bIsTame, m_bIsSaddled;
	int m_Type, m_Color, m_Style, m_Armour, m_TimesToTame, m_TameAttemptTimes, m_RearTickCount;

} ;




