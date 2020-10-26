// MobSpawnerEntity.h

// Declares the cMobSpawnerEntity class representing a single mob spawner in the world





#pragma once

#include "BlockEntity.h"
#include "../Entities/Player.h"





// tolua_begin
class cMobSpawnerEntity :
	public cBlockEntity
{
	// tolua_end

	using Super = cBlockEntity;

public:  // tolua_export

	cMobSpawnerEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World);

	// cBlockEntity overrides:
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual bool UsedBy(cPlayer * a_Player) override;
	virtual bool Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	// tolua_begin

	/** Update the active flag from the mob spawner. This function will called every 5 seconds from the Tick() function. */
	void UpdateActiveState(void);

	/** Sets the spawn delay to a new random value. */
	void ResetTimer(void);

	/** Spawns the entity. This function automatically change the spawn delay! */
	void SpawnEntity(void);

	// Getters
	int GetNearbyMonsterNum(eMonsterType a_EntityType);
	int GetNearbyPlayersNum(void);

	eMonsterType GetEntity(void) const       { return m_Entity; }
	short GetSpawnCount(void) const          { return m_SpawnCount; }
	short GetSpawnRange(void) const          { return m_SpawnRange; }
	short GetSpawnDelay(void) const          { return m_SpawnDelay; }
	short GetMinSpawnDelay(void) const       { return m_MinSpawnDelay; }
	short GetMaxSpawnDelay(void) const       { return m_MaxSpawnDelay; }
	short GetMaxNearbyEntities(void) const   { return m_MaxNearbyEntities; }
	short GetRequiredPlayerRange(void) const { return m_RequiredPlayerRange; }

	// Setters
	void SetEntity(eMonsterType a_EntityType)                { m_Entity = a_EntityType; }
	void SetSpawnDelay(short a_Delay)                        { m_SpawnDelay = a_Delay; }
	void SetSpawnCount(short a_SpawnCount)                   { m_SpawnCount = a_SpawnCount; }
	void SetSpawnRange(short a_SpawnRange)                   { m_SpawnRange = a_SpawnRange; }
	void SetMinSpawnDelay(short a_Min)                       { m_MinSpawnDelay = a_Min; }
	void SetMaxSpawnDelay(short a_Max)                       { m_MaxSpawnDelay = a_Max; }
	void SetMaxNearbyEntities(short a_MaxNearbyEntities)     { m_MaxNearbyEntities = a_MaxNearbyEntities; }
	void SetRequiredPlayerRange(short a_RequiredPlayerRange) { m_RequiredPlayerRange = a_RequiredPlayerRange; }

	// tolua_end

private:

	/** The entity to spawn. */
	eMonsterType m_Entity;

	/** Time in ticks until the next entity spawns */
	short m_SpawnDelay;

	bool m_IsActive;

	/** Number of entities the spawner tries to spawn each activation. */
	short m_SpawnCount = 4;

	/** Diameter of the square the new monsters are spawned in */
	short m_SpawnRange = 8;

	short m_MinSpawnDelay = 200;

	short m_MaxSpawnDelay = 800;

	/** Maximum amount of the same entity type in proximity. */
	short m_MaxNearbyEntities = 6;

	/** Maximum distance to player for activation */
	short m_RequiredPlayerRange = 16;

} ;  // tolua_end
