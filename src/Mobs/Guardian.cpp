
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Guardian.h"
#include "../Vector3.h"
#include "../Chunk.h"





cGuardian::cGuardian(void) :
	super("Guardian", mtGuardian, "mob.guardian.idle", "mob.guardian.death", 0.95, 0.95)
{
}





void cGuardian::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	// Drops 0-3 Ink Sacs
	int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_PRISMARINE_SHARD);
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_RAW_FISH);
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_PRISMARINE_CRYSTALS); // ToDo: Prismarine Crystals only drop if the raw fish drop is 0
}





void cGuardian::Tick(float a_Dt, cChunk & a_Chunk)
{
	// We must first process current location, and only then tick, otherwise we risk processing a location in a chunk
	// that is not where the entity currently resides (FS #411)
	
	Vector3d Pos = GetPosition();

	// TODO: Not a real behavior, but cool :D
	int RelY = (int)floor(Pos.y);
	if ((RelY < 0) || (RelY >= cChunkDef::Height))
	{
		return;
	}
	int RelX = (int)floor(Pos.x) - a_Chunk.GetPosX() * cChunkDef::Width;
	int RelZ = (int)floor(Pos.z) - a_Chunk.GetPosZ() * cChunkDef::Width;
	BLOCKTYPE BlockType;
	if (a_Chunk.UnboundedRelGetBlockType(RelX, RelY, RelZ, BlockType) && !IsBlockWater(BlockType) && !IsOnFire())
	{
		// Burn for 10 ticks, then decide again
		StartBurning(10);
	}

	super::Tick(a_Dt, a_Chunk);
}




