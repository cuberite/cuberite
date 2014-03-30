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

#include "PassiveAggressiveMonster.h"
#include "../Entities/Entity.h"





class cWolf :
	public cPassiveAggressiveMonster
{
	typedef cPassiveAggressiveMonster super;
	
public:
	cWolf(void);

	CLASS_PROTODEF(cWolf);

	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void TickFollowPlayer();
	virtual void Attack(float a_Dt) override;

	// Get functions
	bool    IsSitting     (void) const { return m_IsSitting; }
	bool    IsTame        (void) const { return m_IsTame; }
	bool    IsBegging     (void) const { return m_IsBegging; }
	bool    IsAngry       (void) const { return m_IsAngry; }
	AString GetOwner      (void) const { return m_OwnerName; }
	int     GetCollarColor(void) const { return m_CollarColor; }

	// Set functions
	void SetIsSitting  (bool a_IsSitting)   { m_IsSitting = a_IsSitting; }
	void SetIsTame     (bool a_IsTame)      { m_IsTame = a_IsTame; }
	void SetIsBegging  (bool a_IsBegging)   { m_IsBegging = a_IsBegging; }
	void SetIsAngry    (bool a_IsAngry)     { m_IsAngry = a_IsAngry; }
	void SetOwner      (AString a_NewOwner) { m_OwnerName = a_NewOwner; }
	void SetCollarColor(int a_CollarColor)  { m_CollarColor = a_CollarColor; }

protected:

	bool    m_IsSitting;
	bool    m_IsTame;
	bool    m_IsBegging;
	bool    m_IsAngry;
	AString m_OwnerName;
	int     m_CollarColor;
} ;




