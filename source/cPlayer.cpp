#include "cPlayer.h"
#include "cServer.h"
#include "cInventory.h"
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

#include "Vector3d.h"
#include "Vector3f.h"

#include "../iniFile/iniFile.h"

#ifndef _WIN32 // for mkdir
#include <sys/stat.h>
#include <sys/types.h>
#define sprintf_s(dst, size, format, ...) sprintf(dst, format, __VA_ARGS__ )
#endif

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
};

cPlayer::cPlayer(cClientHandle* a_Client, const char* a_PlayerName)
	: m_bBurnable(true)
	, e_EPMetaState(NORMAL)
	, m_bVisible( true )
	, m_LastGroundHeight( 0 )
	, m_bTouchGround( false )
	, m_Stance( 0.0 )
	, m_Inventory( 0 )
	, m_CurrentWindow( 0 )
	, m_TimeLastPickupCheck( 0.f )
	, m_Color('-')
	, m_FireDamageInterval(0)
	, m_BurnPeriod(0)
	, m_ClientHandle( a_Client )
	, m_pState( new sPlayerState )
{
	m_EntityType = E_PLAYER;
	m_Inventory = new cInventory( this );

	m_TimeLastTeleportPacket = cWorld::GetTime();
	m_TimeLastPickupCheck = cWorld::GetTime();

	m_pState->PlayerName = a_PlayerName;
	m_bDirtyPosition = true; // So chunks are streamed to player at spawn

	if( !LoadFromDisk() )
	{
		m_Inventory->Clear();
		SetPosX( cRoot::Get()->GetWorld()->GetSpawnX() ); // TODO - Get from the correct world?
		SetPosY( cRoot::Get()->GetWorld()->GetSpawnY() );
		SetPosZ( cRoot::Get()->GetWorld()->GetSpawnZ() );
	}

	//MoveToCorrectChunk();
	cRoot::Get()->GetWorld()->AddPlayer( this ); // TODO - Add to correct world? Or get rid of this?
}

cPlayer::~cPlayer(void)
{
	SaveToDisk();
	m_ClientHandle = 0;
	CloseWindow();
	if( m_Inventory )
	{
		delete m_Inventory;
		m_Inventory = 0;
	}
	delete m_pState;
	cRoot::Get()->GetWorld()->RemovePlayer( this ); // TODO - Remove from correct world? Or get rid of this?
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
	if(m_bDirtyOrientation && !m_bDirtyPosition)
	{
		cPacket_EntityLook EntityLook( this );
		InChunk->Broadcast( EntityLook, m_ClientHandle );
		m_bDirtyOrientation = false;
	}
	if(m_bDirtyPosition )
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
	CheckMetaDataBurn();
	if(e_EPMetaState == BURNING){
		InStateBurning(a_Dt);
	}
}

void cPlayer::InStateBurning(float a_Dt) {
	m_FireDamageInterval += a_Dt;
	char block = GetWorld()->GetBlock( (int)m_Pos->x, (int)m_Pos->y, (int)m_Pos->z );
	char bblock = GetWorld()->GetBlock( (int)m_Pos->x, (int)m_Pos->y -1, (int)m_Pos->z );
	if(m_FireDamageInterval > 1000) {
		
		m_FireDamageInterval = 0;
		int rem = rand()%3 + 1; //Burn most of the time
		if(rem >= 2) {
			//printf("OUCH burning!!!\n");
			TakeDamage(1, this);
		}
		m_BurnPeriod++;
		if(block == E_BLOCK_LAVA || block == E_BLOCK_STATIONARY_LAVA || block == E_BLOCK_FIRE
			|| bblock == E_BLOCK_LAVA || bblock == E_BLOCK_STATIONARY_LAVA || bblock == E_BLOCK_FIRE)
			m_BurnPeriod = 0;
		
		if(m_BurnPeriod > 5) {
			
			cChunk* InChunk = GetWorld()->GetChunkUnreliable( m_ChunkX, m_ChunkY, m_ChunkZ );
			e_EPMetaState = NORMAL;
			cPacket_Metadata md(NORMAL, GetUniqueID());
			//md.m_UniqueID = GetUniqueID();
			InChunk->Broadcast(md);
			m_BurnPeriod = 0;
			
		} 

	}
	
}

//----Change Entity MetaData
void cPlayer::CheckMetaDataBurn() {
	char block = GetWorld()->GetBlock( (int)m_Pos->x, (int)m_Pos->y, (int)m_Pos->z );
	char bblock = GetWorld()->GetBlock( (int)m_Pos->x, (int)m_Pos->y -1, (int)m_Pos->z );
	if(m_bBurnable && e_EPMetaState != BURNING && (block == E_BLOCK_LAVA ||  block == E_BLOCK_STATIONARY_LAVA || block == E_BLOCK_FIRE
				 || bblock == E_BLOCK_LAVA ||  bblock == E_BLOCK_STATIONARY_LAVA || bblock == E_BLOCK_FIRE)) {
		cChunk* InChunk = GetWorld()->GetChunkUnreliable( m_ChunkX, m_ChunkY, m_ChunkZ );
		if(!InChunk)
			return;
		printf("I should burn");
		e_EPMetaState = BURNING;
		cPacket_Metadata md(BURNING,GetUniqueID());
		InChunk->Broadcast(md);
	}
}

void cPlayer::SetTouchGround( bool a_bTouchGround )
{
	m_bTouchGround = a_bTouchGround;

	if( !m_bTouchGround )
	{
		cWorld* World = GetWorld();
		char BlockID = World->GetBlock( (int)m_Pos->x, (int)m_Pos->y, (int)m_Pos->z );
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
	if( m_Health < 20 )
	{
		m_Health += (short)a_Health;
		if( m_Health > 20 )
		{
			m_Health = 20;
		}

		cPacket_UpdateHealth Health;
		Health.m_Health = m_Health;
		m_ClientHandle->Send( Health );
	}
}

void cPlayer::TakeDamage( int a_Damage, cEntity* a_Instigator )
{
	if ( !(GetWorld()->GetGameMode() == 1) ) {
		cPawn::TakeDamage( a_Damage, a_Instigator );

		cPacket_UpdateHealth Health;
		Health.m_Health = m_Health;
		m_ClientHandle->Send( Health );
	}
}

void cPlayer::KilledBy( cEntity* a_Killer )
{
	cPawn::KilledBy( a_Killer );

	if( m_Health > 0 ) return; //  not dead yet =]

	m_bVisible = false; // So new clients don't see the player

	// Puke out all the items
	cItem* Items = m_Inventory->GetSlots();
	for( unsigned int i = 1; i < m_Inventory->c_NumSlots; ++i )
	{
		if( !Items[i].IsEmpty() )
		{
			float SpeedX = ((rand()%1000)-500)	/100.f;
			float SpeedY = ((rand()%1000))		/100.f;
			float SpeedZ = ((rand()%1000)-500)	/100.f;
			cPickup* Pickup = new cPickup( (int)(m_Pos->x*32), (int)(m_Pos->y*32), (int)(m_Pos->z*32), Items[i], SpeedX, SpeedY, SpeedZ );
			Pickup->Initialize( GetWorld() );
		}
		Items[i].Empty();
	}
	SaveToDisk(); // Save it, yeah the world is a tough place !
}

void cPlayer::Respawn()
{
	m_Health = 20;

	// Create Respawn player packet
	cPacket_Respawn Packet;
	//Set Gamemode for packet by looking at world's gamemode (Need to check players gamemode.)
	Packet.m_CreativeMode = (char)GetWorld()->GetGameMode();
	//Send Packet
	m_ClientHandle->Send( Packet );
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
	CloseWindow();
	a_Window->Open( *this );
	m_CurrentWindow = a_Window;
}

void cPlayer::CloseWindow()
{
	if( m_CurrentWindow ) m_CurrentWindow->Close( *this );
	m_CurrentWindow = 0;
}

#ifdef SendMessage	// Cause stupid windows.h defines SendMessage as SendMessageA
#undef SendMessage
#endif
void cPlayer::SendMessage( const char* a_Message )
{
	m_ClientHandle->Send( cPacket_Chat( a_Message ) );
}

void cPlayer::TeleportTo( cEntity* a_Entity )
{
	cPawn::TeleportTo( a_Entity );
	cPacket_PlayerPosition PlayerPosition( this );
	m_ClientHandle->Send( PlayerPosition );
}

void cPlayer::TeleportTo( const double & a_PosX, const double & a_PosY, const double & a_PosZ )
{
	cPawn::TeleportTo( a_PosX, a_PosY, a_PosZ );
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

bool cPlayer::LoadFromDisk()
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
	sprintf_s(SourceFile, 128, "world/player/%s.bin", m_pState->PlayerName.c_str() );

	FILE* f;
	#ifdef _WIN32
	if( fopen_s(&f, SourceFile, "rb" ) == 0 )	// no error
	#else
	if( (f = fopen(SourceFile, "rb" ) ) != 0 )	// no error
	#endif
	{
		if( fread( &m_Pos->x, sizeof(double), 1, f) != 1 ) { LOGERROR("ERROR READING FROM FILE %s", SourceFile); fclose(f); return false; }
		if( fread( &m_Pos->y, sizeof(double), 1, f) != 1 ) { LOGERROR("ERROR READING FROM FILE %s", SourceFile); fclose(f); return false; }
		if( fread( &m_Pos->z, sizeof(double), 1, f) != 1 ) { LOGERROR("ERROR READING FROM FILE %s", SourceFile); fclose(f); return false; }
		if( fread( &m_Rot->x, sizeof(float), 1, f) != 1 ) { LOGERROR("ERROR READING FROM FILE %s", SourceFile); fclose(f); return false; }
		if( fread( &m_Rot->y, sizeof(float), 1, f) != 1 ) { LOGERROR("ERROR READING FROM FILE %s", SourceFile); fclose(f); return false; }
		if( fread( &m_Rot->z, sizeof(float), 1, f) != 1 ) { LOGERROR("ERROR READING FROM FILE %s", SourceFile); fclose(f); return false; }
		if( fread( &m_Health, sizeof(m_Health), 1, f) != 1 ) { LOGERROR("ERROR READING FROM FILE %s", SourceFile); fclose(f); return false; }
		if( !m_Inventory->LoadFromFile( f ) ) { LOGERROR("ERROR READING INVENTORY FROM FILE %s", SourceFile); fclose(f); return false; }
		fclose(f);
		return true;
	}
	return false;
}

bool cPlayer::SaveToDisk()
{
    #ifdef _WIN32
	{	// Make sure some folders exist
		SECURITY_ATTRIBUTES Attrib;
		Attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
		Attrib.lpSecurityDescriptor = NULL;
		Attrib.bInheritHandle = false;
		::CreateDirectory("world", &Attrib);
		::CreateDirectory("world/player", &Attrib);
	}
	#else
	{
        mkdir("world", S_IRWXU | S_IRWXG | S_IRWXO);
        mkdir("world/player", S_IRWXU | S_IRWXG | S_IRWXO);
	}
	#endif

	char SourceFile[128];
	sprintf_s(SourceFile, 128, "world/player/%s.bin", m_pState->PlayerName.c_str() );

	FILE* f;
	#ifdef _WIN32
	if( fopen_s(&f, SourceFile, "wb" ) == 0 )	// no error
	#else
	if( (f = fopen(SourceFile, "wb" ) ) != 0 )	// no error
	#endif
	{
		fwrite( &m_Pos->x, sizeof(double), 1, f );
		fwrite( &m_Pos->y, sizeof(double), 1, f );
		fwrite( &m_Pos->z, sizeof(double), 1, f );
		fwrite( &m_Rot->x, sizeof(float), 1, f );
		fwrite( &m_Rot->y, sizeof(float), 1, f );
		fwrite( &m_Rot->z, sizeof(float), 1, f );
		fwrite( &m_Health, sizeof(m_Health), 1, f );
		m_Inventory->WriteToFile( f );
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
