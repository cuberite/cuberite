
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Squid.h"
#include "../Vector3d.h"





cSquid::cSquid(void) :
	super("Squid", 94, "", "")
{
}





void cSquid::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	// Drops 0-3 Ink Sacs
	AddRandomDropItem(a_Drops, 0, 3, E_ITEM_DYE, E_META_DYE_BLACK);
}





void cSquid::Tick(float a_Dt, cChunk & a_Chunk)
{
	// TODO: Rewrite this function to use a_Chunk instead of m_World
	super::Tick(a_Dt, a_Chunk);
	
	Vector3d Pos = GetPosition();

	// TODO: Not a real behavior, but cool :D
	if (!IsBlockWater(GetWorld()->GetBlock((int) Pos.x, (int) Pos.y, (int) Pos.z)) && !IsBurning())
	{
		SetMetaData(BURNING);
	}
}




