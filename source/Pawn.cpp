
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pawn.h"
#include "Root.h"
#include "Server.h"
#include "World.h"
#include "Player.h"
#include "PluginManager.h"
#include "Vector3d.h"
#include "BlockID.h"
#include "Defines.h"





CLASS_DEFINITION( cPawn, cEntity )





cPawn::cPawn()
	: cEntity( 0, 0, 0 )
	, m_LastPosX( 0.0 )
	, m_LastPosY( 0.0 )
	, m_LastPosZ( 0.0 )
	, m_TimeLastTeleportPacket(0)
	, m_bBurnable(true)
	, m_MetaData(NORMAL)
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





void cPawn::TakeDamage(int a_Damage, cEntity * a_Instigator)
{
	TakeDamageInfo TDI;
	TDI.Damage = a_Damage;
	TDI.Instigator = a_Instigator;
	cRoot::Get()->GetPluginManager()->CallHook( cPluginManager::E_PLUGIN_TAKE_DAMAGE, 2, this, &TDI);

	if (TDI.Damage == 0)
	{
		return;
	}
	if (m_Health <= 0)
	{
		// Can't take damage if already dead
		return;
	}

	m_Health -= (short)TDI.Damage;
	if (m_Health < 0)
	{
		m_Health = 0;
	}

	m_World->BroadcastEntityStatus(*this, ENTITY_STATUS_HURT);

	if (m_Health <= 0)
	{
		KilledBy(TDI.Instigator);
	}
}





void cPawn::KilledBy(cEntity * a_Killer)
{
	m_Health = 0;

	if( cRoot::Get()->GetPluginManager()->CallHook( cPluginManager::E_PLUGIN_KILLED, 2, this, a_Killer ) )
	{
		return; // Give plugins a chance to 'unkill' the pawn.
	}

	m_World->BroadcastEntityStatus(*this, ENTITY_STATUS_DEAD);
}





void cPawn::TeleportToEntity(cEntity * a_Entity)
{
	TeleportTo(a_Entity->GetPosX(), a_Entity->GetPosY(), a_Entity->GetPosZ());
}





void cPawn::TeleportTo( const double & a_PosX, const double & a_PosY, const double & a_PosZ )
{
	SetPosition( a_PosX, a_PosY, a_PosZ );

	GetWorld()->BroadcastTeleportEntity(*this);
}





void cPawn::Tick(float a_Dt)
{
	CheckMetaDataBurn();  // Check to see if pawn should burn based on block they are on
	
	if (GetMetaData() == BURNING)
	{
		InStateBurning(a_Dt);
	}
}






void cPawn::SetMetaData(MetaData a_MetaData)
{
	//Broadcast new status to clients in the chunk
	m_MetaData = a_MetaData;
	m_World->BroadcastMetadata(*this);
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

