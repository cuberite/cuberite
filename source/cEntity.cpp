
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cEntity.h"
#include "cWorld.h"
#include "cServer.h"
#include "cRoot.h"
#include "Vector3d.h"
#include "Vector3f.h"
#include "Matrix4f.h"
#include "cReferenceManager.h"
#include "cClientHandle.h"

#include "packets/cPacket_DestroyEntity.h"





int cEntity::m_EntityCount = 0;
cCriticalSection cEntity::m_CSCount;





cEntity::cEntity(const double & a_X, const double & a_Y, const double & a_Z)
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
	, m_EntityType( eEntityType_Entity )
	, m_World( 0 )
	, m_bRemovedFromChunk( false )
{
	cCSLock Lock(m_CSCount);
	m_EntityCount++;
	m_UniqueID = m_EntityCount;
}





cEntity::~cEntity()
{
	LOG("Deleting entity %d at pos {%.2f, %.2f} ~ [%d, %d]; ptr %p", 
		m_UniqueID,
		m_Pos.x, m_Pos.z,
		(int)(m_Pos.x / cChunkDef::Width), (int)(m_Pos.z / cChunkDef::Width),
		this
	);
	
	if( !m_bDestroyed || !m_bRemovedFromChunk )
	{
		LOGERROR("ERROR: Entity deallocated without being destroyed %i or unlinked %i", m_bDestroyed, m_bRemovedFromChunk );
		ASSERT(!"Entity deallocated without being destroyed or unlinked");
	}
	delete m_Referencers;
	delete m_References;
}





void cEntity::Initialize( cWorld* a_World )
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
			if (m_Destroy == NULL)
			{
				m_Destroy = new cPacket_DestroyEntity(m_Entity);
			}
			a_Client->Send(*m_Destroy);
		}
		
		virtual void Added(cClientHandle * a_Client) override
		{
			if (m_Spawn == NULL)
			{
				m_Spawn = m_Entity->GetSpawnPacket();  // Only create the packet when needed
			}
			if (m_Spawn != NULL)
			{
				a_Client->Send(*m_Spawn);
			}
		}

		cPacket * m_Destroy;
		cPacket * m_Spawn;
		bool      m_IgnoreOldChunk;
		cEntity * m_Entity;
		
	public:
		cMover(cEntity * a_Entity, bool a_IgnoreOldChunk) :
			m_Destroy(NULL),
			m_Spawn(NULL),
			m_IgnoreOldChunk(a_IgnoreOldChunk),
			m_Entity(a_Entity)
		{}
		
		~cMover()
		{
			delete m_Spawn;
			delete m_Destroy;
		}
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
	
	m_World->BroadcastToChunk(m_ChunkX, m_ChunkY, m_ChunkZ, cPacket_DestroyEntity(this));
	
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





void cEntity::SpawnOn(cClientHandle * a_Client)
{
	std::auto_ptr<cPacket> SpawnPacket(GetSpawnPacket());
	if (SpawnPacket.get() == NULL)
	{
		return;
	}
	
	if (a_Client == NULL)
	{
		m_World->BroadcastToChunk(m_ChunkX, m_ChunkY, m_ChunkZ, *SpawnPacket.get(), NULL);
	}
	else
	{
		a_Client->Send(*(SpawnPacket.get()));
	}
}





CLASS_DEF_GETCLASS( cEntity );
bool cEntity::IsA( const char* a_EntityType )
{
    //LOG("IsA( cEntity ) : %s", a_EntityType);
	if( strcmp( a_EntityType, "cEntity" ) == 0 ) return true;
	return false;
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
Vector3f cEntity::GetLookVector()
{
	Matrix4f m;
	m.Init( Vector3f(), 0, m_Rot.x, -m_Rot.y );
	Vector3f Look = m.Transform( Vector3f(0, 0, 1) );
	LOG("Look: %0.1f %0.1f %0.1f", Look.x, Look.y, Look.z );
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





void cEntity::SetPosition( const double & a_PosX, const double & a_PosY, const double & a_PosZ )
{
	m_Pos.Set( a_PosX, a_PosY, a_PosZ );
	MoveToCorrectChunk();
	m_bDirtyPosition = true;
}





void cEntity::SetPosX( const double & a_PosX )
{
	m_Pos.x = a_PosX;
	MoveToCorrectChunk();
	m_bDirtyPosition = true;
}





void cEntity::SetPosY( const double & a_PosY )
{
	m_Pos.y = a_PosY;
	MoveToCorrectChunk();
	m_bDirtyPosition = true;
}





void cEntity::SetPosZ( const double & a_PosZ )
{
	m_Pos.z = a_PosZ;
	MoveToCorrectChunk();
	m_bDirtyPosition = true;
}





//////////////////////////////////////////////////////////////////////////
// Reference stuffs
void cEntity::AddReference( cEntity*& a_EntityPtr )
{
	m_References->AddReference( a_EntityPtr );
	a_EntityPtr->ReferencedBy( a_EntityPtr );
}





void cEntity::ReferencedBy( cEntity*& a_EntityPtr )
{
	m_Referencers->AddReference( a_EntityPtr );
}





void cEntity::Dereference( cEntity*& a_EntityPtr )
{
	m_Referencers->Dereference( a_EntityPtr );
}




