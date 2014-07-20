
#pragma once

#include "BroadcastInterface.h"
#include "../Mobs/Monster.h"

class cItems;

typedef cItemCallback<cBlockEntity> cBlockEntityCallback;





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

	/** Calls the callback for the block entity at the specified coords; returns false if there's no block entity at those coords, true if found */
	virtual bool DoWithBlockEntityAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBlockEntityCallback & a_Callback) = 0;

	/** Sends the block on those coords to the player */
	virtual void SendBlockTo(int a_BlockX, int a_BlockY, int a_BlockZ, cPlayer * a_Player) = 0;

	/** Calls the callback for each player in the list; returns true if all players processed, false if the callback aborted by returning true */
	virtual bool ForEachPlayer(cItemCallback<cPlayer> & a_Callback) = 0;

	virtual void SetTimeOfDay(Int64 a_TimeOfDay) = 0;

	/** Wakes up the simulators for the specified block */
	virtual void WakeUpSimulators(int a_BlockX, int a_BlockY, int a_BlockZ) = 0;

};
