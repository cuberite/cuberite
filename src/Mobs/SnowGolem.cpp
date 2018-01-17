
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SnowGolem.h"
#include "../World.h"





cSnowGolem::cSnowGolem(void) :
	super("SnowGolem", mtSnowGolem, "entity.snowman.hurt", "entity.snowman.death", 0.4, 1.8)
{
}





void cSnowGolem::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	UNUSED(a_Killer);
	AddRandomDropItem(a_Drops, 0, 15, E_ITEM_SNOWBALL);
}





void cSnowGolem::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}
	if (IsBiomeNoDownfall(m_World->GetBiomeAt(POSX_TOINT, POSZ_TOINT)))
	{
		TakeDamage(*this);
	}
	else
	{
		BLOCKTYPE BlockBelow = m_World->GetBlock(POSX_TOINT, POSY_TOINT - 1, POSZ_TOINT);
		BLOCKTYPE Block = m_World->GetBlock(POSX_TOINT, POSY_TOINT, POSZ_TOINT);
		if ((Block == E_BLOCK_AIR) && cBlockInfo::IsSolid(BlockBelow))
		{
			m_World->SetBlock(POSX_TOINT, POSY_TOINT, POSZ_TOINT, E_BLOCK_SNOW, 0);
		}
	}
}
