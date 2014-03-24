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

#include "Entity.h"





// tolua_begin
class cFloater :
	public cEntity
{
	typedef cEntity super;
	
public:
	//tolua_end

	CLASS_PROTODEF(cFloater);

	cFloater(double a_X, double a_Y, double a_Z, Vector3d a_Speed, int a_PlayerID, int a_CountDownTime);

	virtual void SpawnOn(cClientHandle & a_Client) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	
	// tolua_begin
	bool CanPickup(void)       const { return m_CanPickupItem; }
	int GetOwnerID(void)       const { return m_PlayerID; }
	int GetAttachedMobID(void) const { return m_AttachedMobID; }
	// tolua_end

protected:
	// Position
	Vector3d m_ParticlePos;

	// Bool needed to check if you can get a fish.
	bool m_CanPickupItem;

	// Countdown times
	int m_PickupCountDown;
	int m_CountDownTime;

	// Entity IDs
	int m_PlayerID;
	int m_AttachedMobID;
} ; // tolua_export