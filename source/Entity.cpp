
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Entity.h"
#include "World.h"
#include "Server.h"
#include "Root.h"
#include "Vector3d.h"
#include "Vector3f.h"
#include "Matrix4f.h"
#include "ReferenceManager.h"
#include "ClientHandle.h"





int cEntity::m_EntityCount = 0;
cCriticalSection cEntity::m_CSCount;





cEntity::cEntity(eEntityType a_EntityType, double a_X, double a_Y, double a_Z)
	: m_UniqueID( 0 )
	, m_Referencers( new cReferenceManager( cReferenceManager::RFMNGR_REFERENCERS ) )
	, m_References( new cReferenceManager( cReferenceManager::RFMNGR_REFERENCES ) )
	, m_ChunkX( 0 )
	, m_ChunkY( 0 )
	, m_ChunkZ( 0 )
	, m_Pos( a_X, a_Y, a_Z )
	, m_bDirtyPosition( true )
	, m_bDirtyOrientation( true )
	, m_bDestroyed( false )
	, m_EntityType(a_EntityType)
	, m_World(NULL)
	, m_bRemovedFromChunk( false )
	, m_FireDamageInterval(0.f)
	, m_BurnPeriod(0.f)
{
	cCSLock Lock(m_CSCount);
	m_EntityCount++;
	m_UniqueID = m_EntityCount;
}





cEntity::~cEntity()
{
	LOGD("Deleting entity %d at pos {%.2f, %.2f} ~ [%d, %d]; ptr %p", 
		m_UniqueID,
		m_Pos.x, m_Pos.z,
		(int)(m_Pos.x / cChunkDef::Width), (int)(m_Pos.z / cChunkDef::Width),
		this
	);
	
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
	m_World->AddEntity( this );

	MoveToCorrectChunk(true);
}





void cEntity::WrapRotation()
{
	while (m_Rot.x > 180.f)  m_Rot.x-=360.f; // Wrap it
	while (m_Rot.x < -180.f) m_Rot.x+=360.f;
	while (m_Rot.y > 180.f)  m_Rot.y-=360.f;
	while (m_Rot.y < -180.f) m_Rot.y+=360.f;
}





void cEntity::MoveToCorrectChunk(bool a_bIgnoreOldChunk)
{
	ASSERT(m_World != NULL);  // Entity needs a world to move to a chunk
	if( !m_World ) return;

	int ChunkX = 0, ChunkY = 0, ChunkZ = 0;
	cWorld::BlockToChunk( (int)m_Pos.x, (int)m_Pos.y, (int)m_Pos.z, ChunkX, ChunkY, ChunkZ );
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
	
	HandlePhysics(a_Dt);
}





bool cEntity::IsA(const char * a_ClassName) const
{
	return (strcmp(a_ClassName, "cEntity") == 0);
}





//////////////////////////////////////////////////////////////////////////
// Set orientations
void cEntity::SetRot( const Vector3f & a_Rot )
{
	m_Rot = a_Rot;
	m_bDirtyOrientation = true;
}





void cEntity::SetRotation( float a_Rotation )
{
	m_Rot.x = a_Rotation;
	m_bDirtyOrientation = true;
}





void cEntity::SetPitch( float a_Pitch )
{
	m_Rot.y = a_Pitch;
	m_bDirtyOrientation = true;
}





void cEntity::SetRoll( float a_Roll )
{
	m_Rot.z = a_Roll;
	m_bDirtyOrientation = true;
}





//////////////////////////////////////////////////////////////////////////
// Get look vector (this is NOT a rotation!)
Vector3f cEntity::GetLookVector(void) const
{
	Matrix4f m;
	m.Init( Vector3f(), 0, m_Rot.x, -m_Rot.y );
	Vector3f Look = m.Transform( Vector3f(0, 0, 1) );
	return Look;
}





//////////////////////////////////////////////////////////////////////////
// Set position
void cEntity::SetPosition( const Vector3d & a_Pos )
{
	m_Pos = a_Pos;
	MoveToCorrectChunk();
	m_bDirtyPosition = true;
}





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
	m_References->AddReference( a_EntityPtr );
	a_EntityPtr->ReferencedBy( a_EntityPtr );
}





void cEntity::ReferencedBy(cEntity * & a_EntityPtr)
{
	m_Referencers->AddReference( a_EntityPtr );
}





void cEntity::Dereference(cEntity*& a_EntityPtr)
{
	m_Referencers->Dereference( a_EntityPtr );
}




