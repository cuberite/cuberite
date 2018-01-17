// MobSpawnerEntity.h

// Declares the cMobSpawnerEntity class representing a single mob spawner in the world





#pragma once

#include "BlockEntity.h"
#include "../Entities/Player.h"





// tolua_begin

class cMobSpawnerEntity :
	public cBlockEntity
{
	typedef cBlockEntity Super;
public:

	// tolua_end

	cMobSpawnerEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);

	// cBlockEntity overrides:
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual bool UsedBy(cPlayer * a_Player) override;
	virtual bool Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	// tolua_begin

	/** Upate the active flag from the mob spawner. This function will called every 5 seconds from the Tick() function. */
	void UpdateActiveState(void);

	/** Sets the spawn delay to a new random value. */
	void ResetTimer(void);

	/** Spawns the entity. This function automaticly change the spawn delay! */
	void SpawnEntity(void);

	/** Returns the entity type that will be spawn by this mob spawner. */
	eMonsterType GetEntity(void) const { return m_Entity; }

	/** Sets the entity type who will be spawn by this mob spawner. */
	void SetEntity(eMonsterType a_EntityType) { m_Entity = a_EntityType; }

	/** Returns the spawn delay. This is the tick delay that is needed to spawn new monsters. */
	short GetSpawnDelay(void) const { return m_SpawnDelay; }

	/** Sets the spawn delay. */
	void SetSpawnDelay(short a_Delay) { m_SpawnDelay = a_Delay; }

	/** Returns the amount of the nearby players in a 16-block radius. */
	int GetNearbyPlayersNum(void);

	/** Returns the amount of this monster type in a 8-block radius (Y: 4-block radius). */
	int GetNearbyMonsterNum(eMonsterType a_EntityType);

	// tolua_end

	static const char * GetClassStatic(void) { return "cMobSpawnerEntity"; }

private:
	/** The entity to spawn. */
	eMonsterType m_Entity;

	short m_SpawnDelay;

	bool m_IsActive;
} ;  // tolua_end




