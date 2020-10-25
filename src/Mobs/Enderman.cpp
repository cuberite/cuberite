
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Enderman.h"
#include "../Entities/Player.h"
#include "../LineBlockTracer.h"




////////////////////////////////////////////////////////////////////////////////
// cPlayerLookCheck
class cPlayerLookCheck
{
public:
	cPlayerLookCheck(Vector3d a_EndermanPos, int a_SightDistance) :
		m_Player(nullptr),
		m_EndermanPos(a_EndermanPos),
		m_SightDistance(a_SightDistance)
	{
	}

	bool operator () (cPlayer & a_Player)
	{
		// Don't check players who cannot be targeted
		if (!a_Player.CanMobsTarget())
		{
			return false;
		}

		// Don't check players who are more than SightDistance (64) blocks away
		auto Direction = m_EndermanPos - a_Player.GetPosition();
		if (Direction.Length() > m_SightDistance)
		{
			return false;
		}

		// Don't check if the player has a pumpkin on his head
		if (a_Player.GetEquippedHelmet().m_ItemType == E_BLOCK_PUMPKIN)
		{
			return false;
		}

		// If the player's crosshair is within 5 degrees of the enderman, it counts as looking
		auto LookVector = a_Player.GetLookVector();
		auto dot = Direction.Dot(LookVector);
		if (dot <= cos(0.09))  // 0.09 rad ~ 5 degrees
		{
			return false;
		}

		// TODO: Check if endermen are angered through water in Vanilla
		if (!cLineBlockTracer::LineOfSightTrace(*a_Player.GetWorld(), m_EndermanPos, a_Player.GetPosition(), cLineBlockTracer::losAirWater))
		{
			// No direct line of sight
			return false;
		}

		m_Player = &a_Player;
		return true;
	}

	cPlayer * GetPlayer(void) const { return m_Player; }

protected:
	cPlayer * m_Player;
	Vector3d m_EndermanPos;
	int m_SightDistance;
} ;





cEnderman::cEnderman(void) :
	Super("Enderman", mtEnderman, "entity.endermen.hurt", "entity.endermen.death", "entity.endermen.ambient", 0.5, 2.9),
	m_bIsScreaming(false),
	m_CarriedBlock(E_BLOCK_AIR),
	m_CarriedMeta(0)
{
}





void cEnderman::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_ENDER_PEARL);
}





void cEnderman::CheckEventSeePlayer(cChunk & a_Chunk)
{
	if (GetTarget() != nullptr)
	{
		return;
	}

	cPlayerLookCheck Callback(GetPosition(), m_SightDistance);
	if (m_World->ForEachPlayer(Callback))
	{
		return;
	}

	ASSERT(Callback.GetPlayer() != nullptr);

	if (!Callback.GetPlayer()->CanMobsTarget())
	{
		return;
	}

	// Target the player
	cMonster::EventSeePlayer(Callback.GetPlayer(), a_Chunk);
	m_EMState = CHASING;
	m_bIsScreaming = true;
	GetWorld()->BroadcastEntityMetadata(*this);
}





void cEnderman::CheckEventLostPlayer(void)
{
	Super::CheckEventLostPlayer();
	EventLosePlayer();
}





void cEnderman::EventLosePlayer()
{
	Super::EventLosePlayer();
	m_bIsScreaming = false;
	GetWorld()->BroadcastEntityMetadata(*this);
}





void cEnderman::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	// Take damage when wet
	if (
		cChunkDef::IsValidHeight(POSY_TOINT) &&
		(GetWorld()->IsWeatherWetAtXYZ(GetPosition().Floor()) || IsInWater())
	)
	{
		EventLosePlayer();
		TakeDamage(dtEnvironment, nullptr, 1, 0);
		// TODO teleport to a safe location
	}
}
