
#include "Globals.h"

#include "../BoundingBox.h"
#include "../Chunk.h"
#include "Floater.h"
#include "Player.h"
#include "../ClientHandle.h"





////////////////////////////////////////////////////////////////////////////////
// cFloaterEntityCollisionCallback
class cFloaterEntityCollisionCallback :
	public cEntityCallback
{
public:
	cFloaterEntityCollisionCallback(cFloater * a_Floater, const Vector3d & a_Pos, const Vector3d & a_NextPos) :
		m_Floater(a_Floater),
		m_Pos(a_Pos),
		m_NextPos(a_NextPos),
		m_MinCoeff(1),
		m_HitEntity(NULL)
	{
	}
	virtual bool Item(cEntity * a_Entity) override
	{
		if (!a_Entity->IsMob())  // Floaters can only pull mobs not other entities.
		{
			return false;
		}

		cBoundingBox EntBox(a_Entity->GetPosition(), a_Entity->GetWidth() / 2, a_Entity->GetHeight());

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
			m_HitEntity = a_Entity;
		}
		
		// Don't break the enumeration, we want all the entities
		return false;
	}

	/// Returns the nearest entity that was hit, after the enumeration has been completed
	cEntity * GetHitEntity(void) const { return m_HitEntity; }

	/// Returns true if the callback has encountered a true hit
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





////////////////////////////////////////////////////////////////////////////////
// cFloaterCheckEntityExist
class cFloaterCheckEntityExist :
	public cEntityCallback
{
public:
	cFloaterCheckEntityExist(void) :
		m_EntityExists(false)
	{
	}

	bool Item(cEntity * a_Entity) override
	{
		m_EntityExists = true;
		return false;
	}
	
	bool DoesExist(void) const { return m_EntityExists; }
protected:
	bool m_EntityExists;
} ;





cFloater::cFloater(double a_X, double a_Y, double a_Z, Vector3d a_Speed, int a_PlayerID, int a_CountDownTime) :
	cEntity(etFloater, a_X, a_Y, a_Z, 0.2, 0.2),
	m_CanPickupItem(false),
	m_PickupCountDown(0),
	m_CountDownTime(a_CountDownTime),
	m_PlayerID(a_PlayerID),
	m_AttachedMobID(-1)
{
	SetSpeed(a_Speed);
}





void cFloater::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendSpawnObject(*this, 90, m_PlayerID, 0, 0);
}





void cFloater::Tick(float a_Dt, cChunk & a_Chunk)
{
	HandlePhysics(a_Dt, a_Chunk);
	if (IsBlockWater(m_World->GetBlock((int) GetPosX(), (int) GetPosY(), (int) GetPosZ())) && m_World->GetBlockMeta((int) GetPosX(), (int) GetPosY(), (int) GetPosZ()) == 0)
	{
		if ((!m_CanPickupItem) && (m_AttachedMobID == -1))  // Check if you can't already pickup a fish and if the floater isn't attached to a mob.
		{
			if (m_CountDownTime <= 0)
			{
				m_World->BroadcastSoundEffect("random.splash", GetPosX(), GetPosY(), GetPosZ(), 1, 1);
				SetPosY(GetPosY() - 1);
				m_CanPickupItem = true;
				m_PickupCountDown = 20;
				m_CountDownTime = 100 + m_World->GetTickRandomNumber(800);
				LOGD("Floater %i can be picked up", GetUniqueID());
			}
			else if (m_CountDownTime == 20)  // Calculate the position where the particles should spawn and start producing them.
			{
				LOGD("Started producing particles for floater %i", GetUniqueID());
				m_ParticlePos.Set(GetPosX() + (-4 + m_World->GetTickRandomNumber(8)), GetPosY(), GetPosZ() + (-4 + m_World->GetTickRandomNumber(8)));
				m_World->BroadcastParticleEffect("splash", (float) m_ParticlePos.x, (float) m_ParticlePos.y, (float) m_ParticlePos.z, 0, 0, 0, 0, 15);
			}
			else if (m_CountDownTime < 20)
			{
				m_ParticlePos = (m_ParticlePos + (GetPosition() - m_ParticlePos) / 6);
				m_World->BroadcastParticleEffect("splash", (float) m_ParticlePos.x, (float) m_ParticlePos.y, (float) m_ParticlePos.z, 0, 0, 0, 0, 15);
			}
			
			m_CountDownTime--;
			if (m_World->GetHeight((int) GetPosX(), (int) GetPosZ()) == (int) GetPosY())
			{
				if (m_World->IsWeatherWet() && m_World->GetTickRandomNumber(3) == 0)  // 25% chance of an extra countdown when being rained on.
				{
					m_CountDownTime--;
				}
			}
			else  // if the floater is underground it has a 50% chance of not decreasing the countdown.
			{
				if (m_World->GetTickRandomNumber(1) == 0)
				{
					m_CountDownTime++;
				}
			}
		}
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

	if ((GetSpeed().Length() > 4) && (m_AttachedMobID == -1))
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

	cFloaterCheckEntityExist EntityCallback;
	m_World->DoWithEntityByID(m_PlayerID, EntityCallback);
	if (!EntityCallback.DoesExist())  // The owner doesn't exist anymore. Destroy the floater entity.
	{
		Destroy(true);
	}

	if (m_AttachedMobID != -1)
	{
		m_World->DoWithEntityByID(m_AttachedMobID, EntityCallback);  // The mob the floater was attached to doesn't exist anymore.
		if (!EntityCallback.DoesExist())
		{
			m_AttachedMobID = -1;
		}
	}

	SetSpeedX(GetSpeedX() * 0.95);
	SetSpeedZ(GetSpeedZ() * 0.95);

	BroadcastMovementUpdate();
}
