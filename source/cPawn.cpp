
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPawn.h"
#include "cRoot.h"
#include "cServer.h"
#include "cWorld.h"
#include "cPlayer.h"
#include "cPluginManager.h"
#include "Vector3d.h"
#include "BlockID.h"

#include "Defines.h"

#include "packets/cPacket_TeleportEntity.h"
#include "packets/cPacket_EntityStatus.h"
#include "packets/cPacket_Metadata.h"





CLASS_DEFINITION( cPawn, cEntity )





cPawn::cPawn()
	: cEntity( 0, 0, 0 )
	, m_LastPosX( 0.0 )
	, m_LastPosY( 0.0 )
	, m_LastPosZ( 0.0 )
	, m_TimeLastTeleportPacket( 0.f )
	, m_bBurnable(true)
	, m_MetaData(NORMAL)
	, m_FireDamageInterval(0.f)
	, m_BurnPeriod(0.f)
{
	SetMaxHealth(20);
}





cPawn::~cPawn()
{

}





void cPawn::Heal( int a_Health )
{
	(void)a_Health;
}





void cPawn::TakeDamage( int a_Damage, cEntity* a_Instigator )
{
	TakeDamageInfo TDI;
	TDI.Damage = a_Damage;
	TDI.Instigator = a_Instigator;
	cRoot::Get()->GetPluginManager()->CallHook( cPluginManager::E_PLUGIN_TAKE_DAMAGE, 2, this, &TDI );

	if( TDI.Damage == 0 ) return;
	if( m_Health <= 0 ) return; // Can't take damage if already dead

	m_Health -= (short)TDI.Damage;
	if( m_Health < 0 ) m_Health = 0;

	cPacket_EntityStatus Status;
	Status.m_UniqueID = GetUniqueID();
	Status.m_Status =  cPacket_EntityStatus::STATUS_TAKEDAMAGE;
	m_World->BroadcastToChunk(m_ChunkX, m_ChunkY, m_ChunkZ, Status);

	if (m_Health <= 0)
	{
		KilledBy( TDI.Instigator );
	}
}





void cPawn::KilledBy( cEntity* a_Killer )
{
	m_Health = 0;

	if( cRoot::Get()->GetPluginManager()->CallHook( cPluginManager::E_PLUGIN_KILLED, 2, this, a_Killer ) )
	{
		return; // Give plugins a chance to 'unkill' the pawn.
	}

	cPacket_EntityStatus Status;
	Status.m_UniqueID = GetUniqueID();
	Status.m_Status = cPacket_EntityStatus::STATUS_DIE;
	m_World->BroadcastToChunk(m_ChunkX, m_ChunkY, m_ChunkZ, Status);
}





void cPawn::TeleportToEntity( cEntity* a_Entity )
{
	TeleportTo( a_Entity->GetPosX(), a_Entity->GetPosY(), a_Entity->GetPosZ() );
}





void cPawn::TeleportTo( const double & a_PosX, const double & a_PosY, const double & a_PosZ )
{
	SetPosition( a_PosX, a_PosY, a_PosZ );

	cPacket_TeleportEntity TeleportEntity( this );

	cRoot::Get()->GetServer()->Broadcast( TeleportEntity );
}





void cPawn::Tick(float a_Dt)
{
	CheckMetaDataBurn(); //Check to see if pawn should burn based on block they are on
	
	if (GetMetaData() == BURNING)
	{
		InStateBurning(a_Dt);
	}
}






void cPawn::SetMetaData(MetaData a_MetaData)
{
	//Broadcast new status to clients in the chunk
	m_MetaData = a_MetaData;
	cPacket_Metadata md(a_MetaData, GetUniqueID());
	m_World->BroadcastToChunk(m_ChunkX, m_ChunkY, m_ChunkZ, md);
}





//----Change Entity MetaData
void cPawn::CheckMetaDataBurn()
{
	char Block = GetWorld()->GetBlock((int) m_Pos.x, (int) m_Pos.y, (int) m_Pos.z);
	char BlockAbove = GetWorld()->GetBlock((int) m_Pos.x, (int) m_Pos.y + 1, (int) m_Pos.z);
	char BlockBelow = GetWorld()->GetBlock((int) m_Pos.x, (int) m_Pos.y - 1, (int) m_Pos.z);
	
	if (
		(GetMetaData() == BURNING) &&
		(IsBlockWater(Block) || IsBlockWater(BlockAbove) || IsBlockWater(BlockBelow))
	)
	{
		SetMetaData(NORMAL);
 	}
 	else if (
 		m_bBurnable && 
 		(GetMetaData() != BURNING) &&
		(
			IsBlockLava(Block) || (Block == E_BLOCK_FIRE) ||
			IsBlockLava(BlockAbove) || (BlockAbove == E_BLOCK_FIRE) ||
			IsBlockLava(BlockBelow) || (BlockBelow == E_BLOCK_FIRE)
		)
	)
	{
		SetMetaData(BURNING);  
	}
}





//What to do if On fire
void cPawn::InStateBurning(float a_Dt)
{
	m_FireDamageInterval += a_Dt;
	char Block = GetWorld()->GetBlock( (int)m_Pos.x, (int)m_Pos.y, (int)m_Pos.z );
	char BlockAbove = GetWorld()->GetBlock( (int)m_Pos.x, (int)m_Pos.y + 1, (int)m_Pos.z );	
	if (m_FireDamageInterval > 800)
	{

		m_FireDamageInterval = 0;
		TakeDamage(1, this);

		m_BurnPeriod++;
		if (IsBlockLava(Block) || Block == E_BLOCK_FIRE
			|| IsBlockLava(BlockAbove) || BlockAbove == E_BLOCK_FIRE)
		{
			m_BurnPeriod = 0;
			TakeDamage(6, this);
		}
		else
		{
			TakeDamage(1, this);
		}
		
		if (m_BurnPeriod > 7)
		{
			SetMetaData(NORMAL);
			m_BurnPeriod = 0;
		}
	}
}





void cPawn::SetMaxHealth(short a_MaxHealth)
{
	this->m_MaxHealth = a_MaxHealth;

	//Reset health
	m_Health = a_MaxHealth;
}

