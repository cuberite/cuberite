
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Squid.h"
#include "../Chunk.h"





cSquid::cSquid(void) :
	Super("Squid", mtSquid, "entity.squid.hurt", "entity.squid.death", "entity.squid.ambient", 0.95, 0.95)
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

	if (!IsHeadInWater())
	{
		if (m_AirLevel <= 0)
		{
			// Runs the air tick timer to check whether the squid should be damaged
			if (m_AirTickTimer <= 0)
			{
				// Damage squid
				TakeDamage(dtSuffocating, nullptr, 1, 1, 0);
				// Reset timer
				m_AirTickTimer = DROWNING_TICKS;
			}
			else
			{
				m_AirTickTimer--;
			}
		}
		else
		{
			// Reduce air supply
			m_AirLevel--;
		}
	}
	else
	{
		// Set the air back to maximum
		m_AirLevel = MAX_AIR_LEVEL;
		m_AirTickTimer = DROWNING_TICKS;
	}

	Super::Tick(a_Dt, a_Chunk);
}
