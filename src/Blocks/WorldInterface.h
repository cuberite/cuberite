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

#include "BroadcastInterface.h"
#include "../Mobs/Monster.h"

class cItems;

class cWorldInterface
{
public:
	virtual ~cWorldInterface() {}
	
	virtual Int64 GetTimeOfDay(void) const = 0;
	virtual Int64 GetWorldAge(void)  const = 0;
	
	virtual eDimension GetDimension(void) const = 0;
	
	virtual cBroadcastInterface & GetBroadcastManager() = 0;
	
	virtual void DoExplosionAt(double a_ExplosionSize, double a_BlockX, double a_BlockY, double a_BlockZ, bool a_CanCauseFire, eExplosionSource a_Source, void * a_SourceData) = 0; 
	
	/** Spawns item pickups for each item in the list. May compress pickups if too many entities: */
	virtual void SpawnItemPickups(const cItems & a_Pickups, double a_BlockX, double a_BlockY, double a_BlockZ, double a_FlyAwaySpeed = 1.0, bool IsPlayerCreated = false) = 0;
	
	/** Spawns item pickups for each item in the list. May compress pickups if too many entities. All pickups get the speed specified: */
	virtual void SpawnItemPickups(const cItems & a_Pickups, double a_BlockX, double a_BlockY, double a_BlockZ, double a_SpeedX, double a_SpeedY, double a_SpeedZ, bool IsPlayerCreated = false) = 0;
	
	/** Spawns a mob of the specified type. Returns the mob's EntityID if recognized and spawned, <0 otherwise */
	virtual int SpawnMob(double a_PosX, double a_PosY, double a_PosZ, cMonster::eType a_MonsterType) = 0;

	/** Sends the block on those coords to the player */
	virtual void SendBlockTo(int a_BlockX, int a_BlockY, int a_BlockZ, cPlayer * a_Player) = 0;

	/** Calls the callback for each player in the list; returns true if all players processed, false if the callback aborted by returning true */
	virtual bool ForEachPlayer(cItemCallback<cPlayer> & a_Callback) = 0;

	virtual void SetTimeOfDay(Int64 a_TimeOfDay) = 0;

};
