#include "cEntity.h"
#include "cWorld.h"
#include "cChunk.h"
#include "cMCLogger.h"
#include "cServer.h"
#include "cRoot.h"
#include "Vector3d.h"
#include "Vector3f.h"
#include "Matrix4f.h"
#include "cReferenceManager.h"
#include "cClientHandle.h"

#include "packets/cPacket_DestroyEntity.h"

int cEntity::m_EntityCount = 0;

cEntity::cEntity(const double & a_X, const double & a_Y, const double & a_Z)
    : m_UniqueID( 0 )
	, m_Referencers( new cReferenceManager( cReferenceManager::RFMNGR_REFERENCERS ) )
	, m_References( new cReferenceManager( cReferenceManager::RFMNGR_REFERENCES ) )
    , m_ChunkX( 0 )
    , m_ChunkY( 0 )
    , m_ChunkZ( 0 )
	, m_Pos( new Vector3d( a_X, a_Y, a_Z ) )
	, m_bDirtyPosition( true )
	, m_Rot( new Vector3f() )
	, m_bDirtyOrientation( true )
	, m_bDestroyed( false )
	, m_EntityType( E_ENTITY )
	, m_World( 0 )
	, m_bRemovedFromChunk( false )
{
	m_EntityCount++;
	m_UniqueID = m_EntityCount;
}

cEntity::~cEntity()
{
	if( !m_bDestroyed || !m_bRemovedFromChunk )
	{
		LOGERROR("ERROR: Entity deallocated without being destroyed %i or unlinked %i", m_bDestroyed, m_bRemovedFromChunk );
	}
	delete m_Referencers;
	delete m_References;
	delete m_Pos;
	delete m_Rot;
}

void cEntity::Initialize( cWorld* a_World )
{
	m_World = a_World;
	m_World->AddEntity( this );

	cWorld::BlockToChunk( (int)m_Pos->x, (int)m_Pos->y, (int)m_Pos->z, m_ChunkX, m_ChunkY, m_ChunkZ );
	cChunk* Chunk = m_World->GetChunk( m_ChunkX, m_ChunkY, m_ChunkZ );
	if( Chunk )
	{
		//LOG("Adding entity %i to chunk %i %i %i", m_UniqueID, Chunk->GetPosX(), Chunk->GetPosY(), Chunk->GetPosZ() );
		Chunk->AddEntity( *this );
	}
}

void cEntity::WrapRotation()
{
	while(m_Rot->x > 180.f)  m_Rot->x-=360.f; // Wrap it
	while(m_Rot->x < -180.f) m_Rot->x+=360.f;
	while(m_Rot->y > 180.f)  m_Rot->y-=360.f;
	while(m_Rot->y < -180.f) m_Rot->y+=360.f;
}

void cEntity::MoveToCorrectChunk()
{
	if( !m_World ) return; // Entity needs a world to move to a chunk

	int ChunkX = 0, ChunkY = 0, ChunkZ = 0;
	cWorld::BlockToChunk( (int)m_Pos->x, (int)m_Pos->y, (int)m_Pos->z, ChunkX, ChunkY, ChunkZ );
	if( m_ChunkX != ChunkX || m_ChunkY != ChunkY || m_ChunkZ != ChunkZ )
	{
		LOG("From %i %i To %i %i", m_ChunkX, m_ChunkZ, ChunkX, ChunkZ );
		cChunk* Chunk = m_World->GetChunkUnreliable( m_ChunkX, m_ChunkY, m_ChunkZ );

		typedef std::list< cClientHandle* > ClientList;
		ClientList BeforeClients;
		if( Chunk )
		{
			Chunk->RemoveEntity( *this );
			BeforeClients = Chunk->GetClients();
		}
		m_ChunkX = ChunkX; m_ChunkY = ChunkY; m_ChunkZ = ChunkZ;
		cChunk* NewChunk = m_World->GetChunk( m_ChunkX, m_ChunkY, m_ChunkZ );
		ClientList AfterClients;
		if( NewChunk )
		{
			NewChunk->AddEntity( *this );
			AfterClients = NewChunk->GetClients();
		}


		/********************
		 * I reaalllyyyy don't like this piece of code, but it's needed I guess (maybe there's a way to optimize this)
		 **/
		// Now compare clients before with after
		for( ClientList::iterator itr = BeforeClients.begin(); itr != BeforeClients.end(); ++itr )
		{
			bool bFound = false;
			for( ClientList::iterator itr2 = AfterClients.begin(); itr2 != AfterClients.end(); ++itr2 )
			{
				if( *itr2 == *itr )
				{
					bFound = true;
					break;
				}
			}
			if( !bFound )	// Client was in old chunk, but not new, so destroy on that client
			{
				cPacket_DestroyEntity DestroyEntity( this );
				(*itr)->Send( DestroyEntity );
			}
		}

		// Now compare clients after with before
		for( ClientList::iterator itr = AfterClients.begin(); itr != AfterClients.end(); ++itr )
		{
			bool bFound = false;
			for( ClientList::iterator itr2 = BeforeClients.begin(); itr2 != BeforeClients.end(); ++itr2 )
			{
				if( *itr2 == *itr )
				{
					bFound = true;
					break;
				}
			}
			if( !bFound )	// Client is in the new chunk, but not in old, so spawn on the client
			{
				SpawnOn( *itr );
			}
		}
	}
}

void cEntity::Destroy()
{
	if( !m_bDestroyed )
	{
		m_bDestroyed = true;
		if( !m_bRemovedFromChunk )
			RemoveFromChunk(0);
	}
}

void cEntity::RemoveFromChunk( cChunk* a_Chunk )
{
	if( m_World )
	{
		cChunk* Chunk = ( a_Chunk ? a_Chunk : m_World->GetChunkUnreliable( m_ChunkX, m_ChunkY, m_ChunkZ ) );
		if( Chunk )
		{
			cPacket_DestroyEntity DestroyEntity( this );
			Chunk->Broadcast( DestroyEntity );
			Chunk->RemoveEntity( *this );
			m_bRemovedFromChunk = true;
		}
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
	*m_Rot = a_Rot;
	m_bDirtyOrientation = true;
}

void cEntity::SetRotation( float a_Rotation )
{
	m_Rot->x = a_Rotation;
	m_bDirtyOrientation = true;
}

void cEntity::SetPitch( float a_Pitch )
{
	m_Rot->y = a_Pitch;
	m_bDirtyOrientation = true;
}

void cEntity::SetRoll( float a_Roll )
{
	m_Rot->z = a_Roll;
	m_bDirtyOrientation = true;
}

//////////////////////////////////////////////////////////////////////////
// Get orientations
const Vector3f & cEntity::GetRot()
{
	return *m_Rot;
}

float cEntity::GetRotation()
{
	return m_Rot->x;
}

float cEntity::GetPitch()
{
	return m_Rot->y;
}

float cEntity::GetRoll()
{
	return m_Rot->z;
}

//////////////////////////////////////////////////////////////////////////
// Get look vector (this is NOT a rotation!)
Vector3f cEntity::GetLookVector()
{
	Matrix4f m;
	m.Init( Vector3f(), 0, m_Rot->x, -m_Rot->y );
	Vector3f Look = m.Transform( Vector3f(0, 0, 1) );
	LOG("Look: %0.1f %0.1f %0.1f", Look.x, Look.y, Look.z );
	return Look;
}

//////////////////////////////////////////////////////////////////////////
// Set position
void cEntity::SetPosition( const Vector3d & a_Pos )
{
	*m_Pos = a_Pos;
	MoveToCorrectChunk();
	m_bDirtyPosition = true;
}

void cEntity::SetPosition( const double & a_PosX, const double & a_PosY, const double & a_PosZ )
{
	m_Pos->Set( a_PosX, a_PosY, a_PosZ );
	MoveToCorrectChunk();
	m_bDirtyPosition = true;
}

void cEntity::SetPosX( const double & a_PosX )
{
	m_Pos->x = a_PosX;
	MoveToCorrectChunk();
	m_bDirtyPosition = true;
}

void cEntity::SetPosY( const double & a_PosY )
{
	m_Pos->y = a_PosY;
	MoveToCorrectChunk();
	m_bDirtyPosition = true;
}

void cEntity::SetPosZ( const double & a_PosZ )
{
	m_Pos->z = a_PosZ;
	MoveToCorrectChunk();
	m_bDirtyPosition = true;
}

//////////////////////////////////////////////////////////////////////////
// Get position
const Vector3d & cEntity::GetPosition()
{
	return *m_Pos;
}

const double & cEntity::GetPosX()
{
	return m_Pos->x;
}

const double & cEntity::GetPosY()
{
	return m_Pos->y;
}

const double & cEntity::GetPosZ()
{
	return m_Pos->z;
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
