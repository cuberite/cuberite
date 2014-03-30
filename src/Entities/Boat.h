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

// Boat.h

// Declares the cBoat class representing a boat in the world





#pragma once

#include "Entity.h"





class cBoat :
	public cEntity
{
	typedef cEntity super;
	
public:
	CLASS_PROTODEF(cBoat);
		
	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void DoTakeDamage(TakeDamageInfo & TDI) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void HandleSpeedFromAttachee(float a_Forward, float a_Sideways) override;
	
	cBoat(double a_X, double a_Y, double a_Z);
} ;




