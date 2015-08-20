#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ClientHandle.h"
#include "Server.h"
#include "World.h"
#include "Entities/Pickup.h"
#include "Bindings/PluginManager.h"
#include "Entities/Player.h"
#include "Inventory.h"
#include "BlockEntities/BeaconEntity.h"
#include "BlockEntities/ChestEntity.h"
#include "BlockEntities/CommandBlockEntity.h"
#include "BlockEntities/SignEntity.h"
#include "UI/Window.h"
#include "UI/AnvilWindow.h"
#include "UI/BeaconWindow.h"
#include "UI/EnchantingWindow.h"
#include "Item.h"
#include "Mobs/Monster.h"
#include "ChatColor.h"
#include "Items/ItemHandler.h"
#include "Blocks/BlockHandler.h"
#include "Blocks/BlockBed.h"
#include "Blocks/ChunkInterface.h"
#include "BlockInServerPluginInterface.h"

#include "Root.h"

#include "Protocol/Authenticator.h"
#include "Protocol/ProtocolRecognizer.h"
#include "CompositeChat.h"
#include "Items/ItemSword.h"

#include "polarssl/md5.h"



/** Maximum number of explosions to send this tick, server will start dropping if exceeded */
#define MAX_EXPLOSIONS_PER_TICK 20

/** Maximum number of block change interactions a player can perform per tick - exceeding this causes a kick */
#define MAX_BLOCK_CHANGE_INTERACTIONS 20

/** The interval for sending pings to clients.
Vanilla sends one ping every 1 second. */
static const std::chrono::milliseconds PING_TIME_MS = std::chrono::milliseconds(1000);






int cClientHandle::s_ClientCount = 0;





////////////////////////////////////////////////////////////////////////////////
// cClientHandle:

cClientHandle::cClientHandle(const AString & a_IPString, int a_ViewDistance) :
	m_CurrentViewDistance(a_ViewDistance),
	m_RequestedViewDistance(a_ViewDistance),
	m_IPString(a_IPString),
	m_Player(nullptr),
	m_HasSentDC(false),
	m_LastStreamedChunkX(0x7fffffff),  // bogus chunk coords to force streaming upon login
	m_LastStreamedChunkZ(0x7fffffff),
	m_TicksSinceLastPacket(0),
	m_Ping(1000),
	m_PingID(1),
	m_BlockDigAnimStage(-1),
	m_BlockDigAnimSpeed(0),
	m_BlockDigAnimX(0),
	m_BlockDigAnimY(256),  // Invalid Y, so that the coords don't get picked up
	m_BlockDigAnimZ(0),
	m_HasStartedDigging(false),
	m_LastDigBlockX(0),
	m_LastDigBlockY(256),  // Invalid Y, so that the coords don't get picked up
	m_LastDigBlockZ(0),
	m_State(csConnected),
	m_ShouldCheckDownloaded(false),
	m_NumExplosionsThisTick(0),
	m_NumBlockChangeInteractionsThisTick(0),
	m_UniqueID(0),
	m_HasSentPlayerChunk(false),
	m_Locale("en_GB"),
	m_LastPlacedSign(0, -1, 0),
	m_ProtocolVersion(0)
{
	m_Protocol = new cProtocolRecognizer(this);
	
	s_ClientCount++;  // Not protected by CS because clients are always constructed from the same thread
	m_UniqueID = s_ClientCount;
	m_PingStartTime = std::chrono::steady_clock::now();

	LOGD("New ClientHandle created at %p", this);
}





cClientHandle::~cClientHandle()
{
	ASSERT(m_State == csDestroyed);  // Has Destroy() been called?
	
	LOGD("Deleting client \"%s\" at %p", GetUsername().c_str(), this);

	{
		cCSLock Lock(m_CSChunkLists);
		m_LoadedChunks.clear();
		m_ChunksToSend.clear();
	}

	if (m_Player != nullptr)
	{
		cWorld * World = m_Player->GetWorld();
		if (World != nullptr)
		{
			if (!m_Username.empty())
			{
				// Send the Offline PlayerList packet:
				World->BroadcastPlayerListRemovePlayer(*m_Player, this);
			}
	
			World->RemovePlayer(m_Player, true);  // Must be called before cPlayer::Destroy() as otherwise cChunk tries to delete the player, and then we do it again
			m_Player->Destroy();
		}
		delete m_Player;
		m_Player = nullptr;
	}

	if (!m_HasSentDC)
	{
		SendDisconnect("Server shut down? Kthnxbai");
	}
	
	delete m_Protocol;
	m_Protocol = nullptr;
	
	LOGD("ClientHandle at %p deleted", this);
}





void cClientHandle::Destroy(void)
{
	{
		cCSLock Lock(m_CSOutgoingData);
		m_Link.reset();
	}
	{
		cCSLock Lock(m_CSDestroyingState);
		if (m_State >= csDestroying)
		{
			// Already called
			return;
		}
		m_State = csDestroying;
	}
	
	// DEBUG:
	LOGD("%s: client %p, \"%s\"", __FUNCTION__, this, m_Username.c_str());
	
	if ((m_Player != nullptr) && (m_Player->GetWorld() != nullptr))
	{
		RemoveFromAllChunks();
		m_Player->GetWorld()->RemoveClientFromChunkSender(this);
	}
	if (m_Player != nullptr)
	{
		m_Player->RemoveClientHandle();
	}
	m_State = csDestroyed;
}





void cClientHandle::GenerateOfflineUUID(void)
{
	m_UUID = GenerateOfflineUUID(m_Username);
}





AString cClientHandle::FormatChatPrefix(bool ShouldAppendChatPrefixes, AString a_ChatPrefixS, AString m_Color1, AString m_Color2)
{
	if (ShouldAppendChatPrefixes)
	{
		return Printf("%s[%s] %s", m_Color1.c_str(), a_ChatPrefixS.c_str(), m_Color2.c_str());
	}
	else
	{
		return Printf("%s", m_Color1.c_str());
	}
}





AString cClientHandle::FormatMessageType(bool ShouldAppendChatPrefixes, eMessageType a_ChatPrefix, const AString &a_AdditionalData)
{
	switch (a_ChatPrefix)
	{
		case mtCustom:      return "";
		case mtFailure:     return FormatChatPrefix(ShouldAppendChatPrefixes, "INFO",  cChatColor::Rose,   cChatColor::White);
		case mtInformation: return FormatChatPrefix(ShouldAppendChatPrefixes, "INFO",  cChatColor::Yellow, cChatColor::White);
		case mtSuccess:     return FormatChatPrefix(ShouldAppendChatPrefixes, "INFO",  cChatColor::Green,  cChatColor::White);
		case mtWarning:     return FormatChatPrefix(ShouldAppendChatPrefixes, "WARN",  cChatColor::Rose,   cChatColor::White);
		case mtFatal:       return FormatChatPrefix(ShouldAppendChatPrefixes, "FATAL", cChatColor::Red,    cChatColor::White);
		case mtDeath:       return FormatChatPrefix(ShouldAppendChatPrefixes, "DEATH", cChatColor::Gray,   cChatColor::White);
		case mtJoin:        return FormatChatPrefix(ShouldAppendChatPrefixes, "JOIN",  cChatColor::Yellow, cChatColor::White);
		case mtLeave:       return FormatChatPrefix(ShouldAppendChatPrefixes, "LEAVE", cChatColor::Yellow, cChatColor::White);
		case mtPrivateMessage:
		{
			if (ShouldAppendChatPrefixes)
			{
				return Printf("%s[MSG: %s] %s%s", cChatColor::LightBlue, a_AdditionalData.c_str(), cChatColor::White, cChatColor::Italic);
			}
			else
			{
				return Printf("%s: %s", a_AdditionalData.c_str(), cChatColor::LightBlue);
			}
		}
	}
	ASSERT(!"Unhandled chat prefix type!");
	return "";
}





AString cClientHandle::GenerateOfflineUUID(const AString & a_Username)
{
	// Online UUIDs are always version 4 (random)
	// We use Version 3 (MD5 hash) UUIDs for the offline UUIDs
	// This guarantees that they will never collide with an online UUID and can be distinguished.
	// Proper format for a version 3 UUID is:
	// xxxxxxxx-xxxx-3xxx-yxxx-xxxxxxxxxxxx where x is any hexadecimal digit and y is one of 8, 9, A, or B
	// Note that we generate a short UUID (without the dashes)
	
	// First make the username lowercase:
	AString lcUsername = StrToLower(a_Username);

	// Generate an md5 checksum, and use it as base for the ID:
	unsigned char MD5[16];
	md5(reinterpret_cast<const unsigned char *>(lcUsername.c_str()), lcUsername.length(), MD5);
	MD5[6] &= 0x0f;  // Need to trim to 4 bits only...
	MD5[8] &= 0x0f;  // ... otherwise %01x overflows into two chars
	return Printf("%02x%02x%02x%02x%02x%02x3%01x%02x8%01x%02x%02x%02x%02x%02x%02x%02x",
		MD5[0],  MD5[1],  MD5[2],  MD5[3],
		MD5[4],  MD5[5],  MD5[6],  MD5[7],
		MD5[8],  MD5[9],  MD5[10], MD5[11],
		MD5[12], MD5[13], MD5[14], MD5[15]
	);
}





bool cClientHandle::IsUUIDOnline(const AString & a_UUID)
{
	// Online UUIDs are always version 4 (random)
	// We use Version 3 (MD5 hash) UUIDs for the offline UUIDs
	// This guarantees that they will never collide with an online UUID and can be distinguished.
	// The version-specifying char is at pos #12 of raw UUID, pos #14 in dashed-UUID.
	switch (a_UUID.size())
	{
		case 32:
		{
			// This is the UUID format without dashes, the version char is at pos #12:
			return (a_UUID[12] == '4');
		}
		case 36:
		{
			// This is the UUID format with dashes, the version char is at pos #14:
			return (a_UUID[14] == '4');
		}
	}
	return false;
}





void cClientHandle::Kick(const AString & a_Reason)
{
	if (m_State >= csAuthenticating)  // Don't log pings
	{
		LOGINFO("Kicking player %s for \"%s\"", m_Username.c_str(), StripColorCodes(a_Reason).c_str());
	}
	SendDisconnect(a_Reason);
}





void cClientHandle::Authenticate(const AString & a_Name, const AString & a_UUID, const Json::Value & a_Properties)
{
	if (m_State != csAuthenticating)
	{
		return;
	}
	
	ASSERT(m_Player == nullptr);

	m_Username = a_Name;
	
	// Only assign UUID and properties if not already pre-assigned (BungeeCord sends those in the Handshake packet):
	if (m_UUID.empty())
	{
		m_UUID = a_UUID;
	}
	if (m_Properties.empty())
	{
		m_Properties = a_Properties;
	}
	
	// Send login success (if the protocol supports it):
	m_Protocol->SendLoginSuccess();

	// Spawn player (only serversided, so data is loaded)
	m_Player = new cPlayer(m_Self, GetUsername());
	m_Self.reset();

	cWorld * World = cRoot::Get()->GetWorld(m_Player->GetLoadedWorldName());
	if (World == nullptr)
	{
		World = cRoot::Get()->GetDefaultWorld();
	}
	
	if (m_Player->GetGameMode() == eGameMode_NotSet)
	{
		m_Player->LoginSetGameMode(World->GetGameMode());
	}

	m_Player->SetIP (m_IPString);

	if (!cRoot::Get()->GetPluginManager()->CallHookPlayerJoined(*m_Player))
	{
		cRoot::Get()->BroadcastChatJoin(Printf("%s has joined the game", GetUsername().c_str()));
		LOGINFO("Player %s has joined the game", m_Username.c_str());
	}
	
	m_ConfirmPosition = m_Player->GetPosition();

	// Return a server login packet
	m_Protocol->SendLogin(*m_Player, *World);

	// Send Weather if raining:
	if ((World->GetWeather() == 1) || (World->GetWeather() == 2))
	{
		m_Protocol->SendWeather(World->GetWeather());
	}

	// Send time:
	m_Protocol->SendTimeUpdate(World->GetWorldAge(), World->GetTimeOfDay(), World->IsDaylightCycleEnabled());

	// Send contents of the inventory window
	m_Protocol->SendWholeInventory(*m_Player->GetWindow());

	// Send health
	m_Player->SendHealth();

	// Send experience
	m_Player->SendExperience();

	// Send player list items
	SendPlayerListAddPlayer(*m_Player);
	cRoot::Get()->BroadcastPlayerListsAddPlayer(*m_Player);
	cRoot::Get()->SendPlayerLists(m_Player);

	m_Player->Initialize(*World);
	m_State = csAuthenticated;

	// Query player team
	m_Player->UpdateTeam();

	// Send scoreboard data
	World->GetScoreBoard().SendTo(*this);

	// Send statistics
	SendStatistics(m_Player->GetStatManager());

	// Delay the first ping until the client "settles down"
	// This should fix #889, "BadCast exception, cannot convert bit to fm" error in client
	m_PingStartTime = std::chrono::steady_clock::now() + std::chrono::seconds(3);  // Send the first KeepAlive packet in 3 seconds

	cRoot::Get()->GetPluginManager()->CallHookPlayerSpawned(*m_Player);
}





bool cClientHandle::StreamNextChunk(void)
{
	if ((m_State < csAuthenticated) || (m_State >= csDestroying))
	{
		return true;
	}
	ASSERT(m_Player != nullptr);

	int ChunkPosX = m_Player->GetChunkX();
	int ChunkPosZ = m_Player->GetChunkZ();
	if ((m_LastStreamedChunkX == ChunkPosX) && (m_LastStreamedChunkZ == ChunkPosZ))
	{
		// All chunks are already loaded. Abort loading.
		return true;
	}

	// Get the look vector and normalize it.
	Vector3d Position = m_Player->GetEyePosition();
	Vector3d LookVector = m_Player->GetLookVector();
	LookVector.Normalize();

	// Lock the list
	cCSLock Lock(m_CSChunkLists);

	// High priority: Load the chunks that are in the view-direction of the player (with a radius of 3)
	for (int Range = 0; Range < m_CurrentViewDistance; Range++)
	{
		Vector3d Vector = Position + LookVector * cChunkDef::Width * Range;

		// Get the chunk from the x / z coords.
		int RangeX, RangeZ = 0;
		cChunkDef::BlockToChunk(FloorC(Vector.x), FloorC(Vector.z), RangeX, RangeZ);

		for (int X = 0; X < 7; X++)
		{
			for (int Z = 0; Z < 7; Z++)
			{
				int ChunkX = RangeX + ((X >= 4) ? (3 - X) : X);
				int ChunkZ = RangeZ + ((Z >= 4) ? (3 - Z) : Z);
				cChunkCoords Coords(ChunkX, ChunkZ);

				// Checks if the chunk is in distance
				if ((Diff(ChunkX, ChunkPosX) > m_CurrentViewDistance) || (Diff(ChunkZ, ChunkPosZ) > m_CurrentViewDistance))
				{
					continue;
				}

				// If the chunk already loading / loaded -> skip
				if (
					(m_ChunksToSend.find(Coords) != m_ChunksToSend.end()) ||
					(std::find(m_LoadedChunks.begin(), m_LoadedChunks.end(), Coords) != m_LoadedChunks.end())
				)
				{
					continue;
				}

				// Unloaded chunk found -> Send it to the client.
				Lock.Unlock();
				StreamChunk(ChunkX, ChunkZ, ((Range <= 2) ? cChunkSender::E_CHUNK_PRIORITY_HIGH : cChunkSender::E_CHUNK_PRIORITY_MEDIUM));
				return false;
			}
		}
	}

	// Low priority: Add all chunks that are in range. (From the center out to the edge)
	for (int d = 0; d <= m_CurrentViewDistance; ++d)  // cycle through (square) distance, from nearest to furthest
	{
		// For each distance add chunks in a hollow square centered around current position:
		cChunkCoordsList CurcleChunks;
		for (int i = -d; i <= d; ++i)
		{
			CurcleChunks.push_back(cChunkCoords(ChunkPosX + d, ChunkPosZ + i));
			CurcleChunks.push_back(cChunkCoords(ChunkPosX - d, ChunkPosZ + i));
		}
		for (int i = -d + 1; i < d; ++i)
		{
			CurcleChunks.push_back(cChunkCoords(ChunkPosX + i, ChunkPosZ + d));
			CurcleChunks.push_back(cChunkCoords(ChunkPosX + i, ChunkPosZ - d));
		}

		// For each the CurcleChunks list and send the first unloaded chunk:
		for (cChunkCoordsList::iterator itr = CurcleChunks.begin(), end = CurcleChunks.end(); itr != end; ++itr)
		{
			cChunkCoords Coords = *itr;

			// If the chunk already loading / loaded -> skip
			if (
				(m_ChunksToSend.find(Coords) != m_ChunksToSend.end()) ||
				(std::find(m_LoadedChunks.begin(), m_LoadedChunks.end(), Coords) != m_LoadedChunks.end())
			)
			{
				continue;
			}

			// Unloaded chunk found -> Send it to the client.
			Lock.Unlock();
			StreamChunk(Coords.m_ChunkX, Coords.m_ChunkZ, cChunkSender::E_CHUNK_PRIORITY_LOW);
			return false;
		}
	}

	// All chunks are loaded -> Sets the last loaded chunk coordinates to current coordinates
	m_LastStreamedChunkX = ChunkPosX;
	m_LastStreamedChunkZ = ChunkPosZ;
	return true;
}





void cClientHandle::UnloadOutOfRangeChunks(void)
{
	int ChunkPosX = FAST_FLOOR_DIV(static_cast<int>(m_Player->GetPosX()), cChunkDef::Width);
	int ChunkPosZ = FAST_FLOOR_DIV(static_cast<int>(m_Player->GetPosZ()), cChunkDef::Width);

	cChunkCoordsList ChunksToRemove;
	{
		cCSLock Lock(m_CSChunkLists);
		for (cChunkCoordsList::iterator itr = m_LoadedChunks.begin(); itr != m_LoadedChunks.end();)
		{
			int DiffX = Diff((*itr).m_ChunkX, ChunkPosX);
			int DiffZ = Diff((*itr).m_ChunkZ, ChunkPosZ);
			if ((DiffX > m_CurrentViewDistance) || (DiffZ > m_CurrentViewDistance))
			{
				ChunksToRemove.push_back(*itr);
				itr = m_LoadedChunks.erase(itr);
			}
			else
			{
				++itr;
			}
		}

		for (auto itr = m_ChunksToSend.begin(); itr != m_ChunksToSend.end();)
		{
			int DiffX = Diff((*itr).m_ChunkX, ChunkPosX);
			int DiffZ = Diff((*itr).m_ChunkZ, ChunkPosZ);
			if ((DiffX > m_CurrentViewDistance) || (DiffZ > m_CurrentViewDistance))
			{
				itr = m_ChunksToSend.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}

	for (cChunkCoordsList::iterator itr = ChunksToRemove.begin(); itr != ChunksToRemove.end(); ++itr)
	{
		m_Player->GetWorld()->RemoveChunkClient(itr->m_ChunkX, itr->m_ChunkZ, this);
		SendUnloadChunk(itr->m_ChunkX, itr->m_ChunkZ);
	}
}





void cClientHandle::StreamChunk(int a_ChunkX, int a_ChunkZ, cChunkSender::eChunkPriority a_Priority)
{
	if (m_State >= csDestroying)
	{
		// Don't stream chunks to clients that are being destroyed
		return;
	}
	
	cWorld * World = m_Player->GetWorld();
	ASSERT(World != nullptr);

	if (World->AddChunkClient(a_ChunkX, a_ChunkZ, this))
	{
		{
			cCSLock Lock(m_CSChunkLists);
			m_LoadedChunks.push_back(cChunkCoords(a_ChunkX, a_ChunkZ));
			m_ChunksToSend.emplace(a_ChunkX, a_ChunkZ);
		}
		World->SendChunkTo(a_ChunkX, a_ChunkZ, a_Priority, this);
	}
}





// Removes the client from all chunks. Used when switching worlds or destroying the player
void cClientHandle::RemoveFromAllChunks()
{
	cWorld * World = m_Player->GetWorld();
	if (World != nullptr)
	{
		World->RemoveClientFromChunks(this);
	}
	
	{
		// Reset all chunk lists:
		cCSLock Lock(m_CSChunkLists);
		m_LoadedChunks.clear();
		m_ChunksToSend.clear();
		m_SentChunks.clear();

		// Also reset the LastStreamedChunk coords to bogus coords,
		// so that all chunks are streamed in subsequent StreamChunks() call (FS #407)
		m_LastStreamedChunkX = 0x7fffffff;
		m_LastStreamedChunkZ = 0x7fffffff;
	}
}





void cClientHandle::HandleNPCTrade(int a_SlotNum)
{
	// TODO
	LOGWARNING("%s: Not implemented yet", __FUNCTION__);
}





void cClientHandle::HandlePing(void)
{
	// Somebody tries to retrieve information about the server
	AString Reply;
	const cServer & Server = *cRoot::Get()->GetServer();

	Printf(Reply, "%s%s%i%s%i",
		Server.GetDescription().c_str(),
		cChatColor::Delimiter,
		Server.GetNumPlayers(),
		cChatColor::Delimiter,
		Server.GetMaxPlayers()
	);
	Kick(Reply);
}





bool cClientHandle::HandleLogin(UInt32 a_ProtocolVersion, const AString & a_Username)
{
	// If the protocol version hasn't been set yet, set it now:
	if (m_ProtocolVersion == 0)
	{
		m_ProtocolVersion = a_ProtocolVersion;
	}

	m_Username = a_Username;

	// Let the plugins know about this event, they may refuse the player:
	if (cRoot::Get()->GetPluginManager()->CallHookLogin(*this, a_ProtocolVersion, a_Username))
	{
		Destroy();
		return false;
	}

	// Schedule for authentication; until then, let the player wait (but do not block)
	m_State = csAuthenticating;
	cRoot::Get()->GetAuthenticator().Authenticate(GetUniqueID(), GetUsername(), m_Protocol->GetAuthServerID());
	return true;
}





void cClientHandle::HandleCreativeInventory(Int16 a_SlotNum, const cItem & a_HeldItem, eClickAction a_ClickAction)
{
	// This is for creative Inventory changes
	if (!m_Player->IsGameModeCreative())
	{
		LOGWARNING("Got a CreativeInventoryAction packet from user \"%s\" while not in creative mode. Ignoring.", m_Username.c_str());
		return;
	}
	if (m_Player->GetWindow()->GetWindowType() != cWindow::wtInventory)
	{
		LOGWARNING("Got a CreativeInventoryAction packet from user \"%s\" while not in the inventory window. Ignoring.", m_Username.c_str());
		return;
	}
	
	m_Player->GetWindow()->Clicked(*m_Player, 0, a_SlotNum, a_ClickAction, a_HeldItem);
}





void cClientHandle::HandleEnchantItem(UInt8 a_WindowID, UInt8 a_Enchantment)
{
	if (a_Enchantment > 2)
	{
		LOGWARNING("%s attempt to crash the server with invalid enchanting selection (%u)!", GetUsername().c_str(), a_Enchantment);
		Kick("Invalid enchanting!");
		return;
	}

	if (
		(m_Player->GetWindow() == nullptr) ||
		(m_Player->GetWindow()->GetWindowID() != a_WindowID) ||
		(m_Player->GetWindow()->GetWindowType() != cWindow::wtEnchantment)
	)
	{
		return;
	}
	
	cEnchantingWindow * Window = reinterpret_cast<cEnchantingWindow *>(m_Player->GetWindow());
	cItem Item = *Window->m_SlotArea->GetSlot(0, *m_Player);  // Make a copy of the item
	short BaseEnchantmentLevel = Window->GetPropertyValue(a_Enchantment);

	if (Item.EnchantByXPLevels(BaseEnchantmentLevel))
	{
		if (m_Player->IsGameModeCreative() || m_Player->DeltaExperience(-m_Player->XpForLevel(BaseEnchantmentLevel)) >= 0)
		{
			Window->m_SlotArea->SetSlot(0, *m_Player, Item);
			Window->SendSlot(*m_Player, Window->m_SlotArea, 0);
			Window->BroadcastWholeWindow();

			Window->SetProperty(0, 0, *m_Player);
			Window->SetProperty(1, 0, *m_Player);
			Window->SetProperty(2, 0, *m_Player);
		}
	}
}





void cClientHandle::HandlePlayerAbilities(bool a_CanFly, bool a_IsFlying, float FlyingSpeed, float WalkingSpeed)
{
	UNUSED(FlyingSpeed);  // Ignore the client values for these
	UNUSED(WalkingSpeed);

	m_Player->SetCanFly(a_CanFly);
	m_Player->SetFlying(a_IsFlying);
}





void cClientHandle::HandlePlayerPos(double a_PosX, double a_PosY, double a_PosZ, double a_Stance, bool a_IsOnGround)
{
	if ((m_Player == nullptr) || (m_State != csPlaying))
	{
		// The client hasn't been spawned yet and sends nonsense, we know better
		return;
	}

	Vector3d NewPosition(a_PosX, a_PosY, a_PosZ);
	Vector3d OldPosition = GetPlayer()->GetPosition();
	auto PreviousIsOnGround = GetPlayer()->IsOnGround();
	
	// If the player has moved too far, "repair" them:
	if ((OldPosition - NewPosition).SqrLength() > 100 * 100)
	{
		LOGD("Too far away (%0.2f), \"repairing\" the client", (OldPosition - NewPosition).Length());
		SendPlayerMoveLook();
		return;
	}

	if (cRoot::Get()->GetPluginManager()->CallHookPlayerMoving(*m_Player, OldPosition, NewPosition))
	{
		SendPlayerMoveLook();
		return;
	}

	// TODO: should do some checks to see if player is not moving through terrain
	// TODO: Official server refuses position packets too far away from each other, kicking "hacked" clients; we should, too
	
	m_Player->SetPosition(NewPosition);
	m_Player->SetStance(a_Stance);
	m_Player->SetTouchGround(a_IsOnGround);
	m_Player->UpdateMovementStats(NewPosition - OldPosition, PreviousIsOnGround);
}





void cClientHandle::HandlePluginMessage(const AString & a_Channel, const AString & a_Message)
{
	if (a_Channel == "REGISTER")
	{
		if (HasPluginChannel(a_Channel))
		{
			SendPluginMessage("UNREGISTER", a_Channel);
			return;  // Can't register again if already taken - kinda defeats the point of plugin messaging!
		}

		RegisterPluginChannels(BreakApartPluginChannels(a_Message));
	}
	else if (a_Channel == "UNREGISTER")
	{
		UnregisterPluginChannels(BreakApartPluginChannels(a_Message));
	}
	else if (!HasPluginChannel(a_Channel))
	{
		// Ignore if client sent something but didn't register the channel first
		LOGD("Player %s sent a plugin message on channel \"%s\", but didn't REGISTER it first", GetUsername().c_str(), a_Channel.c_str());
		SendPluginMessage("UNREGISTER", a_Channel);
		return;
	}

	cPluginManager::Get()->CallHookPluginMessage(*this, a_Channel, a_Message);
}





AStringVector cClientHandle::BreakApartPluginChannels(const AString & a_PluginChannels)
{
	// Break the string on each NUL character.
	// Note that StringSplit() doesn't work on this because NUL is a special char - string terminator
	size_t len = a_PluginChannels.size();
	size_t first = 0;
	AStringVector res;
	for (size_t i = 0; i < len; i++)
	{
		if (a_PluginChannels[i] != 0)
		{
			continue;
		}
		if (i > first)
		{
			res.push_back(a_PluginChannels.substr(first, i - first));
		}
		first = i + 1;
	}  // for i - a_PluginChannels[]
	if (first < len)
	{
		res.push_back(a_PluginChannels.substr(first, len - first));
	}
	return res;
}





void cClientHandle::RegisterPluginChannels(const AStringVector & a_ChannelList)
{
	for (AStringVector::const_iterator itr = a_ChannelList.begin(), end = a_ChannelList.end(); itr != end; ++itr)
	{
		m_PluginChannels.insert(*itr);
	}  // for itr - a_ChannelList[]
}





void cClientHandle::UnregisterPluginChannels(const AStringVector & a_ChannelList)
{
	for (AStringVector::const_iterator itr = a_ChannelList.begin(), end = a_ChannelList.end(); itr != end; ++itr)
	{
		m_PluginChannels.erase(*itr);
	}  // for itr - a_ChannelList[]
}





void cClientHandle::HandleBeaconSelection(int a_PrimaryEffect, int a_SecondaryEffect)
{
	cWindow * Window = m_Player->GetWindow();
	if ((Window == nullptr) || (Window->GetWindowType() != cWindow::wtBeacon))
	{
		return;
	}
	cBeaconWindow * BeaconWindow = reinterpret_cast<cBeaconWindow *>(Window);

	if (Window->GetSlot(*m_Player, 0)->IsEmpty())
	{
		return;
	}

	cEntityEffect::eType PrimaryEffect = cEntityEffect::effNoEffect;
	if ((a_PrimaryEffect >= 0) && (a_PrimaryEffect <= static_cast<int>(cEntityEffect::effSaturation)))
	{
		PrimaryEffect = static_cast<cEntityEffect::eType>(a_PrimaryEffect);
	}
	cEntityEffect::eType SecondaryEffect = cEntityEffect::effNoEffect;
	if ((a_SecondaryEffect >= 0) && (a_SecondaryEffect <= static_cast<int>(cEntityEffect::effSaturation)))
	{
		SecondaryEffect = static_cast<cEntityEffect::eType>(a_SecondaryEffect);
	}

	Window->SetSlot(*m_Player, 0, cItem());
	BeaconWindow->GetBeaconEntity()->SetPrimaryEffect(PrimaryEffect);

	// Valid effect check. Vanilla don't check this, but we do it :)
	if (
		(SecondaryEffect == cEntityEffect::effNoEffect) ||
		(SecondaryEffect == cEntityEffect::effRegeneration) ||
		(SecondaryEffect == BeaconWindow->GetBeaconEntity()->GetPrimaryEffect())
	)
	{
		BeaconWindow->GetBeaconEntity()->SetSecondaryEffect(SecondaryEffect);
	}
	else
	{
		BeaconWindow->GetBeaconEntity()->SetSecondaryEffect(cEntityEffect::effNoEffect);
	}

	m_Player->CloseWindow(true);
}





void cClientHandle::HandleCommandBlockBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_NewCommand)
{
	cWorld * World = m_Player->GetWorld();
	if (World->AreCommandBlocksEnabled())
	{
		World->SetCommandBlockCommand(a_BlockX, a_BlockY, a_BlockZ, a_NewCommand);
		SendChat("Successfully set command block command", mtSuccess);
	}
	else
	{
		SendChat("Command blocks are not enabled on this server", mtFailure);
	}
}





void cClientHandle::HandleCommandBlockEntityChange(UInt32 a_EntityID, const AString & a_NewCommand)
{
	// TODO
	LOGWARNING("%s: Not implemented yet", __FUNCTION__);
}





void cClientHandle::HandleAnvilItemName(const AString & a_ItemName)
{
	if ((m_Player->GetWindow() == nullptr) || (m_Player->GetWindow()->GetWindowType() != cWindow::wtAnvil))
	{
		return;
	}

	if (a_ItemName.length() <= 30)
	{
		reinterpret_cast<cAnvilWindow *>(m_Player->GetWindow())->SetRepairedItemName(a_ItemName, m_Player);
	}
}





void cClientHandle::HandleLeftClick(int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, UInt8 a_Status)
{
	LOGD("HandleLeftClick: {%i, %i, %i}; Face: %i; Stat: %i",
		a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_Status
	);

	m_NumBlockChangeInteractionsThisTick++;

	if (!CheckBlockInteractionsRate())
	{
		Kick("Too many blocks were destroyed per unit time - hacked client?");
		return;
	}

	if ((a_Status == DIG_STATUS_STARTED) || (a_Status == DIG_STATUS_FINISHED))
	{
		if (a_BlockFace == BLOCK_FACE_NONE)
		{
			return;
		}

		/* Check for clickthrough-blocks:
		When the user breaks a fire block, the client send the wrong block location.
		We must find the right block with the face direction. */
		int BlockX = a_BlockX;
		int BlockY = a_BlockY;
		int BlockZ = a_BlockZ;
		AddFaceDirection(BlockX, BlockY, BlockZ, a_BlockFace);

		if ((BlockY < 0) || (BlockY >= cChunkDef::Height))
		{
			return;
		}

		if (cBlockInfo::GetHandler(m_Player->GetWorld()->GetBlock(BlockX, BlockY, BlockZ))->IsClickedThrough())
		{
			a_BlockX = BlockX;
			a_BlockY = BlockY;
			a_BlockZ = BlockZ;
		}

		if (
			((Diff(m_Player->GetPosX(), static_cast<double>(a_BlockX)) > 6) ||
			(Diff(m_Player->GetPosY(), static_cast<double>(a_BlockY)) > 6) ||
			(Diff(m_Player->GetPosZ(), static_cast<double>(a_BlockZ)) > 6))
		)
		{
			m_Player->GetWorld()->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
			return;
		}
	}

	cPluginManager * PlgMgr = cRoot::Get()->GetPluginManager();
	if (PlgMgr->CallHookPlayerLeftClick(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, static_cast<char>(a_Status)))
	{
		// A plugin doesn't agree with the action, replace the block on the client and quit:
		m_Player->GetWorld()->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
		return;
	}

	switch (a_Status)
	{
		case DIG_STATUS_DROP_HELD:  // Drop held item
		{
			if (PlgMgr->CallHookPlayerTossingItem(*m_Player))
			{
				// A plugin doesn't agree with the tossing. The plugin itself is responsible for handling the consequences (possible inventory mismatch)
				return;
			}

			m_Player->TossEquippedItem();
			return;
		}

		case DIG_STATUS_SHOOT_EAT:
		{
			cItemHandler * ItemHandler = cItemHandler::GetItemHandler(m_Player->GetEquippedItem());
			if (ItemHandler->IsFood() || ItemHandler->IsDrinkable(m_Player->GetEquippedItem().m_ItemDamage))
			{
				m_Player->AbortEating();
				return;
			}
			else
			{
				if (PlgMgr->CallHookPlayerShooting(*m_Player))
				{
					// A plugin doesn't agree with the action. The plugin itself is responsible for handling the consequences (possible inventory mismatch)
					return;
				}
				ItemHandler->OnItemShoot(m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
			}
			return;
		}
		
		case DIG_STATUS_STARTED:
		{
			BLOCKTYPE  OldBlock;
			NIBBLETYPE OldMeta;
			m_Player->GetWorld()->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, OldBlock, OldMeta);
			HandleBlockDigStarted(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, OldBlock, OldMeta);
			return;
		}
		
		case DIG_STATUS_FINISHED:
		{
			BLOCKTYPE  OldBlock;
			NIBBLETYPE OldMeta;
			m_Player->GetWorld()->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, OldBlock, OldMeta);
			HandleBlockDigFinished(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, OldBlock, OldMeta);
			return;
		}
		
		case DIG_STATUS_CANCELLED:
		{
			// Block breaking cancelled by player
			FinishDigAnimation();
			return;
		}

		case DIG_STATUS_DROP_STACK:
		{
			if (PlgMgr->CallHookPlayerTossingItem(*m_Player))
			{
				// A plugin doesn't agree with the tossing. The plugin itself is responsible for handling the consequences (possible inventory mismatch)
				return;
			}
			m_Player->TossEquippedItem(64);  // Toss entire slot - if there aren't enough items, the maximum will be ejected
			return;
		}

		default:
		{
			ASSERT(!"Unhandled DIG_STATUS");
			return;
		}
	}  // switch (a_Status)
}





void cClientHandle::HandleBlockDigStarted(int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, BLOCKTYPE a_OldBlock, NIBBLETYPE a_OldMeta)
{
	if (
		m_HasStartedDigging &&
		(a_BlockX == m_LastDigBlockX) &&
		(a_BlockY == m_LastDigBlockY) &&
		(a_BlockZ == m_LastDigBlockZ)
	)
	{
		// It is a duplicate packet, drop it right away
		return;
	}

	if (
		m_Player->IsGameModeCreative() &&
		ItemCategory::IsSword(m_Player->GetInventory().GetEquippedItem().m_ItemType) &&
		(m_Player->GetWorld()->GetBlock(a_BlockX, a_BlockY, a_BlockZ) != E_BLOCK_FIRE)
	)
	{
		// Players can't destroy blocks with a sword in the hand.
		return;
	}

	if (
		(Diff(m_Player->GetPosX(), static_cast<double>(a_BlockX)) > 6) ||
		(Diff(m_Player->GetPosY(), static_cast<double>(a_BlockY)) > 6) ||
		(Diff(m_Player->GetPosZ(), static_cast<double>(a_BlockZ)) > 6)
	)
	{
		m_Player->GetWorld()->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
		return;
	}

	// Set the last digging coords to the block being dug, so that they can be checked in DIG_FINISHED to avoid dig / aim bug in the client:
	m_HasStartedDigging = true;
	m_LastDigBlockX = a_BlockX;
	m_LastDigBlockY = a_BlockY;
	m_LastDigBlockZ = a_BlockZ;

	if (
		(m_Player->IsGameModeCreative()) ||  // In creative mode, digging is done immediately
		cBlockInfo::IsOneHitDig(a_OldBlock)  // One-hit blocks get destroyed immediately, too
	)
	{
		HandleBlockDigFinished(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_OldBlock, a_OldMeta);
		return;
	}

	// Start dig animation
	// TODO: calculate real animation speed
	// TODO: Send animation packets even without receiving any other packets
	m_BlockDigAnimSpeed = 10;
	m_BlockDigAnimX = a_BlockX;
	m_BlockDigAnimY = a_BlockY;
	m_BlockDigAnimZ = a_BlockZ;
	m_BlockDigAnimStage = 0;
	m_Player->GetWorld()->BroadcastBlockBreakAnimation(static_cast<UInt32>(m_UniqueID), m_BlockDigAnimX, m_BlockDigAnimY, m_BlockDigAnimZ, 0, this);

	cWorld * World = m_Player->GetWorld();
	cChunkInterface ChunkInterface(World->GetChunkMap());
	cBlockHandler * Handler = cBlockInfo::GetHandler(a_OldBlock);
	Handler->OnDigging(ChunkInterface, *World, m_Player, a_BlockX, a_BlockY, a_BlockZ);

	cItemHandler * ItemHandler = cItemHandler::GetItemHandler(m_Player->GetEquippedItem());
	ItemHandler->OnDiggingBlock(World, m_Player, m_Player->GetEquippedItem(), a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
}





void cClientHandle::HandleBlockDigFinished(int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, BLOCKTYPE a_OldBlock, NIBBLETYPE a_OldMeta)
{
	if (
		!m_HasStartedDigging ||           // Hasn't received the DIG_STARTED packet
		(m_LastDigBlockX != a_BlockX) ||  // DIG_STARTED has had different pos
		(m_LastDigBlockY != a_BlockY) ||
		(m_LastDigBlockZ != a_BlockZ)
	)
	{
		LOGD("Prevented a dig / aim bug in the client (finish {%d, %d, %d} vs start {%d, %d, %d}, HSD: %s)",
			a_BlockX, a_BlockY, a_BlockZ,
			m_LastDigBlockX, m_LastDigBlockY, m_LastDigBlockZ,
			(m_HasStartedDigging ? "True" : "False")
		);
		return;
	}

	FinishDigAnimation();

	if (!m_Player->IsGameModeCreative())
	{
		if (a_OldBlock == E_BLOCK_BEDROCK)
		{
			Kick("You can't break a bedrock!");
			return;
		}
		if (a_OldBlock == E_BLOCK_BARRIER)
		{
			Kick("You can't break a barrier!");
			return;
		}
	}

	cWorld * World = m_Player->GetWorld();
	cItemHandler * ItemHandler = cItemHandler::GetItemHandler(m_Player->GetEquippedItem());

	if (cRoot::Get()->GetPluginManager()->CallHookPlayerBreakingBlock(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_OldBlock, a_OldMeta))
	{
		// A plugin doesn't agree with the breaking. Bail out. Send the block back to the client, so that it knows:
		m_Player->GetWorld()->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
		return;
	}

	if (a_OldBlock == E_BLOCK_AIR)
	{
		LOGD("Dug air - what the function?");
		return;
	}

	m_Player->AddFoodExhaustion(0.025);
	ItemHandler->OnBlockDestroyed(World, m_Player, m_Player->GetEquippedItem(), a_BlockX, a_BlockY, a_BlockZ);
	// The ItemHandler is also responsible for spawning the pickups
	cChunkInterface ChunkInterface(World->GetChunkMap());
	BlockHandler(a_OldBlock)->OnDestroyedByPlayer(ChunkInterface, *World, m_Player, a_BlockX, a_BlockY, a_BlockZ);
	World->BroadcastSoundParticleEffect(2001, a_BlockX, a_BlockY, a_BlockZ, a_OldBlock, this);
	World->DigBlock(a_BlockX, a_BlockY, a_BlockZ);

	cRoot::Get()->GetPluginManager()->CallHookPlayerBrokenBlock(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_OldBlock, a_OldMeta);
}





void cClientHandle::FinishDigAnimation()
{
	if (!m_HasStartedDigging)  // Hasn't received the DIG_STARTED packet
	{
		return;
	}

	m_HasStartedDigging = false;
	if (m_BlockDigAnimStage != -1)
	{
		// End dig animation
		m_BlockDigAnimStage = -1;
		// It seems that 10 ends block animation
		m_Player->GetWorld()->BroadcastBlockBreakAnimation(static_cast<UInt32>(m_UniqueID), m_LastDigBlockX, m_LastDigBlockY, m_LastDigBlockZ, 10, this);
	}

	m_BlockDigAnimX = -1;
	m_BlockDigAnimY = -1;
	m_BlockDigAnimZ = -1;
}





void cClientHandle::HandleRightClick(int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, const cItem & a_HeldItem)
{
	// TODO: Rewrite this function

	// Distance from the block's center to the player's eye height
	double dist = (Vector3d(a_BlockX, a_BlockY, a_BlockZ) + Vector3d(0.5, 0.5, 0.5) - m_Player->GetEyePosition()).Length();
	LOGD("HandleRightClick: {%d, %d, %d}, face %d, HeldItem: %s; dist: %.02f",
		a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, ItemToFullString(a_HeldItem).c_str(), dist
	);

	// Check the reach distance:
	// _X 2014-11-25: I've maxed at 5.26 with a Survival client and 5.78 with a Creative client in my tests
	double maxDist = m_Player->IsGameModeCreative() ? 5.78 : 5.26;
	bool AreRealCoords = (dist <= maxDist);

	cWorld * World = m_Player->GetWorld();

	if (
		(a_BlockFace != BLOCK_FACE_NONE) &&  // The client is interacting with a specific block
		IsValidBlock(a_HeldItem.m_ItemType) &&
		!AreRealCoords
	)
	{
		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
		if ((a_BlockX != -1) && (a_BlockY >= 0) && (a_BlockZ != -1))
		{
			World->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
			if (a_BlockY < cChunkDef::Height - 1)
			{
				World->SendBlockTo(a_BlockX, a_BlockY + 1, a_BlockZ, m_Player);  // 2 block high things
			}
			if (a_BlockY > 0)
			{
				World->SendBlockTo(a_BlockX, a_BlockY - 1, a_BlockZ, m_Player);  // 2 block high things
			}
		}
		m_Player->GetInventory().SendEquippedSlot();
		return;
	}

	if (!AreRealCoords)
	{
		a_BlockFace = BLOCK_FACE_NONE;
	}

	cPluginManager * PlgMgr = cRoot::Get()->GetPluginManager();
	if (PlgMgr->CallHookPlayerRightClick(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ))
	{
		// A plugin doesn't agree with the action, replace the block on the client and quit:
		if (AreRealCoords)
		{
			cChunkInterface ChunkInterface(World->GetChunkMap());
			BLOCKTYPE BlockType = World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
			cBlockHandler * BlockHandler = cBlockInfo::GetHandler(BlockType);
			BlockHandler->OnCancelRightClick(ChunkInterface, *World, m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);

			if (a_BlockFace != BLOCK_FACE_NONE)
			{
				AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
				World->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
				if (a_BlockY < cChunkDef::Height - 1)
				{
					World->SendBlockTo(a_BlockX, a_BlockY + 1, a_BlockZ, m_Player);  // 2 block high things
				}
				if (a_BlockY > 1)
				{
					World->SendBlockTo(a_BlockX, a_BlockY - 1, a_BlockZ, m_Player);  // 2 block high things
				}
				m_Player->GetInventory().SendEquippedSlot();
			}
		}
		return;
	}

	m_NumBlockChangeInteractionsThisTick++;
	
	if (!CheckBlockInteractionsRate())
	{
		Kick("Too many blocks were placed / interacted with per unit time - hacked client?");
		return;
	}
	
	const cItem & Equipped = m_Player->GetInventory().GetEquippedItem();

	if ((Equipped.m_ItemType != a_HeldItem.m_ItemType) && (a_HeldItem.m_ItemType != -1))
	{
		// Only compare ItemType, not meta (torches have different metas)
		// The -1 check is there because sometimes the client sends -1 instead of the held item
		// Ref.: http://forum.mc-server.org/showthread.php?tid=549&pid=4502#pid4502
		LOGWARN("Player %s tried to place a block that was not equipped (exp %d, got %d)",
			m_Username.c_str(), Equipped.m_ItemType, a_HeldItem.m_ItemType
		);
		
		// Let's send the current world block to the client, so that it can immediately "let the user know" that they haven't placed the block
		if (a_BlockFace != BLOCK_FACE_NONE)
		{
			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
			World->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
		}
		return;
	}

	if (AreRealCoords)
	{
		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, BlockType, BlockMeta);
		cBlockHandler * BlockHandler = cBlockInfo::GetHandler(BlockType);

		if (BlockHandler->IsUseable() && !m_Player->IsCrouched())
		{
			if (PlgMgr->CallHookPlayerUsingBlock(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, BlockType, BlockMeta))
			{
				// A plugin doesn't agree with using the block, abort
				return;
			}
			cChunkInterface ChunkInterface(World->GetChunkMap());
			BlockHandler->OnUse(ChunkInterface, *World, m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ);
			PlgMgr->CallHookPlayerUsedBlock(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, BlockType, BlockMeta);
			return;
		}
	}
	
	short EquippedDamage = Equipped.m_ItemDamage;
	cItemHandler * ItemHandler = cItemHandler::GetItemHandler(Equipped.m_ItemType);
	
	if (ItemHandler->IsPlaceable() && (a_BlockFace != BLOCK_FACE_NONE))
	{
		if (!ItemHandler->OnPlayerPlace(*World, *m_Player, Equipped, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ))
		{
			// Placement failed, bail out
			return;
		}
	}
	else if ((ItemHandler->IsFood() || ItemHandler->IsDrinkable(EquippedDamage)))
	{
		if (
			(m_Player->IsSatiated() || m_Player->IsGameModeCreative()) &&  // Only creative or hungry players can eat
			ItemHandler->IsFood() &&
			(Equipped.m_ItemType != E_ITEM_GOLDEN_APPLE)  // Golden apple is a special case, it is used instead of eaten
		)
		{
			// The player is satiated or in creative, and trying to eat
			return;
		}
		m_Player->StartEating();
		if (PlgMgr->CallHookPlayerEating(*m_Player))
		{
			// A plugin won't let us eat, abort (send the proper packets to the client, too):
			m_Player->AbortEating();
		}
	}
	else
	{
		if (PlgMgr->CallHookPlayerUsingItem(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ))
		{
			// A plugin doesn't agree with using the item, abort
			return;
		}
		cBlockInServerPluginInterface PluginInterface(*World);
		ItemHandler->OnItemUse(World, m_Player, PluginInterface, Equipped, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
		PlgMgr->CallHookPlayerUsedItem(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ);
	}
}





void cClientHandle::HandleChat(const AString & a_Message)
{
	// We no longer need to postpone message processing, because the messages already arrive in the Tick thread
	
	// If a command, perform it:
	AString Message(a_Message);
	if (cRoot::Get()->GetServer()->Command(*this, Message))
	{
		return;
	}
	
	// Not a command, broadcast as a message:
	cCompositeChat Msg;
	AString Color = m_Player->GetColor();
	if (Color.length() == 3)
	{
		Color = AString("@") + Color[2];
	}
	else
	{
		Color.clear();
	}
	Msg.AddTextPart(AString("<") + m_Player->GetName() + "> ", Color);
	Msg.ParseText(Message);
	Msg.UnderlineUrls();
	cRoot::Get()->BroadcastChat(Msg);
}





void cClientHandle::HandlePlayerLook(float a_Rotation, float a_Pitch, bool a_IsOnGround)
{
	if ((m_Player == nullptr) || (m_State != csPlaying))
	{
		return;
	}
	
	m_Player->SetYaw        (a_Rotation);
	m_Player->SetHeadYaw    (a_Rotation);
	m_Player->SetPitch      (a_Pitch);
	m_Player->SetTouchGround(a_IsOnGround);
}





void cClientHandle::HandlePlayerMoveLook(double a_PosX, double a_PosY, double a_PosZ, double a_Stance, float a_Rotation, float a_Pitch, bool a_IsOnGround)
{
	HandlePlayerPos(a_PosX, a_PosY, a_PosZ, a_Stance, a_IsOnGround);
	HandlePlayerLook(a_Rotation, a_Pitch, a_IsOnGround);
}





void cClientHandle::HandleAnimation(int a_Animation)
{
	if (cPluginManager::Get()->CallHookPlayerAnimation(*m_Player, a_Animation))
	{
		// Plugin disagrees, bail out
		return;
	}

	m_Player->GetWorld()->BroadcastEntityAnimation(*m_Player, static_cast<char>(a_Animation), this);
}





void cClientHandle::HandleSlotSelected(Int16 a_SlotNum)
{
	m_Player->GetInventory().SetEquippedSlotNum(a_SlotNum);
	m_Player->GetWorld()->BroadcastEntityEquipment(*m_Player, 0, m_Player->GetInventory().GetEquippedItem(), this);
}





void cClientHandle::HandleSteerVehicle(float a_Forward, float a_Sideways)
{
	m_Player->SteerVehicle(a_Forward, a_Sideways);
}





void cClientHandle::HandleWindowClose(UInt8 a_WindowID)
{
	m_Player->CloseWindowIfID(static_cast<char>(a_WindowID));
}





void cClientHandle::HandleWindowClick(UInt8 a_WindowID, Int16 a_SlotNum, eClickAction a_ClickAction, const cItem & a_HeldItem)
{
	LOGD("WindowClick: WinID %d, SlotNum %d, action: %s, Item %s x %d",
		a_WindowID, a_SlotNum, ClickActionToString(a_ClickAction),
		ItemToString(a_HeldItem).c_str(), a_HeldItem.m_ItemCount
	);
	
	cWindow * Window = m_Player->GetWindow();
	if (Window == nullptr)
	{
		LOGWARNING("Player \"%s\" clicked in a non-existent window. Ignoring", m_Username.c_str());
		return;
	}
	
	Window->Clicked(*m_Player, a_WindowID, a_SlotNum, a_ClickAction, a_HeldItem);
}





void cClientHandle::HandleUpdateSign(
	int a_BlockX, int a_BlockY, int a_BlockZ,
	const AString & a_Line1, const AString & a_Line2,
	const AString & a_Line3, const AString & a_Line4
)
{
	if (m_LastPlacedSign.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
	{
		m_LastPlacedSign.Set(0, -1, 0);
		m_Player->GetWorld()->SetSignLines(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4, m_Player);
	}
}





void cClientHandle::HandleUseEntity(UInt32 a_TargetEntityID, bool a_IsLeftClick)
{
	// TODO: Let plugins interfere via a hook
	
	// If it is a right click, call the entity's OnRightClicked() handler:
	if (!a_IsLeftClick)
	{
		class cRclkEntity : public cEntityCallback
		{
			cPlayer & m_Player;
			virtual bool Item(cEntity * a_Entity) override
			{
				if (cPluginManager::Get()->CallHookPlayerRightClickingEntity(m_Player, *a_Entity))
				{
					return false;
				}
				a_Entity->OnRightClicked(m_Player);
				return false;
			}
		public:
			cRclkEntity(cPlayer & a_Player) : m_Player(a_Player) {}
		} Callback (*m_Player);
		
		cWorld * World = m_Player->GetWorld();
		World->DoWithEntityByID(a_TargetEntityID, Callback);
		return;
	}

	// If it is a left click, attack the entity:
	class cDamageEntity : public cEntityCallback
	{
		virtual bool Item(cEntity * a_Entity) override
		{
			if (!a_Entity->GetWorld()->IsPVPEnabled())
			{
				// PVP is disabled, disallow players hurting other players:
				if (a_Entity->IsPlayer())
				{
					// Player is hurting another player which is not allowed when PVP is disabled so ignore it
					return true;
				}
			}
			a_Entity->TakeDamage(*m_Attacker);
			return false;
		}
	public:
		cPawn * m_Attacker;
	} Callback;

	Callback.m_Attacker = m_Player;

	cWorld * World = m_Player->GetWorld();
	if (World->DoWithEntityByID(a_TargetEntityID, Callback))
	{
		// Any kind of an attack implies food exhaustion
		m_Player->AddFoodExhaustion(0.3);
	}
}





void cClientHandle::HandleRespawn(void)
{
	if (m_Player == nullptr)
	{
		Destroy();
		return;
	}
	m_Player->Respawn();
	cRoot::Get()->GetPluginManager()->CallHookPlayerSpawned(*m_Player);
}





void cClientHandle::HandleKeepAlive(UInt32 a_KeepAliveID)
{
	if (a_KeepAliveID == m_PingID)
	{
		m_Ping = std::chrono::steady_clock::now() - m_PingStartTime;
	}
}





bool cClientHandle::CheckMultiLogin(const AString & a_Username)
{
	// If the multilogin is allowed, skip this check entirely:
	if ((cRoot::Get()->GetServer()->DoesAllowMultiLogin()))
	{
		return true;
	}
	
	// Check if the player is waiting to be transferred to the World.
	if (cRoot::Get()->GetServer()->IsPlayerInQueue(a_Username))
	{
		Kick("A player of the username is already logged in");
		return false;
	}

	class cCallback :
		public cPlayerListCallback
	{
		virtual bool Item(cPlayer * a_Player) override
		{
			return true;
		}
	} Callback;

	// Check if the player is in any World.
	if (cRoot::Get()->DoWithPlayer(a_Username, Callback))
	{
		Kick("A player of the username is already logged in");
		return false;
	}
	return true;
}





bool cClientHandle::HandleHandshake(const AString & a_Username)
{
	if (!cRoot::Get()->GetPluginManager()->CallHookHandshake(*this, a_Username))
	{
		if (cRoot::Get()->GetServer()->GetNumPlayers() >= cRoot::Get()->GetServer()->GetMaxPlayers())
		{
			Kick("The server is currently full :(-- Try again later");
			return false;
		}
	}

	return CheckMultiLogin(a_Username);
}





void cClientHandle::HandleEntityCrouch(UInt32 a_EntityID, bool a_IsCrouching)
{
	if (a_EntityID != m_Player->GetUniqueID())
	{
		// We should only receive entity actions from the entity that is performing the action
		return;
	}

	m_Player->SetCrouch(a_IsCrouching);
}





void cClientHandle::HandleEntityLeaveBed(UInt32 a_EntityID)
{
	if (a_EntityID != m_Player->GetUniqueID())
	{
		// We should only receive entity actions from the entity that is performing the action
		return;
	}

	cChunkInterface Interface(GetPlayer()->GetWorld()->GetChunkMap());
	cBlockBedHandler::SetBedOccupationState(Interface, GetPlayer()->GetLastBedPos(), false);
	GetPlayer()->SetIsInBed(false);
}





void cClientHandle::HandleEntitySprinting(UInt32 a_EntityID, bool a_IsSprinting)
{
	if (a_EntityID != m_Player->GetUniqueID())
	{
		// We should only receive entity actions from the entity that is performing the action
		return;
	}

	m_Player->SetSprint(a_IsSprinting);
}





void cClientHandle::HandleUnmount(void)
{
	if (m_Player == nullptr)
	{
		return;
	}
	m_Player->Detach();
}





void cClientHandle::HandleTabCompletion(const AString & a_Text)
{
	AStringVector Results;
	m_Player->GetWorld()->TabCompleteUserName(a_Text, Results);
	cRoot::Get()->GetPluginManager()->TabCompleteCommand(a_Text, Results, m_Player);
	if (Results.empty())
	{
		return;
	}
	std::sort(Results.begin(), Results.end());
	SendTabCompletionResults(Results);
}





void cClientHandle::SendData(const char * a_Data, size_t a_Size)
{
	if (m_HasSentDC)
	{
		// This could crash the client, because they've already unloaded the world etc., and suddenly a wild packet appears (#31)
		return;
	}

	cCSLock Lock(m_CSOutgoingData);
	m_OutgoingData.append(a_Data, a_Size);
}





void cClientHandle::RemoveFromWorld(void)
{
	// Remove all associated chunks:
	cChunkCoordsList Chunks;
	{
		cCSLock Lock(m_CSChunkLists);
		std::swap(Chunks, m_LoadedChunks);
		m_ChunksToSend.clear();
	}
	for (cChunkCoordsList::iterator itr = Chunks.begin(), end = Chunks.end(); itr != end; ++itr)
	{
		m_Protocol->SendUnloadChunk(itr->m_ChunkX, itr->m_ChunkZ);
	}  // for itr - Chunks[]

	// Here, we set last streamed values to bogus ones so everything is resent
	m_LastStreamedChunkX = 0x7fffffff;
	m_LastStreamedChunkZ = 0x7fffffff;

	m_HasSentPlayerChunk = false;
}





bool cClientHandle::CheckBlockInteractionsRate(void)
{
	ASSERT(m_Player != nullptr);
	ASSERT(m_Player->GetWorld() != nullptr);

	if (m_NumBlockChangeInteractionsThisTick > MAX_BLOCK_CHANGE_INTERACTIONS)
	{
		return false;
	}

	return true;
}





void cClientHandle::Tick(float a_Dt)
{
	// Process received network data:
	AString IncomingData;
	{
		cCSLock Lock(m_CSIncomingData);
		std::swap(IncomingData, m_IncomingData);
	}
	if (!IncomingData.empty())
	{
		m_Protocol->DataReceived(IncomingData.data(), IncomingData.size());
	}

	// Send any queued outgoing data:
	AString OutgoingData;
	{
		cCSLock Lock(m_CSOutgoingData);
		std::swap(OutgoingData, m_OutgoingData);
	}
	if (!OutgoingData.empty())
	{
		cTCPLinkPtr Link(m_Link);  // Grab a copy of the link in a multithread-safe way
		if ((Link != nullptr))
		{
			Link->Send(OutgoingData.data(), OutgoingData.size());
		}
	}
	
	m_TicksSinceLastPacket += 1;
	if (m_TicksSinceLastPacket > 600)  // 30 seconds time-out
	{
		SendDisconnect("Nooooo!! You timed out! D: Come back!");
		return;
	}
	
	if (m_Player == nullptr)
	{
		return;
	}

	// If the chunk the player's in was just sent, spawn the player:
	if (m_HasSentPlayerChunk && (m_State == csDownloadingWorld))
	{
		m_Protocol->SendPlayerMoveLook();
		m_State = csPlaying;
	}

	// Send a ping packet:
	if (m_State == csPlaying)
	{
		if ((m_PingStartTime + PING_TIME_MS <= std::chrono::steady_clock::now()))
		{
			m_PingID++;
			m_PingStartTime = std::chrono::steady_clock::now();
			m_Protocol->SendKeepAlive(m_PingID);
		}
	}

	if ((m_State >= csAuthenticated) && (m_State < csDestroying))
	{
		// Stream 4 chunks per tick
		for (int i = 0; i < 4; i++)
		{
			// Stream the next chunk
			if (StreamNextChunk())
			{
				// Streaming finished. All chunks are loaded.
				break;
			}
		}

		// Unload all chunks that are out of the view distance (all 5 seconds)
		if ((m_Player->GetWorld()->GetWorldAge() % 100) == 0)
		{
			UnloadOutOfRangeChunks();
		}
	}

	// Handle block break animation:
	if (m_BlockDigAnimStage > -1)
	{
		int lastAnimVal = m_BlockDigAnimStage;
		m_BlockDigAnimStage += static_cast<int>(m_BlockDigAnimSpeed * a_Dt);
		if (m_BlockDigAnimStage > 9000)
		{
			m_BlockDigAnimStage = 9000;
		}
		if (m_BlockDigAnimStage / 1000 != lastAnimVal / 1000)
		{
			m_Player->GetWorld()->BroadcastBlockBreakAnimation(static_cast<UInt32>(m_UniqueID), m_BlockDigAnimX, m_BlockDigAnimY, m_BlockDigAnimZ, static_cast<char>(m_BlockDigAnimStage / 1000), this);
		}
	}
	
	// Reset explosion & block change counters:
	m_NumExplosionsThisTick = 0;
	m_NumBlockChangeInteractionsThisTick = 0;
}





void cClientHandle::ServerTick(float a_Dt)
{
	// Process received network data:
	AString IncomingData;
	{
		cCSLock Lock(m_CSIncomingData);
		std::swap(IncomingData, m_IncomingData);
	}
	if (!IncomingData.empty())
	{
		m_Protocol->DataReceived(IncomingData.data(), IncomingData.size());
	}
	
	// Send any queued outgoing data:
	AString OutgoingData;
	{
		cCSLock Lock(m_CSOutgoingData);
		std::swap(OutgoingData, m_OutgoingData);
	}
	if ((m_Link != nullptr) && !OutgoingData.empty())
	{
		m_Link->Send(OutgoingData.data(), OutgoingData.size());
	}
	
	if (m_State == csAuthenticated)
	{
		StreamNextChunk();

		// Remove the client handle from the server, it will be ticked from its cPlayer object from now on
		cRoot::Get()->GetServer()->ClientMovedToWorld(this);
		
		// Add the player to the world (start ticking from there):
		m_State = csDownloadingWorld;
		m_Player->GetWorld()->AddPlayer(m_Player);
		return;
	}
	
	m_TicksSinceLastPacket += 1;
	if (m_TicksSinceLastPacket > 600)  // 30 seconds
	{
		SendDisconnect("Nooooo!! You timed out! D: Come back!");
	}
}





void cClientHandle::SendAttachEntity(const cEntity & a_Entity, const cEntity * a_Vehicle)
{
	m_Protocol->SendAttachEntity(a_Entity, a_Vehicle);
}





void cClientHandle::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType)
{
	m_Protocol->SendBlockAction(a_BlockX, a_BlockY, a_BlockZ, a_Byte1, a_Byte2, a_BlockType);
}





void cClientHandle::SendBlockBreakAnim(UInt32 a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage)
{
	m_Protocol->SendBlockBreakAnim(a_EntityID, a_BlockX, a_BlockY, a_BlockZ, a_Stage);
}





void cClientHandle::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	int ChunkX, ChunkZ = 0;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	cChunkCoords ChunkCoords = cChunkCoords(ChunkX, ChunkZ);

	// Do not send block changes in chunks that weren't sent to the client yet:
	cCSLock Lock(m_CSChunkLists);
	if (std::find(m_SentChunks.begin(), m_SentChunks.end(), ChunkCoords) != m_SentChunks.end())
	{
		Lock.Unlock();
		m_Protocol->SendBlockChange(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
	}
}





void cClientHandle::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(!a_Changes.empty());  // We don't want to be sending empty change packets!

	// Do not send block changes in chunks that weren't sent to the client yet:
	cChunkCoords ChunkCoords = cChunkCoords(a_ChunkX, a_ChunkZ);
	cCSLock Lock(m_CSChunkLists);
	if (std::find(m_SentChunks.begin(), m_SentChunks.end(), ChunkCoords) != m_SentChunks.end())
	{
		Lock.Unlock();
		m_Protocol->SendBlockChanges(a_ChunkX, a_ChunkZ, a_Changes);
	}
}





void cClientHandle::SendChat(const AString & a_Message, eMessageType a_ChatPrefix, const AString & a_AdditionalData)
{
	cWorld * World = GetPlayer()->GetWorld();
	if (World == nullptr)
	{
		World = cRoot::Get()->GetWorld(GetPlayer()->GetLoadedWorldName());
		if (World == nullptr)
		{
			World = cRoot::Get()->GetDefaultWorld();
		}
	}

	AString Message = FormatMessageType(World->ShouldUseChatPrefixes(), a_ChatPrefix, a_AdditionalData);
	m_Protocol->SendChat(Message.append(a_Message));
}





void cClientHandle::SendChat(const cCompositeChat & a_Message)
{
	m_Protocol->SendChat(a_Message);
}





void cClientHandle::SendChatAboveActionBar(const AString & a_Message, eMessageType a_ChatPrefix, const AString & a_AdditionalData)
{
	cWorld * World = GetPlayer()->GetWorld();
	if (World == nullptr)
	{
		World = cRoot::Get()->GetWorld(GetPlayer()->GetLoadedWorldName());
		if (World == nullptr)
		{
			World = cRoot::Get()->GetDefaultWorld();
		}
	}

	AString Message = FormatMessageType(World->ShouldUseChatPrefixes(), a_ChatPrefix, a_AdditionalData);
	m_Protocol->SendChatAboveActionBar(Message.append(a_Message));
}





void cClientHandle::SendChatAboveActionBar(const cCompositeChat & a_Message)
{
	m_Protocol->SendChatAboveActionBar(a_Message);
}





void cClientHandle::SendChatSystem(const AString & a_Message, eMessageType a_ChatPrefix, const AString & a_AdditionalData)
{
	cWorld * World = GetPlayer()->GetWorld();
	if (World == nullptr)
	{
		World = cRoot::Get()->GetWorld(GetPlayer()->GetLoadedWorldName());
		if (World == nullptr)
		{
			World = cRoot::Get()->GetDefaultWorld();
		}
	}

	AString Message = FormatMessageType(World->ShouldUseChatPrefixes(), a_ChatPrefix, a_AdditionalData);
	m_Protocol->SendChatSystem(Message.append(a_Message));
}





void cClientHandle::SendChatSystem(const cCompositeChat & a_Message)
{
	m_Protocol->SendChatSystem(a_Message);
}





void cClientHandle::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	ASSERT(m_Player != nullptr);
	
	// Check chunks being sent, erase them from m_ChunksToSend:
	bool Found = false;
	{
		cCSLock Lock(m_CSChunkLists);
		auto itr = m_ChunksToSend.find(cChunkCoords{a_ChunkX, a_ChunkZ});
		if (itr != m_ChunksToSend.end())
		{
			m_ChunksToSend.erase(itr);
			Found = true;
		}
	}
	if (!Found)
	{
		// This just sometimes happens. If you have a reliably replicatable situation for this, go ahead and fix it
		// It's not a big issue anyway, just means that some chunks may be compressed several times
		// LOGD("Refusing to send    chunk [%d, %d] to client \"%s\" at [%d, %d].", ChunkX, ChunkZ, m_Username.c_str(), m_Player->GetChunkX(), m_Player->GetChunkZ());
		return;
	}
	
	m_Protocol->SendChunkData(a_ChunkX, a_ChunkZ, a_Serializer);

	// Add the chunk to the list of chunks sent to the player:
	{
		cCSLock Lock(m_CSChunkLists);
		m_SentChunks.push_back(cChunkCoords(a_ChunkX, a_ChunkZ));
	}

	// If it is the chunk the player's in, make them spawn (in the tick thread):
	if ((m_State == csAuthenticated) || (m_State == csDownloadingWorld))
	{
		if ((a_ChunkX == m_Player->GetChunkX()) && (a_ChunkZ == m_Player->GetChunkZ()))
		{
			m_HasSentPlayerChunk = true;
		}
	}
}





void cClientHandle::SendCollectEntity(const cEntity & a_Entity, const cPlayer & a_Player)
{
	m_Protocol->SendCollectEntity(a_Entity, a_Player);
}





void cClientHandle::SendDestroyEntity(const cEntity & a_Entity)
{
	m_Protocol->SendDestroyEntity(a_Entity);
}





void cClientHandle::SendDisconnect(const AString & a_Reason)
{
	// Destruction (Destroy()) is called when the client disconnects, not when a disconnect packet (or anything else) is sent
	// Otherwise, the cClientHandle instance is can be unexpectedly removed from the associated player - Core/#142
	if (!m_HasSentDC)
	{
		LOGD("Sending a DC: \"%s\"", StripColorCodes(a_Reason).c_str());
		m_Protocol->SendDisconnect(a_Reason);
		m_HasSentDC = true;
	}
}





void cClientHandle::SendEditSign(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	m_LastPlacedSign.Set(a_BlockX, a_BlockY, a_BlockZ);
	m_Protocol->SendEditSign(a_BlockX, a_BlockY, a_BlockZ);
}





void cClientHandle::SendEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration)
{
	m_Protocol->SendEntityEffect(a_Entity, a_EffectID, a_Amplifier, a_Duration);
}





void cClientHandle::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	m_Protocol->SendEntityEquipment(a_Entity, a_SlotNum, a_Item);
}





void cClientHandle::SendEntityHeadLook(const cEntity & a_Entity)
{
	ASSERT(a_Entity.GetUniqueID() != m_Player->GetUniqueID());  // Must not send for self
	
	m_Protocol->SendEntityHeadLook(a_Entity);
}





void cClientHandle::SendEntityLook(const cEntity & a_Entity)
{
	ASSERT(a_Entity.GetUniqueID() != m_Player->GetUniqueID());  // Must not send for self
	
	m_Protocol->SendEntityLook(a_Entity);
}





void cClientHandle::SendEntityMetadata(const cEntity & a_Entity)
{
	m_Protocol->SendEntityMetadata(a_Entity);
}





void cClientHandle::SendEntityRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(a_Entity.GetUniqueID() != m_Player->GetUniqueID());  // Must not send for self
	
	m_Protocol->SendEntityRelMove(a_Entity, a_RelX, a_RelY, a_RelZ);
}





void cClientHandle::SendEntityRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(a_Entity.GetUniqueID() != m_Player->GetUniqueID());  // Must not send for self
	
	m_Protocol->SendEntityRelMoveLook(a_Entity, a_RelX, a_RelY, a_RelZ);
}





void cClientHandle::SendEntityStatus(const cEntity & a_Entity, char a_Status)
{
	m_Protocol->SendEntityStatus(a_Entity, a_Status);
}





void cClientHandle::SendEntityVelocity(const cEntity & a_Entity)
{
	m_Protocol->SendEntityVelocity(a_Entity);
}





void cClientHandle::SendExplosion(double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion)
{
	if (m_NumExplosionsThisTick > MAX_EXPLOSIONS_PER_TICK)
	{
		LOGD("Dropped an explosion!");
		return;
	}
	
	// Update the statistics:
	m_NumExplosionsThisTick++;
	
	m_Protocol->SendExplosion(a_BlockX, a_BlockY, a_BlockZ, a_Radius, a_BlocksAffected, a_PlayerMotion);
}





void cClientHandle::SendGameMode(eGameMode a_GameMode)
{
	m_Protocol->SendGameMode(a_GameMode);
}





void cClientHandle::SendHealth(void)
{
	m_Protocol->SendHealth();
}





void cClientHandle::SendHideTitle(void)
{
	m_Protocol->SendHideTitle();
}





void cClientHandle::SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	m_Protocol->SendInventorySlot(a_WindowID, a_SlotNum, a_Item);
}





void cClientHandle::SendMapData(const cMap & a_Map, int a_DataStartX, int a_DataStartY)
{
	m_Protocol->SendMapData(a_Map, a_DataStartX, a_DataStartY);
}





void cClientHandle::SendParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount)
{
	m_Protocol->SendParticleEffect(a_ParticleName, a_SrcX, a_SrcY, a_SrcZ, a_OffsetX, a_OffsetY, a_OffsetZ, a_ParticleData, a_ParticleAmount);
}





void cClientHandle::SendParticleEffect(const AString & a_ParticleName, const Vector3f a_Src, const Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data)
{
	m_Protocol->SendParticleEffect(a_ParticleName, a_Src, a_Offset, a_ParticleData, a_ParticleAmount, a_Data);
}





void cClientHandle::SendPickupSpawn(const cPickup & a_Pickup)
{
	m_Protocol->SendPickupSpawn(a_Pickup);
}




void cClientHandle::SendPaintingSpawn(const cPainting & a_Painting)
{
	m_Protocol->SendPaintingSpawn(a_Painting);
}





void cClientHandle::SendEntityAnimation(const cEntity & a_Entity, char a_Animation)
{
	m_Protocol->SendEntityAnimation(a_Entity, a_Animation);
}





void cClientHandle::SendPlayerAbilities()
{
	m_Protocol->SendPlayerAbilities();
}





void cClientHandle::SendPlayerListAddPlayer(const cPlayer & a_Player)
{
	m_Protocol->SendPlayerListAddPlayer(a_Player);
}





void cClientHandle::SendPlayerListRemovePlayer(const cPlayer & a_Player)
{
	m_Protocol->SendPlayerListRemovePlayer(a_Player);
}





void cClientHandle::SendPlayerListUpdateGameMode(const cPlayer & a_Player)
{
	m_Protocol->SendPlayerListUpdateGameMode(a_Player);
}





void cClientHandle::SendPlayerListUpdatePing(const cPlayer & a_Player)
{
	m_Protocol->SendPlayerListUpdatePing(a_Player);
}





void cClientHandle::SendPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName)
{
	m_Protocol->SendPlayerListUpdateDisplayName(a_Player, a_CustomName);
}





void cClientHandle::SendPlayerMaxSpeed(void)
{
	m_Protocol->SendPlayerMaxSpeed();
}





void cClientHandle::SendPlayerMoveLook(void)
{
	/*
	LOGD("Sending PlayerMoveLook: {%0.2f, %0.2f, %0.2f}, stance %0.2f, OnGround: %d",
		m_Player->GetPosX(), m_Player->GetPosY(), m_Player->GetPosZ(), m_Player->GetStance(), m_Player->IsOnGround() ? 1 : 0
	);
	*/
	m_Protocol->SendPlayerMoveLook();
}





void cClientHandle::SendPlayerPosition(void)
{
	m_Protocol->SendPlayerPosition();
}





void cClientHandle::SendPlayerSpawn(const cPlayer & a_Player)
{
	if (a_Player.GetUniqueID() == m_Player->GetUniqueID())
	{
		// Do NOT send this packet to myself
		return;
	}
	
	LOGD("Spawning player \"%s\" on client \"%s\" @ %s",
		a_Player.GetName().c_str(), GetPlayer()->GetName().c_str(), GetIPString().c_str()
	);
	
	m_Protocol->SendPlayerSpawn(a_Player);
}





void cClientHandle::SendPluginMessage(const AString & a_Channel, const AString & a_Message)
{
	m_Protocol->SendPluginMessage(a_Channel, a_Message);
}





void cClientHandle::SendRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID)
{
	m_Protocol->SendRemoveEntityEffect(a_Entity, a_EffectID);
}





void cClientHandle::SendResetTitle()
{
	m_Protocol->SendResetTitle();
}





void cClientHandle::SendRespawn(eDimension a_Dimension, bool a_ShouldIgnoreDimensionChecks)
{
	m_Protocol->SendRespawn(a_Dimension, a_ShouldIgnoreDimensionChecks);
}





void cClientHandle::SendExperience(void)
{
	m_Protocol->SendExperience();
}





void cClientHandle::SendExperienceOrb(const cExpOrb & a_ExpOrb)
{
	m_Protocol->SendExperienceOrb(a_ExpOrb);
}





void cClientHandle::SendScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode)
{
	m_Protocol->SendScoreboardObjective(a_Name, a_DisplayName, a_Mode);
}





void cClientHandle::SendScoreUpdate(const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode)
{
	m_Protocol->SendScoreUpdate(a_Objective, a_Player, a_Score, a_Mode);
}





void cClientHandle::SendDisplayObjective(const AString & a_Objective, cScoreboard::eDisplaySlot a_Display)
{
	m_Protocol->SendDisplayObjective(a_Objective, a_Display);
}





void cClientHandle::SendSetSubTitle(const cCompositeChat & a_SubTitle)
{
	m_Protocol->SendSetSubTitle(a_SubTitle);
}





void cClientHandle::SendSetRawSubTitle(const AString & a_SubTitle)
{
	m_Protocol->SendSetRawSubTitle(a_SubTitle);
}





void cClientHandle::SendSetTitle(const cCompositeChat & a_Title)
{
	m_Protocol->SendSetTitle(a_Title);
}





void cClientHandle::SendSetRawTitle(const AString & a_Title)
{
	m_Protocol->SendSetRawTitle(a_Title);
}





void cClientHandle::SendSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch)
{
	m_Protocol->SendSoundEffect(a_SoundName, a_X, a_Y, a_Z, a_Volume, a_Pitch);
}





void cClientHandle::SendSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
	m_Protocol->SendSoundParticleEffect(a_EffectID, a_SrcX, a_SrcY, a_SrcZ, a_Data);
}





void cClientHandle::SendSpawnFallingBlock(const cFallingBlock & a_FallingBlock)
{
	m_Protocol->SendSpawnFallingBlock(a_FallingBlock);
}





void cClientHandle::SendSpawnMob(const cMonster & a_Mob)
{
	m_Protocol->SendSpawnMob(a_Mob);
}





void cClientHandle::SendSpawnObject(const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch)
{
	m_Protocol->SendSpawnObject(a_Entity, a_ObjectType, a_ObjectData, a_Yaw, a_Pitch);
}





void cClientHandle::SendSpawnVehicle(const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType)  // VehicleSubType is specific to Minecarts
{
	m_Protocol->SendSpawnVehicle(a_Vehicle, a_VehicleType, a_VehicleSubType);
}





void cClientHandle::SendStatistics(const cStatManager & a_Manager)
{
	m_Protocol->SendStatistics(a_Manager);
}





void cClientHandle::SendTabCompletionResults(const AStringVector & a_Results)
{
	m_Protocol->SendTabCompletionResults(a_Results);
}





void cClientHandle::SendTeleportEntity(const cEntity & a_Entity)
{
	m_Protocol->SendTeleportEntity(a_Entity);
}





void cClientHandle::SendThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	m_Protocol->SendThunderbolt(a_BlockX, a_BlockY, a_BlockZ);
}





void cClientHandle::SendTitleTimes(int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks)
{
	m_Protocol->SendTitleTimes(a_FadeInTicks, a_DisplayTicks, a_FadeOutTicks);
}





void cClientHandle::SendTimeUpdate(Int64 a_WorldAge, Int64 a_TimeOfDay, bool a_DoDaylightCycle)
{
	m_Protocol->SendTimeUpdate(a_WorldAge, a_TimeOfDay, a_DoDaylightCycle);
}





void cClientHandle::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	// Remove the chunk from the list of chunks sent to the client:
	{
		cCSLock Lock(m_CSChunkLists);
		m_SentChunks.remove(cChunkCoords(a_ChunkX, a_ChunkZ));
	}

	m_Protocol->SendUnloadChunk(a_ChunkX, a_ChunkZ);
}




void cClientHandle::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	m_Protocol->SendUpdateBlockEntity(a_BlockEntity);
}





void cClientHandle::SendUpdateSign(
	int a_BlockX, int a_BlockY, int a_BlockZ,
	const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4
)
{
	m_Protocol->SendUpdateSign(
		a_BlockX, a_BlockY, a_BlockZ,
		a_Line1, a_Line2, a_Line3, a_Line4
	);
}





void cClientHandle::SendUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	m_Protocol->SendUseBed(a_Entity, a_BlockX, a_BlockY, a_BlockZ);
}




void cClientHandle::SendWeather(eWeather a_Weather)
{
	m_Protocol->SendWeather(a_Weather);
}





void cClientHandle::SendWholeInventory(const cWindow & a_Window)
{
	m_Protocol->SendWholeInventory(a_Window);
}





void cClientHandle::SendWindowClose(const cWindow & a_Window)
{
	m_Protocol->SendWindowClose(a_Window);
}





void cClientHandle::SendWindowOpen(const cWindow & a_Window)
{
	m_Protocol->SendWindowOpen(a_Window);
}





void cClientHandle::SendWindowProperty(const cWindow & a_Window, short a_Property, short a_Value)
{
	m_Protocol->SendWindowProperty(a_Window, a_Property, a_Value);
}





const AString & cClientHandle::GetUsername(void) const
{
	return m_Username;
}





void cClientHandle::SetUsername( const AString & a_Username)
{
	m_Username = a_Username;
}





void cClientHandle::SetViewDistance(int a_ViewDistance)
{
	m_RequestedViewDistance = a_ViewDistance;
	LOGD("%s is requesting ViewDistance of %d!", GetUsername().c_str(), m_RequestedViewDistance);

	// Set the current view distance based on the requested VD and world max VD:
	cWorld * world = m_Player->GetWorld();
	if (world != nullptr)
	{
		m_CurrentViewDistance = Clamp(a_ViewDistance, cClientHandle::MIN_VIEW_DISTANCE, world->GetMaxViewDistance());
	}
}





bool cClientHandle::HasPluginChannel(const AString & a_PluginChannel)
{
	return (m_PluginChannels.find(a_PluginChannel) != m_PluginChannels.end());
}





bool cClientHandle::WantsSendChunk(int a_ChunkX, int a_ChunkZ)
{
	if (m_State >= csDestroying)
	{
		return false;
	}
	
	cCSLock Lock(m_CSChunkLists);
	return m_ChunksToSend.find(cChunkCoords(a_ChunkX, a_ChunkZ)) != m_ChunksToSend.end();
}





void cClientHandle::AddWantedChunk(int a_ChunkX, int a_ChunkZ)
{
	if (m_State >= csDestroying)
	{
		return;
	}
	
	LOGD("Adding chunk [%d, %d] to wanted chunks for client %p", a_ChunkX, a_ChunkZ, this);
	cCSLock Lock(m_CSChunkLists);
	if (m_ChunksToSend.find(cChunkCoords(a_ChunkX, a_ChunkZ)) == m_ChunksToSend.end())
	{
		m_ChunksToSend.emplace(a_ChunkX, a_ChunkZ);
	}
}





void cClientHandle::PacketBufferFull(void)
{
	// Too much data in the incoming queue, the server is probably too busy, kick the client:
	LOGERROR("Too much data in queue for client \"%s\" @ %s, kicking them.", m_Username.c_str(), m_IPString.c_str());
	SendDisconnect("Server busy");
}





void cClientHandle::PacketUnknown(UInt32 a_PacketType)
{
	LOGERROR("Unknown packet type 0x%x from client \"%s\" @ %s", a_PacketType, m_Username.c_str(), m_IPString.c_str());

	AString Reason;
	Printf(Reason, "Unknown [C->S] PacketType: 0x%x", a_PacketType);
	SendDisconnect(Reason);
}





void cClientHandle::PacketError(UInt32 a_PacketType)
{
	LOGERROR("Protocol error while parsing packet type 0x%02x; disconnecting client \"%s\"", a_PacketType, m_Username.c_str());
	SendDisconnect("Protocol error");
}





void cClientHandle::SocketClosed(void)
{
	// The socket has been closed for any reason

	if (!m_Username.empty())  // Ignore client pings
	{
		LOGD("Client %s @ %s disconnected", m_Username.c_str(), m_IPString.c_str());
		cRoot::Get()->GetPluginManager()->CallHookDisconnect(*this, "Player disconnected");
	}
	if ((m_State < csDestroying) && (m_Player != nullptr))
	{
		cWorld * World = m_Player->GetWorld();
		if (World != nullptr)
		{
			World->RemovePlayer(m_Player, true);  // Must be called before cPlayer::Destroy() as otherwise cChunk tries to delete the player, and then we do it again
		}
	}
	Destroy();
}





void cClientHandle::SetSelf(cClientHandlePtr a_Self)
{
	ASSERT(m_Self == nullptr);
	m_Self = a_Self;
}





void cClientHandle::OnLinkCreated(cTCPLinkPtr a_Link)
{
	m_Link = a_Link;
}





void cClientHandle::OnReceivedData(const char * a_Data, size_t a_Length)
{
	// Reset the timeout:
	m_TicksSinceLastPacket = 0;

	// Queue the incoming data to be processed in the tick thread:
	cCSLock Lock(m_CSIncomingData);
	m_IncomingData.append(a_Data, a_Length);
}





void cClientHandle::OnRemoteClosed(void)
{
	{
		cCSLock Lock(m_CSOutgoingData);
		m_Link.reset();
	}
	SocketClosed();
}





void cClientHandle::OnError(int a_ErrorCode, const AString & a_ErrorMsg)
{
	LOGD("An error has occurred on client link for %s @ %s: %d (%s). Client disconnected.",
		m_Username.c_str(), m_IPString.c_str(), a_ErrorCode, a_ErrorMsg.c_str()
	);
	{
		cCSLock Lock(m_CSOutgoingData);
		m_Link.reset();
	}
	SocketClosed();
}




