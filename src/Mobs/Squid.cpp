
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Squid.h"
#include "../Vector3.h"
#include "../Chunk.h"





cSquid::cSquid(void) :
	super("Squid", mtSquid, "", "", 0.95, 0.95)
{
}





void cSquid::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	// Drops 0-3 Ink Sacs
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 3 + LootingLevel, E_ITEM_DYE, E_META_DYE_BLACK);
}





void cSquid::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	m_PathfinderActivated = false;  // Disable Pathfinding until it's fixed. TODO

	// We must first process current location, and only then tick, otherwise we risk processing a location in a chunk
	// that is not where the entity currently resides (FS #411)
	Vector3d Pos = GetPosition();

	// TODO: Not a real behavior, but cool :D
	int RelY = FloorC(Pos.y);
	if ((RelY < 0) || (RelY >= cChunkDef::Height))
	{
		return;
	}
	int RelX = FloorC(Pos.x) - a_Chunk.GetPosX() * cChunkDef::Width;
	int RelZ = FloorC(Pos.z) - a_Chunk.GetPosZ() * cChunkDef::Width;
	BLOCKTYPE BlockType;
	if (a_Chunk.UnboundedRelGetBlockType(RelX, RelY, RelZ, BlockType) && !IsBlockWater(BlockType) && !IsOnFire())
	{
		// Burn for 10 ticks, then decide again
		StartBurning(10);
	}

	super::Tick(a_Dt, a_Chunk);
}



