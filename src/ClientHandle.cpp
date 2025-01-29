#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ClientHandle.h"
#include "BlockInfo.h"
#include "Server.h"
#include "World.h"
#include "Chunk.h"
#include "Entities/Pickup.h"
#include "Bindings/PluginManager.h"
#include "Entities/Player.h"
#include "Entities/Minecart.h"
#include "Inventory.h"
#include "BlockEntities/BeaconEntity.h"
#include "BlockEntities/ChestEntity.h"
#include "BlockEntities/CommandBlockEntity.h"
#include "BlockEntities/SignEntity.h"
#include "UI/InventoryWindow.h"
#include "UI/CraftingWindow.h"
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
#include "Protocol/Protocol.h"
#include "CompositeChat.h"
#include "Items/ItemSword.h"

#include "mbedtls/md5.h"



/** Maximum number of explosions to send this tick, server will start dropping if exceeded. */
#define MAX_EXPLOSIONS_PER_TICK 20

/** Maximum number of block change interactions a player can perform per tick - exceeding this causes a kick. */
#define MAX_BLOCK_CHANGE_INTERACTIONS 20

/** Maximum number of bytes that a chat message sent by a player may consist of. */
#define MAX_CHAT_MSG_LENGTH 1024

/** Maximum number of chunks to stream per tick. */
#define MAX_CHUNKS_STREAMED_PER_TICK 4





int cClientHandle::s_ClientCount = 0;


float cClientHandle::FASTBREAK_PERCENTAGE;

Vector3i cClientHandle::s_IllegalPosition = {0, cChunkDef::Height + 1, 0};



////////////////////////////////////////////////////////////////////////////////
// cClientHandle:

cClientHandle::cClientHandle(const AString & a_IPString, int a_ViewDistance) :
	m_CurrentViewDistance(a_ViewDistance),
	m_RequestedViewDistance(a_ViewDistance),
	m_IPString(a_IPString),
	m_Player(nullptr),
	m_CachedSentChunk(std::numeric_limits<decltype(m_CachedSentChunk.m_ChunkX)>::max(), std::numeric_limits<decltype(m_CachedSentChunk.m_ChunkZ)>::max()),
	m_ProxyConnection(false),
	m_HasSentDC(false),
	m_LastStreamedChunkX(std::numeric_limits<decltype(m_LastStreamedChunkX)>::max()),  // bogus chunk coords to force streaming upon login
	m_LastStreamedChunkZ(std::numeric_limits<decltype(m_LastStreamedChunkZ)>::max()),
	m_TicksSinceLastPacket(0),
	m_TimeSinceLastUnloadCheck(0),
	m_Ping(1000),
	m_PingID(1),
	m_BlockDigAnimStage(-1),
	m_BlockDigAnimSpeed(0),
	m_BlockDigAnimPos(s_IllegalPosition),
	m_HasStartedDigging(false),
	m_LastDigBlockPos(s_IllegalPosition),
	m_State(csConnected),
	m_NumExplosionsThisTick(0),
	m_NumBlockChangeInteractionsThisTick(0),
	m_Allowslisting(true),
	m_UniqueID(0),
	m_HasSentPlayerChunk(false),
	m_Locale("en_GB"),
	m_LastPlacedSign(s_IllegalPosition),
	m_ProtocolVersion(0)
{
	s_ClientCount++;  // Not protected by CS because clients are always constructed from the same thread
	m_UniqueID = s_ClientCount;

	LOGD("New ClientHandle created at %p", static_cast<void *>(this));
}





cClientHandle::~cClientHandle()
{
	ASSERT(m_State == csDestroyed);  // Has Destroy() been called?

	LOGD("Deleting client \"%s\" at %p", GetUsername().c_str(), static_cast<void *>(this));

	LOGD("ClientHandle at %p deleted", static_cast<void *>(this));
}





void cClientHandle::Destroy(void)
{
	if (!SetState(csDestroyed))
	{
		// Already called
		LOGD("%s: client %p, \"%s\" already destroyed, bailing out", __FUNCTION__, static_cast<void *>(this), m_Username.c_str());
		return;
	}

	LOGD("%s: destroying client %p, \"%s\" @ %s", __FUNCTION__, static_cast<void *>(this), m_Username.c_str(), m_IPString.c_str());

	{
		cCSLock Lock(m_CSOutgoingData);
		m_Protocol.HandleOutgoingData(m_OutgoingData);  // Finalise any encryption.
		m_Link->Send(m_OutgoingData.data(), m_OutgoingData.size());  // Flush remaining data.
		m_Link->Shutdown();  // Cleanly close the connection.
		m_Link.reset();  // Release the strong reference cTCPLink holds to ourself.
	}
}





AString cClientHandle::FormatChatPrefix(
	bool ShouldAppendChatPrefixes, const AString & a_ChatPrefixS,
	const AString & m_Color1, const AString & m_Color2
)
{
	if (ShouldAppendChatPrefixes)
	{
		return fmt::format(FMT_STRING("{}[{}] {}"), m_Color1, a_ChatPrefixS, m_Color2);
	}
	else
	{
		return m_Color1;
	}
}





AString cClientHandle::FormatMessageType(bool ShouldAppendChatPrefixes, eMessageType a_ChatPrefix, const AString & a_AdditionalData)
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
				return fmt::format(FMT_STRING("{}[MSG: {}] {}{}"), cChatColor::LightBlue, a_AdditionalData, cChatColor::White, cChatColor::Italic);
			}
			else
			{
				return fmt::format(FMT_STRING("{}: {}"), a_AdditionalData, cChatColor::LightBlue);
			}
		}
		case mtMaxPlusOne: break;
	}
	return "";
}





cUUID cClientHandle::GenerateOfflineUUID(const AString & a_Username)
{
	// Online UUIDs are always version 4 (random)
	// We use Version 3 (MD5 hash) UUIDs for the offline UUIDs
	// This guarantees that they will never collide with an online UUID and can be distinguished.
	// This is also consistent with the vanilla offline UUID scheme.

	return cUUID::GenerateVersion3("OfflinePlayer:" + a_Username);
}





bool cClientHandle::IsUUIDOnline(const cUUID & a_UUID)
{
	// Online UUIDs are always version 4 (random)
	// We use Version 3 (MD5 hash) UUIDs for the offline UUIDs
	// This guarantees that they will never collide with an online UUID and can be distinguished.
	return (a_UUID.Version() == 4);
}





void cClientHandle::ProxyInit(const AString & a_IPString, const cUUID & a_UUID)
{
	this->SetIPString(a_IPString);
	this->SetUUID(a_UUID);

	this->m_ProxyConnection = true;
}





void cClientHandle::ProxyInit(const AString & a_IPString, const cUUID & a_UUID, const Json::Value & a_Properties)
{
	this->SetProperties(a_Properties);
	this->ProxyInit(a_IPString, a_UUID);
}





void cClientHandle::ProcessProtocolIn(void)
{
	// Process received network data:
	decltype(m_IncomingData) IncomingData;
	{
		cCSLock Lock(m_CSIncomingData);

		// Bail out when nothing was received:
		if (m_IncomingData.empty())
		{
			return;
		}

		std::swap(IncomingData, m_IncomingData);
	}

	try
	{
		m_Protocol.HandleIncomingData(*this, IncomingData);
	}
	catch (const std::exception & Oops)
	{
		Kick(Oops.what());
	}
}





void cClientHandle::ProcessProtocolOut()
{
	decltype(m_OutgoingData) OutgoingData;
	{
		cCSLock Lock(m_CSOutgoingData);

		// Bail out when there's nothing to send to avoid TCPLink::Send overhead:
		if (m_OutgoingData.empty())
		{
			return;
		}

		std::swap(OutgoingData, m_OutgoingData);
	}

	// Due to cTCPLink's design of holding a strong pointer to ourself, we need to explicitly reset m_Link.
	// This means we need to check it's not nullptr before trying to send, but also capture the link,
	// to prevent it being reset between the null check and the Send:
	if (auto Link = m_Link; Link != nullptr)
	{
		m_Protocol.HandleOutgoingData(OutgoingData);
		Link->Send(OutgoingData.data(), OutgoingData.size());
	}
}





void cClientHandle::Kick(const AString & a_Reason)
{
	if (m_State >= csAuthenticating)  // Don't log pings
	{
		LOGINFO("Kicking player %s for \"%s\"", m_Username.c_str(), StripColorCodes(a_Reason).c_str());
	}
	SendDisconnect(a_Reason);
}





bool cClientHandle::BungeeAuthenticate()
{
	if (!m_ProxyConnection && cRoot::Get()->GetServer()->OnlyAllowBungeeCord())
	{
		Kick("You can only connect to this server using a Proxy.");

		return false;
	}

	cServer * Server = cRoot::Get()->GetServer();

	// Proxy Shared Secret Check (BungeeGuard)
	const AString & ForwardSecret = Server->GetProxySharedSecret();
	const bool AllowBungee = Server->ShouldAllowBungeeCord();
	const bool RequireForwardSecret = AllowBungee && !ForwardSecret.empty();

	if (RequireForwardSecret)
	{
		for (auto & Node : GetProperties())
		{
			if (Node.get("name", "").asString() == "bungeeguard-token")
			{
				AString SentToken = Node.get("value", "").asString();

				if (ForwardSecret.compare(SentToken) == 0)
				{
					return true;
				}

				break;
			}
		}

		Kick("Unable to authenticate.");
		return false;
	}
	else if (m_ProxyConnection)
	{
		LOG("A player connected through a proxy without requiring a forwarding secret. If open to the internet, this is very insecure!");
	}

	return true;
}





void cClientHandle::Authenticate(AString && a_Name, const cUUID & a_UUID, Json::Value && a_Properties)
{
	cCSLock Lock(m_CSState);
	/*
	LOGD("Processing authentication for client %s @ %s (%p), state = %d",
		m_Username.c_str(), m_IPString.c_str(), static_cast<void *>(this), m_State.load()
	);
	//*/

	if (m_State != csAuthenticating)
	{
		return;
	}

	ASSERT(m_Player == nullptr);

	if (!BungeeAuthenticate())
	{
		return;
	}

	m_Username = std::move(a_Name);

	// Only assign UUID and properties if not already pre-assigned (BungeeCord sends those in the Handshake packet):
	if (m_UUID.IsNil())
	{
		m_UUID = a_UUID;
	}
	if (m_Properties.empty())
	{
		m_Properties = std::move(a_Properties);
	}

	// Send login success (if the protocol supports it):
	m_Protocol->SendLoginSuccess();

	// Serverside spawned player (so data are loaded).
	std::unique_ptr<cPlayer> Player;

	try
	{
		Player = std::make_unique<cPlayer>(shared_from_this());
	}
	catch (const std::exception & Oops)
	{
		LOGWARNING("Player \"%s\" save or statistics file loading failed: %s", GetUsername().c_str(), Oops.what());
		Kick("Contact an operator.\n\nYour player's save files could not be parsed.\nTo avoid data loss you are prevented from joining.");
		return;
	}

	m_Player = Player.get();
	m_temp_player = std::move(Player);

	if (m_ProtocolVersion > static_cast<UInt32>(cProtocol::Version::v1_20))
	{
		// do nothing I guess
	}
	else  // <= 1.20
	{
		if (m_ForgeHandshake.IsForgeClient)
		{
			m_ForgeHandshake.BeginForgeHandshake(*this);
		}
		else
		{
			FinishAuthenticate();
		}
	}
}





void cClientHandle::FinishAuthenticate()
{
	/*
	LOGD("Created a new cPlayer object at %p for client %s @ %s (%p)",
		static_cast<void *>(m_Player),
		m_Username.c_str(), m_IPString.c_str(), static_cast<void *>(this)
	);
	//*/
	m_Player->SendPlayerInventoryJoin();

	cWorld * World = cRoot::Get()->GetWorld(m_Player->GetLoadedWorldName());
	if (World == nullptr)
	{
		World = cRoot::Get()->GetDefaultWorld();
	}

	// Atomically increment player count (in server thread):
	cRoot::Get()->GetServer()->PlayerCreated();

	if (!cRoot::Get()->GetPluginManager()->CallHookPlayerJoined(*m_Player))
	{
		cRoot::Get()->BroadcastChatJoin(fmt::format(FMT_STRING("{} has joined the game"), m_Username));
		LOGINFO("Player %s has joined the game", m_Username);
	}

	// TODO: this accesses the world spawn from the authenticator thread
	// World spawn should be sent in OnAddedToWorld.
	// Return a server login packet:
	m_Protocol->SendLogin(*m_Player, *World);

	// Send the player's permission level.
	// The key effect is to allow 1.9+ clients to open the command block UI.
	SendPlayerPermissionLevel();

	if (m_Player->GetKnownRecipes().empty())
	{
		SendInitRecipes(0);
	}
	else
	{
		for (const auto KnownRecipe : m_Player->GetKnownRecipes())
		{
			SendInitRecipes(KnownRecipe);
		}
	}

	// Send player list items:
	SendPlayerListAddPlayer(*m_Player);  // Add ourself
	cRoot::Get()->BroadcastPlayerListsAddPlayer(*m_Player);  // Add ourself to everyone else
	cRoot::Get()->SendPlayerLists(m_Player);  // Add everyone else to ourself

	// Send statistics:
	SendStatistics(m_Player->GetStatistics());

	// Delay the first ping until the client "settles down"
	// This should fix #889, "BadCast exception, cannot convert bit to fm" error in client
	m_PingStartTime = std::chrono::steady_clock::now() + std::chrono::seconds(3);  // Send the first KeepAlive packet in 3 seconds

	// Remove the client handle from the server, it will be ticked from its cPlayer object from now on:
	cRoot::Get()->GetServer()->ClientMovedToWorld(this);

	SetState(csDownloadingWorld);
	m_Player->Initialize(std::move(m_temp_player), *World);
	m_Player->ResendRenderDistanceCenter();
	m_Protocol->SendInitialChunksComing();
	m_Protocol->SendCommandTree();
	// LOGD("Client %s @ %s (%p) has been fully authenticated", m_Username.c_str(), m_IPString.c_str(), static_cast<void *>(this));
}





void cClientHandle::StreamNextChunks(void)
{
	ASSERT(m_Player != nullptr);

	int ChunkPosX = m_Player->GetChunkX();
	int ChunkPosZ = m_Player->GetChunkZ();

	if ((m_LastStreamedChunkX == ChunkPosX) && (m_LastStreamedChunkZ == ChunkPosZ))
	{
		// All chunks are already loaded and the player has not moved, work is done:
		return;
	}

	// Player moved chunks and / or loading is not finished, reset to bogus (GH #4531):
	m_LastStreamedChunkX = std::numeric_limits<decltype(m_LastStreamedChunkX)>::max();
	m_LastStreamedChunkZ = std::numeric_limits<decltype(m_LastStreamedChunkZ)>::max();

	int StreamedChunks = 0;
	Vector3d Position = m_Player->GetEyePosition();
	Vector3d LookVector = m_Player->GetLookVector();

	// Get the look vector and normalize it.
	LookVector.Normalize();

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
				{
					cCSLock Lock(m_CSChunkLists);
					if (
						(m_ChunksToSend.find(Coords) != m_ChunksToSend.end()) ||
						(m_LoadedChunks.find(Coords) != m_LoadedChunks.end())
					)
					{
						continue;
					}
				}

				// Unloaded chunk found -> Send it to the client.
				StreamChunk(ChunkX, ChunkZ, ((Range <= 2) ? cChunkSender::Priority::Critical : cChunkSender::Priority::Medium));

				if (++StreamedChunks == MAX_CHUNKS_STREAMED_PER_TICK)
				{
					return;
				}
			}
		}
	}

	// Low priority: Add all chunks that are in range. (From the center out to the edge)
	for (int d = 0; d <= m_CurrentViewDistance; ++d)  // cycle through (square) distance, from nearest to furthest
	{
		const auto StreamIfUnloaded = [this](const cChunkCoords Chunk)
		{
			// If the chunk already loading / loaded -> skip
			{
				cCSLock Lock(m_CSChunkLists);
				if (
					(m_ChunksToSend.find(Chunk) != m_ChunksToSend.end()) ||
					(m_LoadedChunks.find(Chunk) != m_LoadedChunks.end())
				)
				{
					return false;
				}
			}

			// Unloaded chunk found -> Send it to the client.
			StreamChunk(Chunk.m_ChunkX, Chunk.m_ChunkZ, cChunkSender::Priority::Low);
			return true;
		};

		// For each distance, send the first unloaded chunk in a hollow square centered around current position:
		for (int i = -d; i <= d; ++i)
		{
			if (StreamIfUnloaded({ ChunkPosX + d, ChunkPosZ + i }) || StreamIfUnloaded({ ChunkPosX - d, ChunkPosZ + i }))
			{
				if (++StreamedChunks == MAX_CHUNKS_STREAMED_PER_TICK)
				{
					return;
				}
			}
		}
		for (int i = -d + 1; i < d; ++i)
		{
			if (StreamIfUnloaded({ ChunkPosX + i, ChunkPosZ + d }) || StreamIfUnloaded({ ChunkPosX + i, ChunkPosZ - d }))
			{
				if (++StreamedChunks == MAX_CHUNKS_STREAMED_PER_TICK)
				{
					return;
				}
			}
		}
	}

	// All chunks are loaded -> Sets the last loaded chunk coordinates to current coordinates
	m_LastStreamedChunkX = ChunkPosX;
	m_LastStreamedChunkZ = ChunkPosZ;
}





void cClientHandle::UnloadOutOfRangeChunks(void)
{
	int ChunkPosX = FAST_FLOOR_DIV(static_cast<int>(m_Player->GetPosX()), cChunkDef::Width);
	int ChunkPosZ = FAST_FLOOR_DIV(static_cast<int>(m_Player->GetPosZ()), cChunkDef::Width);

	cChunkCoordsList ChunksToRemove;
	{
		cCSLock Lock(m_CSChunkLists);
		for (auto itr = m_LoadedChunks.begin(); itr != m_LoadedChunks.end();)
		{
			const auto DiffX = Diff((*itr).m_ChunkX, ChunkPosX);
			const auto DiffZ = Diff((*itr).m_ChunkZ, ChunkPosZ);
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
			const auto DiffX = Diff((*itr).m_ChunkX, ChunkPosX);
			const auto DiffZ = Diff((*itr).m_ChunkZ, ChunkPosZ);
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





void cClientHandle::StreamChunk(int a_ChunkX, int a_ChunkZ, cChunkSender::Priority a_Priority)
{
	cWorld * World = m_Player->GetWorld();
	ASSERT(World != nullptr);

	if (World->AddChunkClient(a_ChunkX, a_ChunkZ, this))
	{
		{
			cCSLock Lock(m_CSChunkLists);
			m_LoadedChunks.emplace(a_ChunkX, a_ChunkZ);
			m_ChunksToSend.emplace(a_ChunkX, a_ChunkZ);
		}
		World->SendChunkTo(a_ChunkX, a_ChunkZ, a_Priority, this);
	}
}





void cClientHandle::HandleAnimation(const bool a_SwingMainHand)
{
	if (cPluginManager::Get()->CallHookPlayerAnimation(*m_Player, a_SwingMainHand ? 0 : 1))
	{
		// Plugin disagrees, bail out:
		return;
	}

	m_Player->GetWorld()->BroadcastEntityAnimation(*m_Player, a_SwingMainHand ? EntityAnimation::PlayerMainHandSwings : EntityAnimation::PlayerOffHandSwings, this);
}





void cClientHandle::HandleNPCTrade(int a_SlotNum)
{
	// TODO
	LOGWARNING("%s: Not implemented yet", __FUNCTION__);
}





void cClientHandle::HandleOpenHorseInventory()
{
	m_Player->OpenHorseInventory();
}





void cClientHandle::HandlePing(void)
{
	/* TODO: unused function, handles Legacy Server List Ping
	http://wiki.vg/Protocol#Legacy_Server_List_Ping suggests that servers SHOULD handle this packet */

	// Somebody tries to retrieve information about the server
	const cServer & Server = *cRoot::Get()->GetServer();

	auto Reply = fmt::format(FMT_STRING("{}{}{}{}{}"),
		Server.GetDescription(), cChatColor::Delimiter,
		Server.GetNumPlayers(),  cChatColor::Delimiter,
		Server.GetMaxPlayers()
	);
	Kick(Reply);
}





bool cClientHandle::HandleLogin()
{
	{
		cCSLock lock(m_CSState);
		if (m_State != csConnected)
		{
			/*
			LOGD("Client %s @ %s (%p, state %d) has disconnected before logging in, bailing out of login",
				a_Username.c_str(), m_IPString.c_str(), static_cast<void *>(this), m_State.load()
			);
			//*/
			return false;
		}

		// LOGD("Handling login for client %s @ %s (%p), state = %d", a_Username.c_str(), m_IPString.c_str(), static_cast<void *>(this), m_State.load());

		// Let the plugins know about this event, they may refuse the player:
		if (cRoot::Get()->GetPluginManager()->CallHookLogin(*this, m_ProtocolVersion, GetUsername()))
		{
			SendDisconnect("Login Rejected!");
			return false;
		}

		m_State = csAuthenticating;
	}  // lock(m_CSState)

	// Schedule for authentication; until then, let the player wait (but do not block):
	cRoot::Get()->GetAuthenticator().Authenticate(GetUniqueID(), m_Username, m_Protocol->GetAuthServerID());
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





void cClientHandle::HandleCrouch(const bool a_IsCrouching)
{
	m_Player->SetCrouch(a_IsCrouching);
}





void cClientHandle::HandleEnchantItem(UInt8 a_WindowID, UInt8 a_Enchantment)
{
	if (a_Enchantment > 2)
	{
		LOGD("Player \"%s\" tried to select an invalid enchantment - hacked client?", m_Username.c_str());
		return;
	}

	// Bail out if something's wrong with the window:
	if (
		(m_Player->GetWindow() == nullptr) ||
		(m_Player->GetWindow()->GetWindowID() != a_WindowID) ||
		(m_Player->GetWindow()->GetWindowType() != cWindow::wtEnchantment)
	)
	{
		LOGD("Player \"%s\" tried to enchant without a valid window - hacked client?", m_Username.c_str());
		return;
	}

	cEnchantingWindow * Window = static_cast<cEnchantingWindow *>(m_Player->GetWindow());
	const auto BaseEnchantmentLevel = Window->GetProperty(a_Enchantment);

	// Survival players must be checked they can afford enchantment and have lapis removed:
	if (!m_Player->IsGameModeCreative())
	{
		const auto XpRequired = m_Player->XpForLevel(BaseEnchantmentLevel);
		auto LapisStack = *Window->m_SlotArea->GetSlot(1, *m_Player);  // A copy of the lapis stack.
		const auto LapisRequired = a_Enchantment + 1;

		// Only allow enchantment if the player has sufficient levels and lapis to enchant:
		if ((m_Player->GetCurrentXp() >= XpRequired) && (LapisStack.m_ItemCount >= LapisRequired))
		{
			// We need to reduce the player's level by the number of lapis required.
			// However we need to keep the resulting percentage filled the same.

			const auto TargetLevel = m_Player->GetXpLevel() - LapisRequired;
			const auto CurrentFillPercent = m_Player->GetXpPercentage();

			// The experience to remove in order to reach the start (0% fill) of the target level.
			const auto DeltaForLevel = -m_Player->GetCurrentXp() + m_Player->XpForLevel(TargetLevel);

			// The experience to add to get the same fill percent.
			const auto DeltaForPercent = CurrentFillPercent * (m_Player->XpForLevel(TargetLevel + 1) - m_Player->XpForLevel(TargetLevel));

			// Apply the experience delta, rounded for greater accuracy:
			m_Player->DeltaExperience(static_cast<int>(std::lround(DeltaForLevel + DeltaForPercent)));

			// Now reduce the lapis in our stack and send it back:
			LapisStack.AddCount(static_cast<char>(-LapisRequired));
			Window->m_SlotArea->SetSlot(1, *m_Player, LapisStack);
		}
		else
		{
			// Not creative and can't afford enchantment, so exit:
			LOGD("Player \"%s\" selected unavailable enchantment - hacked client?", m_Username.c_str());
			return;
		}
	}

	// The enchanted item corresponding to our chosen option (top, middle, bottom).
	cItem EnchantedItem = Window->m_SlotArea->SelectEnchantedOption(a_Enchantment);

	// Set the item slot to our new enchanted item:
	Window->m_SlotArea->SetSlot(0, *m_Player, EnchantedItem);
	m_Player->PermuteEnchantmentSeed();
}





void cClientHandle::HandlePlayerAbilities(bool a_IsFlying, float FlyingSpeed, float WalkingSpeed)
{
	UNUSED(FlyingSpeed);  // Ignore the client values for these
	UNUSED(WalkingSpeed);

	m_Player->SetFlying(a_IsFlying);
}





void cClientHandle::HandlePluginMessage(const AString & a_Channel, const ContiguousByteBufferView a_Message)
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
	else if (a_Channel == "FML|HS")
	{
		m_ForgeHandshake.DataReceived(*this, a_Message);
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





AStringVector cClientHandle::BreakApartPluginChannels(const ContiguousByteBufferView a_PluginChannels)
{
	// Break the string on each NUL character.
	// Note that StringSplit() doesn't work on this because NUL is a special char - string terminator
	size_t len = a_PluginChannels.size();
	size_t first = 0;
	AStringVector res;
	for (size_t i = 0; i < len; i++)
	{
		if (a_PluginChannels[i] != std::byte(0))
		{
			continue;
		}
		if (i > first)
		{
			const auto Part = a_PluginChannels.substr(first, i - first);
			res.emplace_back(reinterpret_cast<const char *>(Part.data()), Part.size());
		}
		first = i + 1;
	}  // for i - a_PluginChannels[]
	if (first < len)
	{
		const auto Part = a_PluginChannels.substr(first, len - first);
		res.emplace_back(reinterpret_cast<const char *>(Part.data()), Part.size());
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





void cClientHandle::HandleBeaconSelection(unsigned a_PrimaryEffect, unsigned a_SecondaryEffect)
{
	cWindow * Window = m_Player->GetWindow();
	if ((Window == nullptr) || (Window->GetWindowType() != cWindow::wtBeacon))
	{
		return;
	}
	cBeaconWindow * BeaconWindow = static_cast<cBeaconWindow *>(Window);

	if (Window->GetSlot(*m_Player, 0)->IsEmpty())
	{
		return;
	}

	cEntityEffect::eType PrimaryEffect = cEntityEffect::effNoEffect;
	if (a_PrimaryEffect <= static_cast<int>(cEntityEffect::effSaturation))
	{
		PrimaryEffect = static_cast<cEntityEffect::eType>(a_PrimaryEffect);
	}
	cEntityEffect::eType SecondaryEffect = cEntityEffect::effNoEffect;
	if (a_SecondaryEffect <= static_cast<int>(cEntityEffect::effSaturation))
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





void cClientHandle::HandleCommandBlockBlockChange(Vector3i a_BlockPos, const AString & a_NewCommand, CommandBlockType a_Type, bool a_TrackOutput, bool a_Conditional, bool a_AlwaysActive)
{
	if (a_NewCommand.empty())
	{
		LOGD("Player \"%s\" send an empty command block string - hacked client?", m_Username.c_str());
		return;
	}

	if ((m_Player == nullptr) || !m_Player->HasPermission("cuberite.commandblock.set"))
	{
		SendChat("You cannot edit command blocks on this server", mtFailure);
		return;
	}

	cWorld * World = m_Player->GetWorld();
	if (World->AreCommandBlocksEnabled())
	{
		World->SetCommandBlockCommand(a_BlockPos, a_NewCommand);
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
		static_cast<cAnvilWindow *>(m_Player->GetWindow())->SetRepairedItemName(a_ItemName, m_Player);
	}
}





void cClientHandle::HandleLeftClick(Vector3i a_BlockPos, eBlockFace a_BlockFace, UInt8 a_Status)
{
	FLOGD("HandleLeftClick: {0}; Face: {1}; Stat: {2}",
		a_BlockPos, a_BlockFace, a_Status
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
		if (
			const auto InterferingPosition = AddFaceDirection(a_BlockPos, a_BlockFace);
			cChunkDef::IsValidHeight(InterferingPosition) && cBlockInfo::IsClickedThrough(m_Player->GetWorld()->GetBlock(InterferingPosition))
		)
		{
			a_BlockPos = InterferingPosition;
		}

		if (!IsWithinReach(a_BlockPos))
		{
			m_Player->SendBlocksAround(a_BlockPos, 2);
			return;
		}
	}

	cPluginManager * PlgMgr = cRoot::Get()->GetPluginManager();
	if (m_Player->IsFrozen() || PlgMgr->CallHookPlayerLeftClick(*m_Player, a_BlockPos, a_BlockFace, static_cast<char>(a_Status)))
	{
		// A plugin doesn't agree with the action, replace the block on the client and quit:
		m_Player->SendBlocksAround(a_BlockPos, 2);
		SendPlayerPosition();  // Prevents the player from falling through the block that was temporarily broken client side.
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
			auto & ItemHandler = m_Player->GetEquippedItem().GetHandler();
			if (ItemHandler.IsFood() || ItemHandler.IsDrinkable(m_Player->GetEquippedItem().m_ItemDamage))
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
				// When bow is in off-hand / shield slot
				if (m_Player->GetInventory().GetShieldSlot().m_ItemType == Item::Bow)
				{
					m_Player->GetInventory().GetShieldSlot().GetHandler().OnItemShoot(m_Player, a_BlockPos, a_BlockFace);
				}
				else
				{
					ItemHandler.OnItemShoot(m_Player, a_BlockPos, a_BlockFace);
				}
			}
			return;
		}

		case DIG_STATUS_STARTED:
		{
			HandleBlockDigStarted(a_BlockPos, a_BlockFace);
			return;
		}

		case DIG_STATUS_FINISHED:
		{
			HandleBlockDigFinished(a_BlockPos, a_BlockFace);
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

		case DIG_STATUS_SWAP_ITEM_IN_HAND:
		{

			cItem EquippedItem = m_Player->GetEquippedItem();
			cItem OffhandItem = m_Player->GetOffHandEquipedItem();

			cInventory & Inventory = m_Player->GetInventory();
			Inventory.SetShieldSlot(EquippedItem);
			Inventory.SetEquippedItem(OffhandItem);

			return;
		}

		default:
		{
			ASSERT(!"Unhandled DIG_STATUS");
			return;
		}
	}  // switch (a_Status)
}





void cClientHandle::HandleBlockDigStarted(Vector3i a_BlockPos, eBlockFace a_BlockFace)
{
	if (m_Player->IsGameModeAdventure())
	{
		// Players in adventure mode can't destroy blocks
		return;
	}

	if (
		m_HasStartedDigging && (a_BlockPos == m_LastDigBlockPos)
	)
	{
		// It is a duplicate packet, drop it right away
		return;
	}

	BlockState DiggingBlock;

	if (
		m_Player->GetWorld()->GetBlock(a_BlockPos, DiggingBlock) &&m_Player->IsGameModeCreative() &&
		ItemCategory::IsSword(m_Player->GetInventory().GetEquippedItem().m_ItemType) &&
		(DiggingBlock.Type() != BlockType::Fire)
	)
	{
		// Players can't destroy blocks with a sword in the hand.
		return;
	}

	// Set the last digging coords to the block being dug, so that they can be checked in DIG_FINISHED to avoid dig / aim bug in the client:
	m_HasStartedDigging = true;
	m_LastDigBlockPos = a_BlockPos;

	if (
		m_Player->IsGameModeCreative() ||         // In creative mode, digging is done immediately
		m_Player->CanInstantlyMine(DiggingBlock)  // Sometimes the player is fast enough to instantly mine
	)
	{
		// Immediately done:
		m_BreakProgress = 1;

		HandleBlockDigFinished(a_BlockPos, a_BlockFace);
		return;
	}

	m_BreakProgress = 0;

	// Start dig animation
	// TODO: calculate real animation speed
	// TODO: Send animation packets even without receiving any other packets
	m_BlockDigAnimSpeed = 10;
	m_BlockDigAnimPos = a_BlockPos;
	m_BlockDigAnimStage = 0;
	m_Player->GetWorld()->BroadcastBlockBreakAnimation(static_cast<UInt32>(m_UniqueID), m_BlockDigAnimPos, 0, this);

	cWorld * World = m_Player->GetWorld();
	cChunkInterface ChunkInterface(World->GetChunkMap());
	cBlockHandler::For(DiggingBlock.Type()).OnDigging(ChunkInterface, *World, *m_Player, a_BlockPos);

	m_Player->GetEquippedItem().GetHandler().OnDiggingBlock(World, m_Player, m_Player->GetEquippedItem(), a_BlockPos, a_BlockFace);
}





void cClientHandle::HandleBlockDigFinished(Vector3i a_BlockPos, eBlockFace a_BlockFace)
{
	if (
		!m_HasStartedDigging ||           // Hasn't received the DIG_STARTED packet
		(m_LastDigBlockPos != a_BlockPos)  // DIG_STARTED has had different pos
	)
	{
		FLOGD("Prevented a dig / aim bug in the client (finish {0} vs start {1}, HSD: {2})",
			a_BlockPos,
			m_LastDigBlockPos,
			(m_HasStartedDigging ? "True" : "False")
		);
		return;
	}

	FinishDigAnimation();

	BlockState DugBlock;
	if (!m_Player->GetWorld()->GetBlock(a_BlockPos, DugBlock))
	{
		return;
	}

	if (!m_Player->IsGameModeCreative())
	{
		m_BreakProgress += m_Player->GetMiningProgressPerTick(DugBlock);

		// Check for very fast tools. Maybe instead of FASTBREAK_PERCENTAGE we should check we are within x multiplied by the progress per tick
		if (m_BreakProgress < FASTBREAK_PERCENTAGE)
		{
			LOGD("Break progress of player %s was less than expected: %f < %f\n", m_Player->GetName().c_str(), m_BreakProgress * 100, FASTBREAK_PERCENTAGE * 100);
			// AntiFastBreak doesn't agree with the breaking. Bail out. Send the block back to the client, so that it knows:
			m_Player->SendBlocksAround(a_BlockPos, 2);
			SendPlayerPosition();  // Prevents the player from falling through the block that was temporarily broken client side.
			m_Player->SendMessage("FastBreak?");  // TODO Anticheat hook
			return;
		}
	}

	cWorld * World = m_Player->GetWorld();

	if (cRoot::Get()->GetPluginManager()->CallHookPlayerBreakingBlock(*m_Player, a_BlockPos, a_BlockFace, DugBlock))
	{
		// A plugin doesn't agree with the breaking. Bail out. Send the block back to the client, so that it knows:
		m_Player->SendBlocksAround(a_BlockPos, 2);
		SendPlayerPosition();  // Prevents the player from falling through the block that was temporarily broken client side.
		return;
	}

	if (IsBlockAir(DugBlock))
	{
		return;
	}

	// Apply hunger:
	m_Player->AddFoodExhaustion(0.025);

	// Damage the tool, but not for 0 hardness blocks:
	m_Player->UseEquippedItem(cBlockInfo::IsOneHitDig(DugBlock) ? cItemHandler::dlaBreakBlockInstant : cItemHandler::dlaBreakBlock);

	cChunkInterface ChunkInterface(World->GetChunkMap());
	Vector3i absPos(a_BlockPos);
	if (m_Player->IsGameModeSurvival())
	{
		World->DropBlockAsPickups(absPos, m_Player, &m_Player->GetEquippedItem());
	}
	else
	{
		World->DigBlock(absPos, m_Player);
	}

	World->BroadcastSoundParticleEffect(EffectID::PARTICLE_BLOCK_BREAK, absPos, 0, this);
	cRoot::Get()->GetPluginManager()->CallHookPlayerBrokenBlock(*m_Player, a_BlockPos, a_BlockFace, DugBlock);
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
		m_Player->GetWorld()->BroadcastBlockBreakAnimation(static_cast<UInt32>(m_UniqueID), m_LastDigBlockPos, 10, this);
	}

	m_LastDigBlockPos = s_IllegalPosition;
}





void cClientHandle::HandleRightClick(Vector3i a_BlockPos, eBlockFace a_BlockFace, Vector3i a_CursorPos, bool a_UsedMainHand)
{
	/* This function handles three actions:
	(1) Place a block;
	(2) "Use" a block: Interactive with the block, like opening a chest/crafting table/furnace;
	(3) Use the held item targeting a block. E.g. farming.

	Sneaking player will not use the block if hand is not empty.
	Frozen player can do nothing.
	In Game Mode Spectator, player cannot use item or place block, but can interactive with some block depending on cBlockInfo::IsUseableBySpectator(BlockType)

	If the action failed, we need to send an update of the placed block or inventory to the client.

	Actions rejected by plugin will not lead to other attempts.
	E.g., when opening a chest with a dirt in hand, if the plugin rejects opening the chest, the dirt will not be placed. */

	if ((a_BlockFace == BLOCK_FACE_NONE) || !cChunkDef::IsValidHeight(a_BlockPos))
	{
		LOGD("Player \"%s\" sent an invalid click - hacked client?", m_Username.c_str());
		return;
	}

	// TODO: We are still consuming the items in main hand. Remove this override when the off-hand consumption is handled correctly.
	a_UsedMainHand = true;

	cWorld * World = m_Player->GetWorld();
	cPluginManager * PlgMgr = cRoot::Get()->GetPluginManager();
	const cItem & HeldItem = a_UsedMainHand ? m_Player->GetEquippedItem() : m_Player->GetInventory().GetShieldSlot();

	FLOGD("HandleRightClick: {0}, face {1}, Cursor {2}, Hand: {3}, HeldItem: {4}", a_BlockPos, a_BlockFace, a_CursorPos, a_UsedMainHand, ItemToFullString(HeldItem));

	if (!PlgMgr->CallHookPlayerRightClick(*m_Player, a_BlockPos, a_BlockFace, a_CursorPos) && IsWithinReach(a_BlockPos) && !m_Player->IsFrozen())
	{
		auto ClickedBlock = World->GetBlock(a_BlockPos);
		const auto & BlockHandler = cBlockHandler::For(ClickedBlock.Type());
		const auto & ItemHandler = HeldItem.GetHandler();
		const bool BlockUsable = BlockHandler.IsUseable() && (m_Player->IsGameModeSpectator() ? cBlockInfo::IsUseableBySpectator(ClickedBlock) : !(m_Player->IsCrouched() && !HeldItem.IsEmpty()));
		const bool ItemPlaceable = ItemHandler.IsPlaceable() && !m_Player->IsGameModeAdventure() && !m_Player->IsGameModeSpectator();
		const bool ItemUseable = !m_Player->IsGameModeSpectator();

		if (BlockUsable)
		{
			cChunkInterface ChunkInterface(World->GetChunkMap());
			if (!PlgMgr->CallHookPlayerUsingBlock(*m_Player, a_BlockPos, a_BlockFace, a_CursorPos, ClickedBlock))
			{
				// Use a block:
				if (BlockHandler.OnUse(ChunkInterface, *World, *m_Player, a_BlockPos, a_BlockFace, a_CursorPos))
				{
					PlgMgr->CallHookPlayerUsedBlock(*m_Player, a_BlockPos, a_BlockFace, a_CursorPos, ClickedBlock);
					return;  // Block use was successful, we're done.
				}

				// If block use failed, fall back to placement:
				if (ItemPlaceable)
				{
					// Place a block:
					ItemHandler.OnPlayerPlace(*m_Player, HeldItem, a_BlockPos, ClickedBlock, a_BlockFace, a_CursorPos);
				}

				return;
			}
		}
		else if (ItemPlaceable)
		{
			// TODO: Double check that we don't need this for using item and for packet out of range
			m_NumBlockChangeInteractionsThisTick++;
			if (!CheckBlockInteractionsRate())
			{
				Kick("Too many blocks were placed / interacted with per unit time - hacked client?");
				return;
			}

			// Place a block:
			ItemHandler.OnPlayerPlace(*m_Player, HeldItem, a_BlockPos, ClickedBlock, a_BlockFace, a_CursorPos);
			return;
		}
		else if (ItemUseable)
		{
			if (!PlgMgr->CallHookPlayerUsingItem(*m_Player, a_BlockPos, a_BlockFace, a_CursorPos))
			{
				// All plugins agree with using the item.
				// Use an item in hand with a target block.

				cBlockInServerPluginInterface PluginInterface(*World);
				ItemHandler.OnItemUse(World, m_Player, PluginInterface, HeldItem, a_BlockPos, a_BlockFace);
				PlgMgr->CallHookPlayerUsedItem(*m_Player, a_BlockPos, a_BlockFace, a_CursorPos);
				return;
			}
		}
		else
		{
			// (x) None of the above.
			return;
		}
	}

	// TODO: delete OnItemUse bool return, delete onCancelRightClick

	// Update the target block including the block above and below for 2 block high things:
	m_Player->SendBlocksAround(a_BlockPos, 2);

	// TODO: Send corresponding slot based on hand
	m_Player->GetInventory().SendEquippedSlot();
}





void cClientHandle::HandleChat(const AString & a_Message)
{
	if ((a_Message.size()) > MAX_CHAT_MSG_LENGTH)
	{
		LOGD("Player \"%s\" sent a chat message exceeding the maximum length - hacked client?", m_Username.c_str());
		return;
	}

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
	Msg.AddTextPart("<");
	Msg.ParseText(m_Player->GetPrefix());
	Msg.AddTextPart(m_Player->GetName(), Color);
	Msg.ParseText(m_Player->GetSuffix());
	Msg.AddTextPart("> ");
	if (m_Player->HasPermission("cuberite.chat.format"))
	{
		Msg.ParseText(Message);
	}
	else
	{
		Msg.AddTextPart(Message);
	}
	Msg.UnderlineUrls();
	cRoot::Get()->BroadcastChat(Msg);
}





void cClientHandle::HandlePlayerLook(float a_Rotation, float a_Pitch, bool a_IsOnGround)
{
	m_Player->SetYaw        (a_Rotation);
	m_Player->SetHeadYaw    (a_Rotation);
	m_Player->SetPitch      (a_Pitch);
	m_Player->SetTouchGround(a_IsOnGround);
}





void cClientHandle::HandlePlayerMove(Vector3d a_Pos, bool a_IsOnGround)
{
	const Vector3d OldPosition = GetPlayer()->GetPosition();
	const auto PreviousIsOnGround = GetPlayer()->IsOnGround();

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfloat-equal"
#endif

	if (
		(OldPosition == a_Pos) &&
		(PreviousIsOnGround == a_IsOnGround)
	)
	{
		// Nothing changed, no need to do anything:
		return;
	}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

	if (m_Player->IsFrozen() || (m_Player->GetHealth() <= 0))
	{
		// "Repair" if the client tries to move while frozen or dead:
		SendPlayerMoveLook();
		return;
	}

	// If the player has moved too far, "repair" them:
	if ((OldPosition - a_Pos).SqrLength() > 100 * 100)
	{
		LOGD("Too far away (%0.2f), \"repairing\" the client", (OldPosition - a_Pos).Length());
		SendPlayerMoveLook();
		return;
	}

	if (cRoot::Get()->GetPluginManager()->CallHookPlayerMoving(*m_Player, OldPosition, a_Pos, PreviousIsOnGround))
	{
		SendPlayerMoveLook();
		return;
	}

	// TODO: should do some checks to see if player is not moving through terrain
	// TODO: Official server refuses position packets too far away from each other, kicking "hacked" clients; we should, too

	m_Player->SetPosition(a_Pos);
	m_Player->SetTouchGround(a_IsOnGround);
	m_Player->UpdateMovementStats(a_Pos - OldPosition, PreviousIsOnGround);
}





void cClientHandle::HandlePlayerMoveLook(Vector3d a_Pos, float a_Rotation, float a_Pitch, bool a_IsOnGround)
{
	HandlePlayerMove(a_Pos, a_IsOnGround);
	HandlePlayerLook(a_Rotation, a_Pitch, a_IsOnGround);
}





void cClientHandle::HandleSlotSelected(Int16 a_SlotNum)
{
	m_Player->GetInventory().SetEquippedSlotNum(a_SlotNum);
	m_Player->GetWorld()->BroadcastEntityEquipment(*m_Player, 0, m_Player->GetInventory().GetEquippedItem(), this);
}





void cClientHandle::HandleSpectate(const cUUID & a_PlayerUUID)
{
	if (!m_Player->IsGameModeSpectator())
	{
		LOGD("Player \"%s\" tried to spectate when not in spectator mode - hacked client?", m_Username.c_str());
		return;
	}

	m_Player->GetWorld()->DoWithPlayerByUUID(a_PlayerUUID, [=](cPlayer & a_ToSpectate)
	{
		m_Player->TeleportToEntity(a_ToSpectate);
		return true;
	});
}





void cClientHandle::HandleSprint(const bool a_IsSprinting)
{
	m_Player->SetSprint(a_IsSprinting);
}





void cClientHandle::HandleStartElytraFlight()
{
	m_Player->SetElytraFlight(true);
}





void cClientHandle::HandleSteerVehicle(float a_Forward, float a_Sideways)
{
	m_Player->SteerVehicle(a_Forward, a_Sideways);
}





void cClientHandle::HandleWindowClose(UInt8 a_WindowID)
{
	m_Player->CloseWindowIfID(static_cast<char>(a_WindowID));
}





void cClientHandle::HandlePlayerSession(cUUID a_SessionID, Int64 a_ExpiresAt, const ContiguousByteBuffer & a_PublicKey, const ContiguousByteBuffer & a_KeySignature)
{
	// hash algorith mchnages somehre between 1.19.4 and 1.21 - maybe???

	// TODO: tidy up and make more readable

	char * tempbfr = new char[a_PublicKey.size() + 16 + 8];
	// ORDER: player UUID (in binary form big endian format) + ExpiresAt in big endian + publickey
	memcpy(tempbfr, GetUUID().ToRaw().data(), 16);
	Int64 toadd = (static_cast<Int64>(htonl(static_cast<UInt32>(a_ExpiresAt))) << 32) | (static_cast<Int64>(htonl(static_cast<UInt32>(a_ExpiresAt >> 32))));
	*(reinterpret_cast<Int64 *>(tempbfr + 16)) = toadd;
	ContiguousByteBuffer toverify;
	toverify.append(reinterpret_cast<std::byte*>(tempbfr), 24);
	toverify.append(a_PublicKey);
	delete[] tempbfr;
	if (!cRoot::Get()->GetMojangAPI().VerifyUsingMojangKeys(toverify, a_KeySignature))
	{
		LOGWARN("Invalid public key sent by %s", GetUsername());
		// TODO: add enforce secure profile somewhere in settings and kick player if the key is invalid
		return;
	}
	Int64 milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	if (milliseconds_since_epoch > a_ExpiresAt)
	{
		LOGWARN("Expired public key sent by %s", GetUsername());
		// TODO: add enforce secure profile somewhere in settings and kick player if the key is invalid
		return;
	}
	m_PlayerSession = cClientHandle::cPlayerSessionData(a_SessionID, a_ExpiresAt, a_PublicKey, a_KeySignature);
	cRoot::Get()->ForEachPlayer([this](const cPlayer & a_Player)
	{
		a_Player.GetClientHandle()->SendPlayerListInitChat(*this->GetPlayer());
		return false;
	});
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
	m_Player->AddKnownItem(a_HeldItem);
	Window->Clicked(*m_Player, a_WindowID, a_SlotNum, a_ClickAction, a_HeldItem);
}





void cClientHandle::HandleUpdateSign(
	Vector3i a_BlockPos,
	const AString & a_Line1, const AString & a_Line2,
	const AString & a_Line3, const AString & a_Line4
)
{
	if (m_LastPlacedSign.Equals(a_BlockPos))
	{
		m_LastPlacedSign.Set(0, -1, 0);
		m_Player->GetWorld()->SetSignLines(a_BlockPos, a_Line1, a_Line2, a_Line3, a_Line4, m_Player);
	}
}





void cClientHandle::HandleUseEntity(UInt32 a_TargetEntityID, bool a_IsLeftClick)
{
	// TODO: Let plugins interfere via a hook

	// If the player is a spectator, let him spectate
	if (m_Player->IsGameModeSpectator() && a_IsLeftClick)
	{
		m_Player->GetWorld()->DoWithEntityByID(a_TargetEntityID, [=](cEntity & a_Entity)
		{
			m_Player->SpectateEntity(&a_Entity);
			return true;
		});
		return;
	}

	// If it is a right click, call the entity's OnRightClicked() handler:
	if (!a_IsLeftClick)
	{
		cWorld * World = m_Player->GetWorld();
		World->DoWithEntityByID(a_TargetEntityID, [=](cEntity & a_Entity)
			{
				if (
					cPluginManager::Get()->CallHookPlayerRightClickingEntity(*m_Player, a_Entity) ||
					(
						m_Player->IsGameModeSpectator() &&  // Spectators cannot interact with every entity
						(
							!a_Entity.IsMinecart() ||  // They can only interact with minecarts
							(
								(static_cast<cMinecart &>(a_Entity).GetPayload() != cMinecart::mpChest) &&  // And only if the type matches a minecart with a chest or
								(static_cast<cMinecart &>(a_Entity).GetPayload() != cMinecart::mpHopper)    // a minecart with a hopper
							)
						)
					)
				)
				{
					return false;
				}
				a_Entity.OnRightClicked(*m_Player);
				return false;
			}
		);
		return;
	}

	// If it is a left click, attack the entity:
	m_Player->GetWorld()->DoWithEntityByID(a_TargetEntityID, [=](cEntity & a_Entity)
		{
			if (!a_Entity.GetWorld()->IsPVPEnabled())
			{
				// PVP is disabled, disallow players hurting other players:
				if (a_Entity.IsPlayer())
				{
					// Player is hurting another player which is not allowed when PVP is disabled so ignore it
					return true;
				}
			}
			a_Entity.TakeDamage(*m_Player);
			m_Player->AddFoodExhaustion(0.3);
			if (a_Entity.IsPawn())
			{
				m_Player->NotifyNearbyWolves(static_cast<cPawn*>(&a_Entity), true);
			}
			return true;
		}
	);
}





void cClientHandle::HandleUseItem(bool a_UsedMainHand)
{
	// Use the held item without targeting a block: eating, drinking, charging a bow, using buckets
	// In version 1.8.x, this function shares the same packet id with HandleRightClick.
	// In version >= 1.9, there is a new packet id for "Use Item".

	// TODO: We are still consuming the items in main hand. Remove this override when the off-hand consumption is handled correctly.
	a_UsedMainHand = true;
	const cItem & HeldItem = a_UsedMainHand ? m_Player->GetEquippedItem() : m_Player->GetInventory().GetShieldSlot();
	auto & ItemHandler = HeldItem.GetHandler();
	cWorld * World = m_Player->GetWorld();
	cPluginManager * PlgMgr = cRoot::Get()->GetPluginManager();

	LOGD("HandleUseItem: Hand: %d; HeldItem: %s", a_UsedMainHand, ItemToFullString(HeldItem).c_str());

	constexpr Vector3i DefaultBlockPos(-1, 255, -1);
	constexpr Vector3i DefaultCursorPos(0, 0, 0);

	if (PlgMgr->CallHookPlayerRightClick(*m_Player, DefaultBlockPos, BLOCK_FACE_NONE, DefaultCursorPos))
	{
		return;  // Plugin denied click action
	}

	// Use item in main / off hand
	// TODO: do we need to sync the current inventory with client if it fails?
	if (m_Player->IsFrozen() || m_Player->IsGameModeSpectator())
	{
		return;
	}

	if (ItemHandler.IsFood() || ItemHandler.IsDrinkable(HeldItem.m_ItemDamage))
	{
		if (
			ItemHandler.IsFood() &&
			(m_Player->IsSatiated() || m_Player->IsGameModeCreative()) &&  // Only non-creative or hungry players can eat
			(HeldItem.m_ItemType != Item::GoldenApple) &&  // Golden apple is a special case, it is used instead of eaten
			(HeldItem.m_ItemType != Item::ChorusFruit)     // Chorus fruit is a special case, it is used instead of eaten
		)
		{
			// The player is satiated or in creative, and trying to eat
			return;
		}
		if (!PlgMgr->CallHookPlayerEating(*m_Player))
		{
			m_Player->StartEating();
		}
	}
	else
	{
		// Use an item in hand without a target block
		if (!PlgMgr->CallHookPlayerUsingItem(*m_Player, DefaultBlockPos, BLOCK_FACE_NONE, DefaultCursorPos))
		{
			// All plugins agree with using the item
			cBlockInServerPluginInterface PluginInterface(*World);
			ItemHandler.OnItemUse(World, m_Player, PluginInterface, HeldItem, DefaultBlockPos, BLOCK_FACE_NONE);
			PlgMgr->CallHookPlayerUsedItem(*m_Player, DefaultBlockPos, BLOCK_FACE_NONE, DefaultCursorPos);
		}
	}
}





void cClientHandle::HandleResourcePack(UInt8 a_Status)
{
	// Kick player if client declined the resource pack
	if ((a_Status == 1) && cRoot::Get()->GetServer()->ShouldRequireResourcePack())
	{
		Kick("You must accept the resource pack");
	}
}





void cClientHandle::HandleRespawn(void)
{
	if (m_Player->GetHealth() > 0)
	{
		LOGD("Player \"%s\" tried to respawn while alive - hacked client?", m_Username.c_str());
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

	// Check if the player is in any World.
	if (cRoot::Get()->DoWithPlayer(a_Username, [](cPlayer &) { return true; }))
	{
		Kick("A player of the username is already logged in");
		return false;
	}
	return true;
}





bool cClientHandle::HandleHandshake(const AString & a_Username)
{
	if (a_Username.length() > 16)
	{
		Kick("Your username is too long (>16 characters)");
		return false;
	}

	if (cRoot::Get()->GetPluginManager()->CallHookHandshake(*this, a_Username))
	{
		Kick("Entry denied by plugin");
		return false;
	}

	return CheckMultiLogin(a_Username);
}





void cClientHandle::HandleLeaveBed()
{
	cChunkInterface Interface(m_Player->GetWorld()->GetChunkMap());
	cBlockBedHandler::VacateBed(Interface, *m_Player);
}





void cClientHandle::HandleUnmount(void)
{
	m_Player->Detach();
}





void cClientHandle::HandleTabCompletion(const AString & a_Text, UInt32 CompletionId)
{
	AStringVector Results;
	// Get player name completions.
	if (cRoot::Get()->GetServer()->ShouldAllowMultiWorldTabCompletion())
	{
		Results = cRoot::Get()->GetPlayerTabCompletionMultiWorld(a_Text);
	}
	else
	{
		m_Player->GetWorld()->TabCompleteUserName(a_Text, Results);
	}

	// Get command completions.
	cRoot::Get()->GetPluginManager()->TabCompleteCommand(a_Text, Results, m_Player);
	if (Results.empty())
	{
		return;
	}

	// Sort and send results.
	std::sort(Results.begin(), Results.end());
	SendTabCompletionResults(Results, CompletionId);
}





void cClientHandle::SendData(const ContiguousByteBufferView a_Data)
{
	if (m_HasSentDC)
	{
		// This could crash the client, because they've already unloaded the world etc., and suddenly a wild packet appears (#31)
		return;
	}

	// LOG("len %d", a_Data.length());

	cCSLock Lock(m_CSOutgoingData);
	m_OutgoingData += a_Data;
}





void cClientHandle::RemoveFromWorld(void)
{
	// Remove all associated chunks:
	{
		cCSLock Lock(m_CSChunkLists);
		m_LoadedChunks.clear();
		m_ChunksToSend.clear();
		m_SentChunks.clear();
	}

	// Flush outgoing data:
	ProcessProtocolOut();

	// No need to send Unload Chunk packets, the client unloads automatically.

	// Here, we set last streamed values to bogus ones so everything is resent:
	m_LastStreamedChunkX = std::numeric_limits<decltype(m_LastStreamedChunkX)>::max();
	m_LastStreamedChunkZ = std::numeric_limits<decltype(m_LastStreamedChunkZ)>::max();

	// Restart player unloaded chunk checking and freezing:
	m_CachedSentChunk = cChunkCoords(std::numeric_limits<decltype(m_CachedSentChunk.m_ChunkX)>::max(), std::numeric_limits<decltype(m_CachedSentChunk.m_ChunkZ)>::max());
}





bool cClientHandle::IsPlayerChunkSent()
{
	return m_HasSentPlayerChunk;
}





bool cClientHandle::CheckBlockInteractionsRate(void)
{
	ASSERT(m_Player != nullptr);
	ASSERT(m_Player->GetWorld() != nullptr);

	if (!cRoot::Get()->GetServer()->ShouldLimitPlayerBlockChanges())
	{
		return true;
	}

	return (m_NumBlockChangeInteractionsThisTick <= MAX_BLOCK_CHANGE_INTERACTIONS);
}





bool cClientHandle::IsWithinReach(const Vector3i a_Position) const
{
	// Distance from the block's center to the player's eye height.
	const double Distance = (Vector3d(0.5, 0.5, 0.5) + a_Position - m_Player->GetEyePosition()).SqrLength();

	// _X 2014-11-25: I've maxed at 5.26 with a Survival client and 5.78 with a Creative client in my tests.
	return Distance <= (m_Player->IsGameModeCreative() ? 33.4084 : 27.6676);
}





void cClientHandle::Tick(std::chrono::milliseconds a_Dt)
{
	using namespace std::chrono_literals;

	if (IsDestroyed())
	{
		return;
	}

	m_TicksSinceLastPacket += 1;
	if (m_TicksSinceLastPacket > 600)  // 30 seconds time-out
	{
		SendDisconnect("Nooooo!! You timed out! D: Come back!");
		return;
	}

	// Freeze the player if they are standing in a chunk not yet sent to the client
	m_HasSentPlayerChunk = false;
	if (m_Player->GetParentChunk() != nullptr)
	{
		// If the chunk is invalid, it has definitely not been sent to the client yet
		if (m_Player->GetParentChunk()->IsValid())
		{
			// Before iterating m_SentChunks, see if the player's coords equal m_CachedSentChunk
			// If so, the chunk has been sent to the client. This is an optimization that saves an iteration of m_SentChunks.
			if (cChunkCoords(m_Player->GetChunkX(), m_Player->GetChunkZ()) == m_CachedSentChunk)
			{
				m_HasSentPlayerChunk = true;
			}
			else
			{
				// This block is entered only when the player moves to a new chunk, invalidating the cached coords.
				// Otherwise the cached coords are used.
				cCSLock Lock(m_CSChunkLists);
				auto itr = std::find(m_SentChunks.begin(), m_SentChunks.end(), cChunkCoords(m_Player->GetChunkX(), m_Player->GetChunkZ()));
				if (itr != m_SentChunks.end())
				{
					m_CachedSentChunk = *itr;
					m_HasSentPlayerChunk = true;
				}
			}
		}
	}

	// If the chunk the player's in was just sent, spawn the player:
	{
		cCSLock lock(m_CSState);
		if (m_HasSentPlayerChunk && (m_State == csDownloadingWorld))
		{
			m_Protocol->SendPlayerMoveLook();
			m_State = csPlaying;

			// Send resource pack (after a MoveLook, because sending it before the initial MoveLook cancels the download screen):
			if (const auto & ResourcePackUrl = cRoot::Get()->GetServer()->GetResourcePackUrl(); !ResourcePackUrl.empty())
			{
				SendResourcePack(ResourcePackUrl);
			}
		}
	}  // lock(m_CSState)

	// Send a ping packet:
	if (m_State == csPlaying)
	{
		const auto Now = std::chrono::steady_clock::now();
		if ((m_PingStartTime + std::chrono::seconds(2)) <= Now)  // 2 second interval between pings
		{
			m_PingID++;
			m_PingStartTime = Now;
			m_Protocol->SendKeepAlive(m_PingID);
		}
	}

	// Send a couple of chunks to the player:
	StreamNextChunks();

	// Unload all chunks that are out of the view distance (every 5 seconds):
	if ((m_TimeSinceLastUnloadCheck += a_Dt) > 5s)
	{
		UnloadOutOfRangeChunks();
		m_TimeSinceLastUnloadCheck = 0s;
	}

	// anticheat fastbreak
	if (m_HasStartedDigging)
	{
		auto Block = m_Player->GetWorld()->GetBlock(m_LastDigBlockPos);
		m_BreakProgress += m_Player->GetMiningProgressPerTick(Block);
	}

	// Handle block break animation:
	if (m_BlockDigAnimStage > -1)
	{
		int lastAnimVal = m_BlockDigAnimStage;
		m_BlockDigAnimStage += static_cast<int>(m_BlockDigAnimSpeed * a_Dt.count());
		if (m_BlockDigAnimStage > 9000)
		{
			m_BlockDigAnimStage = 9000;
		}
		if (m_BlockDigAnimStage / 1000 != lastAnimVal / 1000)
		{
			m_Player->GetWorld()->BroadcastBlockBreakAnimation(static_cast<UInt32>(m_UniqueID), m_BlockDigAnimPos, static_cast<char>(m_BlockDigAnimStage / 1000), this);
		}
	}

	// Reset explosion & block change counters:
	m_NumExplosionsThisTick = 0;
	m_NumBlockChangeInteractionsThisTick = 0;
}





void cClientHandle::ServerTick(float a_Dt)
{
	ProcessProtocolIn();
	ProcessProtocolOut();

	m_TicksSinceLastPacket += 1;
	if (m_TicksSinceLastPacket > 600)  // 30 seconds
	{
		SendDisconnect("Nooooo!! You timed out! D: Come back!");
	}
}





void cClientHandle::SendAcknowledgeBlockChange(int a_SequenceId)
{
	m_Protocol->SendAcknowledgeBlockChange(a_SequenceId);
}





void cClientHandle::SendAttachEntity(const cEntity & a_Entity, const cEntity & a_Vehicle)
{
	m_Protocol->SendAttachEntity(a_Entity, a_Vehicle);
}





void cClientHandle::SendLeashEntity(const cEntity & a_Entity, const cEntity & a_EntityLeashedTo)
{
	m_Protocol->SendLeashEntity(a_Entity, a_EntityLeashedTo);
}





void cClientHandle::SendUnleashEntity(const cEntity & a_Entity)
{
	m_Protocol->SendUnleashEntity(a_Entity);
}





void cClientHandle::SendBlockAction(Vector3i a_BlockPos, char a_Byte1, char a_Byte2, BlockState a_Block)
{
	m_Protocol->SendBlockAction(a_BlockPos, a_Byte1, a_Byte2, a_Block);
}





void cClientHandle::SendBlockBreakAnim(UInt32 a_EntityID, Vector3i a_BlockPos, char a_Stage)
{
	m_Protocol->SendBlockBreakAnim(a_EntityID, a_BlockPos, a_Stage);
}





void cClientHandle::SendBlockChange(Vector3i a_BlockPos, BlockState a_Block)
{
	auto ChunkCoords = cChunkDef::BlockToChunk(a_BlockPos);

	// Do not send block changes in chunks that weren't sent to the client yet:
	cCSLock Lock(m_CSChunkLists);
	if (std::find(m_SentChunks.begin(), m_SentChunks.end(), ChunkCoords) != m_SentChunks.end())
	{
		Lock.Unlock();
		m_Protocol->SendBlockChange(a_BlockPos, a_Block);
	}
}





void cClientHandle::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(!a_Changes.empty());  // We don't want to be sending empty change packets!

	// Do not send block changes in chunks that weren't sent to the client yet:
	{
		cCSLock Lock(m_CSChunkLists);
		if (std::find(m_SentChunks.begin(), m_SentChunks.end(), cChunkCoords(a_ChunkX, a_ChunkZ)) == m_SentChunks.end())
		{
			return;
		}
	}

	// Use a dedicated packet for single changes:
	if (a_Changes.size() == 1)
	{
		const auto & Change = a_Changes[0];
		m_Protocol->SendBlockChange(Change.GetAbsolutePos(), Change.m_Block);
		return;
	}

	m_Protocol->SendBlockChanges(a_ChunkX, a_ChunkZ, a_Changes);
}





void cClientHandle::SendBossBarAdd(UInt32 a_UniqueID, const cCompositeChat & a_Title, float a_FractionFilled, BossBarColor a_Color, BossBarDivisionType a_DivisionType, bool a_DarkenSky, bool a_PlayEndMusic, bool a_CreateFog)
{
	m_Protocol->SendBossBarAdd(a_UniqueID, a_Title, a_FractionFilled, a_Color, a_DivisionType, a_DarkenSky, a_PlayEndMusic, a_CreateFog);
}





void cClientHandle::SendBossBarUpdateFlags(UInt32 a_UniqueID, bool a_DarkenSky, bool a_PlayEndMusic, bool a_CreateFog)
{
	m_Protocol->SendBossBarUpdateFlags(a_UniqueID, a_DarkenSky, a_PlayEndMusic, a_CreateFog);
}





void cClientHandle::SendBossBarUpdateStyle(UInt32 a_UniqueID, BossBarColor a_Color, BossBarDivisionType a_DivisionType)
{
	m_Protocol->SendBossBarUpdateStyle(a_UniqueID, a_Color, a_DivisionType);
}





void cClientHandle::SendBossBarUpdateTitle(UInt32 a_UniqueID, const cCompositeChat & a_Title)
{
	m_Protocol->SendBossBarUpdateTitle(a_UniqueID, a_Title);
}





void cClientHandle::SendBossBarRemove(UInt32 a_UniqueID)
{
	m_Protocol->SendBossBarRemove(a_UniqueID);
}





void cClientHandle::SendBossBarUpdateHealth(UInt32 a_UniqueID, float a_FractionFilled)
{
	m_Protocol->SendBossBarUpdateHealth(a_UniqueID, a_FractionFilled);
}





void cClientHandle::SendCameraSetTo(const cEntity & a_Entity)
{
	m_Protocol->SendCameraSetTo(a_Entity);
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

	bool ShouldUsePrefixes = World->ShouldUseChatPrefixes();
	AString Message = FormatMessageType(ShouldUsePrefixes, a_ChatPrefix, a_AdditionalData);
	m_Protocol->SendChat(Message.append(a_Message), ctChatBox, ShouldUsePrefixes);
}





void cClientHandle::SendChat(const cCompositeChat & a_Message)
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

	bool ShouldUsePrefixes = World->ShouldUseChatPrefixes();
	m_Protocol->SendChat(a_Message, ctChatBox, ShouldUsePrefixes);
}





void cClientHandle::SendChatRaw(const AString & a_MessageRaw, eChatType a_Type)
{
	m_Protocol->SendChatRaw(a_MessageRaw, a_Type);
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
	m_Protocol->SendChat(Message.append(a_Message), ctAboveActionBar);
}





void cClientHandle::SendChatAboveActionBar(const cCompositeChat & a_Message)
{
	m_Protocol->SendChat(a_Message, ctAboveActionBar, GetPlayer()->GetWorld()->ShouldUseChatPrefixes());
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

	auto ShouldUsePrefixes = World->ShouldUseChatPrefixes();
	AString Message = FormatMessageType(ShouldUsePrefixes, a_ChatPrefix, a_AdditionalData);
	m_Protocol->SendChat(Message.append(a_Message), ctSystem, ShouldUsePrefixes);
}





void cClientHandle::SendChatSystem(const cCompositeChat & a_Message)
{
	m_Protocol->SendChat(a_Message, ctSystem, GetPlayer()->GetWorld()->ShouldUseChatPrefixes());
}





void cClientHandle::SendChunkData(int a_ChunkX, int a_ChunkZ, const ContiguousByteBufferView a_ChunkData)
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
		// LOG("Refusing to send    chunk [%d, %d] to client \"%s\" at [%d, %d].", a_ChunkX, a_ChunkZ, m_Username.c_str(), m_Player->GetChunkX(), m_Player->GetChunkZ());
		// 2020 08 21: seems to happen going through nether portals on 1.8.9
		return;
	}

	if (!m_Protocol.VersionRecognitionSuccessful())
	{
		// TODO (#2588): investigate if and why this occurs
		return;
	}

	m_Protocol->SendChunkData(a_ChunkData);

	// Add the chunk to the list of chunks sent to the player:
	{
		cCSLock Lock(m_CSChunkLists);
		m_SentChunks.emplace_back(a_ChunkX, a_ChunkZ);
	}
}





void cClientHandle::SendCollectEntity(const cEntity & a_Collected, const cEntity & a_Collector, unsigned a_Count)
{
	m_Protocol->SendCollectEntity(a_Collected, a_Collector, a_Count);
}





void cClientHandle::SendDestroyEntity(const cEntity & a_Entity)
{
	m_Protocol->SendDestroyEntity(a_Entity);
}





void cClientHandle::SendDetachEntity(const cEntity & a_Entity, const cEntity & a_PreviousVehicle)
{
	m_Protocol->SendDetachEntity(a_Entity, a_PreviousVehicle);
}





void cClientHandle::SendDisconnect(const AString & a_Reason)
{
	LOGD("Sending a DC: \"%s\"", StripColorCodes(a_Reason).c_str());

	m_Protocol.SendDisconnect(*this, a_Reason);
	m_HasSentDC = true;
	Destroy();
}





void cClientHandle::SendEditSign(Vector3i a_BlockPos)
{
	m_LastPlacedSign = a_BlockPos;
	m_Protocol->SendEditSign(a_BlockPos);
}





void cClientHandle::SendEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, int a_Duration)
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





void cClientHandle::SendEntityPosition(const cEntity & a_Entity)
{
	m_Protocol->SendEntityPosition(a_Entity);
}





void cClientHandle::SendEntityProperties(const cEntity & a_Entity)
{
	m_Protocol->SendEntityProperties(a_Entity);
}





void cClientHandle::SendEntityVelocity(const cEntity & a_Entity)
{
	m_Protocol->SendEntityVelocity(a_Entity);
}





void cClientHandle::SendExplosion(const Vector3f a_Position, const float a_Power)
{
	if (m_NumExplosionsThisTick > MAX_EXPLOSIONS_PER_TICK)
	{
		LOGD("Dropped an explosion!");
		return;
	}

	// Update the statistics:
	m_NumExplosionsThisTick++;

	auto & Random = GetRandomProvider();
	const auto SoundPitchMultiplier = 1.0f + (Random.RandReal() - Random.RandReal()) * 0.2f;

	// Sound:
	SendSoundEffect("entity.generic.explode", a_Position, 4.0f, SoundPitchMultiplier * 0.7f);

	const auto ParticleFormula = a_Power * 0.33f;
	auto Spread = ParticleFormula * 0.5f;
	auto ParticleCount = std::min(static_cast<int>(ParticleFormula * 125), 600);

	// Dark smoke particles:
	SendParticleEffect("largesmoke", a_Position, {0.f, 0.f, 0.f}, Spread, static_cast<int>(ParticleCount));

	Spread = ParticleFormula * 0.35f;
	ParticleCount = std::min(static_cast<int>(ParticleFormula * 550), 1800);

	// Light smoke particles:
	SendParticleEffect("explode", a_Position, {0.f, 0.f, 0.f}, Spread, static_cast<int>(ParticleCount));

	// Shockwave effect:
	m_Protocol->SendExplosion(a_Position, a_Power);
}





void cClientHandle::SendFinishConfiguration()
{
	m_Protocol->SendFinishConfiguration();
}





void cClientHandle::SendGameMode(eGameMode a_GameMode)
{
	m_Protocol->SendGameMode(a_GameMode);
}





void cClientHandle::SendHealth(void)
{
	m_Protocol->SendHealth();
}





void cClientHandle::SendHeldItemChange(int a_ItemIndex)
{
	m_Protocol->SendHeldItemChange(a_ItemIndex);
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





void cClientHandle::SendParticleEffect(const AString & a_ParticleName, Vector3f a_Source, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount)
{
	m_Protocol->SendParticleEffect(a_ParticleName, a_Source, a_Offset, a_ParticleData, a_ParticleAmount);
}





void cClientHandle::SendParticleEffect(const AString & a_ParticleName, const Vector3f a_Src, const Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data)
{
	m_Protocol->SendParticleEffect(a_ParticleName, a_Src, a_Offset, a_ParticleData, a_ParticleAmount, a_Data);
}





void cClientHandle::SendPaintingSpawn(const cPainting & a_Painting)
{
	m_Protocol->SendPaintingSpawn(a_Painting);
}





void cClientHandle::SendEntityAnimation(const cEntity & a_Entity, EntityAnimation a_Animation)
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





void cClientHandle::SendPlayerListInitChat(const cPlayer & a_Player)
{
	m_Protocol->SendPlayerListInitChat(a_Player);
}





void cClientHandle::SendPlayerListHeaderFooter(const cCompositeChat & a_Header, const cCompositeChat & a_Footer)
{
	m_Protocol->SendPlayerListHeaderFooter(a_Header, a_Footer);
}





void cClientHandle::SendPlayerListRemovePlayer(const cPlayer & a_Player)
{
	m_Protocol->SendPlayerListRemovePlayer(a_Player);
}





void cClientHandle::SendPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName)
{
	m_Protocol->SendPlayerListUpdateDisplayName(a_Player, a_CustomName);
}





void cClientHandle::SendPlayerListUpdateGameMode(const cPlayer & a_Player)
{
	m_Protocol->SendPlayerListUpdateGameMode(a_Player);
}





void cClientHandle::SendPlayerListUpdatePing()
{
	m_Protocol->SendPlayerListUpdatePing();
}





void cClientHandle::SendPlayerMoveLook (const Vector3d a_Pos, const float a_Yaw, const float a_Pitch, const bool a_IsRelative)
{
	m_Protocol->SendPlayerMoveLook(a_Pos, a_Yaw, a_Pitch, a_IsRelative);
}





void cClientHandle::SendPlayerMoveLook(void)
{
	/*
	FLOGD("Sending PlayerMoveLook: {0:0.2f}, stance {1:0.2f}, OnGround: {2}",
		m_Player->GetPosition(), m_Player->GetStance(), m_Player->IsOnGround()
	);
	*/
	m_Protocol->SendPlayerMoveLook();
}





void cClientHandle::SendPlayerPermissionLevel()
{
	m_Protocol->SendPlayerPermissionLevel();
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





void cClientHandle::SendPluginMessage(const AString & a_Channel, const std::string_view a_Message)
{
	m_Protocol->SendPluginMessage(a_Channel, { reinterpret_cast<const std::byte *>(a_Message.data()), a_Message.size() });
}





void cClientHandle::SendPluginMessage(const AString & a_Channel, const ContiguousByteBufferView a_Message)
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





void cClientHandle::SendRenderDistanceCenter(cChunkCoords a_chunk)
{
	m_Protocol->SendRenderDistanceCenter(a_chunk);
}





void cClientHandle::SendRespawn(const eDimension a_Dimension, const bool a_IsRespawningFromDeath)
{
	if (!a_IsRespawningFromDeath && (a_Dimension == m_Player->GetWorld()->GetDimension()))
	{
		// The client goes crazy if we send a respawn packet with the dimension of the current world
		// So we send a temporary one first.
		// This is not needed when the player dies, hence the a_IsRespawningFromDeath flag.
		// a_IsRespawningFromDeath is true only at cPlayer::Respawn, which is called after the player dies.

		// First send a temporary dimension to placate the client:
		m_Protocol->SendRespawn((a_Dimension == dimOverworld) ? dimNether : dimOverworld);
	}

	m_Protocol->SendRespawn(a_Dimension);
	m_Protocol->SendInitialChunksComing();
}





void cClientHandle::SendExperience(void)
{
	m_Protocol->SendExperience();
}





void cClientHandle::SendExperienceOrb(const cExpOrb & a_ExpOrb)
{
	m_Protocol->SendExperienceOrb(a_ExpOrb);
}





void cClientHandle::SendResourcePack(const AString & a_ResourcePackUrl)
{
	m_Protocol->SendResourcePack(a_ResourcePackUrl);
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
	LOG("SendSoundEffect with double args is deprecated, use version with vector position parameter.");
	SendSoundEffect(a_SoundName, {a_X, a_Y, a_Z}, a_Volume, a_Pitch);
}





void cClientHandle::SendSoundEffect(const AString & a_SoundName, Vector3d a_Position, float a_Volume, float a_Pitch)
{
	m_Protocol->SendSoundEffect(a_SoundName, a_Position, a_Volume, a_Pitch);
}





void cClientHandle::SendSoundParticleEffect(const EffectID a_EffectID, Vector3i a_Source, int a_Data)
{
	m_Protocol->SendSoundParticleEffect(a_EffectID, a_Source, a_Data);
}





void cClientHandle::SendSpawnEntity(const cEntity & a_Entity)
{
	m_Protocol->SendSpawnEntity(a_Entity);
}





void cClientHandle::SendSpawnMob(const cMonster & a_Mob)
{
	m_Protocol->SendSpawnMob(a_Mob);
}





void cClientHandle::SendStatistics(const StatisticsManager & a_Manager)
{
	m_Protocol->SendStatistics(a_Manager);
}





void cClientHandle::SendTabCompletionResults(const AStringVector & a_Results, UInt32 CompletionId)
{
	m_Protocol->SendTabCompletionResults(a_Results, CompletionId);
}





void cClientHandle::SendThunderbolt(Vector3i a_BlockPos)
{
	m_Protocol->SendThunderbolt(a_BlockPos);
}





void cClientHandle::SendTitleTimes(int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks)
{
	m_Protocol->SendTitleTimes(a_FadeInTicks, a_DisplayTicks, a_FadeOutTicks);
}





void cClientHandle::SendTimeUpdate(const cTickTimeLong a_WorldAge, const cTickTimeLong a_WorldDate, const bool a_DoDaylightCycle)
{
	m_Protocol->SendTimeUpdate(a_WorldAge, a_WorldDate, a_DoDaylightCycle);
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
	Vector3i a_BlockPos,
	const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4
)
{
	m_Protocol->SendUpdateSign(
		a_BlockPos,
		a_Line1, a_Line2, a_Line3, a_Line4
	);
}





void cClientHandle::SendUnlockRecipe(UInt32 a_RecipeId)
{
	m_Protocol->SendUnlockRecipe(a_RecipeId);
}





void cClientHandle::SendInitRecipes(UInt32 a_RecipeId)
{
	m_Protocol->SendInitRecipes(a_RecipeId);
}





void cClientHandle::HandleCraftRecipe(UInt32 a_RecipeId)
{
	auto * Window = m_Player->GetWindow();
	if (Window == nullptr)
	{
		return;
	}

	if (Window->GetWindowType() == cWindow::wtInventory)
	{
		static_cast<cInventoryWindow *>(Window)->LoadRecipe(*m_Player, a_RecipeId);
	}
	else if (Window->GetWindowType() == cWindow::wtWorkbench)
	{
		static_cast<cCraftingWindow *>(Window)->LoadRecipe(*m_Player, a_RecipeId);
	}
}





void cClientHandle::SendWeather(eWeather a_Weather)
{
	m_Protocol->SendWeather(a_Weather);
}





void cClientHandle::SendGameStateChange(eGameStateReason a_Reason, float a_Value)
{
	m_Protocol->SendGameStateChange(a_Reason, a_Value);
}





void cClientHandle::SendWholeInventory(const cWindow & a_Window, const cItem & a_CursorStack)
{
	m_Protocol->SendWholeInventory(a_Window, a_CursorStack);
}





void cClientHandle::SendWindowClose(const cWindow & a_Window)
{
	m_Protocol->SendWindowClose(a_Window);
}





void cClientHandle::SendWindowOpen(const cWindow & a_Window)
{
	m_Protocol->SendWindowOpen(a_Window);
}





void cClientHandle::SendWindowProperty(const cWindow & a_Window, size_t a_Property, short a_Value)
{
	m_Protocol->SendWindowProperty(a_Window, a_Property, a_Value);
}





const AString & cClientHandle::GetUsername(void) const
{
	return m_Username;
}





void cClientHandle::SetUsername(AString && a_Username)
{
	m_Username = std::move(a_Username);
}





void cClientHandle::SetViewDistance(int a_ViewDistance)
{
	m_RequestedViewDistance = a_ViewDistance;
	LOGD("%s is requesting ViewDistance of %d!", GetUsername().c_str(), m_RequestedViewDistance);

	cWorld * world = m_Player->GetWorld();
	if (world != nullptr)
	{
		// Set the current view distance based on the requested VD and world max VD:
		m_CurrentViewDistance = Clamp(a_ViewDistance, cClientHandle::MIN_VIEW_DISTANCE, world->GetMaxViewDistance());

		// Restart chunk streaming to respond to new view distance:
		m_LastStreamedChunkX = std::numeric_limits<decltype(m_LastStreamedChunkX)>::max();
		m_LastStreamedChunkZ = std::numeric_limits<decltype(m_LastStreamedChunkZ)>::max();
	}
}





bool cClientHandle::HasPluginChannel(const AString & a_PluginChannel)
{
	return (m_PluginChannels.find(a_PluginChannel) != m_PluginChannels.end());
}





bool cClientHandle::WantsSendChunk(int a_ChunkX, int a_ChunkZ)
{
	if (m_State >= csDestroyed)
	{
		return false;
	}

	cCSLock Lock(m_CSChunkLists);
	return m_ChunksToSend.find(cChunkCoords(a_ChunkX, a_ChunkZ)) != m_ChunksToSend.end();
}





void cClientHandle::AddWantedChunk(int a_ChunkX, int a_ChunkZ)
{
	if (m_State >= csDestroyed)
	{
		return;
	}

	LOGD("Adding chunk [%d, %d] to wanted chunks for client %p", a_ChunkX, a_ChunkZ, static_cast<void *>(this));
	cCSLock Lock(m_CSChunkLists);
	if (m_ChunksToSend.find(cChunkCoords(a_ChunkX, a_ChunkZ)) == m_ChunksToSend.end())
	{
		m_ChunksToSend.emplace(a_ChunkX, a_ChunkZ);
	}
}





void cClientHandle::PacketBufferFull(void)
{
	// Too much data in the incoming queue, the server is probably too busy, kick the client:
	LOGERROR("Too much data in queue for client \"%s\" @ %s, kicking them.", m_Username, m_IPString);
	SendDisconnect("The server is busy; please try again later.");
}





void cClientHandle::PacketUnknown(UInt32 a_PacketType)
{
	LOGERROR("Unknown packet type 0x%x from client \"%s\" @ %s", a_PacketType, m_Username, m_IPString);

	SendDisconnect(fmt::format(FMT_STRING("Unknown [C->S] PacketType: 0x{:x}"), a_PacketType));
}





void cClientHandle::PacketError(UInt32 a_PacketType)
{
	LOGERROR("Protocol error while parsing packet type 0x%02x; disconnecting client \"%s\"", a_PacketType, m_Username);
	SendDisconnect("Protocol error");
}





void cClientHandle::SocketClosed(void)
{
	// The socket has been closed for any reason
	/*
	LOGD("SocketClosed for client %s @ %s (%p), state = %d, m_Player = %p",
		m_Username.c_str(), m_IPString.c_str(), static_cast<void *>(this), m_State.load(), static_cast<void *>(m_Player)
	);
	//*/

	// Log into console, unless it's a client ping:
	if (!m_Username.empty())
	{
		LOGD("Client %s @ %s disconnected", m_Username.c_str(), m_IPString.c_str());
		cRoot::Get()->GetPluginManager()->CallHookDisconnect(*this, "Player disconnected");
	}

	// Queue self for destruction:
	Destroy();
}





bool cClientHandle::SetState(eState a_NewState)
{
	cCSLock Lock(m_CSState);
	if (a_NewState <= m_State)
	{
		return false;  // Can only advance the state machine
	}
	m_State = a_NewState;
	return true;
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
	m_IncomingData.append(reinterpret_cast<const std::byte *>(a_Data), a_Length);
}





void cClientHandle::OnRemoteClosed(void)
{
	/*
	LOGD("Client socket for %s @ %s has been closed.",
		m_Username.c_str(), m_IPString.c_str()
	);
	//*/
	SocketClosed();
}





void cClientHandle::OnError(int a_ErrorCode, const AString & a_ErrorMsg)
{
	LOGD("An error has occurred on client link for %s @ %s: %d (%s). Client disconnected.",
		m_Username.c_str(), m_IPString.c_str(), a_ErrorCode, a_ErrorMsg.c_str()
	);
	SocketClosed();
}
