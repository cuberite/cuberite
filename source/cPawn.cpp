#include "cPawn.h"
#include "cRoot.h"
#include "cServer.h"
#include "cWorld.h"
#include "cPlayer.h"
#include "cChunk.h"
#include "cMCLogger.h"
#include "cPluginManager.h"

#include "packets/cPacket_TeleportEntity.h"
#include "packets/cPacket_EntityStatus.h"

CLASS_DEFINITION( cPawn, cEntity )

cPawn::cPawn()
	: cEntity( 0, 0, 0 )
	, m_LastPosX( 0.0 )
	, m_LastPosY( 0.0 )
	, m_LastPosZ( 0.0 )
	, m_TimeLastTeleportPacket( 0.f )
{
	m_Health = 20;
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
	cChunk* Chunk = GetWorld()->GetChunkUnreliable( m_ChunkX, m_ChunkY, m_ChunkZ );
	if( Chunk )
		Chunk->Broadcast( Status );

	if( m_Health <= 0 )
		KilledBy( TDI.Instigator );
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
	cChunk* Chunk = GetWorld()->GetChunkUnreliable( m_ChunkX, m_ChunkY, m_ChunkZ );
	if( Chunk )
		Chunk->Broadcast( Status );	// Die
}

void cPawn::TeleportTo( cEntity* a_Entity )
{
	TeleportTo( a_Entity->GetPosX(), a_Entity->GetPosY(), a_Entity->GetPosZ() );
}

void cPawn::TeleportTo( const double & a_PosX, const double & a_PosY, const double & a_PosZ )
{
	SetPosition( a_PosX, a_PosY, a_PosZ );
	cPacket_TeleportEntity TeleportEntity( this );
	if( IsA("cPlayer") )
	{
		cPlayer* Player = (cPlayer*)this;
		cRoot::Get()->GetServer()->Broadcast( TeleportEntity, Player->GetClientHandle() );
	}
	else
	{
		cRoot::Get()->GetServer()->Broadcast( TeleportEntity );
	}
}
