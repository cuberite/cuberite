
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Monster.h"
#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Defines.h"
#include "../MonsterConfig.h"
#include "../MersenneTwister.h"

#include "../Vector3f.h"
#include "../Vector3i.h"
#include "../Vector3d.h"
#include "../Tracer.h"

// #include "../../iniFile/iniFile.h"





cMonster::cMonster(const AString & a_ConfigName, char a_ProtocolMobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height)
	: super(etMob, a_Width, a_Height)
	, m_Target(NULL)
	, m_bMovingToDestination(false)
	, m_DestinationTime( 0 )
	, m_DestroyTimer( 0 )
	, m_Jump(0)
	, m_MobType(a_ProtocolMobType)
	, m_SoundHurt(a_SoundHurt)
	, m_SoundDeath(a_SoundDeath)
	, m_EMState(IDLE)
	, m_SightDistance(25)
	, m_SeePlayerInterval (0)
	, m_EMPersonality(AGGRESSIVE)
	, m_AttackDamage(1.0f)
	, m_AttackRange(5.0f)
	, m_AttackInterval(0)
	, m_AttackRate(3)
	, idle_interval(0)
{
	if (!a_ConfigName.empty())
	{
		GetMonsterConfig(a_ConfigName);
	}
}





void cMonster::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendSpawnMob(*this);
}





void cMonster::MoveToPosition( const Vector3f & a_Position )
{
	m_bMovingToDestination = true;

	m_Destination = a_Position;
}





bool cMonster::ReachedDestination()
{
	Vector3f Distance = (m_Destination) - GetPosition();
	if( Distance.SqrLength() < 2.f )
		return true;

	return false;
}





void cMonster::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if (m_Health <= 0)
	{
		// The mob is dead, but we're still animating the "puff" they leave when they die
		m_DestroyTimer += a_Dt / 1000;
		if (m_DestroyTimer > 1)
		{
			Destroy(true);
		}
		return;
	}

	HandlePhysics(a_Dt,a_Chunk);
	BroadcastMovementUpdate();

	a_Dt /= 1000;

	if (m_bMovingToDestination)
	{
		Vector3f Pos( GetPosition() );
		Vector3f Distance = m_Destination - Pos;
		if( !ReachedDestination() )
		{
			Distance.y = 0;
			Distance.Normalize();
			Distance *= 3;
			SetSpeedX( Distance.x );
			SetSpeedZ( Distance.z );

			if (m_EMState == ESCAPING)
			{	//Runs Faster when escaping :D otherwise they just walk away
				SetSpeedX (GetSpeedX() * 2.f);
				SetSpeedZ (GetSpeedZ() * 2.f);
			}
		}
		else
		{
			m_bMovingToDestination = false;
		}

		if( GetSpeed().SqrLength() > 0.f )
		{
			if( m_bOnGround )
			{
				Vector3f NormSpeed = Vector3f(GetSpeed()).NormalizeCopy();
				Vector3f NextBlock = Vector3f( GetPosition() ) + NormSpeed;
				int NextHeight;
				if (!m_World->TryGetHeight((int)NextBlock.x, (int)NextBlock.z, NextHeight))
				{
					// The chunk at NextBlock is not loaded
					return;
				}
				if( NextHeight > (GetPosY() - 1.0) && (NextHeight - GetPosY()) < 2.5 )
				{
					m_bOnGround = false;
					SetSpeedY(5.f); // Jump!!
				}
			}
		}
	}

	Vector3d Distance = m_Destination - GetPosition();
	if (Distance.SqrLength() > 0.1f)
	{
		double Rotation, Pitch;
		Distance.Normalize();
		VectorToEuler( Distance.x, Distance.y, Distance.z, Rotation, Pitch );
		SetHeadYaw (Rotation);
		SetRotation( Rotation );
		SetPitch( -Pitch );
	}

	switch (m_EMState)
	{
		case IDLE:
		{
			// If enemy passive we ignore checks for player visibility
			InStateIdle(a_Dt);
			break;
		}
	
		case CHASING:
		{
			// If we do not see a player anymore skip chasing action
			InStateChasing(a_Dt);
			break;
		}
	
		case ESCAPING:
		{
			InStateEscaping(a_Dt);
			break;
		}
	}  // switch (m_EMState)
}






void cMonster::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	super::DoTakeDamage(a_TDI);
	if((m_SoundHurt != "") && (m_Health > 0)) m_World->BroadcastSoundEffect(m_SoundHurt, (int)(GetPosX() * 8), (int)(GetPosY() * 8), (int)(GetPosZ() * 8), 1.0f, 0.8f);
	if (a_TDI.Attacker != NULL)
	{
		m_Target = a_TDI.Attacker;
		AddReference(m_Target);
	}
}





void cMonster::KilledBy(cEntity * a_Killer)
{
	super::KilledBy(a_Killer);
	if (m_SoundHurt != "")
	{
		m_World->BroadcastSoundEffect(m_SoundDeath, (int)(GetPosX() * 8), (int)(GetPosY() * 8), (int)(GetPosZ() * 8), 1.0f, 0.8f);
	}
	m_DestroyTimer = 0;
}





//----State Logic

const char *cMonster::GetState()
{
	switch(m_EMState)
	{
		case IDLE:      return "Idle";
		case ATTACKING: return "Attacking";
		case CHASING:   return "Chasing";
		default:        return "Unknown";
	}
}





// for debugging
void cMonster::SetState(const AString & a_State)
{
	if (a_State.compare("Idle") == 0)
	{
		m_EMState = IDLE;
	}
	else if (a_State.compare("Attacking") == 0)
	{
		m_EMState = ATTACKING;
	}
	else if (a_State.compare("Chasing") == 0)
	{
		m_EMState = CHASING;
	}
	else
	{
		LOGD("cMonster::SetState(): Invalid state");
		ASSERT(!"Invalid state");
	}
}





//Checks to see if EventSeePlayer should be fired
//monster sez: Do I see the player
void cMonster::CheckEventSeePlayer(void)
{
	// TODO: Rewrite this to use cWorld's DoWithPlayers()
	cPlayer * Closest = FindClosestPlayer();

	if (Closest != NULL)
	{
		EventSeePlayer(Closest);
	}
}





void cMonster::CheckEventLostPlayer(void)
{
	Vector3f pos;
	cTracer LineOfSight(GetWorld());
	
	if (m_Target != NULL)
	{
		pos = m_Target->GetPosition();
		if ((pos - GetPosition()).Length() > m_SightDistance || LineOfSight.Trace(GetPosition(),(pos - GetPosition()), (int)(pos - GetPosition()).Length()))
		{
			EventLosePlayer();
		}
	}
	else
	{
		EventLosePlayer();
	}
}





// What to do if player is seen
// default to change state to chasing
void cMonster::EventSeePlayer(cEntity * a_SeenPlayer)
{
	m_Target = a_SeenPlayer;
	AddReference(m_Target);
}





void cMonster::EventLosePlayer(void)
{
	Dereference(m_Target);
	m_Target = NULL;
	m_EMState = IDLE;
}





// What to do if in Idle State
void cMonster::InStateIdle(float a_Dt)
{
	idle_interval += a_Dt;
	if (idle_interval > 1)
	{
		// at this interval the results are predictable
		int rem = m_World->GetTickRandomNumber(6) + 1;
		// LOGD("Moving: int: %3.3f rem: %i",idle_interval,rem);
		idle_interval -= 1;		// So nothing gets dropped when the server hangs for a few seconds
		Vector3f Dist;
		Dist.x = (float)(m_World->GetTickRandomNumber(10) - 5);
		Dist.z = (float)(m_World->GetTickRandomNumber(10) - 5);
		if ((Dist.SqrLength() > 2)  && (rem >= 3))
		{
			m_Destination.x = (float)(GetPosX() + Dist.x);
			m_Destination.z = (float)(GetPosZ() + Dist.z);
			int PosY;
			if (m_World->TryGetHeight((int)m_Destination.x, (int)m_Destination.z, PosY))
			{
				m_Destination.y = (float)PosY + 1.2f;
				MoveToPosition(m_Destination);
			}
		}
	}
}





// What to do if in Chasing State
// This state should always be defined in each child class
void cMonster::InStateChasing(float a_Dt)
{
	UNUSED(a_Dt);
}





// What to do if in Escaping State
void cMonster::InStateEscaping(float a_Dt)
{
	UNUSED(a_Dt);
	
	if (m_Target != NULL)
	{
		Vector3d newloc = GetPosition();
		newloc.x = (m_Target->GetPosition().x < newloc.x)? (newloc.x + m_SightDistance): (newloc.x - m_SightDistance);
		newloc.z = (m_Target->GetPosition().z < newloc.z)? (newloc.z + m_SightDistance): (newloc.z - m_SightDistance);
		MoveToPosition(newloc);
	}
	else
	{
		m_EMState = IDLE;  // This shouldnt be required but just to be safe
	}
}





// Do attack here
// a_Dt is passed so we can set attack rate
void cMonster::Attack(float a_Dt)
{
	m_AttackInterval += a_Dt * m_AttackRate;
	if ((m_Target != NULL) && (m_AttackInterval > 3.0))
	{
		// Setting this higher gives us more wiggle room for attackrate
		m_AttackInterval = 0.0;
		((cPawn *)m_Target)->TakeDamage(*this);
	}
}





// Checks for Players close by and if they are visible return the closest
cPlayer * cMonster::FindClosestPlayer(void)
{
	return m_World->FindClosestPlayer(GetPosition(), m_SightDistance);
}





void cMonster::GetMonsterConfig(const AString & a_Name)
{
	cRoot::Get()->GetMonsterConfig()->AssignAttributes(this, a_Name);
}





void cMonster::SetAttackRate(int ar)
{
	m_AttackRate = (float)ar;
}





void cMonster::SetAttackRange(float ar)
{
	m_AttackRange = ar;
}





void cMonster::SetAttackDamage(float ad)
{
	m_AttackDamage = ad;
}





void cMonster::SetSightDistance(float sd)
{
	m_SightDistance = sd;
}





void cMonster::AddRandomDropItem(cItems & a_Drops, unsigned int a_Min, unsigned int a_Max, short a_Item, short a_ItemHealth)
{
	MTRand r1;
	int Count = r1.randInt() % (a_Max + 1 - a_Min) + a_Min;
	if (Count > 0)
	{
		a_Drops.push_back(cItem(a_Item, Count, a_ItemHealth));
	}
}




