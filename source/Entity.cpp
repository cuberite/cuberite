
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Entity.h"
#include "World.h"
#include "Server.h"
#include "Root.h"
#include "Vector3d.h"
#include "Matrix4f.h"
#include "ReferenceManager.h"
#include "ClientHandle.h"





int cEntity::m_EntityCount = 0;
cCriticalSection cEntity::m_CSCount;





cEntity::cEntity(eEntityType a_EntityType, double a_X, double a_Y, double a_Z)
	: m_UniqueID(0)
	, m_AttachedTo(NULL)
	, m_Attachee(NULL)
	, m_Referencers(new cReferenceManager(cReferenceManager::RFMNGR_REFERENCERS))
	, m_References(new cReferenceManager(cReferenceManager::RFMNGR_REFERENCES))
	, m_ChunkX(0)
	, m_ChunkY(0)
	, m_ChunkZ(0)
	, m_Pos(a_X, a_Y, a_Z)
	, m_bDirtyOrientation(true)
	, m_bDirtyPosition(true)
	, m_bDirtySpeed(true)
	, m_bDestroyed(false)
	, m_LastPosX( 0.0 )
	, m_LastPosY( 0.0 )
	, m_LastPosZ( 0.0 )
	, m_TimeLastTeleportPacket(0)
	, m_TimeLastMoveReltPacket(0)
	, m_TimeLastSpeedPacket(0)
	, m_EntityType(a_EntityType)
	, m_World(NULL)
	, m_bRemovedFromChunk(false)
	, m_FireDamageInterval(0.f)
	, m_BurnPeriod(0.f)
{
	cCSLock Lock(m_CSCount);
	m_EntityCount++;
	m_UniqueID = m_EntityCount;
}





cEntity::~cEntity()
{
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
	
	if (!m_bDestroyed || !m_bRemovedFromChunk)
	{
		LOGERROR("ERROR: Entity deallocated without being destroyed %i or unlinked %i", m_bDestroyed, m_bRemovedFromChunk);
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
	m_World = a_World;
	m_World->AddEntity(this);

	MoveToCorrectChunk(true);
}





void cEntity::WrapRotation()
{
	while (m_Rot.x > 180.f)  m_Rot.x-=360.f; // Wrap it
	while (m_Rot.x < -180.f) m_Rot.x+=360.f;
	while (m_Rot.y > 180.f)  m_Rot.y-=360.f;
	while (m_Rot.y < -180.f) m_Rot.y+=360.f;
}

void cEntity::WrapSpeed()
{
	//There shoudn't be a need for flipping the flag on because this function is called 
	//after any update, so the flag is already turned on
	if       (m_Speed.x > 20.0f)   m_Speed.x  =  20.0f;
	else if  (m_Speed.x < -20.0f)  m_Speed.x  = -20.0f;
	if       (m_Speed.y > 20.0f)   m_Speed.y  =  20.0f;
	else if  (m_Speed.y < -20.0f)  m_Speed.y  = -20.0f;
	if       (m_Speed.z > 20.0f)   m_Speed.z  =  20.0f;
	else if  (m_Speed.z < -20.0f)  m_Speed.z  = -20.0f;
}





void cEntity::MoveToCorrectChunk(bool a_bIgnoreOldChunk)
{
	if (!m_World)
	{
		// This is normal for entities being currently loaded
		return;
	}

	int ChunkX = 0, ChunkY = 0, ChunkZ = 0;
	cWorld::BlockToChunk((int)m_Pos.x, (int)m_Pos.y, (int)m_Pos.z, ChunkX, ChunkY, ChunkZ);
	if (!a_bIgnoreOldChunk && (m_ChunkX == ChunkX) && (m_ChunkY == ChunkY) && (m_ChunkZ == ChunkZ))
	{
		return;
	}
	
	class cMover :
		public cClientDiffCallback
	{
		virtual void Removed(cClientHandle * a_Client) override
		{
			if (m_IgnoreOldChunk)
			{
				return;
			}
			a_Client->SendDestroyEntity(*m_Entity);
		}
		
		virtual void Added(cClientHandle * a_Client) override
		{
			m_Entity->SpawnOn(*a_Client);
		}

		bool      m_IgnoreOldChunk;
		cEntity * m_Entity;
		
	public:
		cMover(cEntity * a_Entity, bool a_IgnoreOldChunk) :
			m_IgnoreOldChunk(a_IgnoreOldChunk),
			m_Entity(a_Entity)
		{}
	} Mover(this, a_bIgnoreOldChunk);
	
	m_World->CompareChunkClients(m_ChunkX, m_ChunkY, m_ChunkZ, ChunkX, ChunkY, ChunkZ, Mover);
	m_World->MoveEntityToChunk(this, ChunkX, ChunkY, ChunkZ);
	
	m_ChunkX = ChunkX;
	m_ChunkY = ChunkY;
	m_ChunkZ = ChunkZ;
}





void cEntity::Destroy()
{
	if (m_bDestroyed)
	{
		return;
	}
	if (!m_bRemovedFromChunk)
	{
		RemoveFromChunk();
	}
	
	m_World->BroadcastDestroyEntity(*this);
	
	m_bDestroyed = true;

	Destroyed();
}





void cEntity::RemoveFromChunk(void)
{
	if (m_World == NULL)
	{
		return;
	}
	
	m_World->RemoveEntityFromChunk(this, m_ChunkX, m_ChunkY, m_ChunkZ);
	m_bRemovedFromChunk = true;
}





void cEntity::Tick(float a_Dt, MTRand & a_TickRandom)
{
	UNUSED(a_TickRandom);
	
	if (m_AttachedTo != NULL)
	{
		if ((m_Pos - m_AttachedTo->GetPosition()).Length() > 0.5)
		{
			SetPosition(m_AttachedTo->GetPosition());
		}
	}
	else
	{
		HandlePhysics(a_Dt);
	}
}





void cEntity::BroadcastMovementUpdate(const cClientHandle * a_Exclude)
{
	if (m_bDirtyOrientation && !m_bDirtyPosition)
	{
		//LOGD("Sending (rot,yaw,roll) = (%f,%f,%f)",m_Rot.x,m_Rot.y,m_Rot.z);
		m_World->BroadcastEntLook(*this,a_Exclude);
		m_World->BroadcastEntHeadLook(*this,a_Exclude);
		m_bDirtyOrientation = false;
	}

	if (m_bDirtyPosition)
	{
		float DiffX = (float)(GetPosX() - m_LastPosX);
		float DiffY = (float)(GetPosY() - m_LastPosY);
		float DiffZ = (float)(GetPosZ() - m_LastPosZ);
		float SqrDist = DiffX * DiffX + DiffY * DiffY + DiffZ * DiffZ;
		if (
			(SqrDist > 16)  // 4 blocks is max Relative Move. 16 = 4 ^ 2
			|| (m_World->GetWorldAge() - m_TimeLastTeleportPacket > 400)  // Send an absolute position every 20 seconds
			)
		{
			//LOGD("Teleported from (%f,%f,%f) to (%f,%f,%f); Distance square: %f",m_LastPosX,m_LastPosY,m_LastPosZ, m_Pos.x,m_Pos.y,m_Pos.z,SqrDist );
			m_World->BroadcastEntHeadLook(*this,a_Exclude);
			m_World->BroadcastTeleportEntity(*this,a_Exclude);
			m_TimeLastTeleportPacket = m_World->GetWorldAge();
			m_LastPosX = GetPosX();
			m_LastPosY = GetPosY();
			m_LastPosZ = GetPosZ();
			m_bDirtyPosition = false;
		}
		else
		{
			if ((m_World->GetWorldAge() - m_TimeLastMoveReltPacket > 60)) // Send relative movement every 3 seconds
			{
				//LOGD("Moved from (%f,%f,%f) to (%f,%f,%f)",m_LastPosX,m_LastPosY,m_LastPosZ, m_Pos.x,m_Pos.y,m_Pos.z );
				if (m_bDirtyOrientation)
				{
					m_World->BroadcastEntHeadLook(*this,a_Exclude);
					m_World->BroadcastEntRelMoveLook(*this, (char)(DiffX * 32), (char)(DiffY * 32), (char)(DiffZ * 32),a_Exclude);
					m_bDirtyOrientation = false;
				}
				else
				{
					m_World->BroadcastEntHeadLook(*this,a_Exclude);
					m_World->BroadcastEntRelMove(*this, (char)(DiffX * 32), (char)(DiffY * 32), (char)(DiffZ * 32),a_Exclude);
				}
				m_TimeLastMoveReltPacket = m_World->GetWorldAge();
				m_LastPosX = GetPosX();
				m_LastPosY = GetPosY();
				m_LastPosZ = GetPosZ();
				m_bDirtyPosition = false;
			}
		}
	}
	//We need to keep updating the clients when there is movement or if there was a change in speed and after 1 tick
	if( (m_Speed.SqrLength() > 0.0004f || m_bDirtySpeed) && (m_World->GetWorldAge() - m_TimeLastSpeedPacket >= 1))
	{
		m_World->BroadcastEntVelocity(*this,a_Exclude);
		m_bDirtySpeed = false;
		m_TimeLastSpeedPacket = m_World->GetWorldAge();
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





void cEntity::SetRotation(double a_Rotation)
{
	m_Rot.x = a_Rotation;
	m_bDirtyOrientation = true;
}





void cEntity::SetPitch(double a_Pitch)
{
	m_Rot.y = a_Pitch;
	m_bDirtyOrientation = true;
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




void cEntity::AddSpeed(const Vector3d & a_AddSpeed)
{
	m_Speed += a_AddSpeed;
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
	MoveToCorrectChunk();
	m_bDirtyPosition = true;
}





void cEntity::SetPosX(double a_PosX)
{
	m_Pos.x = a_PosX;
	MoveToCorrectChunk();
	m_bDirtyPosition = true;
}





void cEntity::SetPosY(double a_PosY)
{
	m_Pos.y = a_PosY;
	MoveToCorrectChunk();
	m_bDirtyPosition = true;
}





void cEntity::SetPosZ(double a_PosZ)
{
	m_Pos.z = a_PosZ;
	MoveToCorrectChunk();
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




