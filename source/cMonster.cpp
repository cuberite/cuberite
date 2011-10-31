#include "cMonster.h"
#include "cRoot.h"
#include "cServer.h"
#include "cClientHandle.h"
#include "cWorld.h"
#include "cMCLogger.h"
#include "cChunk.h"
#include "cPlayer.h"
#include "BlockID.h"
#include "Defines.h"

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
#include <string.h>
#endif

#include <string>


cMonster::cMonster()
	: m_Target(0)
	, m_Destination( new Vector3f() )
	, m_bMovingToDestination(false)
	, m_Speed( new Vector3f() )
	, m_DestinationTime( 0 )
	, m_Gravity( -9.81 )
	, m_bOnGround( false )
	, m_DestroyTimer( 0 )
	, m_Jump(0)
	, m_MobType( 0 )
	, m_EMState(IDLE)
	, m_SightDistance(25)
	,m_SeePlayerInterval (0)
	,m_EMPersonality(AGGRESSIVE)
	,m_AttackDamage(1.0)
	,m_AttackRange(5.0)
	,m_AttackInterval(0)
	,m_AttackRate(3)
	,m_bPassiveAggressive(false)
	,idle_interval(0)
	,m_bBurnable(true)
	,m_EMMetaState(NORMAL)
	,m_FireDamageInterval(0)
	,m_BurnPeriod(0)
{
	LOG("cMonster::cMonster()");
	LOG("In state: %s",GetState());
	m_Health = 10;

	int RandVal = rand() % 4;
	if( RandVal == 0 )
		m_MobType = 90; // Pig
	else if( RandVal == 1 )
		m_MobType = 91; // Sheep
	else if( RandVal == 2 )
		m_MobType = 92; // Cow
	else
		m_MobType = 93; // Hen
}

cMonster::~cMonster()
{

	LOG("cMonster::~cMonster()");
	delete m_Destination;
	delete m_Speed;
}

bool cMonster::IsA( const char* a_EntityType )
{
    //LOG("IsA( cMonster ) : %s", a_EntityType);
	if( strcmp( a_EntityType, "cMonster" ) == 0 ) return true;
	return cPawn::IsA( a_EntityType );
}

void cMonster::SpawnOn( cClientHandle* a_Target )
{
	LOG("Spawn monster on client");
	cPacket_SpawnMob Spawn;
	Spawn.m_UniqueID = GetUniqueID();
	Spawn.m_Type = m_MobType;
	*Spawn.m_Pos = Vector3i((*m_Pos)*32);
	Spawn.m_Yaw = 0;
	Spawn.m_Pitch = 0;
	Spawn.m_MetaDataSize = 1;
	Spawn.m_MetaData = new char[Spawn.m_MetaDataSize];
	Spawn.m_MetaData[0] = 0x7f; // not on fire/crouching/riding
	//Spawn.m_MetaData[1] = 0x7f; // terminator
	if( a_Target == 0 )
	{
		cChunk* Chunk = GetWorld()->GetChunk( m_ChunkX, m_ChunkY, m_ChunkZ );
		Chunk->Broadcast( Spawn );
	}
	else
	{
		a_Target->Send( Spawn );
	}
}

void cMonster::MoveToPosition( const Vector3f & a_Position )
{
	m_bMovingToDestination = true;

	*m_Destination = a_Position;
}

bool cMonster::ReachedDestination()
{
	Vector3f Distance = (*m_Destination) - Vector3f( m_Pos );
	if( Distance.SqrLength() < 2.f )
		return true;

	return false;
}

void cMonster::Tick(float a_Dt)
{
	if( m_Health <= 0 )
	{
		m_DestroyTimer += a_Dt/1000;
		if( m_DestroyTimer > 1 )
		{
			Destroy();
		}
		return;
	}

	//a_Dt/=1000;
	a_Dt/=1000;

	if( m_bMovingToDestination )
	{
		Vector3f Pos( m_Pos );
		Vector3f Distance = *m_Destination - Pos;
		if( !ReachedDestination() )
		{
			Distance.y = 0;
			Distance.Normalize();
			Distance*=3;
			m_Speed->x = Distance.x;
			m_Speed->z = Distance.z;
		}
		else
		{
			m_bMovingToDestination = false;
		}

		if( m_Speed->SqrLength() > 0.f )
		{
			if( m_bOnGround )
			{
				Vector3f NormSpeed = m_Speed->NormalizeCopy();
				Vector3f NextBlock = Vector3f( *m_Pos ) + NormSpeed;
				double NextHeight = (double)GetWorld()->GetHeight( (int)NextBlock.x, (int)NextBlock.z );
				if( NextHeight > m_Pos->y - 1.2 && NextHeight - m_Pos->y < 2.5 )
				{
					m_bOnGround = false;
					m_Speed->y = 7.f; // Jump!!
				}
			}
		}
	}

	HandlePhysics( a_Dt );

	ReplicateMovement();
	
	Vector3f Distance = *m_Destination - Vector3f( m_Pos );
	if( Distance.SqrLength() > 0.1f )
	{
		float Rotation, Pitch;
		Distance.Normalize();
		VectorToEuler( Distance.x, Distance.y, Distance.z, Rotation, Pitch );
		SetRotation( Rotation );
		SetPitch( Pitch );
	}
	
	CheckMetaDataBurn(); //Check to see if Enemy should burn based on block they are on
	
	if(m_EMMetaState == BURNING) {
		InStateBurning(a_Dt);
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

	m_SeePlayerInterval += a_Dt;
	if(m_SeePlayerInterval > 1) {
		int rem = rand()%3 + 1; //check most of the time but miss occasionally
		//LOG("See Player Interval: %3.3f",m_SeePlayerInterval);
		m_SeePlayerInterval = 0.0;
		if(rem >= 2) {
			if(m_EMState == IDLE && m_EMPersonality != PASSIVE) {
				CheckEventSeePlayer();
				return;
			} 
			if(m_EMState == CHASING || m_EMState == ESCAPING){
				CheckEventLostPlayer();
				return;
			} 
		}
	}
	

}

void cMonster::ReplicateMovement()
{
	cChunk* InChunk = GetWorld()->GetChunkUnreliable( m_ChunkX, m_ChunkY, m_ChunkZ );
	if( !InChunk ) return;

	if(m_bDirtyOrientation && !m_bDirtyPosition)
	{
		cPacket_EntityLook EntityLook( this );
		InChunk->Broadcast( EntityLook );
		m_bDirtyOrientation = false;
	}
	if( m_bDirtyPosition )
	{

		float DiffX = (float)(GetPosX() - m_LastPosX );
		float DiffY = (float)(GetPosY() - m_LastPosY );
		float DiffZ = (float)(GetPosZ() - m_LastPosZ );
		float SqrDist = DiffX*DiffX + DiffY*DiffY + DiffZ*DiffZ;
		if( SqrDist > 4*4 // 4 blocks is max Relative Move
			|| cWorld::GetTime() - m_TimeLastTeleportPacket > 2 )  // Send an absolute position every 2 seconds
		{
			//LOG("Teleported %f", sqrtf(SqrDist) );
			cPacket_TeleportEntity TeleportEntity( this );
			InChunk->Broadcast( TeleportEntity );
			m_TimeLastTeleportPacket = cWorld::GetTime();
		}
		else
		{	// Relative move sucks balls! It's always wrong wtf!
			if( m_bDirtyOrientation )
			{
				cPacket_RelativeEntityMoveLook RelativeEntityMoveLook;
				RelativeEntityMoveLook.m_UniqueID = GetUniqueID();
				RelativeEntityMoveLook.m_MoveX = (char)(DiffX*32);
				RelativeEntityMoveLook.m_MoveY = (char)(DiffY*32);
				RelativeEntityMoveLook.m_MoveZ = (char)(DiffZ*32);
				RelativeEntityMoveLook.m_Yaw = (char)((GetRotation()/360.f)*256);
				RelativeEntityMoveLook.m_Pitch    = (char)((GetPitch()/360.f)*256);
				InChunk->Broadcast( RelativeEntityMoveLook );
			}
			else
			{
				cPacket_RelativeEntityMove RelativeEntityMove;
				RelativeEntityMove.m_UniqueID = GetUniqueID();
				RelativeEntityMove.m_MoveX = (char)(DiffX*32);
				RelativeEntityMove.m_MoveY = (char)(DiffY*32);
				RelativeEntityMove.m_MoveZ = (char)(DiffZ*32);
				InChunk->Broadcast( RelativeEntityMove );
			}
		}
		m_LastPosX = GetPosX();
		m_LastPosY = GetPosY();
		m_LastPosZ = GetPosZ();
		m_bDirtyPosition = false;
	}
}

void cMonster::HandlePhysics(float a_Dt)
{
	if( m_bOnGround ) // check if it's still on the ground
	{
		cWorld* World = GetWorld();
		if( World->GetBlock( (int)m_Pos->x, (int)m_Pos->y -1, (int)m_Pos->z ) == E_BLOCK_AIR )
		{
			m_bOnGround = false;
		}
		if( World->GetBlock( (int)m_Pos->x, (int)m_Pos->y, (int)m_Pos->z ) != E_BLOCK_AIR ) // If in ground itself, push it out
		{
			m_bOnGround = true;
			m_Pos->y += 0.2;
			m_bDirtyPosition = true;
		}
		m_Speed->x *= 0.7f/(1+a_Dt);
		if( fabs(m_Speed->x) < 0.05 ) m_Speed->x = 0;
		m_Speed->z *= 0.7f/(1+a_Dt);
		if( fabs(m_Speed->z) < 0.05 ) m_Speed->z = 0;
	}

	if( !m_bOnGround )
	{
		float Gravity = -9.81f*a_Dt;
		m_Speed->y += Gravity;
	}

	if( m_Speed->SqrLength() > 0.f )
	{
		cTracer Tracer( GetWorld() );
		int Ret = Tracer.Trace( *m_Pos, *m_Speed, 2 );
		if( Ret ) // Oh noez! we hit something
		{
			// Set to hit position
			if( (*Tracer.RealHit - Vector3f(*m_Pos)).SqrLength() <= ( *m_Speed * a_Dt ).SqrLength() )
			{
				if( Ret == 1 )
				{

					if( Tracer.HitNormal->x != 0.f ) m_Speed->x = 0.f;
					if( Tracer.HitNormal->y != 0.f ) m_Speed->y = 0.f;
					if( Tracer.HitNormal->z != 0.f ) m_Speed->z = 0.f;

					if( Tracer.HitNormal->y > 0 ) // means on ground
					{
						m_bOnGround = true;
					}
				}
				*m_Pos = Tracer.RealHit;
				*m_Pos += *Tracer.HitNormal * 0.2;

			}
			else
				*m_Pos += *m_Speed*a_Dt;
		}
		else
		{	// We didn't hit anything, so move =]
			*m_Pos += *m_Speed*a_Dt;
		}

		m_bDirtyPosition = true;
	}
}

void cMonster::TakeDamage( int a_Damage, cEntity* a_Instigator )
{
	cPawn::TakeDamage( a_Damage, a_Instigator );
		m_Target = a_Instigator;
		AddReference( m_Target );
		if(m_EMPersonality == AGGRESSIVE) {
			m_EMState = CHASING;
		}
		if(m_EMPersonality == COWARDLY || m_EMPersonality == PASSIVE) {
			//m_bPassiveAggressive can be set so if the monster based on time of day for example
			//so the monster will only attack if provoked
			m_EMState = (m_bPassiveAggressive)? CHASING : ESCAPING;
		}
	//LOG("Take damage");
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
void cMonster::SetState(const char* a_str) {
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
void cMonster::CheckEventSeePlayer() {

	//LOG("Checking if I see any players");
	cMonster::ListClosePlayers(this);
	
}

void cMonster::CheckEventLostPlayer() {
	Vector3f pos;
	cTracer LineOfSight(GetWorld() );
	
	//LOG("Checking if I lost my enemy");
	if(m_Target != 0) {
		pos = m_Target->GetPosition();
		if((pos - *m_Pos).Length() > m_SightDistance || LineOfSight.Trace(*m_Pos,(pos - *m_Pos), (int)(pos - *m_Pos).Length())){
			//LOG("Losing Player: %5.5f",(pos - *m_Pos).Length());
			EventLosePlayer();
		}
	} else {
		LOG("Enemy went poof");
		EventLosePlayer();
	}
}

//What to do if player is seen
//default to change state to chasing
void cMonster::EventSeePlayer(cEntity *a_SeenPlayer) {
	m_Target = a_SeenPlayer;
	AddReference( m_Target );
	if(m_EMPersonality == AGGRESSIVE) {
		m_EMState = CHASING;
	}
	if(m_EMPersonality == COWARDLY) {
		m_EMState = ESCAPING;
	}
	//LOG("Saw Player: %s",GetState());
}

void cMonster::EventLosePlayer(){
	Dereference(m_Target);
	m_Target = 0;
	//LOG("Lost Player");
	m_EMState = IDLE;
}

//What to do if in Idle State
void cMonster::InStateIdle(float a_Dt) {
	idle_interval += a_Dt;
	if(idle_interval > 1) { //at this interval the results are predictable
		int rem = rand()%6 + 1;
		//LOG("Moving: int: %3.3f rem: %i",idle_interval,rem);
		idle_interval = 0;
		Vector3f Dist;
		Dist.x = (float)((rand()%11)-5);
		Dist.z = (float)((rand()%11)-5);
		if( Dist.SqrLength() > 2  && rem >= 3)
		{
			m_Destination->x = (float)(m_Pos->x + Dist.x);
			m_Destination->z = (float)(m_Pos->z + Dist.z);
			m_Destination->y = (float)GetWorld()->GetHeight( (int)m_Destination->x, (int)m_Destination->z ) + 1.2f;
			MoveToPosition( *m_Destination );
		}
	}
}

//What to do if On fire
void cMonster::InStateBurning(float a_Dt) {
	m_FireDamageInterval += a_Dt;
	char block = GetWorld()->GetBlock( (int)m_Pos->x, (int)m_Pos->y, (int)m_Pos->z );
	char bblock = GetWorld()->GetBlock( (int)m_Pos->x, (int)m_Pos->y -1, (int)m_Pos->z );	
	if(m_FireDamageInterval > 1) {

		m_FireDamageInterval = 0;
		int rem = rand()%3 + 1; //Burn most of the time
		if(rem >= 2) {
			//printf("OUCH burning!!!\n");
			TakeDamage(1, this);
		}
		m_BurnPeriod++;
		if(block == E_BLOCK_LAVA || block == E_BLOCK_STATIONARY_LAVA || block == E_BLOCK_FIRE
			|| bblock == E_BLOCK_LAVA || bblock == E_BLOCK_STATIONARY_LAVA || bblock == E_BLOCK_FIRE)
			m_BurnPeriod = 0;
		
		if(m_BurnPeriod > 5) {
			
			cChunk* InChunk = GetWorld()->GetChunkUnreliable( m_ChunkX, m_ChunkY, m_ChunkZ );
			m_EMMetaState = NORMAL;
			cPacket_Metadata md(NORMAL, GetUniqueID());
			//md.m_UniqueID = GetUniqueID();
			InChunk->Broadcast(md);
			m_BurnPeriod = 0;
			
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
		Vector3d newloc = *m_Pos;
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
	m_AttackInterval += a_Dt*m_AttackRate;
	if(m_Target != 0 && m_AttackInterval > 3.0) { //Setting this higher gives us more wiggle room for attackrate
		//LOG("ATTACK!");
		m_AttackInterval = 0.0;
		((cPawn *)m_Target)->TakeDamage((int)m_AttackDamage,this);
	}
}
//----Change Entity MetaData
void cMonster::CheckMetaDataBurn() {
	char block = GetWorld()->GetBlock( (int)m_Pos->x, (int)m_Pos->y, (int)m_Pos->z );
	char bblock = GetWorld()->GetBlock( (int)m_Pos->x, (int)m_Pos->y -1, (int)m_Pos->z );	
	if(m_bBurnable && m_EMMetaState != BURNING && (block == E_BLOCK_LAVA ||  block == E_BLOCK_STATIONARY_LAVA || block == E_BLOCK_FIRE
				 || bblock == E_BLOCK_LAVA ||  bblock == E_BLOCK_STATIONARY_LAVA || bblock == E_BLOCK_FIRE)) {
		cChunk* InChunk = GetWorld()->GetChunkUnreliable( m_ChunkX, m_ChunkY, m_ChunkZ );
		if(!InChunk)
			return;
		//printf("I should burn");
		m_EMMetaState = BURNING;
		cPacket_Metadata md(BURNING,GetUniqueID());
		InChunk->Broadcast(md);
	}
}

//----Debug

void cMonster::ListMonsters() {

	cWorld::EntityList Entities = cRoot::Get()->GetWorld()->GetEntities();
	cRoot::Get()->GetWorld()->LockEntities();
	for( cWorld::EntityList::iterator itr = Entities.begin(); itr != Entities.end(); ++itr) {
		if((*itr)->GetEntityType() == cEntity::E_ENTITY){
			LOG("In state: %s type: %i attack rate: %i",((cMonster *)(*itr))->GetState(), ((cMonster *)(*itr))->GetMobType(),((cMonster *)(*itr))->GetAttackRate());
			
		}
	}
	cRoot::Get()->GetWorld()->UnlockEntities();
}

//Checks for Players close by and if they are visible
void cMonster::ListClosePlayers(cMonster *m) {
	int tries = 0;
	cTracer LineOfSight(cRoot::Get()->GetWorld() );
	cWorld::EntityList Entities = cRoot::Get()->GetWorld()->GetEntities();
	for( cWorld::EntityList::iterator itr = Entities.begin(); itr != Entities.end(); ++itr) {
		tries++;
		if((*itr)->GetEntityType() == cEntity::E_PLAYER){
			Vector3f pos = (*itr)->GetPosition();
			if((pos - *(m->m_Pos)).Length() <= m->m_SightDistance){
				if(!LineOfSight.Trace(*(m->m_Pos),(pos - *(m->m_Pos)),(int)(pos - *(m->m_Pos)).Length())){
					//LOG("I SEE PLAYER !!!!!!!!!!!!!!!!!");
					m->EventSeePlayer(*itr);
					return; //get the first one in sight later we can reiterate and check
						//for the closest out of all that match and make it more realistic
						
				}
			}
			
		}
		if(tries > 100) {
			//LOG("I Give Up");
			m->EventLosePlayer();
			return;
		}
	}
}

void cMonster::GetMonsterConfig(const char* pm_name) {
	(void)pm_name;
}

void cMonster::SetAttackRate(int ar) {
	m_AttackRate = (float)ar;
}
void cMonster::SetAttackRange(float ar) {
	m_AttackRange = ar;
}
void cMonster::SetAttackDamage(float ad) {
	m_AttackDamage = ad;
}
void cMonster::SetSightDistance(float sd) {
	m_SightDistance = sd;
}
