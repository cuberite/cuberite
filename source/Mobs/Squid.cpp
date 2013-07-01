
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Squid.h"
#include "../Vector3d.h"
#include "../Chunk.h"





cSquid::cSquid(void) :
	super("Squid", 94, "", "", 0.95, 0.95)
{
}





void cSquid::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	// Drops 0-3 Ink Sacs
	AddRandomDropItem(a_Drops, 0, 3, E_ITEM_DYE, E_META_DYE_BLACK);
}





void cSquid::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	
	Vector3d Pos = GetPosition();

	// TODO: Not a real behavior, but cool :D
	int RelX = (int)floor(Pos.x + 0.5) - a_Chunk.GetPosX() * cChunkDef::Width;
	int RelZ = (int)floor(Pos.z + 0.5) - a_Chunk.GetPosZ() * cChunkDef::Width;
	if (!IsBlockWater(a_Chunk.GetBlock(RelX, (int)Pos.y, RelZ)) && !IsOnFire())
	{
		// Burn for 10 ticks, then decide again
		StartBurning(10);
	}
}




