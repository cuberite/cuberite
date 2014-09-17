
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
	virtual ~cMobSpawnerEntity();

	bool LoadFromJson(const Json::Value & a_Value);
	virtual void SaveToJson(Json::Value & a_Value) override;

	virtual bool Tick(float a_Dt, cChunk & a_Chunk) override;

	// tolua_begin

	/** Returns the entity who will be spawn by this mob spawner. */
	const AString & GetEntityName(void) const { return m_EntityName; }

	// tolua_end

	static const char * GetClassStatic(void) { return "cMobSpawnerEntity"; }
	
	virtual void UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle &) override {}

private:
	/** The entity to spawn. */
	AString m_EntityName;

	int m_SpawnDelay;
	int m_MinSpawnDelay;
	int m_MaxSpawnDelay;

	/** The mob spawner spawns only mobs when the count of nearby entities (without players) is lesser than this number. */
	short m_MaxNearbyEntities;

	/** The mob spawner spawns only mobs when a player is in the range of the mob spawner. */
	short m_ActivatingRange;

	/** The range coefficient for spawning entities around. */
	short m_SpawnRange;
} ;  // tolua_end




