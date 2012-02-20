
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cMonster.h"
#include "cRoot.h"
#include "cServer.h"
#include "cClientHandle.h"
#include "cWorld.h"
#include "cChunk.h"
#include "cPlayer.h"
#include "BlockID.h"
#include "Defines.h"
#include "cPickup.h"
#include "cItem.h"
#include "cMonsterConfig.h"
#include "MersenneTwister.h"

#include "packets/cPacket_SpawnMob.h"
#include "packets/cPacket_EntityLook.h"
#include "packets/cPacket_TeleportEntity.h"
#include "packets/cPacket_RelativeEntityMoveLook.h"
#include "packets/cPacket_RelativeEntityMove.h"
#include "packets/cPacket_Metadata.h"

#include "Vector3f.h"
#include "Vector3i.h"
#include "Vector3d.h"

#include "cTracer.h"
#include "../iniFile/iniFile.h"

#ifndef _WIN32
	#include <cstdlib> // rand
	#include <unistd.h>
#endif





cMonster::cMonster()
	: m_Target(0)
	, m_bMovingToDestination(false)
	, m_DestinationTime( 0 )
	, m_Gravity( -9.81f)
	, m_bOnGround( false )
	, m_DestroyTimer( 0 )
	, m_Jump(0)
	, m_MobType( 0 )
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
	LOG("cMonster::cMonster()");
	LOG("In state: %s", GetState());

	m_bBurnable = true;
	m_MetaData = NORMAL;
}

cMonster::~cMonster()
{
	LOG("cMonster::~cMonster()");
}

bool cMonster::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cMonster" ) == 0 ) return true;
	return cPawn::IsA( a_EntityType );
}





cPacket * cMonster::GetSpawnPacket(void) const
{
	cPacket_SpawnMob * Spawn = new cPacket_SpawnMob;
	Spawn->m_UniqueID = GetUniqueID();
	Spawn->m_Type = m_MobType;
	*Spawn->m_Pos = Vector3i((m_Pos) * 32);
	Spawn->m_Yaw = 0;
	Spawn->m_Pitch = 0;
	Spawn->m_MetaDataSize = 1;
	Spawn->m_MetaData = new char[Spawn->m_MetaDataSize];
	Spawn->m_MetaData[0] = 0x7f; // not on fire/crouching/riding
	return Spawn;
}





void cMonster::MoveToPosition( const Vector3f & a_Position )
{
	m_bMovingToDestination = true;

	m_Destination = a_Position;
}

bool cMonster::ReachedDestination()
{
	Vector3f Distance = (m_Destination) - Vector3f( m_Pos );
	if( Distance.SqrLength() < 2.f )
		return true;

	return false;
}





void cMonster::Tick(float a_Dt)
{
	cPawn::Tick(a_Dt);

	if( m_Health <= 0 )
	{
		m_DestroyTimer += a_Dt/1000;
		if( m_DestroyTimer > 1 )
		{
			Destroy();
		}
		return;
	}

	a_Dt /= 1000;

	if( m_bMovingToDestination )
	{
		Vector3f Pos( m_Pos );
		Vector3f Distance = m_Destination - Pos;
		if( !ReachedDestination() )
		{
			Distance.y = 0;
			Distance.Normalize();
			Distance *= 3;
			m_Speed.x = Distance.x;
			m_Speed.z = Distance.z;

			if (m_EMState == ESCAPING)
			{	//Runs Faster when escaping :D otherwise they just walk away
				m_Speed.x *= 2.f;
				m_Speed.z *= 2.f;
			}
		}
		else
		{
			m_bMovingToDestination = false;
		}

		if( m_Speed.SqrLength() > 0.f )
		{
			if( m_bOnGround )
			{
				Vector3f NormSpeed = m_Speed.NormalizeCopy();
				Vector3f NextBlock = Vector3f( m_Pos ) + NormSpeed;
				double NextHeight = (double)GetWorld()->GetHeight( (int)NextBlock.x, (int)NextBlock.z );
				if( NextHeight > m_Pos.y - 1.2 && NextHeight - m_Pos.y < 2.5 )
				{
					m_bOnGround = false;
					m_Speed.y = 7.f; // Jump!!
				}
			}
		}
	}

	HandlePhysics( a_Dt );

	ReplicateMovement();
	
	Vector3f Distance = m_Destination - Vector3f( m_Pos );
	if( Distance.SqrLength() > 0.1f )
	{
		float Rotation, Pitch;
		Distance.Normalize();
		VectorToEuler( Distance.x, Distance.y, Distance.z, Rotation, Pitch );
		SetRotation( Rotation );
		SetPitch( Pitch );
	}
	
	if(m_EMState == IDLE) { //If enemy passive we ignore checks for player visibility
		InStateIdle(a_Dt);
	}
	
	if(m_EMState == CHASING) { //If we do not see a player anymore skip chasing action
		InStateChasing(a_Dt);
	}
	
	if(m_EMState == ESCAPING) {
		InStateEscaping(a_Dt);
	}

}





void cMonster::ReplicateMovement()
{
	if(m_bDirtyOrientation && !m_bDirtyPosition)
	{
		cPacket_EntityLook EntityLook( this );
		m_World->BroadcastToChunk(m_ChunkX, m_ChunkY, m_ChunkZ, EntityLook );
		m_bDirtyOrientation = false;
	}
	
	if( m_bDirtyPosition )
	{
		float DiffX = (float)(GetPosX() - m_LastPosX );
		float DiffY = (float)(GetPosY() - m_LastPosY );
		float DiffZ = (float)(GetPosZ() - m_LastPosZ );
		float SqrDist = DiffX*DiffX + DiffY*DiffY + DiffZ*DiffZ;
		if (
			(SqrDist > 4 * 4) // 4 blocks is max Relative Move
			|| (cWorld::GetTime() - m_TimeLastTeleportPacket > 2 )  // Send an absolute position every 2 seconds
		)
		{
			//LOG("Teleported %f", sqrtf(SqrDist) );
			cPacket_TeleportEntity TeleportEntity( this );
			m_World->BroadcastToChunk(m_ChunkX, m_ChunkY, m_ChunkZ, TeleportEntity);
			m_TimeLastTeleportPacket = cWorld::GetTime();
		}
		else
		{	// Relative move sucks balls! It's always wrong wtf!
			if( m_bDirtyOrientation )
			{
				cPacket_RelativeEntityMoveLook RelativeEntityMoveLook;
				RelativeEntityMoveLook.m_UniqueID = GetUniqueID();
				RelativeEntityMoveLook.m_MoveX    = (char)(DiffX*32);
				RelativeEntityMoveLook.m_MoveY    = (char)(DiffY*32);
				RelativeEntityMoveLook.m_MoveZ    = (char)(DiffZ*32);
				RelativeEntityMoveLook.m_Yaw      = (char)((GetRotation()/360.f)*256);
				RelativeEntityMoveLook.m_Pitch    = (char)((GetPitch()/360.f)*256);
				m_World->BroadcastToChunk(m_ChunkX, m_ChunkY, m_ChunkZ, RelativeEntityMoveLook );
			}
			else
			{
				cPacket_RelativeEntityMove RelativeEntityMove;
				RelativeEntityMove.m_UniqueID = GetUniqueID();
				RelativeEntityMove.m_MoveX    = (char)(DiffX*32);
				RelativeEntityMove.m_MoveY    = (char)(DiffY*32);
				RelativeEntityMove.m_MoveZ    = (char)(DiffZ*32);
				m_World->BroadcastToChunk(m_ChunkX, m_ChunkY, m_ChunkZ, RelativeEntityMove );
			}
		}
		m_LastPosX = GetPosX();
		m_LastPosY = GetPosY();
		m_LastPosZ = GetPosZ();
		m_bDirtyPosition = false;
	}

	MoveToCorrectChunk();
}





void cMonster::HandlePhysics(float a_Dt)
{
	if( m_bOnGround ) // check if it's still on the ground
	{
		cWorld* World = GetWorld();
		if( World->GetBlock( (int)m_Pos.x, (int)m_Pos.y -1, (int)m_Pos.z ) == E_BLOCK_AIR )
		{
			m_bOnGround = false;
		}
		if( World->GetBlock( (int)m_Pos.x, (int)m_Pos.y, (int)m_Pos.z ) != E_BLOCK_AIR ) // If in ground itself, push it out
		{
			m_bOnGround = true;
			m_Pos.y += 0.2;
			m_bDirtyPosition = true;
		}
		m_Speed.x *= 0.7f/(1+a_Dt);
		if( fabs(m_Speed.x) < 0.05 ) m_Speed.x = 0;
		m_Speed.z *= 0.7f/(1+a_Dt);
		if( fabs(m_Speed.z) < 0.05 ) m_Speed.z = 0;
	}

	if( !m_bOnGround )
	{
		float Gravity = -9.81f*a_Dt;
		m_Speed.y += Gravity;
	}

	if( m_Speed.SqrLength() > 0.f )
	{
		cTracer Tracer( GetWorld() );
		int Ret = Tracer.Trace( m_Pos, m_Speed, 2 );
		if( Ret ) // Oh noez! we hit something
		{
			// Set to hit position
			if( (Tracer.RealHit - Vector3f(m_Pos)).SqrLength() <= ( m_Speed * a_Dt ).SqrLength() )
			{
				if( Ret == 1 )
				{

					if( Tracer.HitNormal.x != 0.f ) m_Speed.x = 0.f;
					if( Tracer.HitNormal.y != 0.f ) m_Speed.y = 0.f;
					if( Tracer.HitNormal.z != 0.f ) m_Speed.z = 0.f;

					if( Tracer.HitNormal.y > 0 ) // means on ground
					{
						m_bOnGround = true;
					}
				}
				m_Pos = Tracer.RealHit;
				m_Pos += Tracer.HitNormal * 0.2f;

			}
			else
				m_Pos += m_Speed*a_Dt;
		}
		else
		{	// We didn't hit anything, so move =]
			m_Pos += m_Speed*a_Dt;
		}

		m_bDirtyPosition = true;
	}
}

void cMonster::TakeDamage(int a_Damage, cEntity* a_Instigator)
{
	cPawn::TakeDamage( a_Damage, a_Instigator );
	m_Target = a_Instigator;
	AddReference( m_Target );
}

void cMonster::KilledBy( cEntity* a_Killer )
{
	cPawn::KilledBy( a_Killer );
	m_DestroyTimer = 0;
}

//----State Logic

const char *cMonster::GetState() {
	switch(m_EMState) {
	case IDLE:
		return "Idle";
		break;
	case ATTACKING:
		return "Attacking";
		break;
	case CHASING:
		return "Chasing";
		break;
	default:
		return "Unknown";
		break;
	}
}

//for debugging
void cMonster::SetState(const char* a_str)
{
	std::string str = a_str;
	if(str.compare("Idle") == 0 ) {
		m_EMState = IDLE;
	} else if(str.compare("Attacking") == 0 ) {
		m_EMState = ATTACKING;
	} else if(str.compare("Chasing") == 0 ) {
		m_EMState = CHASING;
	} else {
		printf("Invalid State");
	}
}

//Checks to see if EventSeePlayer should be fired
//monster sez: Do I see the player
void cMonster::CheckEventSeePlayer()
{
	cPlayer *Closest = FindClosestPlayer();

	if(Closest)
	{
		EventSeePlayer(Closest);
	}
}

void cMonster::CheckEventLostPlayer()
{
	Vector3f pos;
	cTracer LineOfSight(GetWorld() );
	
	if(m_Target != 0) {
		pos = m_Target->GetPosition();
		if((pos - m_Pos).Length() > m_SightDistance || LineOfSight.Trace(m_Pos,(pos - m_Pos), (int)(pos - m_Pos).Length()))
		{
			EventLosePlayer();
		}
	} else {
		EventLosePlayer();
	}
}

//What to do if player is seen
//default to change state to chasing
void cMonster::EventSeePlayer(cEntity *a_SeenPlayer)
{
	m_Target = a_SeenPlayer;
	AddReference( m_Target );
}

void cMonster::EventLosePlayer(){
	Dereference(m_Target);
	m_Target = 0;
	m_EMState = IDLE;
}

//What to do if in Idle State
void cMonster::InStateIdle(float a_Dt) {
	idle_interval += a_Dt;
	if(idle_interval > 1) { //at this interval the results are predictable
		MTRand r1;
		int rem = r1.randInt()%6 + 1;
		//LOG("Moving: int: %3.3f rem: %i",idle_interval,rem);
		idle_interval -= 1;		//So nothing gets dropped when the server hangs for a few seconds
		Vector3f Dist;
		Dist.x = (float)((r1.randInt()%11)-5);
		Dist.z = (float)((r1.randInt()%11)-5);
		if( Dist.SqrLength() > 2  && rem >= 3)
		{
			m_Destination.x = (float)(m_Pos.x + Dist.x);
			m_Destination.z = (float)(m_Pos.z + Dist.z);
			m_Destination.y = (float)GetWorld()->GetHeight( (int)m_Destination.x, (int)m_Destination.z ) + 1.2f;
			MoveToPosition( m_Destination );
		}
	}
}

//What to do if in Chasing State
//This state should always be defined in each child class
void cMonster::InStateChasing(float a_Dt) {
	(void)a_Dt;
}

//What to do if in Escaping State
void cMonster::InStateEscaping(float a_Dt) {
	(void)a_Dt;
	if(m_Target) {
		Vector3d newloc = m_Pos;
		newloc.x = (m_Target->GetPosition().x < newloc.x)? (newloc.x + m_SightDistance): (newloc.x - m_SightDistance);
		newloc.z = (m_Target->GetPosition().z < newloc.z)? (newloc.z + m_SightDistance): (newloc.z - m_SightDistance);
		MoveToPosition(newloc);
	} else {
		m_EMState = IDLE; //this shouldnt be required but just to be safe
	}
}


//Do attack here
//a_Dt is passed so we can set attack rate
void cMonster::Attack(float a_Dt) {
	m_AttackInterval += a_Dt * m_AttackRate;
	if(m_Target != 0 && m_AttackInterval > 3.0) { //Setting this higher gives us more wiggle room for attackrate
		m_AttackInterval = 0.0;
		((cPawn *)m_Target)->TakeDamage((int)m_AttackDamage,this);
	}
}





#if 0
// TODO: Implement this debug function inside cWorld instead - the world owns the entities
void cMonster::ListMonsters()
{

	cWorld::EntityList Entities = cRoot::Get()->GetWorld()->GetEntities();
	cRoot::Get()->GetWorld()->LockEntities();
	for( cWorld::EntityList::iterator itr = Entities.begin(); itr != Entities.end(); ++itr) {
		if((*itr)->GetEntityType() == cEntity::E_ENTITY){
			LOG("In state: %s type: %i attack rate: %i",((cMonster *)(*itr))->GetState(), ((cMonster *)(*itr))->GetMobType(),((cMonster *)(*itr))->GetAttackRate());
			
		}
	}
	cRoot::Get()->GetWorld()->UnlockEntities();
}
#endif





//Checks for Players close by and if they are visible return the closest
cPlayer * cMonster::FindClosestPlayer(void)
{
	return m_World->FindClosestPlayer(m_Pos, m_SightDistance);
}





void cMonster::GetMonsterConfig(const char* pm_name)
{
	cRoot::Get()->GetMonsterConfig()->Get()->AssignAttributes(this, pm_name);
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





void cMonster::DropItem(ENUM_ITEM_ID a_Item, unsigned int a_Count)
{
	if (a_Count > 0)
	{
		cPickup * Pickup = new cPickup( (int)(m_Pos.x * 32), (int)(m_Pos.y * 32), (int)(m_Pos.z * 32), cItem( a_Item, (char) a_Count ) );
		Pickup->Initialize( GetWorld() );
	}
}





void cMonster::RandomDropItem(ENUM_ITEM_ID a_Item, unsigned int a_Min, unsigned int a_Max)
{
	MTRand r1;
	return cMonster::DropItem(a_Item, r1.randInt() % (a_Max + 1 - a_Min) + a_Min);
}