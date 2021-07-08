
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Chunk.h"
#include "Enderman.h"
#include "../Entities/Player.h"
#include "../LineBlockTracer.h"

/** Function used by enderman and chorus fruit.
Checks for valid destinations in a cube of a_LinearLength centred at a_Centre.
Returns true and places destination in a_Destination if successful.
Returns false if destination could be found after a_NumTries attempts.
Details at: https://minecraft.fandom.com/wiki/Enderman#Teleportation
*/
bool AttemptTeleport(cWorld &a_World, Vector3i a_Centre, double a_LinearLength, unsigned int a_HeightRequired, unsigned int a_NumTries, Vector3i &a_Destination)
{
	/*
	Algorithm:
	Choose random destination.
	Seek downwards, regardless of distance until the block is made of movement-blocking material: https://minecraft.fandom.com/wiki/Materials
	Succeeds if no liquid or solid blocks prevents from standing at destination.
	*/
	auto & Random = GetRandomProvider();

	for (unsigned int i=0; i<a_NumTries; i++)
	{
		const int DestX = Random.RandInt(a_LinearLength, a_LinearLength);
		const int DestZ = Random.RandInt(a_LinearLength, a_LinearLength);
		int DestY = Random.RandInt(a_LinearLength, a_LinearLength);

		// Seek downwards from initial destination until we find a solid block or go into the void
		BLOCKTYPE DestBlock == a_World.GetBlock({DestX, DestY, DestZ});
		while ((DestY >= 0) && !cBlockInfo::IsSolid(DestBlock))
		{
			DestBlock = a_World.GetBlock({DestX, DestY, DestZ});
			DestY--;
		}

		// Couldn't find a solid block so move to next attempt
		if (DestY<0)
		{
			continue;
		}

		// Succeed if blocks above destination are empty
		bool Success = true;
		for (unsigned int j=1; j <= a_HeightRequired; j++)
		{
			BLOCKTYPE TestBlock = a_World.GetBlocks(DestX, DestY + j, DestZ);
			if (cBlockInfo::IsSolid(TestBlock) || IsBlockLiquid(TestBlock))
			{
				Success = false;
				break;
			}
		}

		if (!Success)
		{
			continue;
		}

		a_Destination = Vector3i(DestX, DestY, DestZ);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
// cPlayerLookCheck
class cPlayerLookCheck
{
public:

	cPlayerLookCheck(Vector3d a_EndermanHeadPosition, int a_SightDistance) :
		m_Player(nullptr),
		m_EndermanHeadPosition(a_EndermanHeadPosition),
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

		const auto PlayerHeadPosition = a_Player.GetPosition().addedY(a_Player.GetHeight());
		const auto Direction = m_EndermanHeadPosition - PlayerHeadPosition;

		// Don't check players who are more than SightDistance (64) blocks away:
		if (Direction.Length() > m_SightDistance)
		{
			return false;
		}

		// Don't check if the player has a pumpkin on his head:
		if (a_Player.GetEquippedHelmet().m_ItemType == E_BLOCK_PUMPKIN)
		{
			return false;
		}

		const auto LookVector = a_Player.GetLookVector();  // Note: ||LookVector|| is always 1.
		const auto Cosine = Direction.Dot(LookVector) / Direction.Length();  // a.b / (||a|| * ||b||)

		// If the player's crosshair is within 5 degrees of the enderman, it counts as looking:
		if ((Cosine < std::cos(0.09)) || (Cosine > std::cos(0)))  // 0.09 rad ~ 5 degrees
		{
			return false;
		}

		// TODO: Check if endermen are angered through water in Vanilla
		if (!cLineBlockTracer::LineOfSightTrace(*a_Player.GetWorld(), m_EndermanHeadPosition, PlayerHeadPosition, cLineBlockTracer::losAirWater))
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
	Vector3d m_EndermanHeadPosition;
	int m_SightDistance;
} ;





cEnderman::cEnderman(void) :
	Super("Enderman", mtEnderman, "entity.endermen.hurt", "entity.endermen.death", "entity.endermen.ambient", 0.6f, 2.9f),
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

	cPlayerLookCheck Callback(GetPosition().addedY(GetHeight()), m_SightDistance);
	if (m_World->ForEachPlayer(Callback))
	{
		return;
	}

	ASSERT(Callback.GetPlayer() != nullptr);

	// Target the player:
	cAggressiveMonster::EventSeePlayer(Callback.GetPlayer(), a_Chunk);
	m_bIsScreaming = true;
	GetWorld()->BroadcastEntityMetadata(*this);
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

	PREPARE_REL_AND_CHUNK(GetPosition().Floor(), a_Chunk);
	if (!RelSuccess)
	{
		return;
	}

	// Take damage when wet:
	if (IsInWater() || Chunk->IsWeatherWetAt(Rel))
	{
		EventLosePlayer();
		TakeDamage(dtEnvironment, nullptr, 1, 0);
		// TODO teleport to a safe location
	}
}
