
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Squid.h"
#include "../Vector3d.h"
#include "../Chunk.h"





cSquid::cSquid(void) :
	super("Squid", mtSquid, "", "", 0.95, 0.95)
{
}





void cSquid::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	// Drops 0-3 Ink Sacs
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(E_ENCHANTMENT_LOOTING);
	}
	AddRandomDropItem(a_Drops, 0, 3 + LootingLevel, E_ITEM_DYE, E_META_DYE_BLACK);
}





void cSquid::Tick(float a_Dt, cChunk & a_Chunk)
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



