
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
	LOGD("Leash knot created.");
}





void cLeashKnot::OnRightClicked(cPlayer & a_Player)
{
	super::OnRightClicked(a_Player);

	// Check leashed nearby mobs to tight them to this knot
	class LookForLeasheds : public cEntityCallback
	{
	public:
		cLeashKnot * m_Knot;
		cPlayer * m_Player;

		LookForLeasheds(cLeashKnot * a_Knot, cPlayer * a_PlayerLeashedTo) :
			m_Knot(a_Knot),
			m_Player(a_PlayerLeashedTo)
		{
		}

		virtual bool Item(cEntity * a_Entity) override
		{
			// If the entity is not a monster skip it
			if (a_Entity->GetEntityType() != cEntity::eEntityType::etMonster)
			{
				return false;
			}

			cMonster * PotentialLeashed = static_cast<cMonster*>(a_Entity);

			// If can't be leashed skip it
			if (!PotentialLeashed->CanBeLeashed())
			{
				return false;
			}

			// If it's not leashed to the player skip it
			if (
				!PotentialLeashed->IsLeashed() ||
				!PotentialLeashed->GetLeashedTo()->IsPlayer() ||
				(PotentialLeashed->GetLeashedTo()->GetUniqueID() != m_Player->GetUniqueID())
			)
			{
				return false;
			}

			// All conditions met, unleash from player and leash to fence
			PotentialLeashed->GetLeashedTo()->RemoveLeashedMob(PotentialLeashed, false, false);
			m_Knot->AddLeashedMob(PotentialLeashed);
			return false;
		}
	} Callback(this, &a_Player);
	a_Player.GetWorld()->ForEachEntityInBox(cBoundingBox(GetPosition(), 8, 8, -4), Callback);

	GetWorld()->BroadcastEntityMetadata(*this);  // Update clients
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
		a_Items.push_back(cItem(E_ITEM_LEAD));
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
	else
	{
		// Detect if fence is destroyed. Get block at knot position and check if its solid
		BLOCKTYPE BlockID = m_World->GetBlock(FloorC(GetPosX()), FloorC(GetPosY()), FloorC(GetPosZ()));
		if (!cBlockInfo::IsSolid(BlockID))
		{
			m_ShouldSelfDestroy = true;
		}
	}
}




