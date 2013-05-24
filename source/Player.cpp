
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Player.h"
#include "Server.h"
#include "ClientHandle.h"
#include "UI/Window.h"
#include "UI/WindowOwner.h"
#include "World.h"
#include "Pickup.h"
#include "PluginManager.h"
#include "BlockEntity.h"
#include "GroupManager.h"
#include "Group.h"
#include "ChatColor.h"
#include "Item.h"
#include "Tracer.h"
#include "Root.h"
#include "OSSupport/MakeDir.h"
#include "OSSupport/Timer.h"
#include "MersenneTwister.h"

#include "Vector3d.h"
#include "Vector3f.h"

#include "../iniFile/iniFile.h"
#include <json/json.h>

#define float2int(x) ((x)<0 ? ((int)(x))-1 : (int)(x))





cPlayer::cPlayer(cClientHandle* a_Client, const AString & a_PlayerName)
	: super(etPlayer)
	, m_GameMode(eGameMode_NotSet)
	, m_IP("")
	, m_LastBlockActionTime( 0 )
	, m_LastBlockActionCnt( 0 )
	, m_bVisible( true )
	, m_LastGroundHeight( 0 )
	, m_bTouchGround( false )
	, m_Stance( 0.0 )
	, m_Inventory(*this)
	, m_CurrentWindow(NULL)
	, m_InventoryWindow(NULL)
	, m_TimeLastPickupCheck( 0.f )
	, m_Color('-')
	, m_ClientHandle( a_Client )
	, m_FoodExhaustionLevel(0.f)
	, m_FoodTickTimer(0)
{
	LOGD("Created a player object for \"%s\" @ \"%s\" at %p, ID %d", 
		a_PlayerName.c_str(), a_Client->GetIPString().c_str(),
		this, GetUniqueID()
	);
	
	m_InventoryWindow = new cInventoryWindow(*this);
	m_CurrentWindow = m_InventoryWindow;
	m_InventoryWindow->OpenedByPlayer(*this);

	SetMaxHealth(20);
	
	m_MaxFoodLevel = 20;
	m_MaxFoodSaturationLevel = 20.f;
	
	m_FoodLevel = m_MaxFoodLevel;
	m_FoodSaturationLevel = 5.f;

	cTimer t1;
	m_LastPlayerListTime = t1.GetNowTime();

	m_TimeLastTeleportPacket = 0;
	m_TimeLastPickupCheck = 0;
	
	m_PlayerName = a_PlayerName;
	m_bDirtyPosition = true; // So chunks are streamed to player at spawn

	if (!LoadFromDisk())
	{
		m_Inventory.Clear();
		SetPosX(cRoot::Get()->GetDefaultWorld()->GetSpawnX());
		SetPosY(cRoot::Get()->GetDefaultWorld()->GetSpawnY());
		SetPosZ(cRoot::Get()->GetDefaultWorld()->GetSpawnZ());
		
		LOGD("Player \"%s\" is connecting for the first time, spawning at default world spawn {%.2f, %.2f, %.2f}",
			a_PlayerName.c_str(), GetPosX(), GetPosY(), GetPosZ()
		);
	}
	m_LastJumpHeight = (float)(GetPosY());
	m_LastGroundHeight = (float)(GetPosY());
	m_Stance = GetPosY() + 1.62;
}





cPlayer::~cPlayer(void)
{
	LOG("Deleting cPlayer \"%s\" at %p, ID %d", m_PlayerName.c_str(), this, GetUniqueID());
	
	SaveToDisk();

	m_World->RemovePlayer( this );

	m_ClientHandle = NULL;
	
	delete m_InventoryWindow;
	
	LOGD("Player %p deleted", this);
}





void cPlayer::Initialize( cWorld* a_World )
{
	cPawn::Initialize( a_World );
	GetWorld()->AddPlayer(this);
}





void cPlayer::Destroyed()
{
	CloseWindow(-1);
	m_ClientHandle = NULL;
}





void cPlayer::SpawnOn(cClientHandle & a_Client)
{
	/*
	LOGD("cPlayer::SpawnOn(%s) for \"%s\" at pos {%.2f, %.2f, %.2f}",
		a_Client.GetUsername().c_str(), m_PlayerName.c_str(), m_Pos.x, m_Pos.y, m_Pos.z
	);
	*/

	if (m_bVisible && (m_ClientHandle != (&a_Client)))
	{
		a_Client.SendPlayerSpawn(*this);
		a_Client.SendEntHeadLook(*this);
		a_Client.SendEntityEquipment(*this, 0, m_Inventory.GetEquippedItem() );
		a_Client.SendEntityEquipment(*this, 1, m_Inventory.GetEquippedBoots() );
		a_Client.SendEntityEquipment(*this, 2, m_Inventory.GetEquippedLeggings() );
		a_Client.SendEntityEquipment(*this, 3, m_Inventory.GetEquippedChestplate() );
		a_Client.SendEntityEquipment(*this, 4, m_Inventory.GetEquippedHelmet() );
	}
}





void cPlayer::Tick(float a_Dt, cChunk & a_Chunk)
{
	if (!m_ClientHandle->IsPlaying())
	{
		// We're not yet in the game, ignore everything
		return;
	}
	
	super::Tick(a_Dt, a_Chunk);
	if (m_bDirtyPosition)
	{
		cRoot::Get()->GetPluginManager()->CallHookPlayerMoving(*this);
		BroadcastMovementUpdate(m_ClientHandle);
		m_ClientHandle->StreamChunks();
	}
	else
	{
		BroadcastMovementUpdate(m_ClientHandle);
	}

	if (m_Health > 0) // make sure player is alive
	{
		m_World->CollectPickupsByPlayer(this);

		// Handle Health:
		m_FoodTickTimer++;
		if (m_FoodTickTimer >= 80)
		{
			m_FoodTickTimer = 0;

			if (m_FoodLevel >= 17)
			{
				Heal(1);
			}
			else if (m_FoodLevel == 0)
			{
				super::TakeDamage(dtStarving, NULL, 1, 1, 0);
			}
		}

		// TODO: Increase Exhaustion level http://www.minecraftwiki.net/wiki/Hunger#Exhaustion_level_increase
		if (m_FoodExhaustionLevel >= 4.f)
		{
			m_FoodExhaustionLevel -= 4.f;

			if (m_FoodSaturationLevel >= 1.f)
			{
				m_FoodSaturationLevel--;
			}
			else
			{
				m_FoodLevel = std::max(m_FoodLevel - 1, 0);
			}

			SendHealth();
		}
	}
	
	cTimer t1;
	// Send Player List (Once per m_LastPlayerListTime/1000 ms)
	if (m_LastPlayerListTime + cPlayer::PLAYER_LIST_TIME_MS <= t1.GetNowTime())
	{
		m_World->SendPlayerList(this);
		m_LastPlayerListTime = t1.GetNowTime();
	}
}





void cPlayer::SetTouchGround(bool a_bTouchGround)
{
	m_bTouchGround = a_bTouchGround;

	if (!m_bTouchGround)
	{
		if (GetPosY() > m_LastJumpHeight)
		{
			m_LastJumpHeight = (float)GetPosY();
		}
		cWorld * World = GetWorld();
		if ((GetPosY() >= 0) && (GetPosY() < 256))
		{
			BLOCKTYPE BlockType = World->GetBlock( float2int(GetPosX()), float2int(GetPosY()), float2int(GetPosZ()) );
			if (BlockType != E_BLOCK_AIR)
			{
				// LOGD("TouchGround set to true by server");
				m_bTouchGround = true;
			}
			if (
				(BlockType == E_BLOCK_WATER) ||
				(BlockType == E_BLOCK_STATIONARY_WATER) ||
				(BlockType == E_BLOCK_LADDER) ||
				(BlockType == E_BLOCK_VINES)
			)
			{
				// LOGD("Water / Ladder / Torch");
				m_LastGroundHeight = (float)GetPosY();
			}
		}
	}

	if (m_bTouchGround)
	{
		float Dist = (float)(m_LastGroundHeight - floor(GetPosY()));
		int Damage = (int)(Dist - 3.f);
		if(m_LastJumpHeight > m_LastGroundHeight) Damage++;
		m_LastJumpHeight = (float)GetPosY();
		if (Damage > 0)
		{
			super::TakeDamage(dtFalling, NULL, Damage, Damage, 0);
		}

		m_LastGroundHeight = (float)GetPosY();
	}
}





void cPlayer::Heal(int a_Health)
{
	if (m_Health < GetMaxHealth())
	{
		m_Health = (short)std::min((int)a_Health + m_Health, (int)GetMaxHealth());
		SendHealth();
	}
}





bool cPlayer::Feed(short a_Food, float a_Saturation)
{
	if (m_FoodLevel >= GetMaxFoodLevel())
	{
		return false;
	}
	
	m_FoodLevel = std::min((short)(a_Food + m_FoodLevel), GetMaxFoodLevel());
	m_FoodSaturationLevel = std::min(m_FoodSaturationLevel + a_Saturation, GetMaxFoodSaturationLevel());
	
	SendHealth();
	return true;
}





void cPlayer::SendHealth()
{
	if (m_ClientHandle != NULL)
	{
		m_ClientHandle->SendHealth();
	}
}





void cPlayer::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (m_GameMode == eGameMode_Creative)
	{
		// No damage / health in creative mode
		return;
	}
	
	super::DoTakeDamage(a_TDI);
	
	if (a_TDI.Attacker != NULL)
	{
		// Only increase hunger if being attacked by a mob
		AddFoodExhaustion(0.3f);
	}
	
	SendHealth();
}





void cPlayer::KilledBy(cPawn * a_Killer)
{
	cPawn::KilledBy(a_Killer);

	if (m_Health > 0)
	{
		return; //  not dead yet =]
	}

	m_bVisible = false; // So new clients don't see the player

	// Puke out all the items
	cItems Pickups;
	m_Inventory.CopyToItems(Pickups);
	m_Inventory.Clear();
	m_World->SpawnItemPickups(Pickups, GetPosX(), GetPosY(), GetPosZ(), 10);
	SaveToDisk();  // Save it, yeah the world is a tough place !
}





void cPlayer::Respawn(void)
{
	m_Health = GetMaxHealth();

	m_ClientHandle->SendRespawn();
	
	// Set non Burning
	SetMetaData(NORMAL);

	TeleportTo(GetWorld()->GetSpawnX(), GetWorld()->GetSpawnY(), GetWorld()->GetSpawnZ());

	SetVisible(true);
}





double cPlayer::GetEyeHeight()
{
	return m_Stance;
}

Vector3d cPlayer::GetEyePosition()
{
	return Vector3d( GetPosX(), m_Stance, GetPosZ() );
}





void cPlayer::OpenWindow( cWindow* a_Window )
{
	CloseWindow(m_CurrentWindow ? (char)m_CurrentWindow->GetWindowType() : 0);
	a_Window->OpenedByPlayer(*this);
	m_CurrentWindow = a_Window;
}





void cPlayer::CloseWindow(char a_WindowType)
{
	if (m_CurrentWindow != NULL)
	{
		m_CurrentWindow->ClosedByPlayer(*this);
	}
	m_CurrentWindow = m_InventoryWindow;
}





void cPlayer::SetLastBlockActionTime()
{
	if (m_World != NULL)
	{
		m_LastBlockActionTime = m_World->GetWorldAge() / 20.0f;
	}
}





void cPlayer::SetLastBlockActionCnt( int a_LastBlockActionCnt )
{
	m_LastBlockActionCnt = a_LastBlockActionCnt;
}





void cPlayer::SetGameMode(eGameMode a_GameMode)
{
	if ((a_GameMode >= 3) || (a_GameMode < 0))
	{
		LOGWARNING("%s: Setting invalid gamemode: %d", GetName().c_str(), a_GameMode);
		return;
	}
	
	if (m_GameMode == a_GameMode)
	{
		// Gamemode already set
		return;
	}
	
	m_GameMode = a_GameMode;
	m_ClientHandle->SendGameMode(a_GameMode);
}





void cPlayer::LoginSetGameMode( eGameMode a_GameMode )
{
	m_GameMode = a_GameMode;
}





void cPlayer::SetIP(const AString & a_IP)
{
	m_IP = a_IP;
}





void cPlayer::SendMessage(const AString & a_Message)
{
	m_ClientHandle->SendChat(a_Message);
}





void cPlayer::TeleportTo(double a_PosX, double a_PosY, double a_PosZ)
{
	SetPosition( a_PosX, a_PosY, a_PosZ );

	m_World->BroadcastTeleportEntity(*this, GetClientHandle());
	m_ClientHandle->SendPlayerMoveLook();
}





void cPlayer::MoveTo( const Vector3d & a_NewPos )
{
	if ((a_NewPos.y < -990) && (GetPosY() > -100))
	{
		// When attached to an entity, the client sends position packets with weird coords:
		// Y = -999 and X, Z = attempting to create speed, usually up to 0.03
		// We cannot test m_AttachedTo, because when deattaching, the server thinks the client is already deattached while 
		// the client may still send more of these nonsensical packets.
		if (m_AttachedTo != NULL)
		{
			Vector3d AddSpeed(a_NewPos);
			AddSpeed.y = 0;
			m_AttachedTo->AddSpeed(AddSpeed);
		}
		return;
	}
	
	// TODO: should do some checks to see if player is not moving through terrain
	// TODO: Official server refuses position packets too far away from each other, kicking "hacked" clients; we should, too
	
	SetPosition( a_NewPos );
	SetStance(a_NewPos.y + 1.62);
}





void cPlayer::SetVisible(bool a_bVisible)
{
	if (a_bVisible && !m_bVisible) // Make visible
	{
		m_bVisible = true;
		m_World->BroadcastSpawn(*this);
	}
	if (!a_bVisible && m_bVisible)
	{
		m_bVisible = false;
		m_World->BroadcastDestroyEntity(*this, m_ClientHandle);	// Destroy on all clients
	}
}





void cPlayer::AddToGroup( const AString & a_GroupName )
{
	cGroup* Group = cRoot::Get()->GetGroupManager()->GetGroup( a_GroupName );
	m_Groups.push_back( Group );
	LOGD("Added %s to group %s", m_PlayerName.c_str(), a_GroupName.c_str() );
	ResolveGroups();
	ResolvePermissions();
}





void cPlayer::RemoveFromGroup( const AString & a_GroupName )
{
	bool bRemoved = false;
	for( GroupList::iterator itr = m_Groups.begin(); itr != m_Groups.end(); ++itr )
	{
		if( (*itr)->GetName().compare(a_GroupName ) == 0 )
		{
			m_Groups.erase( itr );
			bRemoved = true;
			break;
		}
	}

	if( bRemoved )
	{
		LOGD("Removed %s from group %s", m_PlayerName.c_str(), a_GroupName.c_str() );
		ResolveGroups();
		ResolvePermissions();
	}
	else
	{
		LOGWARN("Tried to remove %s from group %s but was not in that group", m_PlayerName.c_str(), a_GroupName.c_str() );
	}
}





bool cPlayer::CanUseCommand( const AString & a_Command )
{
	for( GroupList::iterator itr = m_Groups.begin(); itr != m_Groups.end(); ++itr )
	{
		if( (*itr)->HasCommand( a_Command ) ) return true;
	}
	return false;
}





bool cPlayer::HasPermission(const AString & a_Permission)
{
	if (a_Permission.empty())
	{
		// Empty permission request is always granted
		return true;
	}
	
	AStringVector Split = StringSplit( a_Permission, "." );
	PermissionMap Possibilities = m_ResolvedPermissions;
	// Now search the namespaces
	while( Possibilities.begin() != Possibilities.end() )
	{
		PermissionMap::iterator itr = Possibilities.begin();
		if( itr->second )
		{
			AStringVector OtherSplit = StringSplit( itr->first, "." );
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





bool cPlayer::IsInGroup( const AString & a_Group )
{
	for( GroupList::iterator itr = m_ResolvedGroups.begin(); itr != m_ResolvedGroups.end(); ++itr )
	{
		if( a_Group.compare( (*itr)->GetName().c_str() ) == 0 )
			return true;
	}
	return false;
}





void cPlayer::ResolvePermissions()
{
	m_ResolvedPermissions.clear();	// Start with an empty map yo~

	// Copy all player specific permissions into the resolved permissions map
	for( PermissionMap::iterator itr = m_Permissions.begin(); itr != m_Permissions.end(); ++itr )
	{
		m_ResolvedPermissions[ itr->first ] = itr->second;
	}

	for( GroupList::iterator GroupItr = m_ResolvedGroups.begin(); GroupItr != m_ResolvedGroups.end(); ++GroupItr )
	{
		const cGroup::PermissionMap & Permissions = (*GroupItr)->GetPermissions();
		for( cGroup::PermissionMap::const_iterator itr = Permissions.begin(); itr != Permissions.end(); ++itr )
		{
			m_ResolvedPermissions[ itr->first ] = itr->second;
		}
	}
}





void cPlayer::ResolveGroups()
{
	// Clear resolved groups first
	m_ResolvedGroups.clear();

	// Get a complete resolved list of all groups the player is in
	std::map< cGroup*, bool > AllGroups;	// Use a map, because it's faster than iterating through a list to find duplicates
	GroupList ToIterate;
	for( GroupList::iterator GroupItr = m_Groups.begin(); GroupItr != m_Groups.end(); ++GroupItr )
	{
		ToIterate.push_back( *GroupItr );
	}
	while( ToIterate.begin() != ToIterate.end() )
	{
		cGroup* CurrentGroup = *ToIterate.begin();
		if( AllGroups.find( CurrentGroup ) != AllGroups.end() )
		{
			LOGWARNING("ERROR: Player \"%s\" is in the group multiple times (\"%s\"). Please fix your settings in users.ini!",
				m_PlayerName.c_str(), CurrentGroup->GetName().c_str()
			);
		}
		else
		{
			AllGroups[ CurrentGroup ] = true;
			m_ResolvedGroups.push_back( CurrentGroup );	// Add group to resolved list
			const cGroup::GroupList & Inherits = CurrentGroup->GetInherits();
			for( cGroup::GroupList::const_iterator itr = Inherits.begin(); itr != Inherits.end(); ++itr )
			{
				if( AllGroups.find( *itr ) != AllGroups.end() )
				{
					LOGERROR("ERROR: Player %s is in the same group multiple times due to inheritance (%s). FIX IT!", m_PlayerName.c_str(), (*itr)->GetName().c_str() );
					continue;
				}
				ToIterate.push_back( *itr );
			}
		}
		ToIterate.erase( ToIterate.begin() );
	}
}





AString cPlayer::GetColor(void) const
{
	if ( m_Color != '-' )
	{
		return cChatColor::MakeColor( m_Color );
	}

	if ( m_Groups.size() < 1 )
	{
		return cChatColor::White;
	}

	return (*m_Groups.begin())->GetColor();
}





void cPlayer::TossItem(
	bool a_bDraggingItem,
	char a_Amount /* = 1 */,
	short a_CreateType /* = 0 */,
	short a_CreateHealth /* = 0 */ 
)
{
	cItems Drops;
	if (a_CreateType != 0)
	{
		// Just create item without touching the inventory (used in creative mode)
		Drops.push_back(cItem(a_CreateType, a_Amount, a_CreateHealth));
	}
	else
	{
		// Drop an item from the inventory:
		if (a_bDraggingItem)
		{
			cItem & Item = GetDraggingItem();
			if (!Item.IsEmpty())
			{
				char OriginalItemAmount = Item.m_ItemCount;
				Item.m_ItemCount = std::min(OriginalItemAmount, a_Amount);
				Drops.push_back(Item);
				if (OriginalItemAmount > a_Amount)
				{
					Item.m_ItemCount = OriginalItemAmount - (char)a_Amount;
				}
				else
				{
					Item.Empty();
				}
			}
		}
		else
		{
			// Else drop equipped item
			cItem DroppedItem(GetInventory().GetEquippedItem());
			if (!DroppedItem.IsEmpty())
			{
				if (GetInventory().RemoveOneEquippedItem())
				{
					DroppedItem.m_ItemCount = 1; // RemoveItem decreases the count, so set it to 1 again
					Drops.push_back(DroppedItem);
				}
			}
		}
	}
	double vX = 0, vY = 0, vZ = 0;
	EulerToVector(-GetRotation(), GetPitch(), vZ, vX, vY);
	vY = -vY * 2 + 1.f;
	m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY() + 1.6f, GetPosZ(), vX * 2, vY * 2, vZ * 2);
}





bool cPlayer::MoveToWorld( const char* a_WorldName )
{
	cWorld * World = cRoot::Get()->GetWorld( a_WorldName );
	if ( World )
	{
		/* Remove all links to the old world */
		m_World->RemovePlayer( this );
		m_ClientHandle->RemoveFromAllChunks();
		m_World->RemoveEntity(this);

		/* Add player to all the necessary parts of the new world */
		SetWorld( World );
		GetWorld()->AddPlayer(this);
		GetClientHandle()->StreamChunks();

		return true;
	}

	return false;
}





void cPlayer::LoadPermissionsFromDisk()
{
	m_Groups.clear();
	m_Permissions.clear();

	cIniFile IniFile("users.ini");
	if( IniFile.ReadFile() )
	{
		std::string Groups = IniFile.GetValue(m_PlayerName, "Groups", "");
		if( Groups.size() > 0 )
		{
			AStringVector Split = StringSplit( Groups, "," );
			for( unsigned int i = 0; i < Split.size(); i++ )
			{
				AddToGroup( Split[i].c_str() );
			}
		}
		else
		{
			AddToGroup("Default");
		}

		m_Color = IniFile.GetValue(m_PlayerName, "Color", "-")[0];
	}
	else
	{
		LOGWARN("WARNING: Failed to read ini file users.ini");
		AddToGroup("Default");
	}
	ResolvePermissions();
}




bool cPlayer::LoadFromDisk()
{
	LoadPermissionsFromDisk();

	// Log player permissions, cause it's what the cool kids do
	LOGINFO("Player %s has permissions:", m_PlayerName.c_str() );
	for( PermissionMap::iterator itr = m_ResolvedPermissions.begin(); itr != m_ResolvedPermissions.end(); ++itr )
	{
		if( itr->second ) LOGINFO("%s", itr->first.c_str() );
	}

	AString SourceFile;
	Printf(SourceFile, "players/%s.json", m_PlayerName.c_str() );

	cFile f;
	if (!f.Open(SourceFile, cFile::fmRead))
	{
		return false;
	}

	AString buffer;
	if (f.ReadRestOfFile(buffer) != f.GetSize())
	{
		LOGERROR("ERROR READING FROM FILE \"%s\"", SourceFile.c_str()); 
		return false;
	}
	f.Close();

	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(buffer, root, false))
	{
		LOGERROR("ERROR WHILE PARSING JSON FROM FILE %s", SourceFile.c_str());
	}

	Json::Value & JSON_PlayerPosition = root["position"];
	if (JSON_PlayerPosition.size() == 3)
	{
		SetPosX(JSON_PlayerPosition[(unsigned int)0].asDouble());
		SetPosY(JSON_PlayerPosition[(unsigned int)1].asDouble());
		SetPosZ(JSON_PlayerPosition[(unsigned int)2].asDouble());
	}

	Json::Value & JSON_PlayerRotation = root["rotation"];
	if (JSON_PlayerRotation.size() == 3)
	{
		SetRotation ((float)JSON_PlayerRotation[(unsigned int)0].asDouble());
		SetPitch    ((float)JSON_PlayerRotation[(unsigned int)1].asDouble());
		SetRoll     ((float)JSON_PlayerRotation[(unsigned int)2].asDouble());
	}

	m_Health = (short)root.get("health", 0 ).asInt();
	m_FoodLevel = (short)root.get("food", m_MaxFoodLevel ).asInt();
	m_FoodSaturationLevel = (float)root.get("foodSaturation", m_MaxFoodSaturationLevel ).asDouble();

	m_GameMode = (eGameMode) root.get("gamemode", eGameMode_NotSet).asInt();
	
	m_Inventory.LoadFromJson(root["inventory"]);

	m_LoadedWorldName = root.get("world", "world").asString();
	
	LOGD("Player \"%s\" was read from file, spawning at {%.2f, %.2f, %.2f} in world \"%s\"",
		m_PlayerName.c_str(), GetPosX(), GetPosY(), GetPosZ(), m_LoadedWorldName.c_str()
	);
	
	return true;
}





bool cPlayer::SaveToDisk()
{
	cMakeDir::MakeDir("players");

	// create the JSON data
	Json::Value JSON_PlayerPosition;
	JSON_PlayerPosition.append( Json::Value( GetPosX() ) );
	JSON_PlayerPosition.append( Json::Value( GetPosY() ) );
	JSON_PlayerPosition.append( Json::Value( GetPosZ() ) );

	Json::Value JSON_PlayerRotation;
	JSON_PlayerRotation.append( Json::Value( GetRotation() ) );
	JSON_PlayerRotation.append( Json::Value( GetPitch() ) );
	JSON_PlayerRotation.append( Json::Value( GetRoll() ) );

	Json::Value JSON_Inventory;
	m_Inventory.SaveToJson( JSON_Inventory );

	Json::Value root;
	root["position"] = JSON_PlayerPosition;
	root["rotation"] = JSON_PlayerRotation;
	root["inventory"] = JSON_Inventory;
	root["health"] = m_Health;
	root["food"] = m_FoodLevel;
	root["foodSaturation"] = m_FoodSaturationLevel;
	root["world"] = GetWorld()->GetName();

	if (m_GameMode == GetWorld()->GetGameMode())
	{
		root["gamemode"] = (int) eGameMode_NotSet;
	}
	else
	{
		root["gamemode"] = (int) m_GameMode;
	}

	Json::StyledWriter writer;
	std::string JsonData = writer.write( root );

	AString SourceFile;
	Printf(SourceFile, "players/%s.json", m_PlayerName.c_str() );

	cFile f;
	if (!f.Open(SourceFile, cFile::fmWrite))
	{
		LOGERROR("ERROR WRITING PLAYER \"%s\" TO FILE \"%s\" - cannot open file", m_PlayerName.c_str(), SourceFile.c_str());
		return false;
	}
	if (f.Write(JsonData.c_str(), JsonData.size()) != (int)JsonData.size())
	{
		LOGERROR("ERROR WRITING PLAYER JSON TO FILE \"%s\"", SourceFile.c_str()); 
		return false;
	}
	return true;
}





cPlayer::StringList cPlayer::GetResolvedPermissions()
{
	StringList Permissions;

	const PermissionMap& ResolvedPermissions = m_ResolvedPermissions;
	for( PermissionMap::const_iterator itr = ResolvedPermissions.begin(); itr != ResolvedPermissions.end(); ++itr )
	{
		if( itr->second ) Permissions.push_back( itr->first );
	}

	return Permissions;
}





void cPlayer::UseEquippedItem()
{
	if (GetGameMode() == gmCreative)  // No damage in creative
	{
		return;
	}
	
	GetInventory().DamageEquippedItem();
}




