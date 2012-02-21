
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cClientHandle.h"
#include "cServer.h"
#include "cWorld.h"
#include "cPickup.h"
#include "cPluginManager.h"
#include "cPlayer.h"
#include "cInventory.h"
#include "cChestEntity.h"
#include "cSignEntity.h"
#include "cWindow.h"
#include "cCraftingWindow.h"
#include "cItem.h"
#include "cTorch.h"
#include "cStairs.h"
#include "cDoors.h"
#include "cLadder.h"
#include "cSign.h"
#include "cRedstone.h"
#include "cPiston.h"
#include "cBlockToPickup.h"
#include "cMonster.h"
#include "cChatColor.h"
#include "cSocket.h"
#include "cTimer.h"

#include "cTracer.h"
#include "Vector3f.h"
#include "Vector3d.h"

#include "cSleep.h"
#include "cRoot.h"

#include "cBlockingTCPLink.h"
#include "cAuthenticator.h"
#include "MersenneTwister.h"

#include "packets/cPacket_KeepAlive.h"
#include "packets/cPacket_PlayerPosition.h"
#include "packets/cPacket_Respawn.h"
#include "packets/cPacket_UpdateHealth.h"
#include "packets/cPacket_RelativeEntityMoveLook.h"
#include "packets/cPacket_Chat.h"
#include "packets/cPacket_Login.h"
#include "packets/cPacket_WindowClick.h"
#include "packets/cPacket_PlayerMoveLook.h"
#include "packets/cPacket_TimeUpdate.h"
#include "packets/cPacket_BlockDig.h"
#include "packets/cPacket_Handshake.h"
#include "packets/cPacket_PlayerLook.h"
#include "packets/cPacket_ArmAnim.h"
#include "packets/cPacket_BlockPlace.h"
#include "packets/cPacket_Flying.h"
#include "packets/cPacket_Disconnect.h"
#include "packets/cPacket_PickupSpawn.h"
#include "packets/cPacket_ItemSwitch.h"
#include "packets/cPacket_EntityEquipment.h"
#include "packets/cPacket_CreativeInventoryAction.h"
#include "packets/cPacket_NewInvalidState.h"
#include "packets/cPacket_UseEntity.h"
#include "packets/cPacket_WindowClose.h"
#include "packets/cPacket_13.h"
#include "packets/cPacket_UpdateSign.h"
#include "packets/cPacket_Ping.h"
#include "packets/cPacket_PlayerListItem.h"
#include "packets/cPacket_NamedEntitySpawn.h"
#include "packets/cPacket_MapChunk.h"





#define AddPistonDir(x, y, z, dir, amount) switch (dir) { case 0: (y)-=(amount); break; case 1: (y)+=(amount); break;\
													 case 2: (z)-=(amount); break; case 3: (z)+=(amount); break;\
													 case 4: (x)-=(amount); break; case 5: (x)+=(amount); break; }

#define MAX_SEMAPHORES (2000)





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cClientHandle:

cClientHandle::cClientHandle(const cSocket & a_Socket)
	: m_ProtocolVersion(23)
	, m_pSendThread(NULL)
	, m_Socket(a_Socket)
	, m_Semaphore(MAX_SEMAPHORES)
	, m_bDestroyed(false)
	, m_Player(NULL)
	, m_bKicking(false)
	, m_TimeLastPacket(cWorld::GetTime())
	, m_bKeepThreadGoing(true)
	, m_Ping(1000)
	, m_State(csConnected)
	, m_LastStreamedChunkX(0x7fffffff)  // bogus chunk coords to force streaming upon login
	, m_LastStreamedChunkZ(0x7fffffff)
{
	cTimer t1;
	m_LastPingTime = t1.GetNowTime();

	// All the packets that can be received from the client
	for (int i = 0; i < ARRAYCOUNT(m_PacketMap); ++i)
	{
		m_PacketMap[i] = NULL;
	}
	m_PacketMap[E_KEEP_ALIVE]                = new cPacket_KeepAlive;
	m_PacketMap[E_HANDSHAKE]                 = new cPacket_Handshake;
	m_PacketMap[E_LOGIN]                     = new cPacket_Login;
	m_PacketMap[E_PLAYERPOS]                 = new cPacket_PlayerPosition;
	m_PacketMap[E_PLAYERLOOK]                = new cPacket_PlayerLook;
	m_PacketMap[E_PLAYERMOVELOOK]            = new cPacket_PlayerMoveLook;
	m_PacketMap[E_CHAT]                      = new cPacket_Chat;
	m_PacketMap[E_ANIMATION]                 = new cPacket_ArmAnim;
	m_PacketMap[E_FLYING]                    = new cPacket_Flying;
	m_PacketMap[E_BLOCK_DIG]                 = new cPacket_BlockDig;
	m_PacketMap[E_BLOCK_PLACE]               = new cPacket_BlockPlace;
	m_PacketMap[E_DISCONNECT]                = new cPacket_Disconnect;
	m_PacketMap[E_ITEM_SWITCH]               = new cPacket_ItemSwitch;
	m_PacketMap[E_ENTITY_EQUIPMENT]          = new cPacket_EntityEquipment;
	m_PacketMap[E_CREATIVE_INVENTORY_ACTION] = new cPacket_CreativeInventoryAction;
	m_PacketMap[E_NEW_INVALID_STATE]         = new cPacket_NewInvalidState;
	m_PacketMap[E_PICKUP_SPAWN]              = new cPacket_PickupSpawn;
	m_PacketMap[E_USE_ENTITY]                = new cPacket_UseEntity;
	m_PacketMap[E_WINDOW_CLOSE]              = new cPacket_WindowClose;
	m_PacketMap[E_WINDOW_CLICK]              = new cPacket_WindowClick;
	m_PacketMap[E_PACKET_13]                 = new cPacket_13;
	m_PacketMap[E_UPDATE_SIGN]               = new cPacket_UpdateSign;
	m_PacketMap[E_RESPAWN]                   = new cPacket_Respawn;
	m_PacketMap[E_PING]                      = new cPacket_Ping;

	//////////////////////////////////////////////////////////////////////////
	m_pSendThread    = new cThread(SendThread,    this, "cClientHandle::SendThread");
	m_pSendThread->Start   (true);
	//////////////////////////////////////////////////////////////////////////

	LOG("New ClientHandle created at %p", this);
}





cClientHandle::~cClientHandle()
{
	LOG("Deleting client \"%s\"", GetUsername().c_str());

	// Remove from cSocketThreads, just in case
	cRoot::Get()->GetServer()->ClientDestroying(this);
	
	m_LoadedChunks.clear();
	m_ChunksToSend.clear();

	if (m_Player != NULL)
	{
		cWorld * World = m_Player->GetWorld();
		if (!m_Username.empty() && (World != NULL))
		{
			// Send the Offline PlayerList packet:
			AString NameColor = (m_Player ? m_Player->GetColor() : "");
			cPacket_PlayerListItem PlayerList(NameColor + GetUsername(), false, (short)9999);
			World->Broadcast(PlayerList, this);

			// Send the Chat packet:
			cPacket_Chat Left(m_Username + " left the game!");
			World->Broadcast(Left, this);
		}
	}

	// First stop sending thread
	m_bKeepThreadGoing = false;

	if (m_Socket.IsValid())
	{
		cPacket_Disconnect Disconnect;
		Disconnect.m_Reason = "Server shut down? Kthnxbai";
		m_Socket.Send(&Disconnect);
		m_Socket.CloseSocket();
	}

	m_Semaphore.Signal();
	delete m_pSendThread;

	if (m_Player != NULL)
	{
		m_Player->SetClientHandle(NULL);
		m_Player->Destroy();
		m_Player = NULL;
	}
	for (int i = 0; i < ARRAYCOUNT(m_PacketMap); i++)
	{
		delete m_PacketMap[i];
	}

	{
		cCSLock Lock(m_SendCriticalSection);
		for (PacketList::iterator itr = m_PendingNrmSendPackets.begin(); itr != m_PendingNrmSendPackets.end(); ++itr)
		{
			delete *itr;
		}
		for (PacketList::iterator itr = m_PendingLowSendPackets.begin(); itr != m_PendingLowSendPackets.end(); ++itr)
		{
			delete *itr;
		}
	}
	
	LOG("ClientHandle at %p destroyed", this);
}





void cClientHandle::Destroy()
{
	m_bDestroyed = true;
	
	if ((m_Player != NULL) && (m_Player->GetWorld() != NULL))
	{
		RemoveFromAllChunks();
	}
	
	if (m_Socket.IsValid())
	{
		m_Socket.CloseSocket();
	}
	
	// Synchronize with the cSocketThreads (so that they don't call us anymore)
	cRoot::Get()->GetServer()->ClientDestroying(this);
}





void cClientHandle::Kick(const AString & a_Reason)
{
	if (m_State >= csAuthenticating)  // Don't log pings
	{
		LOG("Kicking user \"%s\" for \"%s\"", m_Username.c_str(), a_Reason.c_str());
	}
	Send(cPacket_Disconnect(a_Reason));
	m_bKicking = true;
}





void cClientHandle::Authenticate(void)
{
	// Spawn player (only serversided, so data is loaded)
	m_Player = new cPlayer(this, GetUsername());

	cWorld * World = cRoot::Get()->GetWorld(m_Player->GetLoadedWorldName());
	if (World == NULL)
	{
		World = cRoot::Get()->GetDefaultWorld();
	}
	
	m_Player->LoginSetGameMode (World->GetGameMode()); //set player's gamemode to server's gamemode at login. TODO: set to last player's gamemode at logout

	m_Player->SetIP (m_Socket.GetIPString());

	cRoot::Get()->GetPluginManager()->CallHook(cPluginManager::E_PLUGIN_PLAYER_SPAWN, 1, m_Player);

	// Return a server login packet
	cPacket_Login LoginResponse;
	LoginResponse.m_ProtocolVersion = m_Player->GetUniqueID();
	//LoginResponse.m_Username = "";
	LoginResponse.m_ServerMode = m_Player->GetGameMode();  // set gamemode from player.
	LoginResponse.m_MapSeed = cRoot::Get()->GetWorld()->GetWorldSeed();
	LoginResponse.m_Dimension = 0;
	LoginResponse.m_MaxPlayers = (unsigned char)cRoot::Get()->GetWorld()->GetMaxPlayers();
	LoginResponse.m_Difficulty = 2;
	Send(LoginResponse);

	// Send Weather if raining:
	if ((World->GetWeather() == 1) || (World->GetWeather() == 2))
	{
		cPacket_NewInvalidState RainPacket;
		RainPacket.m_Reason = 1; //begin rain
		Send(RainPacket);
	}

	// Send time
	Send(cPacket_TimeUpdate(World->GetWorldTime()));

	// Send inventory
	m_Player->GetInventory().SendWholeInventory(this);

	// Send health
	cPacket_UpdateHealth Health;
	Health.m_Health = (short)m_Player->GetHealth();
	Health.m_Food = m_Player->GetFood();
	Health.m_Saturation = m_Player->GetFoodSaturation();
	Send(Health);
	
	m_Player->Initialize(World);
	m_State = csDownloadingWorld;
	StreamChunks();
}





void cClientHandle::StreamChunks(void)
{
	if (m_State < csDownloadingWorld)
	{
		return;
	}

	ASSERT(m_Player != NULL);
	
	int ChunkPosX = FAST_FLOOR_DIV(m_Player->GetPosX(), 16);
	int ChunkPosZ = FAST_FLOOR_DIV(m_Player->GetPosZ(), 16);
	if ((ChunkPosX == m_LastStreamedChunkX) && (ChunkPosZ == m_LastStreamedChunkZ))
	{
		// Already streamed for this position
		return;
	}
	m_LastStreamedChunkX = ChunkPosX;
	m_LastStreamedChunkZ = ChunkPosZ;
	
	// DEBUG:
	LOGINFO("Streaming chunks centered on [%d, %d]", ChunkPosX, ChunkPosZ);
	
	cWorld * World = m_Player->GetWorld();
	ASSERT(World != NULL);

	// Remove all loaded chunks that are no longer in range:
	{
		cCSLock Lock(m_CSChunkLists);
		for (cChunkCoordsList::iterator itr = m_LoadedChunks.begin(); itr != m_LoadedChunks.end();)
		{
			int RelX = (*itr).m_ChunkX - ChunkPosX;
			int RelZ = (*itr).m_ChunkZ - ChunkPosZ;
			if ((RelX > VIEWDISTANCE) || (RelX < -VIEWDISTANCE) || (RelZ > VIEWDISTANCE) || (RelZ < -VIEWDISTANCE))
			{
				World->RemoveChunkClient(itr->m_ChunkX, itr->m_ChunkY, itr->m_ChunkZ, this);
				itr = m_LoadedChunks.erase(itr);
			}
			else
			{
				++itr;
			}
		}  // for itr - m_LoadedChunks[]
		for (cChunkCoordsList::iterator itr = m_ChunksToSend.begin(); itr != m_ChunksToSend.end();)
		{
			int RelX = (*itr).m_ChunkX - ChunkPosX;
			int RelZ = (*itr).m_ChunkZ - ChunkPosZ;
			if ((RelX > VIEWDISTANCE) || (RelX < -VIEWDISTANCE) || (RelZ > VIEWDISTANCE) || (RelZ < -VIEWDISTANCE))
			{
				itr = m_ChunksToSend.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}
	
	// Add all chunks that are in range and not yet in m_LoadedChunks:
	// Queue these smartly - from the center out to the edge
	for (int d = 0; d <= VIEWDISTANCE; ++d)  // cycle through (square) distance, from nearest to furthest
	{
		// For each distance add chunks in a hollow square centered around current position:
		for (int i = -d; i <= d; ++i)
		{
			StreamChunk(ChunkPosX + d, ZERO_CHUNK_Y, ChunkPosZ + i);
			StreamChunk(ChunkPosX - d, ZERO_CHUNK_Y, ChunkPosZ + i);
		}  // for i
		for (int i = -d + 1; i < d; ++i)
		{
			StreamChunk(ChunkPosX + i, ZERO_CHUNK_Y, ChunkPosZ + d);
			StreamChunk(ChunkPosX + i, ZERO_CHUNK_Y, ChunkPosZ - d);
		}  // for i
	}  // for d
	
	// Touch chunks GENERATEDISTANCE ahead to let them generate:
	for (int d = VIEWDISTANCE + 1; d <= VIEWDISTANCE + GENERATEDISTANCE; ++d)  // cycle through (square) distance, from nearest to furthest
	{
		// For each distance touch chunks in a hollow square centered around current position:
		for (int i = -d; i <= d; ++i)
		{
			World->TouchChunk(ChunkPosX + d, ZERO_CHUNK_Y, ChunkPosZ + i);
			World->TouchChunk(ChunkPosX - d, ZERO_CHUNK_Y, ChunkPosZ + i);
		}  // for i
		for (int i = -d + 1; i < d; ++i)
		{
			World->TouchChunk(ChunkPosX + i, ZERO_CHUNK_Y, ChunkPosZ + d);
			World->TouchChunk(ChunkPosX + i, ZERO_CHUNK_Y, ChunkPosZ - d);
		}  // for i
	}  // for d
}




void cClientHandle::StreamChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	cWorld * World = m_Player->GetWorld();
	ASSERT(World != NULL);

	if (World->AddChunkClient(a_ChunkX, a_ChunkY, a_ChunkZ, this))
	{
		cCSLock Lock(m_CSChunkLists);
		m_LoadedChunks.push_back(cChunkCoords(a_ChunkX, a_ChunkY, a_ChunkZ));
		m_ChunksToSend.push_back(cChunkCoords(a_ChunkX, a_ChunkY, a_ChunkZ));
	}
}





// Removes the client from all chunks. Used when switching worlds or destroying the player
void cClientHandle::RemoveFromAllChunks()
{
	cCSLock Lock(m_CSChunkLists);
	cWorld * World = m_Player->GetWorld();
	if (World != NULL)
	{
		World->RemoveClientFromChunks(this, m_LoadedChunks);
	}
	m_LoadedChunks.clear();
	m_ChunksToSend.clear();
}





void cClientHandle::ChunkJustSent(cChunk * a_ChunkCompleted)
{
	cCSLock Lock(m_CSChunkLists);
	for (cChunkCoordsList::iterator itr = m_ChunksToSend.begin(); itr != m_ChunksToSend.end(); ++itr)
	{
		if (((*itr).m_ChunkX == a_ChunkCompleted->GetPosX()) && ((*itr).m_ChunkZ == a_ChunkCompleted->GetPosZ()))
		{
			m_ChunksToSend.erase(itr);
			if ((m_State == csDownloadingWorld) && (m_ChunksToSend.empty()))
			{
				CheckIfWorldDownloaded();
			}
			return;
		}
	}  // for itr - m_ChunksToSend[]
}





void cClientHandle::HandlePacket(cPacket * a_Packet)
{
	m_TimeLastPacket = cWorld::GetTime();

	// LOG("Recv packet 0x%02x from client \"%s\" (\"%s\")", a_Packet->m_PacketID, m_Socket.GetIPString().c_str(), m_Username.c_str());

	if (m_bKicking)
	{
		return;
	}

	switch (m_State)
	{
		case csConnected:
		{
			switch (a_Packet->m_PacketID)
			{
				case E_NEW_INVALID_STATE: // New/Invalid State packet received. I'm guessing the client only sends it when there's a problem with the bed?
				{
					LOGINFO("Got New Invalid State packet");
					break;
				}
				case E_PING:           HandlePing     (); break;
				case E_HANDSHAKE:      HandleHandshake(reinterpret_cast<cPacket_Handshake *>(a_Packet)); break;
				case E_LOGIN:          HandleLogin    (reinterpret_cast<cPacket_Login *>    (a_Packet)); break;
				
				// Ignored packets:
				case E_PLAYERLOOK:
				case E_PLAYERMOVELOOK:
				case E_PLAYERPOS:
				case E_KEEP_ALIVE:     break;
				default:               HandleUnexpectedPacket(a_Packet); break;
			}  // switch (PacketType)
			break;
		}  // case csConnected
		
		case csAuthenticating:
		{
			// Waiting for external authentication, no packets are handled
			switch (a_Packet->m_PacketID)
			{
				// Ignored packets:
				case E_KEEP_ALIVE:
				case E_PLAYERLOOK:
				case E_PLAYERMOVELOOK:
				case E_PLAYERPOS: break;
				
				default: HandleUnexpectedPacket(a_Packet); break;
			}
			break;
		}
		
		case csDownloadingWorld:
		{
			// Waiting for chunks to stream to client, no packets are handled
			switch (a_Packet->m_PacketID)
			{
				// Ignored packets:
				case E_KEEP_ALIVE:
				case E_PLAYERLOOK:
				case E_PLAYERMOVELOOK:
				case E_PLAYERPOS: break;
				
				default: HandleUnexpectedPacket(a_Packet); break;
			}
			break;
		}
		
		case csConfirmingPos:
		{
			switch (a_Packet->m_PacketID)
			{
				// Ignored packets:
				case E_KEEP_ALIVE:
				case E_PLAYERLOOK:
				case E_PLAYERPOS: break;

				case E_PLAYERMOVELOOK: HandleMoveLookConfirm(reinterpret_cast<cPacket_PlayerMoveLook *>(a_Packet)); break;

				default:
				{
					HandleUnexpectedPacket(a_Packet);
					break;
				}
			}  // switch (PacketType)
			break;
		}  // case csConfirmingPos
		
		case csPlaying:
		{
			switch (a_Packet->m_PacketID)
			{
				case E_CREATIVE_INVENTORY_ACTION: HandleCreativeInventory(reinterpret_cast<cPacket_CreativeInventoryAction *>(a_Packet)); break;
				case E_PLAYERPOS:                 HandlePlayerPos        (reinterpret_cast<cPacket_PlayerPosition *>         (a_Packet)); break;
				case E_BLOCK_DIG:                 HandleBlockDig         (reinterpret_cast<cPacket_BlockDig *>               (a_Packet)); break;
				case E_BLOCK_PLACE:               HandleBlockPlace       (reinterpret_cast<cPacket_BlockPlace *>             (a_Packet)); break;
				case E_PICKUP_SPAWN:              HandlePickupSpawn      (reinterpret_cast<cPacket_PickupSpawn *>            (a_Packet)); break;
				case E_CHAT:                      HandleChat             (reinterpret_cast<cPacket_Chat *>                   (a_Packet)); break;
				case E_PLAYERLOOK:                HandlePlayerLook       (reinterpret_cast<cPacket_PlayerLook *>             (a_Packet)); break;
				case E_PLAYERMOVELOOK:            HandlePlayerMoveLook   (reinterpret_cast<cPacket_PlayerMoveLook *>         (a_Packet)); break;
				case E_ANIMATION:                 HandleAnimation        (reinterpret_cast<cPacket_ArmAnim *>                (a_Packet)); break;
				case E_ITEM_SWITCH:               HandleItemSwitch       (reinterpret_cast<cPacket_ItemSwitch *>             (a_Packet)); break;
				case E_WINDOW_CLOSE:              HandleWindowClose      (reinterpret_cast<cPacket_WindowClose *>            (a_Packet)); break;
				case E_WINDOW_CLICK:              HandleWindowClick      (reinterpret_cast<cPacket_WindowClick *>            (a_Packet)); break;
				case E_UPDATE_SIGN:               HandleUpdateSign       (reinterpret_cast<cPacket_UpdateSign *>             (a_Packet)); break;
				case E_USE_ENTITY:                HandleUseEntity        (reinterpret_cast<cPacket_UseEntity *>              (a_Packet)); break;
				case E_RESPAWN:                   HandleRespawn(); break;
				case E_DISCONNECT:                HandleDisconnect       (reinterpret_cast<cPacket_Disconnect *>             (a_Packet)); break;
				case E_KEEP_ALIVE:                HandleKeepAlive        (reinterpret_cast<cPacket_KeepAlive *>              (a_Packet)); break;
			}  // switch (Packet type)
			break;
		}  // case csPlaying
	}  // switch (m_State)
}





void cClientHandle::HandlePing(void)
{
	// Somebody tries to retrieve information about the server
	AString Reply;
	Printf(Reply, "%s%s%i%s%i", 
		cRoot::Get()->GetWorld()->GetDescription().c_str(), 
		cChatColor::Delimiter.c_str(), 
		cRoot::Get()->GetWorld()->GetNumPlayers(),
		cChatColor::Delimiter.c_str(), 
		cRoot::Get()->GetWorld()->GetMaxPlayers()
	);
	Kick(Reply.c_str());
}





void cClientHandle::HandleHandshake(cPacket_Handshake * a_Packet)
{
	m_Username = a_Packet->m_Username;
	LOG("HANDSHAKE %s", m_Username.c_str());

	if (cRoot::Get()->GetWorld()->GetNumPlayers() >= cRoot::Get()->GetWorld()->GetMaxPlayers())
	{
		Kick("The server is currently full :(-- Try again later");
		return;
	}
	cPacket_Chat Connecting(m_Username + " is connecting.");
	cRoot::Get()->GetServer()->Broadcast(Connecting, this);

	cPacket_Handshake Handshake;
	Handshake.m_Username = cRoot::Get()->GetServer()->GetServerID();
	Send(Handshake);
	LOG("User \"%s\" was sent a handshake", m_Username.c_str());
}





void cClientHandle::HandleLogin(cPacket_Login * a_Packet)
{
	LOG("LOGIN %s", m_Username.c_str());
	if (a_Packet->m_ProtocolVersion < m_ProtocolVersion)
	{
		Kick("Your client is outdated!");
		return;
	}
	else if (a_Packet->m_ProtocolVersion > m_ProtocolVersion)
	{
		Kick("Your client version is higher than the server!");
		return;
	}
	if (m_Username.compare(a_Packet->m_Username) != 0)
	{
		LOGWARNING("Login Username (\"%s\") does not match Handshake username (\"%s\") for client \"%s\")",
			a_Packet->m_Username.c_str(),
			m_Username.c_str(),
			m_Socket.GetIPString().c_str()
		);
		Kick("Hacked client");  // Don't tell them why we don't want them
		return;
	}

	if (cRoot::Get()->GetPluginManager()->CallHook(cPluginManager::E_PLUGIN_LOGIN, 1, a_Packet))
	{
		Destroy();
		return;
	}

	// Schedule for authentication; until then, let them wait (but do not block)
	m_State = csAuthenticating;
	cRoot::Get()->GetAuthenticator().Authenticate(m_Username, cRoot::Get()->GetServer()->GetServerID());
}





void cClientHandle::HandleUnexpectedPacket(cPacket * a_Packet)
{
	LOGWARNING(
		"Invalid packet in state %d: 0x%02x from client \"%s\", username \"%s\"", 
		m_State,
		a_Packet->m_PacketID,
		m_Socket.GetIPString().c_str(),
		m_Username.c_str()
	);
	Kick("Hacked client");  // Don't tell them why we don't like them
}





void cClientHandle::HandleMoveLookConfirm(cPacket_PlayerMoveLook * a_Packet)
{
	Vector3d ReceivedPosition = Vector3d(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ);

	// Test the distance between points with a small/large enough value instead of comparing directly. Floating point inaccuracies might screw stuff up
	double Dist = (ReceivedPosition - m_ConfirmPosition).SqrLength();
	if (Dist < 1.0)
	{
		// Test
		if (ReceivedPosition.Equals(m_ConfirmPosition))
		{
			LOGINFO("Exact position confirmed by client!");
		}
		m_State = csPlaying;
	}
	else
	{
		LOGWARNING("Player \"%s\" sent a weird position confirmation %.2f blocks away, retrying", m_Username.c_str(), Dist);
		m_ConfirmPosition = m_Player->GetPosition();
		Send(cPacket_PlayerMoveLook(m_Player));
	}
}





void cClientHandle::HandleCreativeInventory(cPacket_CreativeInventoryAction * a_Packet)
{
	// This is for creative Inventory changes
	if (m_Player->GetGameMode() == 1)
	{
		m_Player->GetInventory().Clicked(a_Packet);
	}
	else
	{
		LOGWARNING("Got a CreativeInventoryAction packet from user \"%s\" while not in creative mode. Ignoring.", m_Username.c_str());
	}
}





void cClientHandle::HandlePlayerPos(cPacket_PlayerPosition * a_Packet)
{
	// LOG("recv player pos: %0.2f %0.2f %0.2f", PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ);
	m_Player->MoveTo(Vector3d(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ));
	m_Player->SetStance(a_Packet->m_Stance);
	m_Player->SetTouchGround(a_Packet->m_bFlying);
}





void cClientHandle::HandleBlockDig(cPacket_BlockDig * a_Packet)
{
	if (!CheckBlockInteractionsRate())
	{
		return;
	}

	LOG("OnBlockDig: %i %i %i Dir: %i Stat: %i", a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ, a_Packet->m_Direction, a_Packet->m_Status);
	if (a_Packet->m_Status == 0x04)	// Drop block
	{
		m_Player->TossItem(false);
		return;
	}
	
	cWorld* World = m_Player->GetWorld();

	char OldBlock = World->GetBlock(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ);
	char MetaData = World->GetBlockMeta(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ);

	bool bBroken = (
		(a_Packet->m_Status == 0x02) || 
		(g_BlockOneHitDig[(int)OldBlock]) || 
		((a_Packet->m_Status == 0x00) && (m_Player->GetGameMode() == 1)) ||
		((m_Player->GetInventory().GetEquippedItem().m_ItemID == E_ITEM_SHEARS) && (OldBlock == E_BLOCK_LEAVES))
	);

	if ((OldBlock == E_BLOCK_WOODEN_DOOR) && !bBroken)
	{
		cDoors::ChangeDoor(m_Player->GetWorld(), a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ);
	}

	cItem PickupItem;
	if (bBroken && !(m_Player->GetGameMode() == 1)) // broken
	{
		ENUM_ITEM_ID PickupID = cBlockToPickup::ToPickup((ENUM_BLOCK_ID)OldBlock, m_Player->GetInventory().GetEquippedItem().m_ItemID);
		PickupItem.m_ItemID = PickupID;
		PickupItem.m_ItemHealth = MetaData;
		PickupItem.m_ItemCount = cBlockToPickup::PickupCount(OldBlock);
		if (OldBlock == E_BLOCK_LAPIS_ORE)
		{
			PickupItem.m_ItemHealth = 4;
		}
		if (cDoors::IsDoor(OldBlock))
		{
			PickupItem.m_ItemHealth = 1;	//For a complete door this works :D
		}
	}
	
	if (cRoot::Get()->GetPluginManager()->CallHook(cPluginManager::E_PLUGIN_BLOCK_DIG, 2, a_Packet, m_Player, &PickupItem))
	{
		World->SendBlockTo(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ, m_Player);
		return;
	}
	
	int pX = a_Packet->m_PosX, pY = a_Packet->m_PosY, pZ = a_Packet->m_PosZ;

	AddDirection(pX, (char &) pY, pZ, a_Packet->m_Direction);

	char PossibleBlock = World->GetBlock(pX, pY, pZ);

	if (PossibleBlock == E_BLOCK_FIRE)
	{
		a_Packet->m_PosX = pX;
		a_Packet->m_PosY = (char)pY;
		a_Packet->m_PosZ = pZ;
		bBroken = true;
	}

	if (!bBroken)
	{
		return;
	}
	
	if (!World->DigBlock(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ, PickupItem))
	{
		return;
	}
	
	if (OldBlock == E_BLOCK_REDSTONE_TORCH_ON)
	{
		cRedstone Redstone(World);
		Redstone.ChangeRedstone(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ, false);
	}
	if (OldBlock == E_BLOCK_REDSTONE_TORCH_OFF)
	{
		cRedstone Redstone(World);
		Redstone.ChangeRedstone(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ, false);
	}
	if (OldBlock == E_BLOCK_REDSTONE_WIRE)
	{
		cRedstone Redstone(World);
		Redstone.ChangeRedstone(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ, false);
	}
	if ((OldBlock == E_BLOCK_PISTON) || (OldBlock == E_BLOCK_STICKY_PISTON))
	{
		int newX = a_Packet->m_PosX;
		int newY = a_Packet->m_PosY;
		int newZ = a_Packet->m_PosZ;
		AddPistonDir(newX, newY, newZ, MetaData & ~(8), 1);
		if (World->GetBlock(newX, newY, newZ) == E_BLOCK_PISTON_EXTENSION)
		{
			World->SetBlock(newX, newY, newZ, E_BLOCK_AIR, 0);
		}
	}

	if (cDoors::IsDoor(OldBlock))
	{
		// Special actions for destroyed door (Destroy second part)
		if (MetaData & 8)
		{
			// Was upper part of door
			if (cDoors::IsDoor(World->GetBlock(a_Packet->m_PosX, a_Packet->m_PosY - 1, a_Packet->m_PosZ)))
			{
				World->SetBlock(a_Packet->m_PosX, a_Packet->m_PosY - 1, a_Packet->m_PosZ, E_BLOCK_AIR, 0);
			}
		}
		else
		{
			// Was lower part
			if (cDoors::IsDoor(World->GetBlock(a_Packet->m_PosX, a_Packet->m_PosY + 1, a_Packet->m_PosZ)))
			{
				World->SetBlock(a_Packet->m_PosX, a_Packet->m_PosY + 1, a_Packet->m_PosZ, E_BLOCK_AIR, 0);
			}
		}
	}
	
	m_Player->UseEquippedItem();
}





void cClientHandle::HandleBlockPlace(cPacket_BlockPlace * a_Packet)
{
	if (!CheckBlockInteractionsRate())
	{
		return;
	}
	
	cItem & Equipped = m_Player->GetInventory().GetEquippedItem();

	if ((Equipped.m_ItemID != a_Packet->m_ItemType))	// Not valid
	{
		LOGWARN("Player %s tried to place a block that was not selected! (could indicate bot)", m_Username.c_str());
		// TODO: We should probably send the current world block to the client, so that it can immediately "let the user know" that they haven't placed the block
		return;
	}

	if (cRoot::Get()->GetPluginManager()->CallHook(cPluginManager::E_PLUGIN_BLOCK_PLACE, 2, a_Packet, m_Player))
	{
		if (a_Packet->m_Direction > -1)
		{
			AddDirection(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ, a_Packet->m_Direction);
			m_Player->GetWorld()->SendBlockTo(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ, m_Player);
		}
		return;
	}

	//LOG("%i %i %i %i %i %i", a_Packet->m_Count, a_Packet->m_Direction, a_Packet->m_ItemType, a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ);

	//printf("Place Dir:%i %i %i %i : %i\n", a_Packet->m_Direction, a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ, a_Packet->m_ItemType);
	// 'use' useable items instead of placing blocks
	bool bPlaceBlock = true;
	bool UpdateRedstone = false;
	bool AddedCurrent = false;

	if (a_Packet->m_Direction >= 0)
	{
		ENUM_BLOCK_ID BlockID = (ENUM_BLOCK_ID)m_Player->GetWorld()->GetBlock(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ);
		switch (BlockID)
		{
			case E_BLOCK_REDSTONE_REPEATER_ON:
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			{
				// no need to update redstone current with a repeater
				// TODO: Find meta value of repeater and change it to one step more.
				break;
			}
			
			case E_BLOCK_WORKBENCH:
			{
				bPlaceBlock = false;
				cWindow* Window = new cCraftingWindow(0, true);
				m_Player->OpenWindow(Window);
				break;
			}

			case E_BLOCK_FURNACE:
			case E_BLOCK_CHEST:
			{
				bPlaceBlock = false;
				m_Player->GetWorld()->UseBlockEntity(m_Player, a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ);
				break;
			}
			
			case E_BLOCK_WOODEN_DOOR:
			{
				bPlaceBlock = false;
				cDoors::ChangeDoor(m_Player->GetWorld(), a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ);
				break;
			}
			default:
			{
				break;
			}
		}  // switch (BlockID)
	}  // if (Direction >= 0)

	// Some checks to see if it's a placeable item :P
	if (bPlaceBlock)
	{
		cItem Item;
		Item.m_ItemID = Equipped.m_ItemID;
		Item.m_ItemCount = 1;
		LOG("Placing item of type: %i", (int)a_Packet->m_ItemType);

		// Hacked in edible items go!~
		// TODO: Handle hunger
		bool bEat = false;
		bool isDoor = false;
		switch (Item.m_ItemID)
		{
			case E_ITEM_APPLE:
				//m_Player->Heal(4); // 2 hearts
				m_Player->Feed(24); // 2 food bars
				bEat = true;
				break;
			case E_ITEM_GOLDEN_APPLE:
				//m_Player->Heal(20); // 10 hearts
				m_Player->Feed(60); // 5 food
				bEat = true;
				break;
			case E_ITEM_MUSHROOM_SOUP:
				///m_Player->Heal(10); // 5 hearts
				m_Player->Feed(48); // 4 food
				bEat = true;
				break;
			case E_ITEM_BREAD:
				//m_Player->Heal(5); // 2.5 hearts
				m_Player->Feed(30); // 2.5 food
				bEat = true;
				break;
			case E_ITEM_RAW_MEAT:
				//m_Player->Heal(3); // 1.5 hearts
				m_Player->Feed(18); // 1.5 food
				bEat = true;
				break;
			case E_ITEM_COOKED_MEAT:
				//m_Player->Heal(8); // 4 hearts
				m_Player->Feed(48); // 4 food
				bEat = true;
				break;
			case E_ITEM_RAW_FISH:
				//m_Player->Heal(2); // 1 heart
				m_Player->Feed(12); // 1 food
				bEat = true;
				break;
			case E_ITEM_COOKED_FISH:
				//m_Player->Heal(5); // 2.5 hearts
				m_Player->Feed(30); // 2.5 food
				bEat = true;
				break;
			case E_ITEM_RAW_CHICKEN:
				//m_Player->Heal(3);
				m_Player->Feed(12); // 1 food
				bEat = true;
				break;
			case E_ITEM_COOKED_CHICKEN:
				//m_Player->Heal(8);
				m_Player->Feed(36); // 3 food
				bEat = true;
				break;
			case E_ITEM_RAW_BEEF:
				//m_Player->Heal(3);
				m_Player->Feed(18); // 1.5 food
				bEat = true;
				break;
			case E_ITEM_STEAK:
				//m_Player->Heal(8);
				m_Player->Feed(48); // 4 food
				bEat = true;
				break;
			default:
				break;
		};

		if (bEat)
		{
			m_Player->GetInventory().RemoveItem(Item);
			return;
		}

		if (a_Packet->m_Direction < 0)
		{
			// clicked in air
			return;
		}

		//TODO: Wrong Blocks!
		int ClickedBlock = (int)m_Player->GetWorld()->GetBlock(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ);
		char MetaData = (char)Equipped.m_ItemHealth;
		bool LavaBucket = false;
		bool WaterBucket = false;
		bool bRemoveItem = true;
		bool bIgnoreCollision = false;

		if (ClickedBlock == E_BLOCK_STEP)
		{
			if (MetaData == m_Player->GetWorld()->GetBlockMeta(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ) && a_Packet->m_Direction == 1)	//only make double slab if meta values are the same and if player clicked on top of the block (Dir = 1)
			{
				a_Packet->m_ItemType = E_BLOCK_DOUBLE_STEP;
				a_Packet->m_PosY--;
				bIgnoreCollision = true;
			}
		}

		// Special handling for special items:
		switch (a_Packet->m_ItemType)  
		{
			case E_ITEM_BUCKET:
			{
				// TODO: Change this, it is just a small hack to get it working a little bit. seems like the Client sends the position from the first hitable block :s
				ClickedBlock = (int)m_Player->GetWorld()->GetBlock(a_Packet->m_PosX, a_Packet->m_PosY + 1, a_Packet->m_PosZ);
				LOG("Bucket Clicked BlockID: %d", ClickedBlock);
				switch (ClickedBlock)
				{
					case E_BLOCK_WATER:
					case E_BLOCK_STATIONARY_WATER:
					{
						WaterBucket = true;
						break;
					}
					case E_BLOCK_LAVA:
					case E_BLOCK_STATIONARY_LAVA:
					{
						LavaBucket = true;
						break;
					}
				}
				break;
			}  // case E_ITEM_BUCKET			
			
			case E_ITEM_LAVA_BUCKET:
			{
				if ((m_Player->GetGameMode() == 1) || (m_Player->GetInventory().RemoveItem(Item)))
				{
					a_Packet->m_ItemType = E_BLOCK_LAVA;
					if (m_Player->GetGameMode() == 1)
					{
						break;						//No new Bucket for creative players
					}

					cItem NewItem;
					NewItem.m_ItemID = E_ITEM_BUCKET;
					NewItem.m_ItemCount = 1;
					m_Player->GetInventory().AddItem(NewItem);
				}
				break;
			}  // case E_ITEM_LAVA_BUCKET
			
			case E_ITEM_WATER_BUCKET:
			{
				if ((m_Player->GetGameMode() == 1) || (m_Player->GetInventory().RemoveItem(Item)))
				{
					a_Packet->m_ItemType = E_BLOCK_WATER;
					if (m_Player->GetGameMode() == 1)
					{
						break;						//No new Bucket for creative players
					}
					cItem NewItem;
					NewItem.m_ItemID = E_ITEM_BUCKET;
					NewItem.m_ItemCount = 1;
					m_Player->GetInventory().AddItem(NewItem);
				}
				break;
			}
			
			case E_BLOCK_TORCH:
			{
				MetaData = cTorch::DirectionToMetaData(a_Packet->m_Direction);
				break;
			}
			
			case E_BLOCK_REDSTONE_TORCH_OFF:
			{
				MetaData = cTorch::DirectionToMetaData(a_Packet->m_Direction);
				if (g_BlockTransparent[ (int)m_Player->GetWorld()->GetBlock(a_Packet->m_PosX, a_Packet->m_PosY+2, a_Packet->m_PosZ) ] == true) 
				{
					//if block above is transparent
					//printf("transparent above me\n");
				}
				else
				{
					//printf("transparent not above me\n");
				}
				UpdateRedstone = true;
				AddedCurrent = false;
				break;
			}
			
			case E_BLOCK_REDSTONE_TORCH_ON:
			{
				MetaData = cTorch::DirectionToMetaData(a_Packet->m_Direction);
				UpdateRedstone = true;
				AddedCurrent = true;
				break;
			}
			
			case E_ITEM_REDSTONE_DUST:
			{
				MetaData = 0;
				a_Packet->m_ItemType = E_BLOCK_REDSTONE_WIRE;
				UpdateRedstone = true;
				AddedCurrent = false;
				break;
			}
			
			case E_ITEM_REDSTONE_REPEATER:
			{
				MetaData = cRedstone::RepeaterRotationToMetaData(m_Player->GetRotation());
				a_Packet->m_ItemType = E_BLOCK_REDSTONE_REPEATER_OFF;
				UpdateRedstone = true;
				AddedCurrent = false;
				break;
			}

			case E_BLOCK_PISTON:
			case E_BLOCK_STICKY_PISTON:
			{
				MetaData = cPiston::RotationPitchToMetaData(m_Player->GetRotation(), m_Player->GetPitch());
				UpdateRedstone = true;
				AddedCurrent = false;
				break;
			}
			
			case E_ITEM_IRON_DOOR:
			{
				a_Packet->m_ItemType = E_BLOCK_IRON_DOOR;
				MetaData = cDoors::RotationToMetaData(m_Player->GetRotation());
				isDoor = true;
				break;
			}
			
			case E_ITEM_WOODEN_DOOR:
			{
				a_Packet->m_ItemType = E_BLOCK_WOODEN_DOOR;
				MetaData = cDoors::RotationToMetaData(m_Player->GetRotation());
				isDoor = true;
				break;
			}
			
			case E_BLOCK_CHEST:
			case E_BLOCK_FURNACE:
			case E_BLOCK_DISPENSER:
			{
				MetaData = cPiston::RotationPitchToMetaData(m_Player->GetRotation(), 0); // Same orientation as pistons, just ignore pitch
				break;
			}
			
			case E_BLOCK_COBBLESTONE_STAIRS:
			case E_BLOCK_BRICK_STAIRS:
			case E_BLOCK_STONE_BRICK_STAIRS:
			case E_BLOCK_NETHER_BRICK_STAIRS:
			case E_BLOCK_WOODEN_STAIRS:
			{
				MetaData = cStairs::RotationToMetaData(m_Player->GetRotation());
				break;
			}
			case E_BLOCK_LADDER:
			{
				MetaData = cLadder::DirectionToMetaData(a_Packet->m_Direction);
				break;
			}
			case E_ITEM_SIGN:
			{
				LOG("Sign Dir: %i", a_Packet->m_Direction);
				if (a_Packet->m_Direction == 1)
				{
					LOG("Player Rotation: %f", m_Player->GetRotation());
					MetaData = cSign::RotationToMetaData(m_Player->GetRotation());
					LOG("Sign rotation %i", MetaData);
					a_Packet->m_ItemType = E_BLOCK_SIGN_POST;
				}
				else
				{
					MetaData = cSign::DirectionToMetaData(a_Packet->m_Direction);
					a_Packet->m_ItemType = E_BLOCK_WALLSIGN;
				}
				break;
			}
		
			case E_ITEM_FLINT_AND_STEEL:
			{
				a_Packet->m_ItemType = E_ITEM_FIRE;
				m_Player->UseEquippedItem();
				bRemoveItem = false;
				break;
			}
			default:
			{
				break;
			}
		}  // switch (a_Packet->m_ItemType)


		if (LavaBucket)
		{
			if ((m_Player->GetGameMode() == 1) || (m_Player->GetInventory().RemoveItem(Item))) {
				cItem NewItem;
				NewItem.m_ItemID = E_ITEM_LAVA_BUCKET;
				NewItem.m_ItemCount = 1;
				m_Player->GetInventory().AddItem(NewItem);
				m_Player->GetWorld()->SetBlock(a_Packet->m_PosX, a_Packet->m_PosY + 1, a_Packet->m_PosZ, E_BLOCK_AIR, 0);
			}
		}
		else if (WaterBucket)
		{
			if ((m_Player->GetGameMode() == 1) || (m_Player->GetInventory().RemoveItem(Item)))
			{
				cItem NewItem;
				NewItem.m_ItemID = E_ITEM_WATER_BUCKET;
				NewItem.m_ItemCount = 1;
				m_Player->GetInventory().AddItem(NewItem);
				m_Player->GetWorld()->SetBlock(a_Packet->m_PosX, a_Packet->m_PosY + 1, a_Packet->m_PosZ, E_BLOCK_AIR, 0);
			}
		}
		else if (IsValidBlock(a_Packet->m_ItemType))
		{
			int X = a_Packet->m_PosX;
			char Y = a_Packet->m_PosY;
			int Z = a_Packet->m_PosZ;
			AddDirection(X, Y, Z, a_Packet->m_Direction);

			int PlaceBlock = m_Player->GetWorld()->GetBlock(X, Y, Z);
			if (
				(PlaceBlock != E_BLOCK_AIR)
				&& (PlaceBlock != E_BLOCK_WATER)
				&& (PlaceBlock != E_BLOCK_STATIONARY_WATER)
				&& (PlaceBlock != E_BLOCK_LAVA)
				&& (PlaceBlock != E_BLOCK_STATIONARY_LAVA)
				&& !bIgnoreCollision
			)
			{
				//tried to place a block *into* another?
				return;  // happens when you place a block aiming at side of block like torch or stem
			}

			// Check whether selected item is allowed to be placed on specific surface
			bool bIllegalSurface = false;
			ENUM_BLOCK_ID SurfaceBlock = (ENUM_BLOCK_ID)m_Player->GetWorld()->GetBlock(X, Y-1, Z);
			switch (a_Packet->m_ItemType)
			{
				case E_BLOCK_YELLOW_FLOWER:	// Can ONLY be placed on dirt/grass
				case E_BLOCK_RED_ROSE:
				case E_BLOCK_SAPLING:
				{
					switch (SurfaceBlock)
					{
						case E_BLOCK_DIRT:
						case E_BLOCK_GRASS:
						{
							bIllegalSurface = false;
							break;
						}
						default:
						{
							bIllegalSurface = true;
							break;
						}
					};
					break;
				}
				
				case E_BLOCK_BROWN_MUSHROOM:	// Can be placed on pretty much anything, with exceptions
				case E_BLOCK_RED_MUSHROOM:
				{
					switch (SurfaceBlock)
					{
						case E_BLOCK_GLASS:
						case E_BLOCK_YELLOW_FLOWER:
						case E_BLOCK_RED_ROSE:
						case E_BLOCK_BROWN_MUSHROOM:
						case E_BLOCK_RED_MUSHROOM:
						case E_BLOCK_CACTUS:
						{
							bIllegalSurface = true;
							break;
						}
					}
					break;
				}
				
				case E_BLOCK_CACTUS:
				{
					bIllegalSurface = (SurfaceBlock != E_BLOCK_SAND) && (SurfaceBlock != E_BLOCK_CACTUS);  // Cactus can only be placed on sand and itself

					// Check surroundings. Cacti may ONLY be surrounded by air
					cWorld * World = m_Player->GetWorld();
					if (
						(World->GetBlock(X - 1, Y, Z)     != E_BLOCK_AIR) ||
						(World->GetBlock(X + 1, Y, Z)     != E_BLOCK_AIR) ||
						(World->GetBlock(X,     Y, Z - 1) != E_BLOCK_AIR) ||
						(World->GetBlock(X,     Y, Z + 1) != E_BLOCK_AIR)
					)
					{
						bIllegalSurface = true;
					}
					break;
				}
			}  // switch (a_Packet->m_ItemType)

			if (bIllegalSurface)
			{
				return;
			}

			if (bRemoveItem)
			{
				if ((m_Player->GetGameMode() != 1) && !m_Player->GetInventory().RemoveItem(Item))
				{
					return;
				}
			}


			if (isDoor)
			{
				if ((m_Player->GetWorld()->GetBlock(X, Y + 1, Z) == E_BLOCK_AIR) || (m_Player->GetWorld()->GetBlock(X, Y + 1, Z) == E_BLOCK_AIR))
				{
					m_Player->GetWorld()->SetBlock(X, Y + 1, Z, (char)a_Packet->m_ItemType, MetaData + 8);
					m_Player->GetWorld()->SetBlock(X, Y,     Z, (char)a_Packet->m_ItemType, MetaData);
				}
			}
			else
			{
				m_Player->GetWorld()->SetBlock(X, Y, Z, (char)a_Packet->m_ItemType, MetaData);
			}

			if (UpdateRedstone)
			{
				cRedstone Redstone(m_Player->GetWorld());
				Redstone.ChangeRedstone(a_Packet->m_PosX, a_Packet->m_PosY + 1, a_Packet->m_PosZ, AddedCurrent);
			}
		}
	}

	/*
	// FakeTruth's "magic stick of removal" :D
	// TODO: Turn this into a plugin
	if (m_Username.compare("FakeTruth") == 0)
	{
		if (a_Packet->m_ItemType == 280)
		{
			cRoot::Get()->GetWorld()->SetBlock(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ, 0, 0);
		}
	}
	*/
}





void cClientHandle::HandlePickupSpawn(cPacket_PickupSpawn * a_Packet)
{
	LOG("Received packet E_PICKUP_SPAWN");

	cItem DroppedItem;
	DroppedItem.m_ItemID = (ENUM_ITEM_ID)a_Packet->m_Item;
	DroppedItem.m_ItemCount = a_Packet->m_Count;
	DroppedItem.m_ItemHealth = 0x0; // TODO: Somehow figure out what item was dropped, and apply correct health
	if (m_Player->GetInventory().RemoveItem(DroppedItem))
	{
		cPickup * Pickup = new cPickup(a_Packet);
		Pickup->Initialize(m_Player->GetWorld());
	}
}





void cClientHandle::HandleChat(cPacket_Chat * a_Packet)
{
	if (!cRoot::Get()->GetServer()->Command(*this, a_Packet->m_Message.c_str()))
	{
		a_Packet->m_Message.insert(0, "<" + m_Player->GetColor() + m_Username + cChatColor::White + "> ");
		cRoot::Get()->GetServer()->Broadcast(*a_Packet);
	}
}





void cClientHandle::HandlePlayerLook(cPacket_PlayerLook * a_Packet)
{
	m_Player->SetRotation   (a_Packet->m_Rotation);
	m_Player->SetPitch      (a_Packet->m_Pitch);
	m_Player->SetTouchGround(a_Packet->m_bFlying);
	m_Player->WrapRotation();
}





void cClientHandle::HandlePlayerMoveLook(cPacket_PlayerMoveLook * a_Packet)
{
	m_Player->MoveTo(Vector3d(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ));
	m_Player->SetStance     (a_Packet->m_Stance);
	m_Player->SetTouchGround(a_Packet->m_bFlying);
	m_Player->SetRotation   (a_Packet->m_Rotation);
	m_Player->SetPitch      (a_Packet->m_Pitch);
	m_Player->WrapRotation();
}





void cClientHandle::HandleAnimation(cPacket_ArmAnim * a_Packet)
{
	a_Packet->m_EntityID = m_Player->GetUniqueID();
	cRoot::Get()->GetServer()->Broadcast(*a_Packet, this);
}





void cClientHandle::HandleItemSwitch(cPacket_ItemSwitch * a_Packet)
{
	m_Player->GetInventory().SetEquippedSlot(a_Packet->m_SlotNum);

	cPacket_EntityEquipment Equipment;
	Equipment.m_ItemID = (short)m_Player->GetInventory().GetEquippedItem().m_ItemID;
	Equipment.m_Slot = 0;
	Equipment.m_UniqueID = m_Player->GetUniqueID();
	cRoot::Get()->GetServer()->Broadcast(Equipment, this);
}





void cClientHandle::HandleWindowClose(cPacket_WindowClose * a_Packet)
{
	m_Player->CloseWindow(a_Packet->m_Close);
}





void cClientHandle::HandleWindowClick(cPacket_WindowClick * a_Packet)
{
	if (a_Packet->m_WindowID == 0)
	{
		m_Player->GetInventory().Clicked(a_Packet);
		return;
	}
	
	cWindow * Window = m_Player->GetWindow();
	if (Window == NULL)
	{
		LOGWARNING("Player \"%s\" clicked in a non-existent window. Ignoring", m_Username.c_str());
		return;
	}
	
	Window->Clicked(a_Packet, *m_Player);
}





void cClientHandle::HandleUpdateSign(cPacket_UpdateSign * a_Packet)
{
	cWorld * World = m_Player->GetWorld();
	World->UpdateSign(a_Packet->m_PosX, a_Packet->m_PosY, a_Packet->m_PosZ, a_Packet->m_Line1, a_Packet->m_Line2, a_Packet->m_Line3, a_Packet->m_Line4);
}





void cClientHandle::HandleUseEntity(cPacket_UseEntity * a_Packet)
{
	if (!a_Packet->m_bLeftClick)
	{
		return;
	}

	class cDamageEntity : public cEntityCallback
	{
		virtual bool Item(cEntity * a_Entity) override
		{
			if( a_Entity->IsA("cPawn") )
			{
				reinterpret_cast< cPawn* >( a_Entity )->TakeDamage(Damage, Instigator );
			}
			return true;
		}
	public:
		int Damage;
		cEntity * Instigator;
	} Callback;

	Callback.Damage = 1; // TODO: Find proper damage from current item equipped
	Callback.Instigator = m_Player;

	cWorld * World = m_Player->GetWorld();
	World->DoWithEntity( a_Packet->m_TargetID, Callback );
}





void cClientHandle::HandleRespawn(void)
{
	m_Player->Respawn();
}





void cClientHandle::HandleDisconnect(cPacket_Disconnect * a_Packet)
{
	LOG("Received d/c packet from \"%s\"", m_Username.c_str());
	if (!cRoot::Get()->GetPluginManager()->CallHook(cPluginManager::E_PLUGIN_DISCONNECT, 2, a_Packet->m_Reason.c_str(), m_Player))
	{
		cPacket_Chat DisconnectMessage(m_Username + " disconnected: " + a_Packet->m_Reason);
		cRoot::Get()->GetServer()->Broadcast(DisconnectMessage);
	}
	Destroy();
}





void cClientHandle::HandleKeepAlive(cPacket_KeepAlive * a_Packet)
{
	if (a_Packet->m_KeepAliveID == m_PingID)
	{
		cTimer t1;
		m_Ping = (short)((t1.GetNowTime() - m_PingStartTime) / 2);
	}
}





bool cClientHandle::CheckBlockInteractionsRate(void)
{
	int LastActionCnt = m_Player->GetLastBlockActionCnt();
	if ((cRoot::Get()->GetWorld()->GetTime() - m_Player->GetLastBlockActionTime()) < 0.1)
	{
		// Limit the number of block interactions per tick
		m_Player->SetLastBlockActionTime(); //Player tried to interact with a block. Reset last block interation time.
		m_Player->SetLastBlockActionCnt(LastActionCnt + 1);
		if (m_Player->GetLastBlockActionCnt() > MAXBLOCKCHANGEINTERACTIONS)
		{
			// Kick if more than MAXBLOCKCHANGEINTERACTIONS per tick
			LOGWARN("Player %s tried to interact with a block too quickly! (could indicate bot) Was Kicked.", m_Username.c_str());
			Kick("You're a baaaaaad boy!");
			return false;
		}
	}
	else
	{
		m_Player->SetLastBlockActionCnt(0);  // Reset count 
		m_Player->SetLastBlockActionTime();  // Player tried to interact with a block. Reset last block interation time.
	}
	return true;
}





void cClientHandle::Tick(float a_Dt)
{
	(void)a_Dt;
	if (cWorld::GetTime() - m_TimeLastPacket > 30.f)  // 30 seconds time-out
	{
		cPacket_Disconnect DC("Nooooo!! You timed out! D: Come back!");
		m_Socket.Send(&DC);

		// TODO: Cannot sleep in the tick thread!
		cSleep::MilliSleep(1000);  // Give packet some time to be received

		Destroy();
	}
	
	cTimer t1;
	// Send ping packet
	if (m_LastPingTime + cClientHandle::PING_TIME_MS <= t1.GetNowTime())
	{
		m_PingID++;
		cPacket_KeepAlive Ping(m_PingID);
		m_PingStartTime = t1.GetNowTime();
		Send(Ping);
		m_LastPingTime = m_PingStartTime;
	}
	
	if (m_State >= csDownloadingWorld)
	{
		cWorld * World = m_Player->GetWorld();
		cCSLock Lock(m_CSChunkLists);
		int NumSent = 0;
		for (cChunkCoordsList::iterator itr = m_ChunksToSend.begin(); itr != m_ChunksToSend.end();)
		{
			if (!World->SendChunkTo(itr->m_ChunkX, itr->m_ChunkY, itr->m_ChunkZ, this))
			{
				++itr;
				continue;
			}
			itr = m_ChunksToSend.erase(itr);
			NumSent++;
			if (NumSent > 10)
			{
				// Only send up to 10 chunks per tick, otherwise we'd choke the tick thread
				break;
			}
		}  // for itr - m_ChunksToSend[]
		
		// Check even if we didn't send anything - a chunk may have sent a notification that we'd miss otherwise
		CheckIfWorldDownloaded();
	}
}





void cClientHandle::Send(const cPacket * a_Packet, ENUM_PRIORITY a_Priority /* = E_PRIORITY_NORMAL */)
{
	if (m_bKicking) return; // Don't add more packets if player is getting kicked anyway

	// If it is the packet spawning myself for myself, drop it silently:
	if (a_Packet->m_PacketID == E_NAMED_ENTITY_SPAWN)
	{
		if (((cPacket_NamedEntitySpawn *)a_Packet)->m_UniqueID == m_Player->GetUniqueID())
		{
			return;
		}
	}
	
	// Filter out packets that don't belong to a csDownloadingWorld state:
	if (m_State == csDownloadingWorld)
	{
		switch (a_Packet->m_PacketID)
		{
			case E_PLAYERMOVELOOK:
			case E_KEEP_ALIVE:
			case E_PRE_CHUNK:
			{
				// Allow
				break;
			}
			case E_MAP_CHUNK:
			{
				CheckIfWorldDownloaded();
				break;
			}
			
			default: return;
		}
	}
	
	bool bSignalSemaphore = true;
	cCSLock Lock(m_SendCriticalSection);
	if (a_Priority == E_PRIORITY_NORMAL)
	{
		if (a_Packet->m_PacketID == E_REL_ENT_MOVE_LOOK)
		{
			PacketList & Packets = m_PendingNrmSendPackets;
			for (PacketList::iterator itr = Packets.begin(); itr != Packets.end(); ++itr)
			{
				bool bBreak = false;
				switch ((*itr)->m_PacketID)
				{
					case E_REL_ENT_MOVE_LOOK:
					{
						const cPacket_RelativeEntityMoveLook* ThisPacketData = reinterpret_cast< const cPacket_RelativeEntityMoveLook* >(a_Packet);
						cPacket_RelativeEntityMoveLook* PacketData = reinterpret_cast< cPacket_RelativeEntityMoveLook* >(*itr);
						if (ThisPacketData->m_UniqueID == PacketData->m_UniqueID)
						{
							Packets.erase(itr);
							bBreak = true;
							bSignalSemaphore = false; // Because 1 packet is removed, semaphore count is the same
							delete PacketData;
							break;
						}
						break;
					}  // case E_REL_END_MOVE_LOOK
				}  // switch (*itr -> Packet type)
				if (bBreak)
				{
					break;
				}
			}  // for itr - Packets[]
		}  // if (E_REL_ENT_MOVE_LOOK
		m_PendingNrmSendPackets.push_back(a_Packet->Clone());
	}
	else if (a_Priority == E_PRIORITY_LOW)
	{
		m_PendingLowSendPackets.push_back(a_Packet->Clone());
	}
	Lock.Unlock();
	if (bSignalSemaphore)
	{
		m_Semaphore.Signal();
	}
}





void cClientHandle::CheckIfWorldDownloaded(void)
{
	if (m_State != csDownloadingWorld)
	{
		return;
	}
	cCSLock Lock(m_CSChunkLists);
	if (m_ChunksToSend.empty())
	{
		SendConfirmPosition();
	}
}





void cClientHandle::SendConfirmPosition(void)
{
	LOG("Spawning player \"%s\" at {%.2f, %.2f, %.2f}",
		m_Username.c_str(), m_Player->GetPosX(), m_Player->GetPosY(), m_Player->GetPosZ()
	);
	
	m_State = csConfirmingPos;

	if (!cRoot::Get()->GetPluginManager()->CallHook(cPluginManager::E_PLUGIN_PLAYER_JOIN, 1, m_Player))
	{
		// Broadcast that this player has joined the game! Yay~
		cPacket_Chat Joined(m_Username + " joined the game!");
		cRoot::Get()->GetServer()->Broadcast(Joined, this);
	}

	m_ConfirmPosition = m_Player->GetPosition();
	Send(cPacket_PlayerMoveLook(m_Player));
}





void cClientHandle::SendThread(void *lpParam)
{
	cClientHandle* self = (cClientHandle*)lpParam;
	PacketList & NrmSendPackets = self->m_PendingNrmSendPackets;
	PacketList & LowSendPackets = self->m_PendingLowSendPackets;


	while (self->m_bKeepThreadGoing && self->m_Socket.IsValid())
	{
		self->m_Semaphore.Wait();
		cCSLock Lock(self->m_SendCriticalSection);
		if (NrmSendPackets.size() + LowSendPackets.size() > MAX_SEMAPHORES)
		{
			LOGERROR("ERROR: Too many packets in queue for player %s !!", self->m_Username.c_str());
			cPacket_Disconnect DC("Too many packets in queue.");
			self->m_Socket.Send(DC);

			cSleep::MilliSleep(1000); // Give packet some time to be received

			Lock.Unlock();
			self->Destroy();
			break;
		}
		
		if (NrmSendPackets.size() == 0 && LowSendPackets.size() == 0)
		{
			ASSERT(!self->m_bKeepThreadGoing);
			if (self->m_bKeepThreadGoing)
			{
				LOGERROR("ERROR: Semaphore was signaled while no packets to send");
			}
			continue;
		}
		if (NrmSendPackets.size() > MAX_SEMAPHORES / 2)
		{
			LOGINFO("Pending packets: %i Last: 0x%02x", NrmSendPackets.size(), (*NrmSendPackets.rbegin())->m_PacketID);
		}

		cPacket * Packet = NULL;
		if (!NrmSendPackets.empty())
		{
			Packet = *NrmSendPackets.begin();
			NrmSendPackets.erase(NrmSendPackets.begin());
		}
		else if (!LowSendPackets.empty())
		{
			Packet = *LowSendPackets.begin();
			LowSendPackets.erase(LowSendPackets.begin());
		}
		Lock.Unlock();

		if (!self->m_Socket.IsValid())
		{
			break;
		}
		
		// LOG("Sending packet 0x%02x to \"%s\" (\"%s\")", Packet->m_PacketID, self->m_Socket.GetIPString().c_str(), self->m_Username.c_str());
		
		bool bSuccess = self->m_Socket.Send(Packet);
		
		if (!bSuccess)
		{
			LOGERROR("ERROR: While sending packet 0x%02x to client \"%s\"", Packet->m_PacketID, self->m_Username.c_str());
			delete Packet;
			self->Destroy();
			break;
		}
		delete Packet;

		if (self->m_bKicking && (NrmSendPackets.size() + LowSendPackets.size() == 0)) // Disconnect player after all packets have been sent
		{
			cSleep::MilliSleep(1000); // Give all packets some time to be received
			self->Destroy();
			break;
		}
	}

	return;
}





const AString & cClientHandle::GetUsername(void) const
{
	return m_Username;
}





void cClientHandle::DataReceived(const char * a_Data, int a_Size)
{
	// Data is received from the client

	m_ReceivedData.append(a_Data, a_Size);

	// Parse and handle all complete packets in m_ReceivedData:
	while (!m_ReceivedData.empty())
	{
		cPacket* pPacket = m_PacketMap[(unsigned char)m_ReceivedData[0]];
		if (pPacket == NULL)
		{
			LOGERROR("Unknown packet type 0x%02x from client \"%s\"", (unsigned char)m_ReceivedData[0], m_Username.c_str());

			AString Reason;
			Printf(Reason, "[C->S] Unknown PacketID: 0x%02x", m_ReceivedData[0]);
			cPacket_Disconnect DC(Reason);
			m_Socket.Send(&DC);
			cSleep::MilliSleep(1000); // Give packet some time to be received
			Destroy();
			return;
		}
		
		int NumBytes = pPacket->Parse(m_ReceivedData.data() + 1, m_ReceivedData.size() - 1);
		if (NumBytes == PACKET_ERROR)
		{
			LOGERROR("Protocol error while parsing packet type 0x%02x; disconnecting client \"%s\"", (unsigned char)m_ReceivedData[0], m_Username.c_str());
			cPacket_Disconnect DC("Protocol error");
			m_Socket.Send(&DC);
			cSleep::MilliSleep(1000); // Give packet some time to be received
			Destroy();
			return;
		}
		else if (NumBytes == PACKET_INCOMPLETE)
		{
			// Not a complete packet
			break;
		}
		else
		{
			// Packet parsed successfully, add it to internal queue:
			HandlePacket(pPacket);
			// Erase the packet from the buffer:
			ASSERT(m_ReceivedData.size() > (size_t)NumBytes);
			m_ReceivedData.erase(0, NumBytes + 1);
		}
	}  // while (!Received.empty())
}





void cClientHandle::GetOutgoingData(AString & a_Data)
{
	// Data can be sent to client
	
	// TODO
}





void cClientHandle::SocketClosed(void)
{
	// The socket has been closed for any reason
	
	// TODO
	/*
	self->Destroy();
	LOG("Client \"%s\" disconnected", GetLogName().c_str());
	*/
}




