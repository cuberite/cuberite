
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Guardian.h"
#include "../Chunk.h"





cGuardian::cGuardian(void) :
	Super("Guardian", mtGuardian, "entity.guardian.hurt", "entity.guardian.death", "entity.guardian.ambient", 0.875, 0.8)
{
}





void cGuardian::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	// Drops 0-3 Ink Sacs
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_PRISMARINE_SHARD);
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_RAW_FISH);
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_PRISMARINE_CRYSTALS);  // TODO: Prismarine Crystals only drop if the raw fish drop is 0
}





void cGuardian::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	m_PathfinderActivated = false;  // Disable Pathfinding until it's fixed. TODO

	// We must first process current location, and only then tick, otherwise we risk processing a location in a chunk
	// that is not where the entity currently resides (FS #411)
	Vector3d Pos = GetPosition();

	int RelY = FloorC(Pos.y);
	if ((RelY < 0) || (RelY >= cChunkDef::Height))
	{
		return;
	}

	Super::Tick(a_Dt, a_Chunk);
}




