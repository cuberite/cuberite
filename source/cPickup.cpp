#ifndef _WIN32
#include <cstdlib>
#endif

#include "cPickup.h"
#include "cClientHandle.h"
#include "cInventory.h"
#include "cWorld.h"
#include "cWaterSimulator.h"
#include "cServer.h"
#include "cPlayer.h"
#include "cPluginManager.h"
#include "cItem.h"
#include "cRoot.h"
#include "cMCLogger.h"
#include "cTracer.h"
#include "cChunk.h"

#include "packets/cPacket_TeleportEntity.h"
#include "packets/cPacket_PickupSpawn.h"
#include "packets/cPacket_CollectItem.h"

#include "Vector3d.h"
#include "Vector3f.h"

CLASS_DEFINITION( cPickup, cEntity )

cPickup::~cPickup()
{
	delete m_Item;
	delete m_Speed;
	delete m_ResultingSpeed;
	delete m_WaterSpeed;
}

cPickup::cPickup(int a_X, int a_Y, int a_Z, const cItem & a_Item, float a_SpeedX /* = 0.f */, float a_SpeedY /* = 0.f */, float a_SpeedZ /* = 0.f */)
	:	cEntity( ((double)(a_X))/32, ((double)(a_Y))/32, ((double)(a_Z))/32 )
	, m_Speed( new Vector3f( a_SpeedX, a_SpeedY, a_SpeedZ ) )
	, m_ResultingSpeed(new Vector3f())
	, m_WaterSpeed(new Vector3f())
	, m_bOnGround( false )
	, m_bReplicated( false )
	, m_Timer( 0.f )
	, m_Item( new cItem( a_Item ) )
	, m_bCollected( false )
{

	//LOG("New pickup: ID(%i) Amount(%i) Health(%i)", m_Item.m_ItemID, m_Item.m_ItemCount, m_Item.m_ItemHealth );

	// Spawn it on clients
	cPacket_PickupSpawn PickupSpawn;
	PickupSpawn.m_UniqueID = m_UniqueID;
	PickupSpawn.m_Item = (short)m_Item->m_ItemID;
	PickupSpawn.m_Count = m_Item->m_ItemCount;
	PickupSpawn.m_Health = m_Item->m_ItemHealth;
	PickupSpawn.m_PosX = a_X;
	PickupSpawn.m_PosY = a_Y;
	PickupSpawn.m_PosZ = a_Z;
	PickupSpawn.m_Rotation = (char)(m_Speed->x * 8);
	PickupSpawn.m_Pitch = (char)(m_Speed->y * 8);
	PickupSpawn.m_Roll = (char)(m_Speed->z * 8);
	if(PickupSpawn.m_Item != E_ITEM_EMPTY)
		cRoot::Get()->GetServer()->Broadcast( PickupSpawn );

	m_EntityType = E_PICKUP;
}

cPickup::cPickup(cPacket_PickupSpawn* a_PickupSpawnPacket)
	:	cEntity( ((double)a_PickupSpawnPacket->m_PosX)/32, ((double)a_PickupSpawnPacket->m_PosY)/32, ((double)a_PickupSpawnPacket->m_PosZ)/32 )
	, m_Speed( new Vector3f() )
	, m_ResultingSpeed(new Vector3f())
	, m_WaterSpeed(new Vector3f())
	, m_bOnGround( false )
	, m_bReplicated( false )
	, m_Timer( 0.f )
	, m_bCollected( false )
{
	a_PickupSpawnPacket->m_UniqueID = m_UniqueID;

	m_Item = new cItem();
	m_Item->m_ItemID = (ENUM_ITEM_ID)a_PickupSpawnPacket->m_Item;
	m_Item->m_ItemCount = a_PickupSpawnPacket->m_Count;
	m_Item->m_ItemHealth = 0x0;

	m_Speed->x = (float)(a_PickupSpawnPacket->m_Rotation)	/ 8;
	m_Speed->y = (float)(a_PickupSpawnPacket->m_Pitch)		/ 8;
	m_Speed->z = (float)(a_PickupSpawnPacket->m_Roll)		/ 8;

	// Spawn it on clients
	if(a_PickupSpawnPacket->m_Item != E_ITEM_EMPTY)
		cRoot::Get()->GetServer()->Broadcast( *a_PickupSpawnPacket );

	m_EntityType = E_PICKUP;
}

void cPickup::SpawnOn( cClientHandle* a_Target )
{
	cPacket_PickupSpawn PickupSpawn;
	PickupSpawn.m_UniqueID = m_UniqueID;
	PickupSpawn.m_Item = (short)m_Item->m_ItemID;
	PickupSpawn.m_Count = m_Item->m_ItemCount;
	PickupSpawn.m_Health = m_Item->m_ItemHealth;
	PickupSpawn.m_PosX = (int)(m_Pos->x * 32);
	PickupSpawn.m_PosY = (int)(m_Pos->y * 32);
	PickupSpawn.m_PosZ = (int)(m_Pos->z * 32);
	PickupSpawn.m_Rotation = (char)(m_Speed->x * 8);
	PickupSpawn.m_Pitch    = (char)(m_Speed->y * 8);
	PickupSpawn.m_Roll	   = (char)(m_Speed->z * 8);
	if(PickupSpawn.m_Item != E_ITEM_EMPTY)
		a_Target->Send( PickupSpawn );
}

void cPickup::Tick(float a_Dt)
{
	m_Timer += a_Dt;
	a_Dt = a_Dt / 1000.f;
	if(m_bCollected)
	{
		if(m_Timer > 500.f) // 0.5 second
		{
			Destroy();
			return;
		}
	}

	if( m_Timer > 1000*60*5 ) // 5 minutes
	{
		Destroy();
		return;
	}

	if( m_Pos->y < 0 ) // Out of this world!
	{
		Destroy();
		return;
	}

	if(!m_bCollected)
		HandlePhysics( a_Dt );

	if( !m_bReplicated || m_bDirtyPosition )
	{
		MoveToCorrectChunk();
		m_bReplicated = true;
		m_bDirtyPosition = false;
		cChunk* Chunk = GetWorld()->GetChunkUnreliable( m_ChunkX, m_ChunkY, m_ChunkZ );
		if( Chunk )
		{
			cPacket_TeleportEntity TeleportEntity( this );
			Chunk->Broadcast( TeleportEntity );
		}
	}

	//printf("YSpeed: %f, OnGround: %i\n", m_SpeedY, m_bOnGround );
}

void cPickup::HandlePhysics(float a_Dt)
{
	m_ResultingSpeed->Set(0.f, 0.f, 0.f);
	cWorld* World = GetWorld();

	if( m_bOnGround ) // check if it's still on the ground
	{
		int BlockX = (m_Pos->x)<0 ? (int)m_Pos->x-1 : (int)m_Pos->x;
		int BlockZ = (m_Pos->z)<0 ? (int)m_Pos->z-1 : (int)m_Pos->z;
		char BlockBelow = World->GetBlock( BlockX, (int)m_Pos->y -1, BlockZ );
		//Not only air, falls through water ;)
		if( BlockBelow == E_BLOCK_AIR || IsBlockWater(BlockBelow))
		{
			m_bOnGround = false;
		}
		char Block = World->GetBlock( BlockX, (int)m_Pos->y - (int)m_bOnGround, BlockZ );
		char BlockIn = World->GetBlock( BlockX, (int)m_Pos->y, BlockZ );

		if( IsBlockLava(Block) || Block == E_BLOCK_FIRE
			|| IsBlockLava(BlockIn) || BlockIn == E_BLOCK_FIRE)
		{
			m_bCollected = true;
			m_Timer = 0;
			return;
		}

		if( BlockIn != E_BLOCK_AIR && !IsBlockWater(BlockIn) ) // If in ground itself, push it out
		{
			m_bOnGround = true;
			m_Pos->y += 0.2;
			m_bReplicated = false;
		}
		m_Speed->x *= 0.7f/(1+a_Dt);
		if( fabs(m_Speed->x) < 0.05 ) m_Speed->x = 0;
		m_Speed->z *= 0.7f/(1+a_Dt);
		if( fabs(m_Speed->z) < 0.05 ) m_Speed->z = 0;
	}


	//get flowing direction
	Direction WaterDir = World->GetWaterSimulator()->GetFlowingDirection((int) m_Pos->x - 1, (int) m_Pos->y, (int) m_Pos->z - 1);


	*m_WaterSpeed *= 0.9f;		//Keep old speed but lower it

	switch(WaterDir)
	{
		case X_PLUS:
			m_WaterSpeed->x = 1.f;
			m_bOnGround = false;
			break;
		case X_MINUS:
			m_WaterSpeed->x = -1.f;
			m_bOnGround = false;
			break;
		case Z_PLUS:
			m_WaterSpeed->z = 1.f;
			m_bOnGround = false;
			break;
		case Z_MINUS:
			m_WaterSpeed->z = -1.f;
			m_bOnGround = false;
			break;
		
	default:
		break;
	}

	*m_ResultingSpeed += *m_WaterSpeed;
	

	if( !m_bOnGround )
	{

		float Gravity = -9.81f*a_Dt;
		m_Speed->y += Gravity;

		// Set to hit position
		*m_ResultingSpeed += *m_Speed;

		cTracer Tracer( GetWorld() );
		int Ret = Tracer.Trace( *m_Pos, *m_Speed, 2 );
		if( Ret ) // Oh noez! we hit something
		{
			

			if( (*Tracer.RealHit - Vector3f(*m_Pos)).SqrLength() <= ( *m_ResultingSpeed * a_Dt ).SqrLength() )
			{
				m_bReplicated = false; // It's only interesting to replicate when we actually hit something...
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
				*m_Pos += *Tracer.HitNormal * 0.2f;

			}
			else
				*m_Pos += *m_ResultingSpeed*a_Dt;
		}
		else
		{	// We didn't hit anything, so move =]
			*m_Pos += *m_ResultingSpeed * a_Dt;
		}
	}


	//Usable for debugging
	//SetPosition(m_Pos->x, m_Pos->y, m_Pos->z);
	

}

bool cPickup::CollectedBy( cPlayer* a_Dest )
{
	if(m_bCollected) return false; // It's already collected!
	// 800 is to long
	if(m_Timer < 500.f) return false; // Not old enough

	if( cRoot::Get()->GetPluginManager()->CallHook( cPluginManager::E_PLUGIN_COLLECT_ITEM, 2, this, a_Dest ) ) return false;

	if( a_Dest->GetInventory().AddItem( *m_Item ) )
	{
		cPacket_CollectItem CollectItem;
		CollectItem.m_CollectedID = m_UniqueID;
		CollectItem.m_CollectorID = a_Dest->GetUniqueID();
		cRoot::Get()->GetServer()->Broadcast( CollectItem );

		m_bCollected = true;
		m_Timer = 0;
		return true;
	}

	return false;
}
