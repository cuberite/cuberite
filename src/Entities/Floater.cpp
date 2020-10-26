
#include "Globals.h"

#include "../BlockInfo.h"
#include "../BoundingBox.h"
#include "../Chunk.h"
#include "Floater.h"
#include "Player.h"
#include "../ClientHandle.h"




////////////////////////////////////////////////////////////////////////////////
// cFloaterEntityCollisionCallback
class cFloaterEntityCollisionCallback
{
public:
	cFloaterEntityCollisionCallback(cFloater * a_Floater, const Vector3d & a_Pos, const Vector3d & a_NextPos) :
		m_Floater(a_Floater),
		m_Pos(a_Pos),
		m_NextPos(a_NextPos),
		m_MinCoeff(1),
		m_HitEntity(nullptr)
	{
	}
	bool operator () (cEntity & a_Entity)
	{
		if (!a_Entity.IsMob())  // Floaters can only pull mobs not other entities.
		{
			return false;
		}

		auto EntBox = a_Entity.GetBoundingBox();

		double LineCoeff;
		eBlockFace Face;
		EntBox.Expand(m_Floater->GetWidth() / 2, m_Floater->GetHeight() / 2, m_Floater->GetWidth() / 2);
		if (!EntBox.CalcLineIntersection(m_Pos, m_NextPos, LineCoeff, Face))
		{
			// No intersection whatsoever
			return false;
		}

		if (LineCoeff < m_MinCoeff)
		{
			// The entity is closer than anything we've stored so far, replace it as the potential victim
			m_MinCoeff = LineCoeff;
			m_HitEntity = &a_Entity;
		}

		// Don't break the enumeration, we want all the entities
		return false;
	}

	/** Returns the nearest entity that was hit, after the enumeration has been completed */
	cEntity * GetHitEntity(void) const { return m_HitEntity; }

	/** Returns true if the callback has encountered a true hit */
	bool HasHit(void) const { return (m_MinCoeff < 1); }

protected:
	cFloater * m_Floater;
	const Vector3d & m_Pos;
	const Vector3d & m_NextPos;
	double m_MinCoeff;  // The coefficient of the nearest hit on the Pos line

	// Although it's bad(tm) to store entity ptrs from a callback, we can afford it here, because the entire callback
	// is processed inside the tick thread, so the entities won't be removed in between the calls and the final processing
	cEntity * m_HitEntity;  // The nearest hit entity
} ;





cFloater::cFloater(Vector3d a_Pos, Vector3d a_Speed, UInt32 a_PlayerID, int a_CountDownTime) :
	Super(etFloater, a_Pos, 0.2, 0.2),
	m_BitePos(a_Pos),
	m_CanPickupItem(false),
	m_PickupCountDown(0),
	m_CountDownTime(a_CountDownTime),
	m_PlayerID(a_PlayerID),
	m_AttachedMobID(cEntity::INVALID_ID)
{
	SetSpeed(a_Speed);
}





void cFloater::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendSpawnEntity(*this);
}





void cFloater::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	auto & Random = GetRandomProvider();

	HandlePhysics(a_Dt, a_Chunk);
	if (IsBlockWater(m_World->GetBlock(POSX_TOINT, POSY_TOINT, POSZ_TOINT))
		&& (m_World->GetBlockMeta(POSX_TOINT, POSY_TOINT, POSZ_TOINT) == 0))
	{
		if (!m_CanPickupItem && (m_AttachedMobID == cEntity::INVALID_ID))  // Check if you can't already pickup a fish and if the floater isn't attached to a mob.
		{
			if (m_CountDownTime <= 0)
			{
				m_BitePos = GetPosition();
				m_World->BroadcastSoundEffect("entity.bobber.splash", GetPosition(), 1, 1);
				SetPosY(GetPosY() - 1);
				m_CanPickupItem = true;
				m_PickupCountDown = 20;
				m_CountDownTime = Random.RandInt(100, 900);
				LOGD("Floater %i can be picked up", GetUniqueID());
			}
			else if (m_CountDownTime == 20)  // Calculate the position where the particles should spawn and start producing them.
			{
				LOGD("Started producing particles for floater %i", GetUniqueID());
				m_ParticlePos.Set(GetPosX() + Random.RandInt(-4, 4), GetPosY(), GetPosZ() + Random.RandInt(-4, 4));
				m_World->BroadcastParticleEffect("splash", static_cast<Vector3f>(m_ParticlePos), Vector3f{}, 0, 15);
			}
			else if (m_CountDownTime < 20)
			{
				m_ParticlePos = (m_ParticlePos + (GetPosition() - m_ParticlePos) / 6);
				m_World->BroadcastParticleEffect("splash", static_cast<Vector3f>(m_ParticlePos), Vector3f{}, 0, 15);
			}

			m_CountDownTime--;
			if (m_World->GetHeight(POSX_TOINT, POSZ_TOINT) == POSY_TOINT)
			{
				if (m_World->IsWeatherWet() && Random.RandBool(0.25))  // 25% chance of an extra countdown when being rained on.
				{
					m_CountDownTime--;
				}
			}
			else  // if the floater is underground it has a 50% chance of not decreasing the countdown.
			{
				if (Random.RandBool())
				{
					m_CountDownTime++;
				}
			}
		}
	}

	// Check water at the top of floater otherwise it floats into the air above the water
	if (IsBlockWater(m_World->GetBlock(POSX_TOINT, FloorC(GetPosY() + GetHeight()), POSZ_TOINT)))
	{
		SetSpeedY(0.7);
	}

	if (CanPickup())  // Make sure the floater "loses its fish"
	{
		m_PickupCountDown--;
		if (m_PickupCountDown == 0)
		{
			m_CanPickupItem = false;
			LOGD("The fish is gone. Floater %i can not pick an item up.", GetUniqueID());
		}
	}

	if ((GetSpeed().Length() > 4) && (m_AttachedMobID == cEntity::INVALID_ID))
	{
		cFloaterEntityCollisionCallback Callback(this, GetPosition(), GetPosition() + GetSpeed() / 20);

		a_Chunk.ForEachEntity(Callback);
		if (Callback.HasHit())
		{
			AttachTo(Callback.GetHitEntity());
			Callback.GetHitEntity()->TakeDamage(*this);  // TODO: the player attacked the mob not the floater.
			m_AttachedMobID = Callback.GetHitEntity()->GetUniqueID();
		}
	}

	if (!m_World->DoWithEntityByID(m_PlayerID, [](cEntity &) { return true; }))  // The owner doesn't exist anymore. Destroy the floater entity.
	{
		Destroy();
	}

	if (m_AttachedMobID != cEntity::INVALID_ID)
	{
		if (!m_World->DoWithEntityByID(m_AttachedMobID, [](cEntity &) { return true; }))
		{
			// The mob the floater was attached to doesn't exist anymore.
			m_AttachedMobID = cEntity::INVALID_ID;
		}
	}

	SetSpeedX(GetSpeedX() * 0.95);
	SetSpeedZ(GetSpeedZ() * 0.95);

	BroadcastMovementUpdate();
}
