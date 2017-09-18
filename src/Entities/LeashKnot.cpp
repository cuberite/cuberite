
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "LeashKnot.h"
#include "ClientHandle.h"
#include "Player.h"
#include "Mobs/Monster.h"
#include "BoundingBox.h"

// Ticks to wait in Tick function to optimize calculations
#define TICK_STEP 10





cLeashKnot::cLeashKnot(eBlockFace a_BlockFace, double a_X, double a_Y, double a_Z) :
	cHangingEntity(etLeashKnot, a_BlockFace, a_X, a_Y, a_Z),
	m_ShouldSelfDestroy(false),
	m_TicksToSelfDestroy(20 * 1)
{
}





void cLeashKnot::OnRightClicked(cPlayer & a_Player)
{
	super::OnRightClicked(a_Player);

	TiePlayersLeashedMobs(a_Player, true);

	GetWorld()->BroadcastEntityMetadata(*this);  // Update clients
}





void cLeashKnot::TiePlayersLeashedMobs(cPlayer & a_Player, bool a_ShouldBroadcast)
{
	// Check leashed nearby mobs to tie them to this knot
	// taking world from player (instead from this) because this can be called before entity was initialized
	a_Player.GetWorld()->ForEachEntityInBox(cBoundingBox(GetPosition(), 8, 8, -4), [&](cEntity & a_Entity)
		{
			// If the entity is not a monster skip it
			if (a_Entity.GetEntityType() != cEntity::eEntityType::etMonster)
			{
				return false;
			}

			auto & PotentialLeashed = static_cast<cMonster&>(a_Entity);

			// If can't be leashed skip it
			if (!PotentialLeashed.CanBeLeashed())
			{
				return false;
			}

			// If it's not leashed to the player skip it
			if (
				!PotentialLeashed.IsLeashed() ||
				!PotentialLeashed.GetLeashedTo()->IsPlayer() ||
				(PotentialLeashed.GetLeashedTo()->GetUniqueID() != a_Player.GetUniqueID())
			)
			{
				return false;
			}

			// All conditions met, unleash from player and leash to fence
			PotentialLeashed.Unleash(false, false);
			PotentialLeashed.LeashTo(*this, a_ShouldBroadcast);
			return false;
		}
	);
}






void cLeashKnot::KilledBy(TakeDamageInfo & a_TDI)
{
	super::KilledBy(a_TDI);
	m_World->BroadcastSoundEffect("entity.leashknot.break", GetPosX(), GetPosY(), GetPosZ(), 1, 1);
	Destroy();
	return;
}





void cLeashKnot::GetDrops(cItems & a_Items, cEntity * a_Killer)
{
	if ((a_Killer != nullptr) && a_Killer->IsPlayer() && !static_cast<cPlayer *>(a_Killer)->IsGameModeCreative())
	{
		a_Items.push_back(cItem(E_ITEM_LEASH));
	}
}





void cLeashKnot::SpawnOn(cClientHandle & a_ClientHandle)
{
	super::SpawnOn(a_ClientHandle);
	a_ClientHandle.SendSpawnObject(*this, 77, GetProtocolFacing(), static_cast<Byte>(GetYaw()), static_cast<Byte>(GetPitch()));
	a_ClientHandle.SendEntityMetadata(*this);
}




void cLeashKnot::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	m_TicksAlive++;

	if ((m_TicksAlive % TICK_STEP) != 0)
	{
		return;
	}

	if (m_ShouldSelfDestroy)
	{
		m_TicksToSelfDestroy -= TICK_STEP;

		if (m_TicksToSelfDestroy <= 0)
		{
			Destroy();
			m_World->BroadcastSoundEffect("entity.leashknot.break", GetPosX(), GetPosY(), GetPosZ(), 1, 1);
		}
	}
}





cLeashKnot * cLeashKnot::FindKnotAtPos(cWorldInterface & a_WorldInterface, Vector3i a_BlockPos)
{
	cLeashKnot * LeashKnot = nullptr;
	a_WorldInterface.ForEachEntityInBox(cBoundingBox(a_BlockPos, 0.5, 1), [&](cEntity & a_Entity)
		{
			if (a_Entity.IsLeashKnot())
			{
				LeashKnot = static_cast<cLeashKnot *>(&a_Entity);
				return true;
			}
			return false;
		}
	);

	return LeashKnot;
}




