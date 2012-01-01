#include "cPlayer.h"
#include "cServer.h"
#include "cCreativeInventory.h"
#include "cSurvivalInventory.h"
#include "cClientHandle.h"
#include "cWorld.h"
#include "cPickup.h"
#include "cPluginManager.h"
#include "cChunk.h"
#include "cMCLogger.h"
#include "cWindow.h"
#include "cBlockEntity.h"
#include "cGroupManager.h"
#include "cGroup.h"
#include "cChatColor.h"
#include "cItem.h"
#include "cTracer.h"
#include "cRoot.h"
#include "cMakeDir.h"
#include "cTimer.h"
#include "MersenneTwister.h"

#include "packets/cPacket_NamedEntitySpawn.h"
#include "packets/cPacket_EntityLook.h"
#include "packets/cPacket_TeleportEntity.h"
#include "packets/cPacket_RelativeEntityMove.h"
#include "packets/cPacket_RelativeEntityMoveLook.h"
#include "packets/cPacket_UpdateHealth.h"
#include "packets/cPacket_Respawn.h"
#include "packets/cPacket_PlayerPosition.h"
#include "packets/cPacket_DestroyEntity.h"
#include "packets/cPacket_Metadata.h"
#include "packets/cPacket_Chat.h"
#include "packets/cPacket_NewInvalidState.h"
#include "packets/cPacket_PlayerListItem.h"
#include "packets/cPacket_BlockAction.h"

#include "Vector3d.h"
#include "Vector3f.h"

#include "../iniFile/iniFile.h"
#include <json/json.h>

#ifndef _WIN32 // for mkdir
#include <sys/stat.h>
#include <sys/types.h>
#define sprintf_s(dst, size, format, ...) sprintf(dst, format, __VA_ARGS__ )
#endif
#define float2int(x) ((x)<0 ? ((int)(x))-1 : (int)(x))
extern std::vector< std::string > StringSplit( std::string str, std::string delim);

CLASS_DEFINITION( cPlayer, cPawn );

typedef std::map< std::string, bool > PermissionMap;
struct cPlayer::sPlayerState
{
	PermissionMap ResolvedPermissions;
	PermissionMap Permissions;

	cPlayer::GroupList ResolvedGroups;
	cPlayer::GroupList Groups;

	std::string PlayerName;
	std::string LoadedWorldName;
};

cPlayer::cPlayer(cClientHandle* a_Client, const char* a_PlayerName)
	: m_GameMode( 0 )
	, m_IP("")
	, m_LastBlockActionTime( 0 )
	, m_LastBlockActionCnt( 0 )
	, m_bVisible( true )
	, m_LastGroundHeight( 0 )
	, m_bTouchGround( false )
	, m_Stance( 0.0 )
	, m_Inventory( 0 )
	, m_CurrentWindow( 0 )
	, m_TimeLastPickupCheck( 0.f )
	, m_Color('-')
	, m_ClientHandle( a_Client )
	, m_pState( new sPlayerState )
{
	m_EntityType = E_PLAYER;
	SetMaxHealth(20);
	SetMaxFoodLevel(125);
	m_Inventory = new cSurvivalInventory( this );
	m_CreativeInventory = new cCreativeInventory(this);
	cTimer t1;
	m_LastPlayerListTime = t1.GetNowTime();

	m_TimeLastTeleportPacket = cWorld::GetTime();
	m_TimeLastPickupCheck = cWorld::GetTime();
	
	m_pState->PlayerName = a_PlayerName;
	m_bDirtyPosition = true; // So chunks are streamed to player at spawn

	if( !LoadFromDisk() )
	{
		m_Inventory->Clear();
		m_CreativeInventory->Clear();
		SetPosX( cRoot::Get()->GetDefaultWorld()->GetSpawnX() );
		SetPosY( cRoot::Get()->GetDefaultWorld()->GetSpawnY() );
		SetPosZ( cRoot::Get()->GetDefaultWorld()->GetSpawnZ() );
	}
}

void cPlayer::Initialize( cWorld* a_World )
{
	cPawn::Initialize( a_World );
	GetWorld()->AddPlayer( this );
}

cPlayer::~cPlayer(void)
{
	SaveToDisk();
	m_ClientHandle = 0;
	
	CloseWindow(-1);
	if( m_Inventory )
	{
		delete m_Inventory;
		m_Inventory = 0;
	}
	if(m_CreativeInventory)
	{
		delete m_CreativeInventory;
	}
	delete m_pState;
	GetWorld()->RemovePlayer( this ); // TODO - Remove from correct world? Or get rid of this?
}



void cPlayer::SpawnOn( cClientHandle* a_Target )
{
	if( a_Target == m_ClientHandle || !m_bVisible ) return;
	LOG("cPlayer::SpawnOn -> Sending %s to %s", m_pState->PlayerName.c_str(), (a_Target)?a_Target->GetUsername():"Everybody" );
	cPacket_NamedEntitySpawn SpawnPacket;
	SpawnPacket.m_UniqueID = m_UniqueID;
	SpawnPacket.m_PlayerName = m_pState->PlayerName;
	SpawnPacket.m_PosX = (int)(m_Pos->x * 32);
	SpawnPacket.m_PosY = (int)(m_Pos->y * 32);
	SpawnPacket.m_PosZ = (int)(m_Pos->z * 32);
	SpawnPacket.m_Rotation = (char)((m_Rot->x/360.f)*256);
	SpawnPacket.m_Pitch    = (char)((m_Rot->y/360.f)*256);
	SpawnPacket.m_CurrentItem = (short)m_Inventory->GetEquippedItem().m_ItemID;
	if( a_Target == 0 )
	{
		cChunk* Chunk = GetWorld()->GetChunk( m_ChunkX, m_ChunkY, m_ChunkZ );
		Chunk->Broadcast( SpawnPacket, m_ClientHandle );
	}
	else
	{
		a_Target->Send( SpawnPacket );
	}
}

void cPlayer::Tick(float a_Dt)
{
	cChunk* InChunk = GetWorld()->GetChunk( m_ChunkX, m_ChunkY, m_ChunkZ );
	if( !InChunk ) return;

	cPawn::Tick(a_Dt);

	if(m_bDirtyOrientation && !m_bDirtyPosition)
	{
		cPacket_EntityLook EntityLook( this );
		InChunk->Broadcast( EntityLook, m_ClientHandle );
		m_bDirtyOrientation = false;
	} else if(m_bDirtyPosition )
	{
		cRoot::Get()->GetPluginManager()->CallHook( cPluginManager::E_PLUGIN_PLAYER_MOVE, 1, this );

		float DiffX = (float)(GetPosX() - m_LastPosX );
		float DiffY = (float)(GetPosY() - m_LastPosY );
		float DiffZ = (float)(GetPosZ() - m_LastPosZ );
		float SqrDist = DiffX*DiffX + DiffY*DiffY + DiffZ*DiffZ;
		if( SqrDist > 4*4 // 4 blocks is max Relative Move
			|| cWorld::GetTime() - m_TimeLastTeleportPacket > 2 )  // Send an absolute position every 2 seconds
		{
			//LOG("Teleported %f", sqrtf(SqrDist) );
			cPacket_TeleportEntity TeleportEntity( this );
			InChunk->Broadcast( TeleportEntity, m_ClientHandle );
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
				InChunk->Broadcast( RelativeEntityMoveLook, m_ClientHandle );
			}
			else
			{
				cPacket_RelativeEntityMove RelativeEntityMove;
				RelativeEntityMove.m_UniqueID = GetUniqueID();
				RelativeEntityMove.m_MoveX = (char)(DiffX*32);
				RelativeEntityMove.m_MoveY = (char)(DiffY*32);
				RelativeEntityMove.m_MoveZ = (char)(DiffZ*32);
				InChunk->Broadcast( RelativeEntityMove, m_ClientHandle );
			}
		}
		m_LastPosX = GetPosX();
		m_LastPosY = GetPosY();
		m_LastPosZ = GetPosZ();
		m_bDirtyPosition = false;
		m_ClientHandle->StreamChunks();
	}

	if( m_Health > 0 ) // make sure player is alive
	{
		if( cWorld::GetTime() - m_TimeLastPickupCheck > 0.5f ) // Each 0.5 second, check for pickups
		{
			m_TimeLastPickupCheck = cWorld::GetTime();
			// and also check if near a pickup
			// TODO: Don't only check in current chunks, but also close chunks (chunks within range)
			cChunk* Chunk = GetWorld()->GetChunk( m_ChunkX, m_ChunkY, m_ChunkZ );
			Chunk->LockEntities();
			cWorld::EntityList Entities = Chunk->GetEntities();
			for( cWorld::EntityList::iterator itr = Entities.begin(); itr != Entities.end();++itr)
			{
				if( (*itr)->GetEntityType() != cEntity::E_PICKUP ) continue; // Only pickups
				float DiffX = (float)((*itr)->GetPosX() - GetPosX() );
				float DiffY = (float)((*itr)->GetPosY() - GetPosY() );
				float DiffZ = (float)((*itr)->GetPosZ() - GetPosZ() );
				float SqrDist = DiffX*DiffX + DiffY*DiffY + DiffZ*DiffZ;
				if(SqrDist < 1.5f*1.5f) // 1.5 block
				{
					cPickup* Pickup = reinterpret_cast<cPickup*>(*itr);
					Pickup->CollectedBy( this );
				}
			}
			Chunk->UnlockEntities();
		}
	}
	
	cTimer t1;
	// Send Player List (Once per m_LastPlayerListTime/1000 ms)
	if (m_LastPlayerListTime + cPlayer::PLAYER_LIST_TIME_MS <= t1.GetNowTime()) {
		cWorld::PlayerList PlayerList = cRoot::Get()->GetWorld()->GetAllPlayers();
		for( cWorld::PlayerList::iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr )
		{
			if ((*itr) && (*itr)->GetClientHandle() && !((*itr)->GetClientHandle()->IsDestroyed())) {
				cPacket_PlayerListItem PlayerListItem(GetColor() + GetName(), true, GetClientHandle()->GetPing());
				(*itr)->GetClientHandle()->Send( PlayerListItem );
			}
		}
		m_LastPlayerListTime = t1.GetNowTime();
	}

}

void cPlayer::SetTouchGround( bool a_bTouchGround )
{
	m_bTouchGround = a_bTouchGround;

	if( !m_bTouchGround )
	{
		cWorld* World = GetWorld();
		char BlockID = World->GetBlock( float2int(m_Pos->x), float2int(m_Pos->y), float2int(m_Pos->z) );
		if( BlockID != E_BLOCK_AIR )
		{
			m_bTouchGround = true;
		}
		if( BlockID == E_BLOCK_WATER || BlockID == E_BLOCK_STATIONARY_WATER || BlockID == E_BLOCK_LADDER || BlockID == E_BLOCK_TORCH )
		{
			m_LastGroundHeight = (float)m_Pos->y;
		}
	}

	if( m_bTouchGround )
	{
		float Dist = (float)(m_LastGroundHeight - m_Pos->y);
		if( Dist > 4.f ) // Player dropped
		{
			int Damage = (int)(Dist - 4.f);
			if( Damage > 0 )
			{
				TakeDamage( Damage, 0 );
			}
		}

		m_LastGroundHeight = (float)m_Pos->y;
	}
}

void cPlayer::Heal( int a_Health )
{
	if( m_Health < GetMaxHealth() )
	{
		m_Health = (short) MIN(a_Health + m_Health, GetMaxHealth());

		cPacket_UpdateHealth Health;
		Health.m_Health = m_Health;
		Health.m_Food = GetFood();
		Health.m_Saturation = GetFoodSaturation();
		m_ClientHandle->Send( Health );
	}
}

void cPlayer::Feed( short a_Food )
{
	if( m_FoodLevel < GetMaxFoodLevel() )
	{
		m_FoodLevel = MIN(a_Food + m_FoodLevel, GetMaxFoodLevel());

		cPacket_UpdateHealth Health;
		Health.m_Health = m_Health;
		Health.m_Food = GetFood();
		Health.m_Saturation = GetFoodSaturation();
		m_ClientHandle->Send( Health );
	}
}

void cPlayer::TakeDamage( int a_Damage, cEntity* a_Instigator )
{
	if ( !(m_GameMode == 1) ) {
		cPawn::TakeDamage( a_Damage, a_Instigator );

		cPacket_UpdateHealth Health;
		Health.m_Health = m_Health;
		Health.m_Food = GetFood();
		Health.m_Saturation = GetFoodSaturation();
		//TODO: Causes problems sometimes O.o (E.G. Disconnecting when attacked)
		if(m_ClientHandle != 0)
			m_ClientHandle->Send( Health );
	}
}

void cPlayer::KilledBy( cEntity* a_Killer )
{
	cPawn::KilledBy( a_Killer );

	if( m_Health > 0 ) return; //  not dead yet =]

	m_bVisible = false; // So new clients don't see the player

	MTRand r1;
	// Puke out all the items
	cItem* Items = m_Inventory->GetSlots();
	for( unsigned int i = 1; i < m_Inventory->c_NumSlots; ++i )
	{
		if( !Items[i].IsEmpty() )
		{
			float SpeedX = ((r1.randInt()%1000)-500)	/100.f;
			float SpeedY = ((r1.randInt()%1000))		/100.f;
			float SpeedZ = ((r1.randInt()%1000)-500)	/100.f;
			cPickup* Pickup = new cPickup( (int)(m_Pos->x*32), (int)(m_Pos->y*32), (int)(m_Pos->z*32), Items[i], SpeedX, SpeedY, SpeedZ );
			Pickup->Initialize( GetWorld() );
		}
		Items[i].Empty();
	}
	SaveToDisk(); // Save it, yeah the world is a tough place !
}

void cPlayer::Respawn()
{
	m_Health = GetMaxHealth();

	// Create Respawn player packet
	cPacket_Respawn Packet;
	//Set Gamemode for packet by looking at world's gamemode (Need to check players gamemode.)
	//Packet.m_CreativeMode = (char)GetWorld()->GetGameMode();
	Packet.m_CreativeMode = (char)m_GameMode; //Set GameMode packet based on Player's GameMode;

	//TODO Less hardcoded
	Packet.m_World = 0;

	Packet.m_MapSeed = GetWorld()->GetWorldSeed();

	//Send Packet
	m_ClientHandle->Send( Packet );
	
	//Set non Burning
	SetMetaData(NORMAL);

	TeleportTo( GetWorld()->GetSpawnX(), GetWorld()->GetSpawnY(), GetWorld()->GetSpawnZ() );

	SetVisible( true );
}

double cPlayer::GetEyeHeight()
{
	return m_Stance;
}

Vector3d cPlayer::GetEyePosition()
{
	return Vector3d( m_Pos->x, m_Stance, m_Pos->z );
}

void cPlayer::OpenWindow( cWindow* a_Window )
{
	CloseWindow(m_CurrentWindow ? (char)m_CurrentWindow->GetWindowType() : 0);
	a_Window->Open( *this );
	m_CurrentWindow = a_Window;
}

void cPlayer::CloseWindow(char a_WindowType)
{
	if (a_WindowType == 0) { // Inventory
		if(m_Inventory->GetWindow()->GetDraggingItem() && m_Inventory->GetWindow()->GetDraggingItem()->m_ItemCount > 0)
		{
			LOG("Player holds item! Dropping it...");
			TossItem( true, m_Inventory->GetWindow()->GetDraggingItem()->m_ItemCount );
		}

		//Drop whats in the crafting slots (1, 2, 3, 4)
		for( int i = 1; i <= 4; i++ )
		{
			cItem* Item = m_Inventory->GetSlot( i );
			if( Item->m_ItemID > 0 && Item->m_ItemCount > 0 )
			{
				float vX = 0, vY = 0, vZ = 0;
				EulerToVector( -GetRotation(), GetPitch(), vZ, vX, vY );
				vY = -vY*2 + 1.f;
				cPickup* Pickup = new cPickup( (int)(GetPosX()*32), (int)(GetPosY()*32) + (int)(1.6f*32), (int)(GetPosZ()*32), *Item, vX*2, vY*2, vZ*2 );
				Pickup->Initialize( GetWorld() );
			}
			Item->Empty();
		}
	}
	if (m_CurrentWindow)
	{
		if (a_WindowType == 1 && strcmp(m_CurrentWindow->GetWindowTitle().c_str(), "UberChest") == 0) { // Chest
			cBlockEntity *block = m_CurrentWindow->GetOwner()->GetEntity();
			cPacket_BlockAction ChestClose;
			ChestClose.m_PosX = block->GetPosX();
			ChestClose.m_PosY = (short)block->GetPosY();
			ChestClose.m_PosZ = block->GetPosZ();
			ChestClose.m_Byte1 = 1;
			ChestClose.m_Byte2 = 0;
			cWorld::PlayerList PlayerList = cRoot::Get()->GetWorld()->GetAllPlayers();
			for( cWorld::PlayerList::iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr )
			{
				if ((*itr) && (*itr)->GetClientHandle() && !((*itr)->GetClientHandle()->IsDestroyed())) {
					(*itr)->GetClientHandle()->Send( ChestClose );
				}
			}
		}
		
		m_CurrentWindow->Close( *this );
	}
	m_CurrentWindow = 0;
}

void cPlayer::SetLastBlockActionTime()
{
	m_LastBlockActionTime = cRoot::Get()->GetWorld()->GetTime();
}

void cPlayer::SetLastBlockActionCnt( int a_LastBlockActionCnt )
{
	m_LastBlockActionCnt = a_LastBlockActionCnt;
}

void cPlayer::SetGameMode( int a_GameMode )
{
	if ( (a_GameMode < 2) && (a_GameMode >= 0) )
	{
		if (m_GameMode != a_GameMode)
		{
			cInventory *OldInventory = 0;
			if(m_GameMode == 0)
				OldInventory = m_Inventory;
			else
				OldInventory = m_CreativeInventory;

			m_GameMode = a_GameMode;
			cPacket_NewInvalidState GameModePacket;
			GameModePacket.m_Reason = 3; //GameModeChange
			GameModePacket.m_GameMode = (char)a_GameMode; //GameModeChange
			m_ClientHandle->Send ( GameModePacket );
			GetInventory().SendWholeInventory(m_ClientHandle);
			
			OldInventory->SetEquippedSlot(GetInventory().GetEquippedSlot());
		}
	}
}

void cPlayer::LoginSetGameMode( int a_GameMode )
{
	m_GameMode = a_GameMode;
}

void cPlayer::SetIP( std::string a_IP )
{
        m_IP = a_IP;
}

#ifdef SendMessage	// Cause stupid windows.h defines SendMessage as SendMessageA
#undef SendMessage
#endif
void cPlayer::SendMessage( const char* a_Message )
{
	m_ClientHandle->Send( cPacket_Chat( a_Message ) );
}

void cPlayer::TeleportTo( const double & a_PosX, const double & a_PosY, const double & a_PosZ )
{
	SetPosition( a_PosX, a_PosY, a_PosZ );

	cPacket_TeleportEntity TeleportEntity( this );
	cRoot::Get()->GetServer()->Broadcast( TeleportEntity, GetClientHandle() );
	

	cPacket_PlayerPosition PlayerPosition( this );

	m_ClientHandle->Send( PlayerPosition );
}

void cPlayer::MoveTo( const Vector3d & a_NewPos )
{
	// TODO: should do some checks to see if player is not moving through terrain
	SetPosition( a_NewPos );
}

void cPlayer::SetVisible( bool a_bVisible )
{
	if( a_bVisible == true && m_bVisible == false ) // Make visible
	{
		m_bVisible = true;
		SpawnOn( 0 ); // Spawn on everybody
	}
	if( a_bVisible == false && m_bVisible == true )
	{
		m_bVisible = false;
		cPacket_DestroyEntity DestroyEntity( this );
		cChunk* Chunk = GetWorld()->GetChunkUnreliable( m_ChunkX, m_ChunkY, m_ChunkZ );
		if( Chunk )
		{
			Chunk->Broadcast( DestroyEntity );	// Destroy on all clients
		}
	}
}

void cPlayer::AddToGroup( const char* a_GroupName )
{
	cGroup* Group = cRoot::Get()->GetGroupManager()->GetGroup( a_GroupName );
	m_pState->Groups.push_back( Group );
	LOG("Added %s to group %s", m_pState->PlayerName.c_str(), a_GroupName );
	ResolveGroups();
	ResolvePermissions();
}

bool cPlayer::CanUseCommand( const char* a_Command )
{
	for( GroupList::iterator itr = m_pState->Groups.begin(); itr != m_pState->Groups.end(); ++itr )
	{
		if( (*itr)->HasCommand( a_Command ) ) return true;
	}
	return false;
}

bool cPlayer::HasPermission( const char* a_Permission )
{
	std::vector< std::string > Split = StringSplit( a_Permission, "." );
	PermissionMap Possibilities = m_pState->ResolvedPermissions;
	// Now search the namespaces
	while( Possibilities.begin() != Possibilities.end() )
	{
		PermissionMap::iterator itr = Possibilities.begin();
		if( itr->second )
		{
			std::vector< std::string > OtherSplit = StringSplit( itr->first, "." );
			if( OtherSplit.size() <= Split.size() )
			{
				unsigned int i;
				for( i = 0; i < OtherSplit.size(); ++i )
				{
					if( OtherSplit[i].compare( Split[i] ) != 0 )
					{
						if( OtherSplit[i].compare("*") == 0 ) return true; // WildCard man!! WildCard!
						break;
					}
				}
				if( i == Split.size() ) return true;
			}
		}
		Possibilities.erase( itr );
	}

	// Nothing that matched :(
	return false;
}

bool cPlayer::IsInGroup( const char* a_Group )
{
	for( GroupList::iterator itr = m_pState->ResolvedGroups.begin(); itr != m_pState->ResolvedGroups.end(); ++itr )
	{
		if( strcmp( a_Group, (*itr)->GetName().c_str() ) == 0 )
			return true;
	}
	return false;
}

void cPlayer::ResolvePermissions()
{
	m_pState->ResolvedPermissions.clear();	// Start with an empty map yo~

	// Copy all player specific permissions into the resolved permissions map
	for( PermissionMap::iterator itr = m_pState->Permissions.begin(); itr != m_pState->Permissions.end(); ++itr )
	{
		m_pState->ResolvedPermissions[ itr->first ] = itr->second;
	}

	for( GroupList::iterator GroupItr = m_pState->ResolvedGroups.begin(); GroupItr != m_pState->ResolvedGroups.end(); ++GroupItr )
	{
		const cGroup::PermissionMap & Permissions = (*GroupItr)->GetPermissions();
		for( cGroup::PermissionMap::const_iterator itr = Permissions.begin(); itr != Permissions.end(); ++itr )
		{
			m_pState->ResolvedPermissions[ itr->first ] = itr->second;
		}
	}
}

void cPlayer::ResolveGroups()
{
	// Clear resolved groups first
	m_pState->ResolvedGroups.clear();

	// Get a complete resolved list of all groups the player is in
	std::map< cGroup*, bool > AllGroups;	// Use a map, because it's faster than iterating through a list to find duplicates
	GroupList ToIterate;
	for( GroupList::iterator GroupItr = m_pState->Groups.begin(); GroupItr != m_pState->Groups.end(); ++GroupItr )
	{
		ToIterate.push_back( *GroupItr );
	}
	while( ToIterate.begin() != ToIterate.end() )
	{
		cGroup* CurrentGroup = *ToIterate.begin();
		if( AllGroups.find( CurrentGroup ) != AllGroups.end() )
		{
			LOGERROR("ERROR: Player %s is in the same group multiple times (%s). FIX IT!", m_pState->PlayerName.c_str(), CurrentGroup->GetName().c_str() );
		}
		else
		{
			AllGroups[ CurrentGroup ] = true;
			m_pState->ResolvedGroups.push_back( CurrentGroup );	// Add group to resolved list
			const cGroup::GroupList & Inherits = CurrentGroup->GetInherits();
			for( cGroup::GroupList::const_iterator itr = Inherits.begin(); itr != Inherits.end(); ++itr )
			{
				if( AllGroups.find( *itr ) != AllGroups.end() )
				{
					LOGERROR("ERROR: Player %s is in the same group multiple times due to inheritance (%s). FIX IT!", m_pState->PlayerName.c_str(), (*itr)->GetName().c_str() );
					continue;
				}
				ToIterate.push_back( *itr );
			}
		}
		ToIterate.erase( ToIterate.begin() );
	}
}

std::string cPlayer::GetColor()
{
	if( m_Color != '-' )
		return cChatColor::MakeColor( m_Color );

	if( m_pState->Groups.size() < 1 )
		return cChatColor::White;

	return (*m_pState->Groups.begin())->GetColor();
}

void cPlayer::TossItem( bool a_bDraggingItem, int a_Amount /* = 1 */ )
{
	if( a_bDraggingItem )
	{
		cItem* Item = GetInventory().GetWindow()->GetDraggingItem();
		if( Item->m_ItemID > 0 && Item->m_ItemCount >= a_Amount )
		{
			float vX = 0, vY = 0, vZ = 0;
			EulerToVector( -GetRotation(), GetPitch(), vZ, vX, vY );
			vY = -vY*2 + 1.f;
			cPickup* Pickup = new cPickup( (int)(GetPosX()*32), (int)(GetPosY()*32) + (int)(1.6f*32), (int)(GetPosZ()*32), cItem( Item->m_ItemID, (char)a_Amount, Item->m_ItemHealth), vX*2, vY*2, vZ*2 );
			Pickup->Initialize( GetWorld() );
			if( Item->m_ItemCount > a_Amount )
				Item->m_ItemCount -= (char)a_Amount;
			else
				Item->Empty();
		}
		return;
	}

	// Else drop equipped item
	cItem DroppedItem = GetInventory().GetEquippedItem();
	if( DroppedItem.m_ItemID > 0 && DroppedItem.m_ItemCount > 0 )
	{
		DroppedItem.m_ItemCount = 1;
		if( GetInventory().RemoveItem( DroppedItem ) )
		{
			DroppedItem.m_ItemCount = 1; // RemoveItem decreases the count, so set it to 1 again
			float vX = 0, vY = 0, vZ = 0;
			EulerToVector( -GetRotation(), GetPitch(), vZ, vX, vY );
			vY = -vY*2 + 1.f;
			cPickup* Pickup = new cPickup( (int)(GetPosX()*32), (int)(GetPosY()*32) + (int)(1.6f*32), (int)(GetPosZ()*32), DroppedItem, vX*2, vY*2, vZ*2 );
			Pickup->Initialize( GetWorld() );
		}
	}
}

bool cPlayer::MoveToWorld( const char* a_WorldName )
{
	cWorld* World = cRoot::Get()->GetWorld( a_WorldName );
	if( World )
	{
		/* Remove all links to the old world */
		GetWorld()->RemovePlayer( this );
		GetClientHandle()->RemoveFromAllChunks();
		cChunk* Chunk = GetWorld()->GetChunkUnreliable( m_ChunkX, m_ChunkY, m_ChunkZ );
		if( Chunk ) 
		{
			Chunk->RemoveEntity( *this );
			Chunk->Broadcast( cPacket_DestroyEntity( this ) ); // Remove player entity from all clients in old world
		}

		/* Add player to all the necessary parts of the new world */
		SetWorld( World );
		GetWorld()->AddPlayer( this );
		MoveToCorrectChunk(true);
		GetClientHandle()->StreamChunks();

		return true;
	}

	return false;
}

bool cPlayer::LoadFromDisk() // TODO - This should also get/set/whatever the correct world for this player
{
	cIniFile IniFile("users.ini");
	if( IniFile.ReadFile() )
	{
		std::string Groups = IniFile.GetValue(m_pState->PlayerName, "Groups", "");
		if( Groups.size() > 0 )
		{
			std::vector< std::string > Split = StringSplit( Groups, "," );
			for( unsigned int i = 0; i < Split.size(); i++ )
			{
				AddToGroup( Split[i].c_str() );
			}
		}
		else
		{
			AddToGroup("Default");
		}

		m_Color = IniFile.GetValue(m_pState->PlayerName, "Color", "-")[0];
	}
	else
	{
		LOGWARN("WARNING: Failed to read ini file users.ini");
		AddToGroup("Default");
	}
	ResolvePermissions();

	// Log player permissions, cause it's what the cool kids do
	LOGINFO("Player %s has permissions:", m_pState->PlayerName.c_str() );
	for( PermissionMap::iterator itr = m_pState->ResolvedPermissions.begin(); itr != m_pState->ResolvedPermissions.end(); ++itr )
	{
		if( itr->second ) LOGINFO("%s", itr->first.c_str() );
	}

	char SourceFile[128];
	sprintf_s(SourceFile, 128, "players/%s.json", m_pState->PlayerName.c_str() );

	FILE* f;
	#ifdef _WIN32
	if( fopen_s(&f, SourceFile, "rb" ) == 0 )	// no error
	#else
	if( (f = fopen(SourceFile, "rb" ) ) != 0 )	// no error
	#endif
	{
		// Get file size
		fseek (f , 0 , SEEK_END);
		long FileSize = ftell (f);
		rewind(f);

		char* buffer = new char[ FileSize ];
		if( fread( buffer, FileSize, 1, f) != 1 ) { LOGERROR("ERROR READING FROM FILE %s", SourceFile); fclose(f); return false; }
		fclose(f);

		Json::Value root;
		Json::Reader reader;
		if( !reader.parse( buffer, root, false ) )
		{
			LOGERROR("ERROR WHILE PARSING JSON FROM FILE %s", SourceFile);
		}
		
		delete [] buffer;

		Json::Value & JSON_PlayerPosition = root["position"];
		if( JSON_PlayerPosition.size() == 3 )
		{
			m_Pos->x = JSON_PlayerPosition[(unsigned int)0].asDouble();
			m_Pos->y = JSON_PlayerPosition[(unsigned int)1].asDouble();
			m_Pos->z = JSON_PlayerPosition[(unsigned int)2].asDouble();
		}

		Json::Value & JSON_PlayerRotation = root["rotation"];
		if( JSON_PlayerRotation.size() == 3 )
		{
			m_Rot->x = (float)JSON_PlayerRotation[(unsigned int)0].asDouble();
			m_Rot->y = (float)JSON_PlayerRotation[(unsigned int)1].asDouble();
			m_Rot->z = (float)JSON_PlayerRotation[(unsigned int)2].asDouble();
		}

		m_Health = (short)root.get("health", 0 ).asInt();
		m_FoodLevel = (short)root.get("food", 0 ).asInt();
		m_Inventory->LoadFromJson(root["inventory"]);
		m_CreativeInventory->LoadFromJson(root["creativeinventory"]);

		m_pState->LoadedWorldName = root.get("world", "world").asString();
		
		return true;
	}
	return false;
}

bool cPlayer::SaveToDisk()
{
	cMakeDir::MakeDir("players");

	// create the JSON data
	Json::Value JSON_PlayerPosition;
	JSON_PlayerPosition.append( Json::Value( m_Pos->x ) );
	JSON_PlayerPosition.append( Json::Value( m_Pos->y ) );
	JSON_PlayerPosition.append( Json::Value( m_Pos->z ) );

	Json::Value JSON_PlayerRotation;
	JSON_PlayerRotation.append( Json::Value( m_Rot->x ) );
	JSON_PlayerRotation.append( Json::Value( m_Rot->y ) );
	JSON_PlayerRotation.append( Json::Value( m_Rot->z ) );

	Json::Value JSON_Inventory;
	m_Inventory->SaveToJson( JSON_Inventory );

	Json::Value JSON_CreativeInventory;
	m_CreativeInventory->SaveToJson( JSON_CreativeInventory );

	Json::Value root;
	root["position"] = JSON_PlayerPosition;
	root["rotation"] = JSON_PlayerRotation;
	root["inventory"] = JSON_Inventory;
	root["creativeinventory"] = JSON_CreativeInventory;
	root["health"] = m_Health;
	root["food"] = m_FoodLevel;
	root["world"] = GetWorld()->GetName();

	Json::StyledWriter writer;
	std::string JsonData = writer.write( root );

	char SourceFile[128];
	sprintf_s(SourceFile, 128, "players/%s.json", m_pState->PlayerName.c_str() );

	FILE* f;
	#ifdef _WIN32
	if( fopen_s(&f, SourceFile, "wb" ) == 0 )	// no error
	#else
	if( (f = fopen(SourceFile, "wb" ) ) != 0 )	// no error
	#endif
	{
		if( fwrite( JsonData.c_str(), JsonData.size(), 1, f ) != 1 ) { LOGERROR("ERROR WRITING PLAYER JSON TO FILE %s", SourceFile ); return false; }
		fclose(f);
		return true;
	}

	LOGERROR("ERROR WRITING PLAYER %s TO FILE %s", m_pState->PlayerName.c_str(), SourceFile);
	return false;
}


const char* cPlayer::GetName()
{
	return m_pState->PlayerName.c_str();
}

void cPlayer::SetName( const char* a_Name )
{
	m_pState->PlayerName = a_Name;
}

const cPlayer::GroupList & cPlayer::GetGroups()
{
	return m_pState->Groups;
}

const char* cPlayer::GetLoadedWorldName()
{
	return m_pState->LoadedWorldName.c_str();
}

void cPlayer::UseEquippedItem()
{
	if(GetGameMode() != 1)		//No damage in creative
		if (GetInventory().GetEquippedItem().DamageItem()) 
		{
			LOG("Player %s Broke ID: %i", GetClientHandle()->GetUsername(), GetInventory().GetEquippedItem().m_ItemID);
			GetInventory().RemoveItem( GetInventory().GetEquippedItem());
		}
}