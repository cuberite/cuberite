
#pragma once

#include "BroadcastInterface.h"
#include "../Mobs/Monster.h"

class cItems;

class cWorldInterface
{
public:

	virtual Int64 GetTimeOfDay(void) const = 0;
	virtual Int64 GetWorldAge(void)  const = 0;
	
	virtual eDimension GetDimension(void) const = 0;
	
	virtual cBroadcastInterface & GetBroadcastManager() = 0;
	
	virtual void DoExplosionAt(double a_ExplosionSize, double a_BlockX, double a_BlockY, double a_BlockZ, bool a_CanCauseFire, eExplosionSource a_Source, void * a_SourceData) = 0; 
	
	/** Spawns an experience orb at the given location with the given reward. It returns the UniqueID of the spawned experience orb. */
	virtual int SpawnExperienceOrb(double a_X, double a_Y, double a_Z, int a_Reward) = 0;
	
	/** Spawns item pickups for each item in the list. May compress pickups if too many entities: */
	virtual void SpawnItemPickups(const cItems & a_Pickups, double a_BlockX, double a_BlockY, double a_BlockZ, double a_FlyAwaySpeed = 1.0, bool IsPlayerCreated = false) = 0;
	
	/** Spawns item pickups for each item in the list. May compress pickups if too many entities. All pickups get the speed specified: */
	virtual void SpawnItemPickups(const cItems & a_Pickups, double a_BlockX, double a_BlockY, double a_BlockZ, double a_SpeedX, double a_SpeedY, double a_SpeedZ, bool IsPlayerCreated = false) = 0;
	
	/** Spawns a mob of the specified type. Returns the mob's EntityID if recognized and spawned, <0 otherwise */
	virtual int SpawnMob(double a_PosX, double a_PosY, double a_PosZ, cMonster::eType a_MonsterType) = 0;
};
