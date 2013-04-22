
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Entity.h"
#include "World.h"
#include "Server.h"
#include "Root.h"
#include "Vector3d.h"
#include "Matrix4f.h"
#include "ReferenceManager.h"
#include "ClientHandle.h"
#include "Tracer.h"
#include "Chunk.h"
#include "Simulator/FluidSimulator.h"





int cEntity::m_EntityCount = 0;
cCriticalSection cEntity::m_CSCount;





cEntity::cEntity(eEntityType a_EntityType, double a_X, double a_Y, double a_Z)
	: m_UniqueID(0)
	, m_AttachedTo(NULL)
	, m_Attachee(NULL)
	, m_Referencers(new cReferenceManager(cReferenceManager::RFMNGR_REFERENCERS))
	, m_References(new cReferenceManager(cReferenceManager::RFMNGR_REFERENCES))
	, m_HeadYaw( 0.0 )
	, m_Pos(a_X, a_Y, a_Z)
	, m_bDirtyHead(true)
	, m_bDirtyOrientation(true)
	, m_bDirtyPosition(true)
	, m_bDirtySpeed(true)
	, m_bOnGround( false )
	, m_Gravity( -9.81f )
	, m_IsInitialized(false)
	, m_LastPosX( 0.0 )
	, m_LastPosY( 0.0 )
	, m_LastPosZ( 0.0 )
	, m_TimeLastTeleportPacket(0)
	, m_TimeLastMoveReltPacket(0)
	, m_TimeLastSpeedPacket(0)
	, m_EntityType(a_EntityType)
	, m_World(NULL)
	, m_FireDamageInterval(0.f)
	, m_BurnPeriod(0.f)
	, m_WaterSpeed( 0.0 , 0.0 , 0.0 )
{
	cCSLock Lock(m_CSCount);
	m_EntityCount++;
	m_UniqueID = m_EntityCount;
}





cEntity::~cEntity()
{
	ASSERT(!m_World->HasEntity(m_UniqueID));  // Before deleting, the entity needs to have been removed from the world
	
	LOGD("Deleting entity %d at pos {%.2f, %.2f, %.2f} ~ [%d, %d]; ptr %p", 
		m_UniqueID,
		m_Pos.x, m_Pos.y, m_Pos.z,
		(int)(m_Pos.x / cChunkDef::Width), (int)(m_Pos.z / cChunkDef::Width),
		this
		);

	if (m_AttachedTo != NULL)
	{
		Detach();
	}
	if (m_Attachee != NULL)
	{
		m_Attachee->Detach();
	}

	if (m_IsInitialized)
	{
		LOGWARNING("ERROR: Entity deallocated without being destroyed");
		ASSERT(!"Entity deallocated without being destroyed or unlinked");
	}
	delete m_Referencers;
	delete m_References;
}





const char * cEntity::GetClass(void) const
{
	return "cEntity";
}





const char * cEntity::GetClassStatic(void)
{
	return "cEntity";
}





const char * cEntity::GetParentClass(void) const
{
	return "";
}





void cEntity::Initialize(cWorld * a_World)
{
	LOGD("Initializing entity #%d (%s) at {%.02f, %.02f, %.02f}",
		m_UniqueID, GetClass(), m_Pos.x, m_Pos.y, m_Pos.z
	);
	m_IsInitialized = true;
	m_World = a_World;
	m_World->AddEntity(this);
}





void cEntity::WrapHeadYaw(void)
{
	while (m_HeadYaw > 180.f)  m_HeadYaw -= 360.f; // Wrap it
	while (m_HeadYaw < -180.f) m_HeadYaw += 360.f;
}





void cEntity::WrapRotation(void)
{
	while (m_Rot.x > 180.f)  m_Rot.x -= 360.f; // Wrap it
	while (m_Rot.x < -180.f) m_Rot.x += 360.f;
	while (m_Rot.y > 180.f)  m_Rot.y -= 360.f;
	while (m_Rot.y < -180.f) m_Rot.y += 360.f;
}




void cEntity::WrapSpeed(void)
{
	// There shoudn't be a need for flipping the flag on because this function is called 
	// after any update, so the flag is already turned on
	if       (m_Speed.x > 20.0f)   m_Speed.x  =  20.0f;
	else if  (m_Speed.x < -20.0f)  m_Speed.x  = -20.0f;
	if       (m_Speed.y > 20.0f)   m_Speed.y  =  20.0f;
	else if  (m_Speed.y < -20.0f)  m_Speed.y  = -20.0f;
	if       (m_Speed.z > 20.0f)   m_Speed.z  =  20.0f;
	else if  (m_Speed.z < -20.0f)  m_Speed.z  = -20.0f;
}





void cEntity::Destroy(void)
{
	if (!m_IsInitialized)
	{
		return;
	}
	m_World->BroadcastDestroyEntity(*this);

	m_IsInitialized = false;

	Destroyed();
}





void cEntity::Tick(float a_Dt, cChunk & a_Chunk)
{
	if (m_AttachedTo != NULL)
	{
		if ((m_Pos - m_AttachedTo->GetPosition()).Length() > 0.5)
		{
			SetPosition(m_AttachedTo->GetPosition());
		}
	}
	else
	{
		HandlePhysics(a_Dt, a_Chunk);
	}
}





void cEntity::HandlePhysics(float a_Dt, cChunk & a_Chunk)
{
	//TODO Add collision detection with entities.
	a_Dt /= 1000;
	Vector3d NextPos = Vector3d(GetPosX(),GetPosY(),GetPosZ());
	Vector3d NextSpeed = Vector3d(GetSpeedX(),GetSpeedY(),GetSpeedZ());
	int BlockX = (int) floor(NextPos.x);
	int BlockY = (int) floor(NextPos.y);
	int BlockZ = (int) floor(NextPos.z);
	//Make sure we got the correct chunk and a valid one. No one ever knows...
	cChunk * NextChunk = a_Chunk.GetNeighborChunk(BlockX,BlockZ);
	if (NextChunk != NULL)
	{
		int RelBlockX = BlockX - (NextChunk->GetPosX() * cChunkDef::Width);
		int RelBlockZ = BlockZ - (NextChunk->GetPosZ() * cChunkDef::Width);
		BLOCKTYPE BlockIn = NextChunk->GetBlock( RelBlockX, BlockY, RelBlockZ );
		if( BlockIn == E_BLOCK_AIR || IsBlockWater(BlockIn) || BlockIn == E_BLOCK_FIRE || IsBlockLava(BlockIn) ) // If not in ground itself or in water or in fire or in lava
		{
			if( m_bOnGround ) // check if it's still on the ground
			{
				BLOCKTYPE BlockBelow = NextChunk->GetBlock( RelBlockX, BlockY - 1, RelBlockZ );
				if(BlockBelow == E_BLOCK_AIR || IsBlockWater(BlockBelow) || BlockBelow == E_BLOCK_FIRE || IsBlockLava(BlockBelow)) //Check if block below is air or water.
				{
					m_bOnGround = false;
				}
			}
		}
		else
		{
			//Push out entity.
			m_bOnGround = true;
			NextPos.y += 0.2;
			LOGD("Entity #%d (%s) is inside a block at {%d,%d,%d}",
				m_UniqueID, GetClass(), BlockX, BlockY, BlockZ);
		}

		if (!m_bOnGround)
		{
			float fallspeed;
			if (!IsBlockWater(BlockIn))
			{
				fallspeed = m_Gravity * a_Dt;
			}
			else
			{
				fallspeed = -3.0f * a_Dt; //Fall slower in water.
			}
			NextSpeed.y += fallspeed;
		}
		else
		{
			//Friction
			if (NextSpeed.SqrLength() > 0.0004f)
			{
				NextSpeed.x *= 0.7f/(1+a_Dt);
				if ( fabs(NextSpeed.x) < 0.05 ) NextSpeed.x = 0;
				NextSpeed.z *= 0.7f/(1+a_Dt);
				if ( fabs(NextSpeed.z) < 0.05 ) NextSpeed.z = 0;
			}
		}
		
		//Get water direction
		Direction WaterDir = m_World->GetWaterSimulator()->GetFlowingDirection(BlockX, BlockY, BlockZ);

		m_WaterSpeed *= 0.9f;		//Reduce speed each tick

		switch(WaterDir)
		{
			case X_PLUS:
				m_WaterSpeed.x = 1.f;
				m_bOnGround = false;
				break;
			case X_MINUS:
				m_WaterSpeed.x = -1.f;
				m_bOnGround = false;
				break;
			case Z_PLUS:
				m_WaterSpeed.z = 1.f;
				m_bOnGround = false;
				break;
			case Z_MINUS:
				m_WaterSpeed.z = -1.f;
				m_bOnGround = false;
				break;
			
		default:
			break;
		}

		if (fabs(m_WaterSpeed.x) < 0.05)
		{
			m_WaterSpeed.x = 0;
		}

		if (fabs(m_WaterSpeed.z) < 0.05)
		{
			m_WaterSpeed.z = 0;
		}

		NextSpeed += m_WaterSpeed;

		if( NextSpeed.SqrLength() > 0.f )
		{
			cTracer Tracer( GetWorld() );
			int Ret = Tracer.Trace( NextPos, NextSpeed, 2 );
			if( Ret ) // Oh noez! we hit something
			{
				// Set to hit position
				if( (Tracer.RealHit - NextPos).SqrLength() <= ( NextSpeed * a_Dt ).SqrLength() )
				{
					if( Ret == 1 )
					{

						if( Tracer.HitNormal.x != 0.f ) NextSpeed.x = 0.f;
						if( Tracer.HitNormal.y != 0.f ) NextSpeed.y = 0.f;
						if( Tracer.HitNormal.z != 0.f ) NextSpeed.z = 0.f;

						if( Tracer.HitNormal.y > 0 ) // means on ground
						{
							m_bOnGround = true;
						}
					}
					NextPos.Set(Tracer.RealHit.x,Tracer.RealHit.y,Tracer.RealHit.z);
					NextPos.x += Tracer.HitNormal.x * 0.5f;
					NextPos.z += Tracer.HitNormal.z * 0.5f;
				}
				else
					NextPos += (NextSpeed * a_Dt);
			}
			else
			{	// We didn't hit anything, so move =]
				NextPos += (NextSpeed * a_Dt);
			}
		}
		BlockX = (int) floor(NextPos.x);
	    BlockZ = (int) floor(NextPos.z);
		NextChunk = NextChunk->GetNeighborChunk(BlockX,BlockZ);
		//See if we can commit our changes. If not, we will discard them.
		if (NextChunk != NULL)
		{
			if (NextPos.x != GetPosX()) SetPosX(NextPos.x);
			if (NextPos.y != GetPosY()) SetPosY(NextPos.y);
			if (NextPos.z != GetPosZ()) SetPosZ(NextPos.z);
			if (NextSpeed.x != GetSpeedX()) SetSpeedX(NextSpeed.x);
			if (NextSpeed.y != GetSpeedY()) SetSpeedY(NextSpeed.y);
			if (NextSpeed.z != GetSpeedZ()) SetSpeedZ(NextSpeed.z);
		}
	}
}





void cEntity::BroadcastMovementUpdate(const cClientHandle * a_Exclude)
{
	//We need to keep updating the clients when there is movement or if there was a change in speed and after 2 ticks
	if( (m_Speed.SqrLength() > 0.0004f || m_bDirtySpeed) && (m_World->GetWorldAge() - m_TimeLastSpeedPacket >= 2))
	{
		m_World->BroadcastEntVelocity(*this,a_Exclude);
		m_bDirtySpeed = false;
		m_TimeLastSpeedPacket = m_World->GetWorldAge();
	}

	//Have to process position related packets this every two ticks
	if (m_World->GetWorldAge() % 2 == 0)
	{
		int DiffX = (int) (floor(GetPosX() * 32.0) - floor(m_LastPosX * 32.0));
		int DiffY = (int) (floor(GetPosY() * 32.0) - floor(m_LastPosY * 32.0));
		int DiffZ = (int) (floor(GetPosZ() * 32.0) - floor(m_LastPosZ * 32.0));
		Int64 DiffTeleportPacket = m_World->GetWorldAge() - m_TimeLastTeleportPacket;
		// 4 blocks is max Relative So if the Diff is greater than 127 or. Send an absolute position every 20 seconds
		if (DiffTeleportPacket >= 400 || 
			((DiffX > 127) || (DiffX < -128) ||
			(DiffY > 127) || (DiffY < -128) ||
			(DiffZ > 127) || (DiffZ < -128)))
		{
			//
			m_World->BroadcastTeleportEntity(*this,a_Exclude);
			m_TimeLastTeleportPacket = m_World->GetWorldAge();
			m_TimeLastMoveReltPacket = m_TimeLastTeleportPacket; //Must synchronize.
			m_LastPosX = GetPosX();
			m_LastPosY = GetPosY();
			m_LastPosZ = GetPosZ();
			m_bDirtyPosition = false;
			m_bDirtyOrientation = false;
		}
		else
		{
			Int64 DiffMoveRelPacket = m_World->GetWorldAge() - m_TimeLastMoveReltPacket;
			//if the change is big enough.
			if ((abs(DiffX) >= 4 || abs(DiffY) >= 4 || abs(DiffZ) >= 4 || DiffMoveRelPacket >= 60) && m_bDirtyPosition)
			{
				if (m_bDirtyOrientation)
				{
					m_World->BroadcastEntRelMoveLook(*this, (char)DiffX, (char)DiffY, (char)DiffZ,a_Exclude);
					m_bDirtyOrientation = false;
				}
				else
				{
					m_World->BroadcastEntRelMove(*this, (char)DiffX, (char)DiffY, (char)DiffZ,a_Exclude);
				}
				m_LastPosX = GetPosX();
				m_LastPosY = GetPosY();
				m_LastPosZ = GetPosZ();
				m_bDirtyPosition = false;
				m_TimeLastMoveReltPacket = m_World->GetWorldAge();
			}
			else
			{
				if (m_bDirtyOrientation)
				{
					m_World->BroadcastEntLook(*this,a_Exclude);
					m_bDirtyOrientation = false;
				}
			}		
		}
		if (m_bDirtyHead)
		{
			m_World->BroadcastEntHeadLook(*this,a_Exclude);
			m_bDirtyHead = false;
		}
	}
}





void cEntity::AttachTo(cEntity * a_AttachTo)
{
	if (m_AttachedTo == a_AttachTo)
	{
		// Already attached to that entity, nothing to do here
		return;
	}

	// Detach from any previous entity:
	Detach();

	// Attach to the new entity:
	m_AttachedTo = a_AttachTo;
	a_AttachTo->m_Attachee = this;
	m_World->BroadcastAttachEntity(*this, a_AttachTo);
}





void cEntity::Detach(void)
{
	if (m_AttachedTo == NULL)
	{
		// Attached to no entity, our work is done
		return;
	}
	m_AttachedTo->m_Attachee = NULL;
	m_AttachedTo = NULL;
	m_World->BroadcastAttachEntity(*this, NULL);
}





bool cEntity::IsA(const char * a_ClassName) const
{
	return (strcmp(a_ClassName, "cEntity") == 0);
}





//////////////////////////////////////////////////////////////////////////
// Set orientations
void cEntity::SetRot(const Vector3f & a_Rot)
{
	m_Rot = a_Rot;
	m_bDirtyOrientation = true;
}





void cEntity::SetHeadYaw(double a_HeadYaw)
{
	m_HeadYaw = a_HeadYaw;
	m_bDirtyHead = true;
	WrapHeadYaw();
}





void cEntity::SetRotation(double a_Rotation)
{
	m_Rot.x = a_Rotation;
	m_bDirtyOrientation = true;
	WrapRotation();
}





void cEntity::SetPitch(double a_Pitch)
{
	m_Rot.y = a_Pitch;
	m_bDirtyOrientation = true;
	WrapRotation();
}





void cEntity::SetRoll(double a_Roll)
{
	m_Rot.z = a_Roll;
	m_bDirtyOrientation = true;
}





void cEntity::SetSpeed(double a_SpeedX, double a_SpeedY, double a_SpeedZ)
{
	m_Speed.Set(a_SpeedX, a_SpeedY, a_SpeedZ);
	m_bDirtySpeed = true;
	WrapSpeed();
}




void cEntity::SetSpeedX(double a_SpeedX)
{
	m_Speed.x = a_SpeedX;
	m_bDirtySpeed = true;
	WrapSpeed();
}




void cEntity::SetSpeedY(double a_SpeedY)
{
	m_Speed.y = a_SpeedY;
	m_bDirtySpeed = true;
	WrapSpeed();
}




void cEntity::SetSpeedZ(double a_SpeedZ)
{
	m_Speed.z = a_SpeedZ;
	m_bDirtySpeed = true;
	WrapSpeed();
}




void cEntity::AddPosX(double a_AddPosX)
{
	m_Pos.x += a_AddPosX;
	m_bDirtyPosition = true;
}




void cEntity::AddPosY(double a_AddPosY)
{
	m_Pos.y += a_AddPosY;
	m_bDirtyPosition = true;
}




void cEntity::AddPosZ(double a_AddPosZ)
{
	m_Pos.z += a_AddPosZ;
	m_bDirtyPosition = true;
}




void cEntity::AddPosition(double a_AddPosX, double a_AddPosY, double a_AddPosZ)
{
	m_Pos.x += a_AddPosX;
	m_Pos.y += a_AddPosY;
	m_Pos.z += a_AddPosZ;
	m_bDirtyPosition = true;
}




void cEntity::AddSpeed(double a_AddSpeedX, double a_AddSpeedY, double a_AddSpeedZ)
{
	m_Speed.x += a_AddSpeedX;
	m_Speed.y += a_AddSpeedY;
	m_Speed.z += a_AddSpeedZ;
	m_bDirtySpeed = true;
	WrapSpeed();
}





void cEntity::AddSpeedX(double a_AddSpeedX)
{
	m_Speed.x += a_AddSpeedX;
	m_bDirtySpeed = true;
	WrapSpeed();
}





void cEntity::AddSpeedY(double a_AddSpeedY)
{
	m_Speed.y += a_AddSpeedY;
	m_bDirtySpeed = true;
	WrapSpeed();
}





void cEntity::AddSpeedZ(double a_AddSpeedZ)
{
	m_Speed.z += a_AddSpeedZ;
	m_bDirtySpeed = true;
	WrapSpeed();
}





//////////////////////////////////////////////////////////////////////////
// Get look vector (this is NOT a rotation!)
Vector3d cEntity::GetLookVector(void) const
{
	Matrix4d m;
	m.Init(Vector3f(), 0, m_Rot.x, -m_Rot.y);
	Vector3d Look = m.Transform(Vector3d(0, 0, 1));
	return Look;
}





//////////////////////////////////////////////////////////////////////////
// Set position
void cEntity::SetPosition(double a_PosX, double a_PosY, double a_PosZ)
{
	m_Pos.Set(a_PosX, a_PosY, a_PosZ);
	m_bDirtyPosition = true;
}





void cEntity::SetPosX(double a_PosX)
{
	m_Pos.x = a_PosX;
	m_bDirtyPosition = true;
}





void cEntity::SetPosY(double a_PosY)
{
	m_Pos.y = a_PosY;
	m_bDirtyPosition = true;
}





void cEntity::SetPosZ(double a_PosZ)
{
	m_Pos.z = a_PosZ;
	m_bDirtyPosition = true;
}





//////////////////////////////////////////////////////////////////////////
// Reference stuffs
void cEntity::AddReference(cEntity * & a_EntityPtr)
{
	m_References->AddReference(a_EntityPtr);
	a_EntityPtr->ReferencedBy(a_EntityPtr);
}





void cEntity::ReferencedBy(cEntity * & a_EntityPtr)
{
	m_Referencers->AddReference(a_EntityPtr);
}





void cEntity::Dereference(cEntity * & a_EntityPtr)
{
	m_Referencers->Dereference(a_EntityPtr);
}




