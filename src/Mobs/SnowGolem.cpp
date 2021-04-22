
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Chunk.h"
#include "SnowGolem.h"
#include "../BlockInfo.h"
#include "../World.h"





cSnowGolem::cSnowGolem(void) :
	Super("SnowGolem", mtSnowGolem, "entity.snowman.hurt", "entity.snowman.death", "entity.snowman.ambient", 0.7f, 1.9f)
{
}





void cSnowGolem::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	UNUSED(a_Killer);
	AddRandomDropItem(a_Drops, 0, 15, E_ITEM_SNOWBALL);
}





void cSnowGolem::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	PREPARE_REL_AND_CHUNK(GetPosition().Floor(), a_Chunk);
	if (!RelSuccess)
	{
		return;
	}

	if (IsBiomeNoDownfall(Chunk->GetBiomeAt(Rel.x, Rel.z)))
	{
		TakeDamage(dtEnvironment, nullptr, GetRawDamageAgainst(*this), GetKnockbackAmountAgainst(*this));
	}
	else if (const auto Below = Rel.addedY(-1); Below.y >= 0)
	{
		if ((Chunk->GetBlock(Rel) == E_BLOCK_AIR) && cBlockInfo::IsSolid(Chunk->GetBlock(Below)))
		{
			Chunk->SetBlock(Rel, E_BLOCK_SNOW, 0);
		}
	}
}
