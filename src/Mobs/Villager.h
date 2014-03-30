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





class cVillager :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:

	enum eVillagerType
	{
		vtFarmer     = 0,
		vtLibrarian  = 1,
		vtPriest     = 2,
		vtBlacksmith = 3,
		vtButcher    = 4,
		vtGeneric    = 5,
		vtMax
	} ;

	cVillager(eVillagerType VillagerType);

	CLASS_PROTODEF(cVillager);

	// cEntity overrides
	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void Tick        (float a_Dt, cChunk & a_Chunk) override;

	// cVillager functions
	/** return true if the given blocktype are: crops, potatoes or carrots.*/
	bool IsBlockFarmable(BLOCKTYPE a_BlockType);

	//////////////////////////////////////////////////////////////////
	// Farmer functions
	/** It searches in a 11x7x11 area for crops. If it found some it will navigate to them.*/
	void HandleFarmerPrepareFarmCrops();

	/** Looks if the farmer has reached it's destination, and if it's still crops and the destination is closer then 2 blocks it will harvest them.*/
	void HandleFarmerTryHarvestCrops();

	/** Replaces the crops he harvested.*/
	void HandleFarmerPlaceCrops();

	// Get and set functions.
	int      GetVilType(void)              const { return m_Type; }	
	Vector3i GetCropsPos(void)             const { return m_CropsPos; }
	bool     DoesHaveActionActivated(void) const { return m_VillagerAction; }

private:

	int m_ActionCountDown;
	int m_Type;
	bool m_VillagerAction;
	Vector3i m_CropsPos;

} ;




