
#pragma once

#include "BlockEntity.h"
#include "../Entities/Player.h"





namespace Json
{
	class Value;
}





// tolua_begin

class cMobSpawnerEntity :
	public cBlockEntity
{
	typedef cBlockEntity super;
public:

	// tolua_end
	
	cMobSpawnerEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);

	virtual void SendTo(cClientHandle & a_Client) override;
	virtual void UsedBy(cPlayer * a_Player) override;
	virtual bool Tick(float a_Dt, cChunk & a_Chunk) override;

	// tolua_begin

	/** Upate the active flag from the mob spawner. This function will called every 5 seconds from the Tick() function. */
	void UpdateActiveState(void);

	/** Sets the spawn delay to a new random value. */
	void ResetTimer(void);

	/** Spawns the entity. This function automaticly change the spawn delay! */
	void SpawnEntity(void);

	/** Returns the entity type who will be spawn by this mob spawner. */
	eMonsterType GetEntity(void) const { return m_Entity; }

	/** Sets the entity type who will be spawn by this mob spawner. */
	void SetEntity(eMonsterType a_EntityType) { m_Entity = a_EntityType; }

	/** Returns the entity name. (Required by the protocol) */
	AString GetEntityName(void) const;

	/** Returns the spawn delay. */
	int GetSpawnDelay(void) const { return m_SpawnDelay; }

	int GetNearbyPlayersNum(void);
	int GetNearbyMonsterNum(eMonsterType a_EntityType);

	// tolua_end

	bool LoadFromJson(const Json::Value & a_Value);
	virtual void SaveToJson(Json::Value & a_Value) override;

	static const char * GetClassStatic(void) { return "cMobSpawnerEntity"; }

private:
	/** The entity to spawn. */
	eMonsterType m_Entity;

	int m_SpawnDelay;

	bool m_IsActive;
} ;  // tolua_end




