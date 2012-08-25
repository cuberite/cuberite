
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
#include "cDoors.h"
#include "cLadder.h"
#include "cVine.h"
#include "cSign.h"
#include "cRedstone.h"
#include "cPiston.h"
#include "cBlockToPickup.h"
#include "cMonster.h"
#include "cChatColor.h"
#include "cSocket.h"
#include "cTimer.h"
#include "items/Item.h"
#include "blocks/Block.h"

#include "cTracer.h"
#include "Vector3f.h"
#include "Vector3d.h"

#include "cSleep.h"
#include "cRoot.h"

#include "cBlockingTCPLink.h"
#include "cAuthenticator.h"
#include "MersenneTwister.h"

#include "packets/cPacket_13.h"
#include "packets/cPacket_ArmAnim.h"
#include "packets/cPacket_BlockAction.h"
#include "packets/cPacket_BlockChange.h"
#include "packets/cPacket_BlockDig.h"
#include "packets/cPacket_BlockPlace.h"
#include "packets/cPacket_Chat.h"
#include "packets/cPacket_CollectItem.h"
#include "packets/cPacket_CreativeInventoryAction.h"
#include "packets/cPacket_DestroyEntity.h"
#include "packets/cPacket_Disconnect.h"
#include "packets/cPacket_EntityEquipment.h"
#include "packets/cPacket_EntityLook.h"
#include "packets/cPacket_EntityStatus.h"
#include "packets/cPacket_Flying.h"
#include "packets/cPacket_Handshake.h"
#include "packets/cPacket_InventoryProgressBar.h"
#include "packets/cPacket_InventorySlot.h"
#include "packets/cPacket_ItemSwitch.h"
#include "packets/cPacket_KeepAlive.h"
#include "packets/cPacket_Login.h"
#include "packets/cPacket_MapChunk.h"
#include "packets/cPacket_Metadata.h"
#include "packets/cPacket_MultiBlock.h"
#include "packets/cPacket_NamedEntitySpawn.h"
#include "packets/cPacket_NewInvalidState.h"
#include "packets/cPacket_PickupSpawn.h"
#include "packets/cPacket_Ping.h"
#include "packets/cPacket_Player.h"
#include "packets/cPacket_PreChunk.h"
#include "packets/cPacket_RelativeEntityMove.h"
#include "packets/cPacket_RelativeEntityMoveLook.h"
#include "packets/cPacket_Respawn.h"
#include "packets/cPacket_SpawnMob.h"
#include "packets/cPacket_TeleportEntity.h"
#include "packets/cPacket_TimeUpdate.h"
#include "packets/cPacket_UpdateHealth.h"
#include "packets/cPacket_UpdateSign.h"
#include "packets/cPacket_UseEntity.h"
#include "packets/cPacket_WholeInventory.h"
#include "packets/cPacket_WindowClick.h"
#include "packets/cPacket_WindowClose.h"
#include "packets/cPacket_WindowOpen.h"





#define AddPistonDir(x, y, z, dir, amount) switch (dir) { case 0: (y)-=(amount); break; case 1: (y)+=(amount); break;\
													 case 2: (z)-=(amount); break; case 3: (z)+=(amount); break;\
													 case 4: (x)-=(amount); break; case 5: (x)+=(amount); break; }





/// If the number of queued outgoing packets reaches this, the client will be kicked
#define MAX_OUTGOING_PACKETS 2000





#define RECI_RAND_MAX (1.f/RAND_MAX)
inline int fRadRand(MTRand & r1, int a_BlockCoord)
{
	return a_BlockCoord * 32 + (int)(16 * ((float)r1.rand() * RECI_RAND_MAX) * 16 - 8);
}





int cClientHandle::s_ClientCount = 0;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cClientHandle:

cClientHandle::cClientHandle(const cSocket & a_Socket, int a_ViewDistance)
	: m_ViewDistance(a_ViewDistance)
	, m_ProtocolVersion(MCS_PROTOCOL_VERSION)
	, m_ReceivedData(64 KiB)
	, m_Socket(a_Socket)
	, m_bDestroyed(false)
	, m_Player(NULL)
	, m_bKicking(false)
	, m_TimeLastPacket(cWorld::GetTime())
	, m_bKeepThreadGoing(true)
	, m_Ping(1000)
	, m_State(csConnected)
	, m_LastStreamedChunkX(0x7fffffff)  // bogus chunk coords to force streaming upon login
	, m_LastStreamedChunkZ(0x7fffffff)
	, m_UniqueID(0)
{
	s_ClientCount++;	// Not protected by CS because clients are always constructed from the same thread
	m_UniqueID = s_ClientCount;

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
	m_PacketMap[E_PLAYER_ABILITIES]          = new cPacket_PlayerAbilities;
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

	LOG("New ClientHandle created at %p", this);
}





cClientHandle::~cClientHandle()
{
	LOG("Deleting client \"%s\" at %p", GetUsername().c_str(), this);

	// Remove from cSocketThreads, we're not to be called anymore:
	cRoot::Get()->GetServer()->ClientDestroying(this);
	
	{
		cCSLock Lock(m_CSChunkLists);
		m_LoadedChunks.clear();
		m_ChunksToSend.clear();
	}

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
		if (World != NULL)
		{
			World->RemovePlayer(m_Player);
		}
	}

	if (m_Socket.IsValid())
	{
		if(!m_bKicking)
		{
			cPacket_Disconnect Disconnect;
			Disconnect.m_Reason = "Server shut down? Kthnxbai";
			m_Socket.Send(&Disconnect);
		}
	}
	
	if (m_Player != NULL)
	{
		m_Player->Destroy();
		m_Player = NULL;
	}
	for (int i = 0; i < ARRAYCOUNT(m_PacketMap); i++)
	{
		delete m_PacketMap[i];
	}

	// Queue all remaining outgoing packets to cSocketThreads:
	{
		cCSLock Lock(m_CSPackets);
		for (PacketList::iterator itr = m_PendingNrmSendPackets.begin(); itr != m_PendingNrmSendPackets.end(); ++itr)
		{
			AString Data;
			(*itr)->Serialize(Data);
			cRoot::Get()->GetServer()->WriteToClient(&m_Socket, Data);
			delete *itr;
		}
		m_PendingNrmSendPackets.clear();
		for (PacketList::iterator itr = m_PendingLowSendPackets.begin(); itr != m_PendingLowSendPackets.end(); ++itr)
		{
			AString Data;
			(*itr)->Serialize(Data);
			cRoot::Get()->GetServer()->WriteToClient(&m_Socket, Data);
			delete *itr;
		}
		m_PendingLowSendPackets.clear();
	}
	
	// Queue the socket to close as soon as it sends all outgoing data:
	cRoot::Get()->GetServer()->QueueClientClose(&m_Socket);
	
	// We need to remove the socket from SocketThreads because we own it and it gets destroyed after this destructor finishes
	// TODO: The socket needs to stay alive, someone else has to own it
	cRoot::Get()->GetServer()->RemoveClient(&m_Socket);
	
	LOG("ClientHandle at %p deleted", this);
}





void cClientHandle::Destroy()
{
	// Setting m_bDestroyed was moved to the bottom of Destroy(), 
	// otherwise the destructor may be called within another thread before the client is removed from chunks
	// http://forum.mc-server.org/showthread.php?tid=366
	
	if ((m_Player != NULL) && (m_Player->GetWorld() != NULL))
	{
		RemoveFromAllChunks();
		m_Player->GetWorld()->RemoveClientFromChunkSender(this);
	}

	m_bDestroyed = true;
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
	if (m_State != csAuthenticating)
	{
		return;
	}
	
	ASSERT( m_Player == NULL );

	// Spawn player (only serversided, so data is loaded)
	m_Player = new cPlayer(this, GetUsername());

	cWorld * World = cRoot::Get()->GetWorld(m_Player->GetLoadedWorldName());
	if (World == NULL)
	{
		World = cRoot::Get()->GetDefaultWorld();
	}
	
	if(m_Player->GetGameMode() == eGameMode_NotSet)
		m_Player->LoginSetGameMode(World->GetGameMode());

	m_Player->SetIP (m_Socket.GetIPString());

	cRoot::Get()->GetPluginManager()->CallHook(cPluginManager::HOOK_PLAYER_SPAWN, 1, m_Player);

	// Return a server login packet
	cPacket_Login LoginResponse;
	LoginResponse.m_ProtocolVersion = m_Player->GetUniqueID();
	//LoginResponse.m_Username = "";
	LoginResponse.m_ServerMode = m_Player->GetGameMode();  // set gamemode from player.
	LoginResponse.m_Dimension = 0;
	LoginResponse.m_MaxPlayers = (unsigned char)cRoot::Get()->GetDefaultWorld()->GetMaxPlayers();
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
	m_Player->SendHealth();
	
	m_Player->Initialize(World);
	StreamChunks();
	m_State = csDownloadingWorld;
}





void cClientHandle::StreamChunks(void)
{
	if (m_State < csAuthenticating)
	{
		return;
	}

	ASSERT(m_Player != NULL);
	
	int ChunkPosX = FAST_FLOOR_DIV(m_Player->GetPosX(), cChunkDef::Width);
	int ChunkPosZ = FAST_FLOOR_DIV(m_Player->GetPosZ(), cChunkDef::Width);
	if ((ChunkPosX == m_LastStreamedChunkX) && (ChunkPosZ == m_LastStreamedChunkZ))
	{
		// Already streamed for this position
		return;
	}
	m_LastStreamedChunkX = ChunkPosX;
	m_LastStreamedChunkZ = ChunkPosZ;
	
	LOGD("Streaming chunks centered on [%d, %d], view distance %d", ChunkPosX, ChunkPosZ, m_ViewDistance);
	
	cWorld * World = m_Player->GetWorld();
	ASSERT(World != NULL);

	// Remove all loaded chunks that are no longer in range; deferred to out-of-CS:
	cChunkCoordsList RemoveChunks;
	{
		cCSLock Lock(m_CSChunkLists);
		for (cChunkCoordsList::iterator itr = m_LoadedChunks.begin(); itr != m_LoadedChunks.end();)
		{
			int RelX = (*itr).m_ChunkX - ChunkPosX;
			int RelZ = (*itr).m_ChunkZ - ChunkPosZ;
			if ((RelX > m_ViewDistance) || (RelX < -m_ViewDistance) || (RelZ > m_ViewDistance) || (RelZ < -m_ViewDistance))
			{
				RemoveChunks.push_back(*itr);
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
			if ((RelX > m_ViewDistance) || (RelX < -m_ViewDistance) || (RelZ > m_ViewDistance) || (RelZ < -m_ViewDistance))
			{
				itr = m_ChunksToSend.erase(itr);
			}
			else
			{
				++itr;
			}
		}  // for itr - m_ChunksToSend[]
	}
	for (cChunkCoordsList::iterator itr = RemoveChunks.begin(); itr != RemoveChunks.end(); ++itr)
	{
		World->RemoveChunkClient(itr->m_ChunkX, itr->m_ChunkY, itr->m_ChunkZ, this);
		Send(cPacket_PreChunk(itr->m_ChunkX, itr->m_ChunkZ, false));
	}  // for itr - RemoveChunks[]
	
	// Add all chunks that are in range and not yet in m_LoadedChunks:
	// Queue these smartly - from the center out to the edge
	for (int d = 0; d <= m_ViewDistance; ++d)  // cycle through (square) distance, from nearest to furthest
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
	for (int d = m_ViewDistance + 1; d <= m_ViewDistance + GENERATEDISTANCE; ++d)  // cycle through (square) distance, from nearest to furthest
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
		{
			cCSLock Lock(m_CSChunkLists);
			m_LoadedChunks.push_back(cChunkCoords(a_ChunkX, a_ChunkY, a_ChunkZ));
			m_ChunksToSend.push_back(cChunkCoords(a_ChunkX, a_ChunkY, a_ChunkZ));
		}
		World->SendChunkTo(a_ChunkX, a_ChunkY, a_ChunkZ, this);
	}
}





// Removes the client from all chunks. Used when switching worlds or destroying the player
void cClientHandle::RemoveFromAllChunks()
{
	cWorld * World = m_Player->GetWorld();
	if (World != NULL)
	{
		World->RemoveClientFromChunks(this);
	}
	
	{
		cCSLock Lock(m_CSChunkLists);
		m_LoadedChunks.clear();
		m_ChunksToSend.clear();
	}
}





void cClientHandle::HandlePacket(cPacket * a_Packet)
{
	// TODO: _X: This function will get out-sourced into a separate cProtocol class
	// and the switch statements will be split into virtual functions of that class
	// Therefore I keep this function huge and untidy for the time being
	// ( http://forum.mc-server.org/showthread.php?tid=524 )
	
	// LOGD("Recv packet %02x", a_Packet->m_PacketID);
	
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
				case E_HANDSHAKE:
				{
					cPacket_Handshake * Handshake = reinterpret_cast<cPacket_Handshake *>(a_Packet);
					HandleHandshake(Handshake->m_Username);
					break;
				}
				case E_LOGIN:
				{
					cPacket_Login * Login = reinterpret_cast<cPacket_Login *>(a_Packet);
					HandleLogin(Login->m_ProtocolVersion, Login->m_Username);
					break;
				}
				
				// Ignored packets:
				case E_PLAYERLOOK:
				case E_CHAT:
				case E_PLAYERMOVELOOK:
				case E_PLAYERPOS:
				case E_KEEP_ALIVE:     break;
				default:               HandleUnexpectedPacket(a_Packet->m_PacketID); break;
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
				case E_CHAT:
				case E_FLYING:
				case E_PLAYERLOOK:
				case E_PLAYERMOVELOOK:
				case E_PLAYERPOS: break;
				
				default: HandleUnexpectedPacket(a_Packet->m_PacketID); break;
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
				case E_CHAT:
				case E_FLYING:
				case E_PLAYERLOOK:
				case E_PLAYERMOVELOOK:
				case E_PLAYERPOS: break;
				
				default: HandleUnexpectedPacket(a_Packet->m_PacketID); break;
			}
			break;
		}
		
		case csConfirmingPos:
		{
			switch (a_Packet->m_PacketID)
			{
				// Ignored packets:
				case E_KEEP_ALIVE:
				case E_CHAT:
				case E_FLYING:
				case E_PLAYERLOOK:
				case E_PLAYERPOS: break;

				case E_PLAYERMOVELOOK:
				{
					cPacket_PlayerMoveLook * MoveLook = reinterpret_cast<cPacket_PlayerMoveLook *>(a_Packet);
					HandleMoveLookConfirm(MoveLook->m_PosX, MoveLook->m_PosY, MoveLook->m_PosZ);
					break;
				}

				default:
				{
					HandleUnexpectedPacket(a_Packet->m_PacketID);
					break;
				}
			}  // switch (PacketType)
			break;
		}  // case csConfirmingPos
		
		case csPlaying:
		{
			switch (a_Packet->m_PacketID)
			{
				case E_CREATIVE_INVENTORY_ACTION:
				{
					cPacket_CreativeInventoryAction * cia = reinterpret_cast<cPacket_CreativeInventoryAction *>(a_Packet);
					HandleCreativeInventory(cia->m_SlotNum, cia->m_ClickedItem);
					break;
				}
				case E_PLAYERPOS:
				{
					cPacket_PlayerPosition * pp = reinterpret_cast<cPacket_PlayerPosition *>(a_Packet);
					HandlePlayerPos(pp->m_PosX, pp->m_PosY, pp->m_PosZ, pp->m_Stance, pp->m_IsOnGround);
					break;
				}
				case E_BLOCK_DIG:
				{
					cPacket_BlockDig * bd = reinterpret_cast<cPacket_BlockDig *>(a_Packet);
					HandleBlockDig(bd->m_PosX, bd->m_PosY, bd->m_PosZ, bd->m_Direction, bd->m_Status);
					break;
				}
				case E_BLOCK_PLACE:
				{
					cPacket_BlockPlace * bp = reinterpret_cast<cPacket_BlockPlace *>(a_Packet);
					HandleBlockPlace(bp->m_PosX, bp->m_PosY, bp->m_PosZ, bp->m_Direction, bp->m_HeldItem);
					break;
				}
				case E_CHAT:
				{
					cPacket_Chat * ch = reinterpret_cast<cPacket_Chat *>(a_Packet);
					HandleChat(ch->m_Message);
					break;
				}
				case E_PLAYERLOOK:
				{
					cPacket_PlayerLook * pl = reinterpret_cast<cPacket_PlayerLook *>(a_Packet);
					HandlePlayerLook(pl->m_Rotation, pl->m_Pitch, pl->m_IsOnGround);
					break;
				}
				case E_PLAYERMOVELOOK:
				{
					cPacket_PlayerMoveLook * pml = reinterpret_cast<cPacket_PlayerMoveLook *>(a_Packet);
					HandlePlayerMoveLook(pml->m_PosX, pml->m_PosY, pml->m_PosZ, pml->m_Stance, pml->m_Rotation, pml->m_Pitch, pml->m_IsOnGround);
					break;
				}
				case E_ANIMATION:
				{
					cPacket_ArmAnim * aa = reinterpret_cast<cPacket_ArmAnim *>(a_Packet);
					HandleAnimation(aa->m_Animation);
					break;
				}
				case E_SLOT_SELECTED:
				{
					cPacket_ItemSwitch * isw = reinterpret_cast<cPacket_ItemSwitch *>(a_Packet);
					HandleSlotSelected(isw->m_SlotNum);
					break;
				}
				case E_WINDOW_CLOSE:
				{
					cPacket_WindowClose * wc = reinterpret_cast<cPacket_WindowClose *>(a_Packet);
					HandleWindowClose(wc->m_WindowID);
					break;
				}
				case E_WINDOW_CLICK:
				{
					cPacket_WindowClick * wc = reinterpret_cast<cPacket_WindowClick *>(a_Packet);
					HandleWindowClick(wc->m_WindowID, wc->m_SlotNum, wc->m_IsRightClick, wc->m_IsShiftPressed, wc->m_HeldItem);
					break;
				}
				case E_UPDATE_SIGN:
				{
					cPacket_UpdateSign * us = reinterpret_cast<cPacket_UpdateSign *>(a_Packet);
					HandleUpdateSign(us->m_BlockX, us->m_BlockY, us->m_BlockZ, us->m_Line1, us->m_Line2, us->m_Line3, us->m_Line4);
					break;
				}
				case E_USE_ENTITY:
				{
					cPacket_UseEntity * ue = reinterpret_cast<cPacket_UseEntity *>(a_Packet);
					HandleUseEntity(ue->m_TargetEntityID, ue->m_IsLeftClick);
					break;
				}
				case E_RESPAWN:
				{
					HandleRespawn();
					break;
				}
				case E_DISCONNECT:
				{
					cPacket_Disconnect * dc = reinterpret_cast<cPacket_Disconnect *>(a_Packet);
					HandleDisconnect(dc->m_Reason);
					break;
				}
				case E_KEEP_ALIVE:
				{
					cPacket_KeepAlive * ka = reinterpret_cast<cPacket_KeepAlive *>(a_Packet);
					HandleKeepAlive(ka->m_KeepAliveID);
					break;
				}
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
		cRoot::Get()->GetDefaultWorld()->GetDescription().c_str(), 
		cChatColor::Delimiter.c_str(), 
		cRoot::Get()->GetDefaultWorld()->GetNumPlayers(),
		cChatColor::Delimiter.c_str(), 
		cRoot::Get()->GetDefaultWorld()->GetMaxPlayers()
	);
	Kick(Reply.c_str());
}





void cClientHandle::HandleHandshake(const AString & a_Username)
{
	AStringVector UserData = StringSplit(a_Username, ";"); // "FakeTruth;localhost:25565"
	if (UserData.empty())
	{
		Kick("Did not receive username");
		return;
	}
	m_Username = UserData[0];

	LOGD("HANDSHAKE %s", m_Username.c_str());

	if (cRoot::Get()->GetDefaultWorld()->GetNumPlayers() >= cRoot::Get()->GetDefaultWorld()->GetMaxPlayers())
	{
		Kick("The server is currently full :(-- Try again later");
		return;
	}
	cPacket_Chat Connecting(m_Username + " is connecting.");
	cRoot::Get()->GetServer()->Broadcast(Connecting, this);

	SendHandshake(cRoot::Get()->GetServer()->GetServerID());
	LOG("User \"%s\" was sent a handshake", m_Username.c_str());
}





void cClientHandle::HandleLogin(int a_ProtocolVersion, const AString & a_Username)
{
	LOGD("LOGIN %s", a_Username.c_str());
	if (a_ProtocolVersion < m_ProtocolVersion)
	{
		Kick("Your client is outdated!");
		return;
	}
	else if (a_ProtocolVersion > m_ProtocolVersion)
	{
		Kick("Your client version is higher than the server!");
		return;
	}
	if (m_Username.compare(a_Username) != 0)
	{
		LOGWARNING("Login Username (\"%s\") does not match Handshake username (\"%s\") for client @ \"%s\")",
			a_Username.c_str(),
			m_Username.c_str(),
			m_Socket.GetIPString().c_str()
		);
		Kick("Hacked client");  // Don't tell them why we don't want them
		return;
	}

	if (cRoot::Get()->GetPluginManager()->CallHookLogin(this, a_ProtocolVersion, a_Username))
	{
		Destroy();
		return;
	}

	// Schedule for authentication; until then, let them wait (but do not block)
	m_State = csAuthenticating;
	cRoot::Get()->GetAuthenticator().Authenticate(GetUniqueID(), GetUsername(), cRoot::Get()->GetServer()->GetServerID());
}





void cClientHandle::HandleUnexpectedPacket(int a_PacketType)
{
	LOGWARNING(
		"Invalid packet in state %d: 0x%02x from client \"%s\", username \"%s\"", 
		m_State,
		a_PacketType,
		m_Socket.GetIPString().c_str(),
		m_Username.c_str()
	);
	Kick("Hacked client");  // Don't tell them why we don't like them
}





void cClientHandle::HandleMoveLookConfirm(double a_PosX, double a_PosY, double a_PosZ)
{
	Vector3d ReceivedPosition = Vector3d(a_PosX, a_PosY, a_PosZ);
	// LOGD("Received MoveLook confirmation: {%0.2f %0.2f %0.2f}", a_PosX, a_PosY, a_PosZ);
	
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
		SendPlayerMoveLook();
	}
}





void cClientHandle::HandleCreativeInventory(short a_SlotNum, const cItem & a_HeldItem)
{
	// This is for creative Inventory changes
	if (m_Player->GetGameMode() == 1)
	{
		m_Player->GetInventory().Clicked(a_SlotNum, false, false, a_HeldItem);
	}
	else
	{
		LOGWARNING("Got a CreativeInventoryAction packet from user \"%s\" while not in creative mode. Ignoring.", m_Username.c_str());
	}
}





void cClientHandle::HandlePlayerPos(double a_PosX, double a_PosY, double a_PosZ, double a_Stance, bool a_IsOnGround)
{
	/*
		// TODO: Invalid stance check
	if ((a_PosY >= a_Stance) || (a_Stance > a_PosY + 1.65))
	{
		LOGD("Invalid stance");
		SendPlayerMoveLook();
		return;
	}
	*/
	
	// LOGD("recv player pos: {%0.2f %0.2f %0.2f}, ground: %d", a_PosX, a_PosY, a_PosZ, a_IsOnGround ? 1 : 0);
	Vector3d Pos(a_PosX, a_PosY, a_PosZ);
	if ((m_Player->GetPosition() - Pos).SqrLength() > 100 * 100)
	{
		LOGD("Too far away (%0.2f), \"repairing\" the client", (m_Player->GetPosition() - Pos).Length());
		SendPlayerMoveLook();
		return;
	}
	m_Player->MoveTo(Pos);
	m_Player->SetStance(a_Stance);
	m_Player->SetTouchGround(a_IsOnGround);
}





void cClientHandle::HandleBlockDig(int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status)
{
	if (!CheckBlockInteractionsRate())
	{
		return;
	}

	LOGD("OnBlockDig: {%i, %i, %i}; Face: %i; Stat: %i",
		a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_Status
	);

	// Do we want plugins to disable tossing items? Probably no, so toss item before asking plugins for permission
	if (a_Status == DIG_STATUS_DROP_HELD)  // Drop held item
	{
		m_Player->TossItem(false);
		return;
	}

	if (a_Status == DIG_STATUS_SHOOT_EAT)
	{
		LOGINFO("BlockDig: Status SHOOT/EAT not implemented");
		return;
	}
	
	cWorld * World = m_Player->GetWorld();
	BLOCKTYPE  OldBlock;
	NIBBLETYPE OldMeta;
	World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, OldBlock, OldMeta);

	if (cRoot::Get()->GetPluginManager()->CallHookBlockDig(m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_Status, OldBlock, OldMeta))
	{
		// The plugin doesn't agree with the digging, replace the block on the client and quit:
		World->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
		return;
	}
	
	bool bBroken = (
		(a_Status == DIG_STATUS_FINISHED) || 
		(g_BlockOneHitDig[(int)OldBlock]) || 
		((a_Status == DIG_STATUS_STARTED) && (m_Player->GetGameMode() == 1))
	);

	cItem & Equipped = m_Player->GetInventory().GetEquippedItem();
	cItemHandler * ItemHandler = cItemHandler::GetItemHandler(Equipped.m_ItemID);

	if (bBroken)
	{
		ItemHandler->OnBlockDestroyed(World, m_Player, &Equipped, a_BlockX, a_BlockY, a_BlockZ);
		
		BlockHandler(OldBlock)->OnDestroyedByPlayer(World, m_Player, a_BlockX, a_BlockY, a_BlockZ);
		World->DigBlock(a_BlockX, a_BlockY, a_BlockZ);
	}
	else
	{
		cBlockHandler * Handler = cBlockHandler::GetBlockHandler(OldBlock);
		Handler->OnDigging(World, m_Player, a_BlockX, a_BlockY, a_BlockZ);

		ItemHandler->OnDiggingBlock(World, m_Player, &Equipped, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
		

		// Check for clickthrough-blocks:
		int pX = a_BlockX;
		unsigned char pY = a_BlockY;
		int pZ = a_BlockZ;
		AddDirection(pX, pY, pZ, a_BlockFace);

		Handler = cBlockHandler::GetBlockHandler(World->GetBlock(pX, pY, pZ));
		if (Handler->IsClickedThrough())
		{
			Handler->OnDigging(World, m_Player, pX, pY, pZ);
		}
	}
}





void cClientHandle::HandleBlockPlace(int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, const cItem & a_HeldItem)
{
	if (!CheckBlockInteractionsRate())
	{
		return;
	}
	
	cItem & Equipped = m_Player->GetInventory().GetEquippedItem();

	if ((Equipped.m_ItemType != a_HeldItem.m_ItemType))	// Not valid
	{
		LOGWARN("Player %s tried to place a block that was not equipped (exp %d, got %d)",
			m_Username.c_str(), Equipped.m_ItemType, a_HeldItem.m_ItemType
		);
		
		// Let's send the current world block to the client, so that it can immediately "let the user know" that they haven't placed the block
		if (a_BlockFace > -1)
		{
			AddDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
			m_Player->GetWorld()->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
		}
		return;
	}

	if (cRoot::Get()->GetPluginManager()->CallHookBlockPlace(m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_HeldItem))
	{
		if (a_BlockFace > -1)
		{
			AddDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
			m_Player->GetWorld()->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
		}
		return;
	}
	
	cWorld * World = m_Player->GetWorld();

	cBlockHandler *Handler = cBlockHandler::GetBlockHandler(World->GetBlock(a_BlockX, a_BlockY, a_BlockZ));
	if (Handler->IsUseable())
	{
		Handler->OnUse(World, m_Player, a_BlockX, a_BlockY, a_BlockZ);
	}
	else
	{
		cItemHandler * ItemHandler = cItemHandler::GetItemHandler(Equipped.m_ItemID);
		
		if (ItemHandler->OnItemUse(World, m_Player, &Equipped, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace))
		{
			// Nothing here :P
		}
		else if (ItemHandler->IsPlaceable())
		{
			if (a_BlockFace < 0)
			{
				// clicked in air
				return;
			}

			BLOCKTYPE ClickedBlock = World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
			cBlockHandler *Handler = cBlockHandler::GetBlockHandler(ClickedBlock);

			if(Handler->IgnoreBuildCollision())
			{
				Handler->OnDestroyedByPlayer(World, m_Player, a_BlockX, a_BlockY, a_BlockZ);
				World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
			}
			else
			{
				AddDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
				// Check for Blocks not allowing placement on top
				if ((a_BlockFace == BLOCK_FACE_TOP) && !Handler->AllowBlockOnTop())
				{
					// Resend the old block
					// Some times the client still places the block O.o

					World->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
					return;
				}


				int PlaceBlock = m_Player->GetWorld()->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
				if (!BlockHandler(PlaceBlock)->IgnoreBuildCollision())
				{
					// Tried to place a block *into* another?
					return;  // Happens when you place a block aiming at side of block like torch or stem
				}
			}
			
			cBlockHandler * NewBlock = BlockHandler(ItemHandler->GetBlockType());

			// Cannot be placed on the side of an other block
			if ((a_BlockFace != BLOCK_FACE_TOP) && !NewBlock->CanBePlacedOnSide())
			{
				return;
			}

			if (NewBlock->CanBePlacedAt(World, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace))
			{
				ItemHandler->PlaceBlock(World, m_Player, &m_Player->GetInventory().GetEquippedItem(), a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
			}
			else
			{
				World->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);  // Send the old block back to the player
				return;
			}
			
		}
		else if (ItemHandler->IsFood())
		{
			cItem Item;
			Item.m_ItemID = Equipped.m_ItemID;
			Item.m_ItemCount = 1;
			if (ItemHandler->EatItem(m_Player, &Item))
			{
				ItemHandler->OnFoodEaten(World, m_Player, &Item);
				m_Player->GetInventory().RemoveItem(Item);
				return;
			}
		}
	}
}





void cClientHandle::HandleChat(const AString & a_Message)
{
	if (!cRoot::Get()->GetServer()->Command(*this, a_Message))
	{
		AString Msg;
		Printf(Msg, "<%s%s%s> %s",
			m_Player->GetColor().c_str(),
			m_Player->GetName().c_str(),
			cChatColor::White.c_str(),
			a_Message.c_str()
		);
		m_Player->GetWorld()->BroadcastChat(Msg);
	}
}





void cClientHandle::HandlePlayerLook(float a_Rotation, float a_Pitch, bool a_IsOnGround)
{
	m_Player->SetRotation   (a_Rotation);
	m_Player->SetPitch      (a_Pitch);
	m_Player->SetTouchGround(a_IsOnGround);
	m_Player->WrapRotation();
}





void cClientHandle::HandlePlayerMoveLook(double a_PosX, double a_PosY, double a_PosZ, double a_Stance, float a_Rotation, float a_Pitch, bool a_IsOnGround)
{
	/*
	// TODO: Invalid stance check
	if ((a_PosY >= a_Stance) || (a_Stance > a_PosY + 1.65))
	{
		LOGD("Invalid stance");
		SendPlayerMoveLook();
		return;
	}
	*/
	
	m_Player->MoveTo(Vector3d(a_PosX, a_PosY, a_PosZ));
	m_Player->SetStance     (a_Stance);
	m_Player->SetTouchGround(a_IsOnGround);
	m_Player->SetRotation   (a_Rotation);
	m_Player->SetPitch      (a_Pitch);
	m_Player->WrapRotation();
}





void cClientHandle::HandleAnimation(char a_Animation)
{
	m_Player->GetWorld()->BroadcastPlayerAnimation(*m_Player, a_Animation, this);
}





void cClientHandle::HandleSlotSelected(short a_SlotNum)
{
	m_Player->GetInventory().SetEquippedSlot(a_SlotNum);
	m_Player->GetWorld()->BroadcastEntityEquipment(*m_Player, 0, m_Player->GetInventory().GetEquippedItem(), this);
}





void cClientHandle::HandleWindowClose(char a_WindowID)
{
	m_Player->CloseWindow(a_WindowID);
}





void cClientHandle::HandleWindowClick(char a_WindowID, short a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, const cItem & a_HeldItem)
{
	if (a_WindowID == 0)
	{
		m_Player->GetInventory().Clicked(a_SlotNum, a_IsRightClick, a_IsShiftPressed, a_HeldItem);
		return;
	}
	
	cWindow * Window = m_Player->GetWindow();
	if (Window == NULL)
	{
		LOGWARNING("Player \"%s\" clicked in a non-existent window. Ignoring", m_Username.c_str());
		return;
	}
	
	Window->Clicked(*m_Player, a_WindowID, a_SlotNum, a_IsRightClick, a_IsShiftPressed, a_HeldItem);
}





void cClientHandle::HandleUpdateSign(
	int a_BlockX, int a_BlockY, int a_BlockZ, 
	const AString & a_Line1, const AString & a_Line2, 
	const AString & a_Line3, const AString & a_Line4
)
{
	cWorld * World = m_Player->GetWorld();
	World->UpdateSign(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4);
}





void cClientHandle::HandleUseEntity(int a_TargetEntityID, bool a_IsLeftClick)
{
	if (!a_IsLeftClick)
	{
		// TODO: we don't handle right-clicking yet
		return;
	}

	class cDamageEntity : public cEntityCallback
	{
		virtual bool Item(cEntity * a_Entity) override
		{
			if (a_Entity->IsA("cPawn"))
			{
				reinterpret_cast<cPawn *>(a_Entity)->TakeDamage(Damage, Instigator);
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
	World->DoWithEntityByID(a_TargetEntityID, Callback);
}





void cClientHandle::HandleRespawn(void)
{
	m_Player->Respawn();
}





void cClientHandle::HandleDisconnect(const AString & a_Reason)
{
	LOGD("Received d/c packet from \"%s\" with reason \"%s\"", m_Username.c_str(), a_Reason.c_str());
	if (!cRoot::Get()->GetPluginManager()->CallHookDisconnect(m_Player, a_Reason))
	{
		AString DisconnectMessage;
		Printf(DisconnectMessage, "%s disconnected: %s", m_Username.c_str(), a_Reason.c_str());
		m_Player->GetWorld()->BroadcastChat(DisconnectMessage, this);
	}
	Destroy();
}





void cClientHandle::HandleKeepAlive(int a_KeepAliveID)
{
	if (a_KeepAliveID == m_PingID)
	{
		cTimer t1;
		m_Ping = (short)((t1.GetNowTime() - m_PingStartTime) / 2);
	}
}





bool cClientHandle::CheckBlockInteractionsRate(void)
{
	ASSERT(m_Player != NULL);
	ASSERT(m_Player->GetWorld() != NULL);
	int LastActionCnt = m_Player->GetLastBlockActionCnt();
	if ((m_Player->GetWorld()->GetTime() - m_Player->GetLastBlockActionTime()) < 0.1)
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
}





void cClientHandle::Send(const cPacket & a_Packet, ENUM_PRIORITY a_Priority /* = E_PRIORITY_NORMAL */)
{
	if (m_bKicking) return; // Don't add more packets if player is getting kicked anyway

	// If it is the packet spawning myself for myself, drop it silently:
	if (a_Packet.m_PacketID == E_NAMED_ENTITY_SPAWN)
	{
		if (((cPacket_NamedEntitySpawn &)a_Packet).m_UniqueID == m_Player->GetUniqueID())
		{
			return;
		}
	}
	
	// Filter out packets that don't belong to a csDownloadingWorld state:
	if (m_State == csDownloadingWorld)
	{
		switch (a_Packet.m_PacketID)
		{
			case E_PLAYERMOVELOOK:
			case E_KEEP_ALIVE:
			case E_PRE_CHUNK:
			case E_MAP_CHUNK:
			{
				// Allow
				break;
			}
			default: return;
		}
	}
	
	// Filter out map chunks that the client doesn't want anymore:
	if (a_Packet.m_PacketID == E_MAP_CHUNK)
	{
		// Check chunks being sent, erase them from m_ChunksToSend:
		int ChunkX = ((cPacket_MapChunk &)a_Packet).m_PosX;
		int ChunkZ = ((cPacket_MapChunk &)a_Packet).m_PosZ;
		bool Found = false;
		cCSLock Lock(m_CSChunkLists);
		for (cChunkCoordsList::iterator itr = m_ChunksToSend.begin(); itr != m_ChunksToSend.end(); ++itr)
		{
			if ((itr->m_ChunkX == ChunkX) && (itr->m_ChunkZ == ChunkZ))
			{
				m_ChunksToSend.erase(itr);
				
				// TODO: _X: Decouple this from packet sending, it creates a deadlock possibility
				//  -- postpone till Tick() instead, using a bool flag
				CheckIfWorldDownloaded();
				
				Found = true;
				break;
			}
		}  // for itr - m_ChunksToSend[]
		if (!Found)
		{
			// This just sometimes happens. If you have a reliably replicatable situation for this, go ahead and fix it
			// It's not a big issue anyway, just means that some chunks may be compressed several times
			// LOGD("Refusing to send    chunk [%d, %d] to client \"%s\" at [%d, %d].", ChunkX, ChunkZ, m_Username.c_str(), m_Player->GetChunkX(), m_Player->GetChunkZ());
			return;
		}
	}
	
	// Filter out pre chunks that the client doesn't want anymore:
	if ((a_Packet.m_PacketID == E_PRE_CHUNK) && ((cPacket_PreChunk &)a_Packet).m_bLoad)
	{
		int ChunkX = ((cPacket_PreChunk &)a_Packet).m_PosX;
		int ChunkZ = ((cPacket_PreChunk &)a_Packet).m_PosZ;
		bool Found = false;
		cCSLock Lock(m_CSChunkLists);
		for (cChunkCoordsList::iterator itr = m_ChunksToSend.begin(); itr != m_ChunksToSend.end(); ++itr)
		{
			if ((itr->m_ChunkX == ChunkX) && (itr->m_ChunkZ == ChunkZ))
			{
				Found = true;
				break;
			}
		}  // for itr - m_ChunksToSend[]
		if (!Found)
		{
			// This just sometimes happens. If you have a reliably replicatable situation for this, go ahead and fix it
			// It's not a big issue anyway, just means that some chunks may be compressed several times
			// LOGD("Refusing to send PREchunk [%d, %d] to client \"%s\" at [%d, %d].", ChunkX, ChunkZ, m_Username.c_str(), m_Player->GetChunkX(), m_Player->GetChunkZ());
			return;
		}
	}
	
	// Optimize away multiple queued RelativeEntityMoveLook packets:
	static int NumRelEntMoveLookTotal   = 0;
	static int NumRelEntMoveLookRemoved = 0;
	cCSLock Lock(m_CSPackets);
	if (a_Priority == E_PRIORITY_NORMAL)
	{
		if (a_Packet.m_PacketID == E_REL_ENT_MOVE_LOOK)
		{
			NumRelEntMoveLookTotal++;
			PacketList & Packets = m_PendingNrmSendPackets;
			const cPacket_RelativeEntityMoveLook & ThisPacketData = reinterpret_cast< const cPacket_RelativeEntityMoveLook &>(a_Packet);
			for (PacketList::iterator itr = Packets.begin(); itr != Packets.end(); ++itr)
			{
				bool bBreak = false;
				switch ((*itr)->m_PacketID)
				{
					case E_REL_ENT_MOVE_LOOK:
					{
						cPacket_RelativeEntityMoveLook * PacketData = reinterpret_cast< cPacket_RelativeEntityMoveLook *>(*itr);
						if (ThisPacketData.m_UniqueID == PacketData->m_UniqueID)
						{
							Packets.erase(itr);
							bBreak = true;
							delete PacketData;
							NumRelEntMoveLookRemoved++;
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
			if ((NumRelEntMoveLookTotal % 1000) == 10)  // print out a debug statistics every 1000 packets sent
			{
				LOGD("RelEntMoveLook optimization: removed %d out of %d packets, saved %d bytes (%d KiB) of bandwidth",
					NumRelEntMoveLookRemoved, NumRelEntMoveLookTotal,
					NumRelEntMoveLookRemoved * sizeof(cPacket_RelativeEntityMoveLook),
					NumRelEntMoveLookRemoved * sizeof(cPacket_RelativeEntityMoveLook) / 1024
				);
			}
		}  // if (E_REL_ENT_MOVE_LOOK
		m_PendingNrmSendPackets.push_back(a_Packet.Clone());
	}
	else if (a_Priority == E_PRIORITY_LOW)
	{
		m_PendingLowSendPackets.push_back(a_Packet.Clone());
	}
	Lock.Unlock();
	
	// Notify SocketThreads that we have something to write:
	cRoot::Get()->GetServer()->NotifyClientWrite(this);
}





void cClientHandle::SendDisconnect(const AString & a_Reason)
{
	cPacket_Disconnect DC(a_Reason);
	m_Socket.Send(&DC);  // Send it immediately to the socket, bypassing any packet buffers
}





void cClientHandle::SendHandshake(const AString & a_ServerName)
{
	cPacket_Handshake Handshake;
	Handshake.m_Username = a_ServerName;
	Send(Handshake);
}





void cClientHandle::SendInventorySlot(int a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	cPacket_InventorySlot Packet;
	Packet.m_WindowID  = (char)a_WindowID;
	Packet.m_SlotNum   = a_SlotNum;
	Packet.m_ItemID    = (short)(a_Item.m_ItemID);
	Packet.m_ItemCount = a_Item.m_ItemCount;
	Packet.m_ItemUses  = a_Item.m_ItemHealth;
	Send(Packet);
}





void cClientHandle::SendChat(const AString & a_Message)
{
	cPacket_Chat Chat(a_Message);
	Send(Chat);
}





void cClientHandle::SendPlayerAnimation(const cPlayer & a_Player, char a_Animation)
{
	cPacket_ArmAnim Anim;
	Anim.m_EntityID  = a_Player.GetUniqueID();
	Anim.m_Animation = a_Animation;
	Send(Anim);
}





void cClientHandle::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	cPacket_EntityEquipment ee;
	ee.m_UniqueID   = a_Entity.GetUniqueID();
	ee.m_SlotNum    = a_SlotNum;
	ee.m_ItemType   = a_Item.m_ItemType;
	ee.m_ItemDamage = a_Item.m_ItemDamage;
	Send(ee);
}





void cClientHandle::SendWindowOpen(char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots)
{
	cPacket_WindowOpen WindowOpen;
	WindowOpen.m_WindowID      = a_WindowID;
	WindowOpen.m_InventoryType = a_WindowType;
	WindowOpen.m_WindowTitle   = a_WindowTitle;
	WindowOpen.m_NumSlots      = a_NumSlots;
	Send(WindowOpen);
}





void cClientHandle::SendWindowClose(char a_WindowID)
{
	cPacket_WindowClose wc;
	wc.m_WindowID = a_WindowID;
	Send(wc);
}





void cClientHandle::SendWholeInventory(const cInventory & a_Inventory)
{
	cPacket_WholeInventory wi(a_Inventory);
	Send(wi);
}





void cClientHandle::SendWholeInventory(const cWindow & a_Window)
{
	cPacket_WholeInventory wi(a_Window);
	Send(wi);
}





void cClientHandle::SendTeleportEntity(const cEntity & a_Entity)
{
	cPacket_TeleportEntity te(a_Entity);
	Send(te);
}





void cClientHandle::SendPlayerListItem(const cPlayer & a_Player)
{
	cPacket_PlayerListItem pli(a_Player.GetColor() + a_Player.GetName(), true, a_Player.GetClientHandle()->GetPing());
	Send(pli);
}





void cClientHandle::SendPlayerPosition(void)
{
	cPacket_PlayerPosition pp(m_Player);
	Send(pp);
}





void cClientHandle::SendRelEntMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(a_Entity.GetUniqueID() != m_Player->GetUniqueID());  // Must not send for self
	
	cPacket_RelativeEntityMoveLook reml;
	reml.m_UniqueID = a_Entity.GetUniqueID();
	reml.m_MoveX    = a_RelX;
	reml.m_MoveY    = a_RelY;
	reml.m_MoveZ    = a_RelZ;
	reml.m_Yaw      = (char)((a_Entity.GetRotation() / 360.f) * 256);
	reml.m_Pitch    = (char)((a_Entity.GetPitch()    / 360.f) * 256);
	Send(reml);
}





void cClientHandle::SendRelEntMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(a_Entity.GetUniqueID() != m_Player->GetUniqueID());  // Must not send for self
	
	cPacket_RelativeEntityMove rem;
	rem.m_UniqueID = a_Entity.GetUniqueID();
	rem.m_MoveX    = a_RelX;
	rem.m_MoveY    = a_RelY;
	rem.m_MoveZ    = a_RelZ;
	Send(rem);
}





void cClientHandle::SendEntLook(const cEntity & a_Entity)
{
	ASSERT(a_Entity.GetUniqueID() != m_Player->GetUniqueID());  // Must not send for self
	
	cPacket_EntityLook el;
	el.m_UniqueID = a_Entity.GetUniqueID();
	el.m_Rotation = (char)((a_Entity.GetRotation() / 360.f) * 256);
	el.m_Pitch    = (char)((a_Entity.GetPitch()    / 360.f) * 256);
	Send(el);
}





void cClientHandle::SendEntHeadLook(const cEntity & a_Entity)
{
	ASSERT(a_Entity.GetUniqueID() != m_Player->GetUniqueID());  // Must not send for self
	
	cPacket_EntityHeadLook ehl(a_Entity);
	Send(ehl);
}





void cClientHandle::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2)
{
	cPacket_BlockAction ba;
	ba.m_BlockX = a_BlockX;
	ba.m_BlockY = (short)a_BlockY;
	ba.m_BlockZ = a_BlockZ;
	ba.m_Byte1 = a_Byte1;
	ba.m_Byte2 = a_Byte2;
	Send(ba);
}





void cClientHandle::SendHealth(void)
{
	cPacket_UpdateHealth Health;
	Health.m_Health     = m_Player->GetHealth();
	Health.m_Food       = m_Player->GetFoodLevel();
	Health.m_Saturation = m_Player->GetFoodSaturationLevel();
	Send(Health);
}





void cClientHandle::SendRespawn(void)
{
	cPacket_Respawn Packet;
	Packet.m_CreativeMode = (char)m_Player->GetGameMode();  // Set GameMode packet based on Player's GameMode;
	Send(Packet);
}





void cClientHandle::SendGameMode(char a_GameMode)
{
	cPacket_NewInvalidState nis;
	nis.m_Reason = 3;
	nis.m_GameMode = a_GameMode;
	Send(nis);
}





void cClientHandle::SendDestroyEntity(const cEntity & a_Entity)
{
	cPacket_DestroyEntity de;
	de.m_UniqueID = a_Entity.GetUniqueID();
	Send(de);
}





void cClientHandle::SendPlayerMoveLook(void)
{
	cPacket_PlayerMoveLook pml(*m_Player);
	/*
	LOGD("Sending PlayerMoveLook: {%0.2f, %0.2f, %0.2f}, stance %0.2f, OnGround: %d",
		m_Player->GetPosX(), m_Player->GetPosY(), m_Player->GetPosZ(), m_Player->GetStance(), m_Player->IsOnGround() ? 1 : 0
	);
	*/
	Send(pml);
}





void cClientHandle::SendEntityStatus(const cEntity & a_Entity, char a_Status)
{
	cPacket_EntityStatus es;
	es.m_Status   = a_Status;
	es.m_UniqueID = a_Entity.GetUniqueID();
	Send(es);
}





void cClientHandle::SendMetadata(const cPawn & a_Pawn)
{
	cPacket_Metadata md(a_Pawn.GetMetaData(), a_Pawn.GetUniqueID());
	Send(md);
}





void cClientHandle::SendInventoryProgress(char a_WindowID, short a_ProgressBar, short a_Value)
{
	cPacket_InventoryProgressBar Progress;
	Progress.m_WindowID    = a_WindowID;
	Progress.m_ProgressBar = a_ProgressBar;
	Progress.m_Value       = a_Value;
	Progress.m_WindowID    = a_WindowID;
	Send(Progress);
}





void cClientHandle::SendPlayerSpawn(const cPlayer & a_Player)
{
	cPacket_NamedEntitySpawn SpawnPacket;
	SpawnPacket.m_UniqueID    = a_Player.GetUniqueID();
	SpawnPacket.m_PlayerName  = a_Player.GetName();
	SpawnPacket.m_PosX        = (int)(a_Player.GetPosX() * 32);
	SpawnPacket.m_PosY        = (int)(a_Player.GetPosY() * 32);
	SpawnPacket.m_PosZ        = (int)(a_Player.GetPosZ() * 32);
	SpawnPacket.m_Rotation    = (char)((a_Player.GetRot().x / 360.f) * 256);
	SpawnPacket.m_Pitch       = (char)((a_Player.GetRot().y / 360.f) * 256);
	const cItem & HeldItem    = a_Player.GetEquippedItem();
	SpawnPacket.m_CurrentItem = HeldItem.IsEmpty() ? 0 : HeldItem.m_ItemType;  // Unlike -1 in inventory, the named entity packet uses 0 for "empty"
	Send(SpawnPacket);
}





void cClientHandle::SendPickupSpawn(const cPickup & a_Pickup)
{
	cPacket_PickupSpawn PickupSpawn;
	PickupSpawn.m_UniqueID   = a_Pickup.GetUniqueID();
	PickupSpawn.m_ItemType   = a_Pickup.GetItem()->m_ItemType;
	PickupSpawn.m_ItemCount  = a_Pickup.GetItem()->m_ItemCount;
	PickupSpawn.m_ItemDamage = a_Pickup.GetItem()->m_ItemHealth;
	PickupSpawn.m_PosX       = (int) (a_Pickup.GetPosX() * 32);
	PickupSpawn.m_PosY       = (int) (a_Pickup.GetPosY() * 32);
	PickupSpawn.m_PosZ       = (int) (a_Pickup.GetPosZ() * 32);
	PickupSpawn.m_Rotation   = (char)(a_Pickup.GetSpeed().x * 8);
	PickupSpawn.m_Pitch      = (char)(a_Pickup.GetSpeed().y * 8);
	PickupSpawn.m_Roll       = (char)(a_Pickup.GetSpeed().z * 8);
	Send(PickupSpawn);
}





void cClientHandle::SendSpawnMob(const cMonster & a_Mob)
{
	cPacket_SpawnMob Spawn;
	Spawn.m_UniqueID = a_Mob.GetUniqueID();
	Spawn.m_Type     = a_Mob.GetMobType();
	Spawn.m_Pos      = ((Vector3i)(a_Mob.GetPosition())) * 32;
	Spawn.m_Yaw      = 0;
	Spawn.m_Pitch    = 0;
	Spawn.m_MetaDataSize = 1;
	Spawn.m_MetaData     = new char[Spawn.m_MetaDataSize];
	Spawn.m_MetaData[0]  = 0x7f;  // not on fire/crouching/riding
	Send(Spawn);
}





void cClientHandle::SendUpdateSign(
	int a_BlockX, int a_BlockY, int a_BlockZ,
	const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4
)
{
	cPacket_UpdateSign us;
	us.m_BlockX = a_BlockX;
	us.m_BlockY = a_BlockY;
	us.m_BlockZ = a_BlockZ;
	us.m_Line1  = a_Line1;
	us.m_Line2  = a_Line2;
	us.m_Line3  = a_Line3;
	us.m_Line4  = a_Line4;
	Send(us);
}





void cClientHandle::SendCollectPickup(const cPickup & a_Pickup, const cPlayer & a_Player)
{
	cPacket_CollectItem ci;
	ci.m_CollectedID = a_Pickup.GetUniqueID();
	ci.m_CollectorID = a_Player.GetUniqueID();
	Send(ci);
}





void cClientHandle::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cPacket_BlockChange BlockChange;
	BlockChange.m_PosX = a_BlockX;
	BlockChange.m_PosY = (unsigned char)a_BlockY;
	BlockChange.m_PosZ = a_BlockZ;
	BlockChange.m_BlockType = a_BlockType;
	BlockChange.m_BlockMeta = a_BlockMeta;
	Send(BlockChange);
}





void cClientHandle::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	if (a_Changes.size() == 1)
	{
		// Special packet for single-block changes
		const sSetBlock & blk = a_Changes.front();
		SendBlockChange(a_ChunkX * cChunkDef::Width + blk.x, blk.y, a_ChunkZ * cChunkDef::Height + blk.z, blk.BlockType, blk.BlockMeta);
		return;
	}
	
	cPacket_MultiBlock MultiBlock;
	MultiBlock.m_ChunkX = a_ChunkX;
	MultiBlock.m_ChunkZ = a_ChunkZ;
	MultiBlock.m_NumBlocks = (short)a_Changes.size();
	MultiBlock.m_Data = new cPacket_MultiBlock::sBlockChange[a_Changes.size()];
	int i = 0;
	for (sSetBlockVector::const_iterator itr = a_Changes.begin(), end = a_Changes.end(); itr != end; ++itr, i++)
	{
		unsigned int Coords = itr->y | (itr->z << 8) | (itr->x << 12);
		unsigned int Blocks = itr->BlockMeta | (itr->BlockType << 4);
		MultiBlock.m_Data[i].Data = Coords << 16 | Blocks;
	}
	Send(MultiBlock);
}





void cClientHandle::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	cPacket_PreChunk UnloadPacket;
	UnloadPacket.m_PosX = a_ChunkX;
	UnloadPacket.m_PosZ = a_ChunkZ;
	UnloadPacket.m_bLoad = false;  // Unload
	Send(UnloadPacket);
}





void cClientHandle::CheckIfWorldDownloaded(void)
{
	if (m_State != csDownloadingWorld)
	{
		return;
	}
	
	bool ShouldSendConfirm = false;
	{
		cCSLock Lock(m_CSChunkLists);
		ShouldSendConfirm = m_ChunksToSend.empty();
	}
	
	if (ShouldSendConfirm)
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

	if (!cRoot::Get()->GetPluginManager()->CallHook(cPluginManager::HOOK_PLAYER_JOIN, 1, m_Player))
	{
		// Broadcast that this player has joined the game! Yay~
		cPacket_Chat Joined(m_Username + " joined the game!");
		cRoot::Get()->GetServer()->Broadcast(Joined, this);
	}

	m_ConfirmPosition = m_Player->GetPosition();
	SendPlayerMoveLook();
}





const AString & cClientHandle::GetUsername(void) const
{
	return m_Username;
}





void cClientHandle::SetViewDistance(int a_ViewDistance)
{
	if (a_ViewDistance < MIN_VIEW_DISTANCE)
	{
		a_ViewDistance = MIN_VIEW_DISTANCE;
	}
	if (a_ViewDistance > MAX_VIEW_DISTANCE)
	{
		a_ViewDistance = MAX_VIEW_DISTANCE;
	}
	m_ViewDistance = a_ViewDistance;
	
	// Need to re-stream chunks for the change to become apparent:
	StreamChunks();
}





bool cClientHandle::WantsSendChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunkLists);
	return (std::find(m_ChunksToSend.begin(), m_ChunksToSend.end(), cChunkCoords(a_ChunkX, a_ChunkY, a_ChunkZ)) != m_ChunksToSend.end());
}





void cClientHandle::AddWantedChunk(int a_ChunkX, int a_ChunkZ)
{
	LOGD("Adding chunk [%d, %d] to wanted chunks for client %p", a_ChunkX, a_ChunkZ, this);
	cCSLock Lock(m_CSChunkLists);
	if (std::find(m_ChunksToSend.begin(), m_ChunksToSend.end(), cChunkCoords(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ)) == m_ChunksToSend.end())
	{
		m_ChunksToSend.push_back(cChunkCoords(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ));
	}
}





void cClientHandle::DataReceived(const char * a_Data, int a_Size)
{
	// Data is received from the client
	
	if (!m_ReceivedData.Write(a_Data, a_Size))
	{
		// Too much data in the incoming queue, the server is probably too busy, kick the client:
		LOGERROR("Too much data in queue for client \"%s\" @ %s, kicking them.", m_Username.c_str(), m_Socket.GetIPString().c_str());
		SendDisconnect("Server busy");
		// TODO: QueueDestroy();
		cSleep::MilliSleep(1000); // Give packet some time to be received
		Destroy();
		return;
	}

	// Parse and handle all complete packets in m_ReceivedData:
	while (m_ReceivedData.CanReadBytes(1))
	{
		unsigned char PacketType;
		m_ReceivedData.ReadByte(PacketType);
		cPacket* pPacket = m_PacketMap[PacketType];
		if (pPacket == NULL)
		{
			LOGERROR("Unknown packet type 0x%02x from client \"%s\" @ %s", PacketType, m_Username.c_str(), m_Socket.GetIPString().c_str());

			AString Reason;
			Printf(Reason, "[C->S] Unknown PacketID: 0x%02x", PacketType);
			SendDisconnect(Reason);
			// TODO: QueueDestroy();
			cSleep::MilliSleep(1000); // Give packet some time to be received
			Destroy();
			return;
		}
		
		int NumBytes = pPacket->Parse(m_ReceivedData);
		if (NumBytes == PACKET_ERROR)
		{
			LOGERROR("Protocol error while parsing packet type 0x%02x; disconnecting client \"%s\"", PacketType, m_Username.c_str());
			SendDisconnect("Protocol error");
			// TODO: QueueDestroy();
			cSleep::MilliSleep(1000); // Give packet some time to be received
			Destroy();
			return;
		}
		else if (NumBytes == PACKET_INCOMPLETE)
		{
			// Not a complete packet
			m_ReceivedData.ResetRead();
			break;
		}
		else
		{
			// Packet parsed successfully, add it to internal queue:
			HandlePacket(pPacket);
			// Erase the packet from the buffer:
			m_ReceivedData.CommitRead();
		}
	}  // while (!Received.CanReadBytes(1))
}





void cClientHandle::GetOutgoingData(AString & a_Data)
{
	// Data can be sent to client
	
	cCSLock Lock(m_CSPackets);
	if (m_PendingNrmSendPackets.size() + m_PendingLowSendPackets.size() > MAX_OUTGOING_PACKETS)
	{
		LOGERROR("ERROR: Too many packets in queue for player %s !!", m_Username.c_str());
		cPacket_Disconnect DC("Too many packets in queue.");
		m_Socket.Send(DC);
		
		// DEBUG: Dump all outstanding packets' types to the log:
		int Idx = 0;
		int ChunkX = m_Player->GetChunkX();
		int ChunkZ = m_Player->GetChunkZ();
		for (PacketList::const_iterator itr = m_PendingNrmSendPackets.begin(); itr != m_PendingNrmSendPackets.end(); ++itr)
		{
			switch ((*itr)->m_PacketID)
			{
				case E_MAP_CHUNK:
				{
					int x = ((cPacket_MapChunk *)(*itr))->m_PosX;
					int z = ((cPacket_MapChunk *)(*itr))->m_PosZ;
					bool IsWanted = (abs(x - ChunkX) <= m_ViewDistance) && (abs(z - ChunkZ) <= m_ViewDistance);
					LOG("Packet %4d: type %2x (MapChunk: %d, %d, %s)", 
						Idx++, (*itr)->m_PacketID,
						x, z,
						IsWanted ? "wanted" : "unwanted"
					);
					break;
				}
				
				case E_PRE_CHUNK:
				{
					int x = ((cPacket_PreChunk *)(*itr))->m_PosX;
					int z = ((cPacket_PreChunk *)(*itr))->m_PosZ;
					bool IsWanted = (abs(x - ChunkX) <= m_ViewDistance) && (abs(z - ChunkZ) <= m_ViewDistance);
					bool Loading = ((cPacket_PreChunk *)(*itr))->m_bLoad;
					LOG("Packet %4d: type %2x (PreChunk: %d, %d, %s, %s)", 
						Idx++, (*itr)->m_PacketID,
						x, z,
						Loading ? "loading" : "unloading",
						IsWanted ? "wanted" : "unwanted"
					);
					break;
				}
				
				case E_BLOCK_CHANGE:
				{
					int x = ((cPacket_BlockChange *)(*itr))->m_PosX;
					int z = ((cPacket_BlockChange *)(*itr))->m_PosZ;
					char ToBlock = ((cPacket_BlockChange *)(*itr))->m_BlockType;
					int y, cx, cz;
					cChunkDef::AbsoluteToRelative(x, y, z, cx, cz);
					bool IsWanted = (abs(cx - ChunkX) <= m_ViewDistance) && (abs(cz - ChunkZ) <= m_ViewDistance);
					LOG("Packet %4d: type %2x (BlockChange: [%d, %d], %s chunk; to block %d)", 
						Idx++, (*itr)->m_PacketID,
						cx, cz,
						IsWanted ? "wanted" : "unwanted",
						ToBlock
					);
					break;
				}
				
				case E_MULTI_BLOCK:
				{
					int cx = ((cPacket_MultiBlock *)(*itr))->m_ChunkX;
					int cz = ((cPacket_MultiBlock *)(*itr))->m_ChunkZ;
					int NumBlocks = ((cPacket_MultiBlock *)(*itr))->m_NumBlocks;
					bool IsWanted = (abs(cx - ChunkX) <= m_ViewDistance) && (abs(cz - ChunkZ) <= m_ViewDistance);
					LOG("Packet %4d: type %2x (MultiBlock: [%d, %d], %s chunk, %d blocks)", 
						Idx++, (*itr)->m_PacketID,
						cx, cz,
						IsWanted ? "wanted" : "unwanted",
						NumBlocks
					);
					break;
				}
				
				default:
				{
					LOG("Packet %4d: type %2x", Idx++, (*itr)->m_PacketID); 
					break;
				}
			}
		}
		
		Lock.Unlock();
		Destroy();
		return;
	}
	
	if ((m_PendingNrmSendPackets.size() == 0) && (m_PendingLowSendPackets.size() == 0))
	{
		return;
	}

	if (m_PendingNrmSendPackets.size() > MAX_OUTGOING_PACKETS / 2)
	{
		LOGINFO("Suspiciously many pending packets: %i; client \"%s\", LastType: 0x%02x", m_PendingNrmSendPackets.size(), m_Username.c_str(), (*m_PendingNrmSendPackets.rbegin())->m_PacketID);
	}

	AString Data;
	Data.reserve(1100);
	// Serialize normal-priority packets up to 1000 bytes
	while (!m_PendingNrmSendPackets.empty() && (Data.size() < 1000))
	{
		m_PendingNrmSendPackets.front()->Serialize(Data);
		// LOGD("Sending packet 0x%02x", m_PendingNrmSendPackets.front()->m_PacketID);
		delete m_PendingNrmSendPackets.front();
		m_PendingNrmSendPackets.erase(m_PendingNrmSendPackets.begin());
	}
	// Serialize one low-priority packet:
	if (!m_PendingLowSendPackets.empty() && Data.empty())
	{
		m_PendingLowSendPackets.front()->Serialize(Data);
		delete m_PendingLowSendPackets.front();
		m_PendingLowSendPackets.erase(m_PendingLowSendPackets.begin());
	}
	Lock.Unlock();
	
	a_Data.append(Data);

	// Disconnect player after all packets have been sent
	if (m_bKicking && (m_PendingNrmSendPackets.size() + m_PendingLowSendPackets.size() == 0))
	{
		Destroy();
	}
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






