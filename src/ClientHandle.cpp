#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ClientHandle.h"
#include "Server.h"
#include "World.h"
#include "Entities/Pickup.h"
#include "Bindings/PluginManager.h"
#include "Entities/Player.h"
#include "Inventory.h"
#include "BlockEntities/ChestEntity.h"
#include "BlockEntities/SignEntity.h"
#include "UI/Window.h"
#include "Item.h"
#include "Piston.h"
#include "Mobs/Monster.h"
#include "ChatColor.h"
#include "OSSupport/Socket.h"
#include "OSSupport/Timer.h"
#include "Items/ItemHandler.h"
#include "Blocks/BlockHandler.h"
#include "Blocks/BlockSlab.h"

#include "Vector3f.h"
#include "Vector3d.h"

#include "Root.h"

#include "Authenticator.h"
#include "MersenneTwister.h"

#include "Protocol/ProtocolRecognizer.h"





#define AddPistonDir(x, y, z, dir, amount) switch (dir) { case 0: (y)-=(amount); break; case 1: (y)+=(amount); break;\
													 case 2: (z)-=(amount); break; case 3: (z)+=(amount); break;\
													 case 4: (x)-=(amount); break; case 5: (x)+=(amount); break; }





/// If the number of queued outgoing packets reaches this, the client will be kicked
#define MAX_OUTGOING_PACKETS 2000

/// How many explosions per single game tick are allowed
static const int MAX_EXPLOSIONS_PER_TICK = 100;

/// How many explosions in the recent history are allowed
static const int MAX_RUNNING_SUM_EXPLOSIONS = cClientHandle::NUM_CHECK_EXPLOSIONS_TICKS * MAX_EXPLOSIONS_PER_TICK / 8;

/// How many ticks before the socket is closed after the client is destroyed (#31)
static const int TICKS_BEFORE_CLOSE = 20;





#define RECI_RAND_MAX (1.f/RAND_MAX)
inline int fRadRand(MTRand & r1, int a_BlockCoord)
{
	return a_BlockCoord * 32 + (int)(16 * ((float)r1.rand() * RECI_RAND_MAX) * 16 - 8);
}





int cClientHandle::s_ClientCount = 0;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cClientHandle:

cClientHandle::cClientHandle(const cSocket * a_Socket, int a_ViewDistance) :
	m_ViewDistance(a_ViewDistance),
	m_IPString(a_Socket->GetIPString()),
	m_OutgoingData(64 KiB),
	m_Player(NULL),
	m_HasSentDC(false),
	m_LastStreamedChunkX(0x7fffffff),  // bogus chunk coords to force streaming upon login
	m_LastStreamedChunkZ(0x7fffffff),
	m_TimeSinceLastPacket(0),
	m_Ping(1000),
	m_PingID(1),
	m_BlockDigAnimStage(-1),
	m_HasStartedDigging(false),
	m_TicksSinceDestruction(0),
	m_State(csConnected),
	m_ShouldCheckDownloaded(false),
	m_CurrentExplosionTick(0),
	m_RunningSumExplosions(0),
	m_UniqueID(0),
	m_HasSentPlayerChunk(false)
{
	m_Protocol = new cProtocolRecognizer(this);
	
	s_ClientCount++;	// Not protected by CS because clients are always constructed from the same thread
	m_UniqueID = s_ClientCount;

	cTimer t1;
	m_LastPingTime = t1.GetNowTime();

	LOGD("New ClientHandle created at %p", this);
}





cClientHandle::~cClientHandle()
{
	ASSERT(m_State >= csDestroyedWaiting);  // Has Destroy() been called?
	
	LOGD("Deleting client \"%s\" at %p", GetUsername().c_str(), this);

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
			World->BroadcastPlayerListItem(*m_Player, false, this);
		}
		if (World != NULL)
		{
			World->RemovePlayer(m_Player);
			m_Player->Destroy();
		}
		delete m_Player;
		m_Player = NULL;
	}

	if (!m_HasSentDC)
	{
		SendDisconnect("Server shut down? Kthnxbai");
	}
	
	// Queue all remaining outgoing packets to cSocketThreads:
	{
		cCSLock Lock(m_CSOutgoingData);
		AString Data;
		m_OutgoingData.ReadAll(Data);
		m_OutgoingData.CommitRead();
		cRoot::Get()->GetServer()->WriteToClient(this, Data);
	}
	
	// Queue the socket to close as soon as it sends all outgoing data:
	cRoot::Get()->GetServer()->QueueClientClose(this);
	cRoot::Get()->GetServer()->RemoveClient(this);
	
	delete m_Protocol;
	m_Protocol = NULL;
	
	LOGD("ClientHandle at %p deleted", this);
}





void cClientHandle::Destroy(void)
{
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
	
	if ((m_Player != NULL) && (m_Player->GetWorld() != NULL))
	{
		RemoveFromAllChunks();
		m_Player->GetWorld()->RemoveClientFromChunkSender(this);
	}
	m_State = csDestroyedWaiting;
}





void cClientHandle::Kick(const AString & a_Reason)
{
	if (m_State >= csAuthenticating)  // Don't log pings
	{
		LOGINFO("Kicking player %s for \"%s\"", m_Username.c_str(), StripColorCodes(a_Reason).c_str());
	}
	SendDisconnect(a_Reason);
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
	
	if (m_Player->GetGameMode() == eGameMode_NotSet)
	{
		m_Player->LoginSetGameMode(World->GetGameMode());
	}

	m_Player->SetIP (m_IPString);

	cRoot::Get()->GetPluginManager()->CallHookPlayerJoined(*m_Player);
	
	m_ConfirmPosition = m_Player->GetPosition();

	// Return a server login packet
	m_Protocol->SendLogin(*m_Player, *World);

	// Send Weather if raining:
	if ((World->GetWeather() == 1) || (World->GetWeather() == 2))
	{
		m_Protocol->SendWeather(World->GetWeather());
	}

	// Send time
	m_Protocol->SendTimeUpdate(World->GetWorldAge(), World->GetTimeOfDay());

	// Send contents of the inventory window
	m_Protocol->SendWholeInventory(*m_Player->GetWindow());

	// Send health
	m_Player->SendHealth();

	// Send experience
	m_Player->SendExperience();
	
	m_Player->Initialize(World);
	m_State = csAuthenticated;

	cRoot::Get()->GetPluginManager()->CallHookPlayerSpawned(*m_Player);
}





void cClientHandle::StreamChunks(void)
{
	if ((m_State < csAuthenticated) || (m_State >= csDestroying))
	{
		return;
	}

	ASSERT(m_Player != NULL);
	
	int ChunkPosX = FAST_FLOOR_DIV((int)m_Player->GetPosX(), cChunkDef::Width);
	int ChunkPosZ = FAST_FLOOR_DIV((int)m_Player->GetPosZ(), cChunkDef::Width);
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
		World->RemoveChunkClient(itr->m_ChunkX, itr->m_ChunkZ, this);
		m_Protocol->SendUnloadChunk(itr->m_ChunkX, itr->m_ChunkZ);
	}  // for itr - RemoveChunks[]
	
	// Add all chunks that are in range and not yet in m_LoadedChunks:
	// Queue these smartly - from the center out to the edge
	for (int d = 0; d <= m_ViewDistance; ++d)  // cycle through (square) distance, from nearest to furthest
	{
		// For each distance add chunks in a hollow square centered around current position:
		for (int i = -d; i <= d; ++i)
		{
			StreamChunk(ChunkPosX + d, ChunkPosZ + i);
			StreamChunk(ChunkPosX - d, ChunkPosZ + i);
		}  // for i
		for (int i = -d + 1; i < d; ++i)
		{
			StreamChunk(ChunkPosX + i, ChunkPosZ + d);
			StreamChunk(ChunkPosX + i, ChunkPosZ - d);
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




void cClientHandle::StreamChunk(int a_ChunkX, int a_ChunkZ)
{
	if (m_State >= csDestroying)
	{
		// Don't stream chunks to clients that are being destroyed
		return;
	}
	
	cWorld * World = m_Player->GetWorld();
	ASSERT(World != NULL);

	if (World->AddChunkClient(a_ChunkX, a_ChunkZ, this))
	{
		{
			cCSLock Lock(m_CSChunkLists);
			m_LoadedChunks.push_back(cChunkCoords(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ));
			m_ChunksToSend.push_back(cChunkCoords(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ));
		}
		World->SendChunkTo(a_ChunkX, a_ChunkZ, this);
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
		
		// Also reset the LastStreamedChunk coords to bogus coords,
		// so that all chunks are streamed in subsequent StreamChunks() call (FS #407)
		m_LastStreamedChunkX = 0x7fffffff;
		m_LastStreamedChunkZ = 0x7fffffff;
	}
}





void cClientHandle::HandlePing(void)
{
	// Somebody tries to retrieve information about the server
	AString Reply;
	Printf(Reply, "%s%s%i%s%i", 
		cRoot::Get()->GetServer()->GetDescription().c_str(),
		cChatColor::Delimiter.c_str(),
		cRoot::Get()->GetServer()->GetNumPlayers(),
		cChatColor::Delimiter.c_str(),
		cRoot::Get()->GetServer()->GetMaxPlayers()
	);
	Kick(Reply.c_str());
}





bool cClientHandle::HandleLogin(int a_ProtocolVersion, const AString & a_Username)
{
	LOGD("LOGIN %s", a_Username.c_str());
	m_Username = a_Username;

	if (cRoot::Get()->GetPluginManager()->CallHookLogin(this, a_ProtocolVersion, a_Username))
	{
		Destroy();
		return false;
	}

	// Schedule for authentication; until then, let them wait (but do not block)
	m_State = csAuthenticating;
	cRoot::Get()->GetAuthenticator().Authenticate(GetUniqueID(), GetUsername(), m_Protocol->GetAuthServerID());
	return true;
}





void cClientHandle::HandleCreativeInventory(short a_SlotNum, const cItem & a_HeldItem)
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
	
	m_Player->GetWindow()->Clicked(*m_Player, 0, a_SlotNum, (a_SlotNum >= 0) ? caLeftClick : caLeftClickOutside, a_HeldItem);
}





void cClientHandle::HandlePlayerAbilities(bool a_CanFly, bool a_IsFlying, float FlyingSpeed, float WalkingSpeed)
{
	UNUSED(FlyingSpeed); // Ignore the client values for these
	UNUSED(WalkingSpeed);

	m_Player->SetCanFly(a_CanFly);
	m_Player->SetFlying(a_IsFlying);
}





void cClientHandle::HandlePlayerPos(double a_PosX, double a_PosY, double a_PosZ, double a_Stance, bool a_IsOnGround)
{
	if ((m_Player == NULL) || (m_State != csPlaying))
	{
		// The client hasn't been spawned yet and sends nonsense, we know better
		return;
	}

	/*
	// TODO: Invalid stance check
	if ((a_PosY >= a_Stance) || (a_Stance > a_PosY + 1.65))
	{
		LOGD("Invalid stance");
		SendPlayerMoveLook();
		return;
	}
	*/
	
	// If the player has moved too far, "repair" them:
	Vector3d Pos(a_PosX, a_PosY, a_PosZ);
	if ((m_Player->GetPosition() - Pos).SqrLength() > 100 * 100)
	{
		LOGD("Too far away (%0.2f), \"repairing\" the client", (m_Player->GetPosition() - Pos).Length());
		SendPlayerMoveLook();
		return;
	}
	
	// If a jump just started, process food exhaustion:
	if ((a_PosY > m_Player->GetPosY()) && !a_IsOnGround && m_Player->IsOnGround())
	{
		// we only add this exhaustion if the player is not swimming - otherwise we end up with both jump + swim exhaustion

		if (!m_Player->IsSwimming())
		{
			m_Player->AddFoodExhaustion(m_Player->IsSprinting() ? 0.8 : 0.2);
		}
	}
	
	m_Player->MoveTo(Pos);
	m_Player->SetStance(a_Stance);
	m_Player->SetTouchGround(a_IsOnGround);
}





void cClientHandle::HandlePluginMessage(const AString & a_Channel, const AString & a_Message)
{
	cPluginManager::Get()->CallHookPluginMessage(*this, a_Channel, a_Message);
}





void cClientHandle::HandleLeftClick(int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status)
{
	LOGD("HandleLeftClick: {%i, %i, %i}; Face: %i; Stat: %i",
		a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_Status
	);

	cPluginManager * PlgMgr = cRoot::Get()->GetPluginManager();
	if (PlgMgr->CallHookPlayerLeftClick(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_Status))
	{
		// A plugin doesn't agree with the action, replace the block on the client and quit:
		m_Player->GetWorld()->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
		return;
	}
	
	if (!CheckBlockInteractionsRate())
	{
		// Too many interactions per second, simply ignore. Probably a hacked client, so don't even send bak the block
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
			m_Player->TossItem(false);
			return;
		}

		case DIG_STATUS_SHOOT_EAT:
		{
			cItemHandler * ItemHandler = cItemHandler::GetItemHandler(m_Player->GetEquippedItem());
			if (ItemHandler->IsFood())
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
			return;
		}

		default:
		{
			ASSERT(!"Unhandled DIG_STATUS");
			return;
		}
	}  // switch (a_Status)
}





void cClientHandle::HandleBlockDigStarted(int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_OldBlock, NIBBLETYPE a_OldMeta)
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
	
	if (cRoot::Get()->GetPluginManager()->CallHookPlayerBreakingBlock(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_OldBlock, a_OldMeta))
	{
		// A plugin doesn't agree with the breaking. Bail out. Send the block back to the client, so that it knows:
		m_Player->GetWorld()->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
		return;
	}
	
	// Set the last digging coords to the block being dug, so that they can be checked in DIG_FINISHED to avoid dig/aim bug in the client:
	m_HasStartedDigging = true;
	m_LastDigBlockX = a_BlockX;
	m_LastDigBlockY = a_BlockY;
	m_LastDigBlockZ = a_BlockZ;

	if (
		(m_Player->IsGameModeCreative()) ||  // In creative mode, digging is done immediately
		g_BlockOneHitDig[a_OldBlock]                        // One-hit blocks get destroyed immediately, too
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
	m_Player->GetWorld()->BroadcastBlockBreakAnimation(m_UniqueID, m_BlockDigAnimX, m_BlockDigAnimY, m_BlockDigAnimZ, 0, this);

	cWorld * World = m_Player->GetWorld();
	
	cBlockHandler * Handler = cBlockHandler::GetBlockHandler(a_OldBlock);
	Handler->OnDigging(World, m_Player, a_BlockX, a_BlockY, a_BlockZ);

	cItemHandler * ItemHandler = cItemHandler::GetItemHandler(m_Player->GetEquippedItem());
	ItemHandler->OnDiggingBlock(World, m_Player, m_Player->GetEquippedItem(), a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);

	// Check for clickthrough-blocks:
	if (a_BlockFace != BLOCK_FACE_NONE)
	{
		int pX = a_BlockX;
		int pY = a_BlockY;
		int pZ = a_BlockZ;

		AddFaceDirection(pX, pY, pZ, a_BlockFace); // Get the block in front of the clicked coordinates (m_bInverse defaulted to false)
		Handler = cBlockHandler::GetBlockHandler(World->GetBlock(pX, pY, pZ));

		if (Handler->IsClickedThrough())
		{
			Handler->OnDigging(World, m_Player, pX, pY, pZ);
		}
	}
}





void cClientHandle::HandleBlockDigFinished(int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_OldBlock, NIBBLETYPE a_OldMeta)
{
	if (
		!m_HasStartedDigging ||           // Hasn't received the DIG_STARTED packet
		(m_LastDigBlockX != a_BlockX) ||  // DIG_STARTED has had different pos
		(m_LastDigBlockY != a_BlockY) ||
		(m_LastDigBlockZ != a_BlockZ)
	)
	{
		LOGD("Prevented a dig/aim bug in the client (finish {%d, %d, %d} vs start {%d, %d, %d}, HSD: %s)",
			a_BlockX, a_BlockY, a_BlockZ,
			m_LastDigBlockX, m_LastDigBlockY, m_LastDigBlockZ,
			m_HasStartedDigging
		);
		return;
	}

	m_HasStartedDigging = false;
	if (m_BlockDigAnimStage != -1)
	{
		// End dig animation
		m_BlockDigAnimStage = -1;
		// It seems that 10 ends block animation
		m_Player->GetWorld()->BroadcastBlockBreakAnimation(m_UniqueID, m_BlockDigAnimX, m_BlockDigAnimY, m_BlockDigAnimZ, 10, this);
	}

	cItemHandler * ItemHandler = cItemHandler::GetItemHandler(m_Player->GetEquippedItem());
	
	if (a_OldBlock == E_BLOCK_AIR)
	{
		LOGD("Dug air - what the function?");
		return;
	}
	
	cWorld * World = m_Player->GetWorld();
	ItemHandler->OnBlockDestroyed(World, m_Player, m_Player->GetEquippedItem(), a_BlockX, a_BlockY, a_BlockZ);
	// The ItemHandler is also responsible for spawning the pickups
	
	BlockHandler(a_OldBlock)->OnDestroyedByPlayer(World, m_Player, a_BlockX, a_BlockY, a_BlockZ);
	World->BroadcastSoundParticleEffect(2001, a_BlockX, a_BlockY, a_BlockZ, a_OldBlock, this);
	World->DigBlock(a_BlockX, a_BlockY, a_BlockZ);

	cRoot::Get()->GetPluginManager()->CallHookPlayerBrokenBlock(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_OldBlock, a_OldMeta);
}





void cClientHandle::HandleRightClick(int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, const cItem & a_HeldItem)
{
	LOGD("HandleRightClick: {%d, %d, %d}, face %d, HeldItem: %s",
		a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, ItemToFullString(a_HeldItem).c_str()
	);
	
	cPluginManager * PlgMgr = cRoot::Get()->GetPluginManager();
	if (PlgMgr->CallHookPlayerRightClick(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ))
	{
		// A plugin doesn't agree with the action, replace the block on the client and quit:
		if (a_BlockFace > -1)
		{
			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
			m_Player->GetWorld()->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
		}
		return;
	}
	
	if (!CheckBlockInteractionsRate())
	{
		LOGD("Too many block interactions, aborting placement");
		return;
	}
	
	const cItem & Equipped = m_Player->GetInventory().GetEquippedItem();

	if ((Equipped.m_ItemType != a_HeldItem.m_ItemType) && (a_HeldItem.m_ItemType != -1))
	{
		// Only compare ItemType, not meta (torches have different metas)
		// The -1 check is there because sometimes the client sends -1 instead of the held item 
		//  ( http://forum.mc-server.org/showthread.php?tid=549&pid=4502#pid4502 )
		LOGWARN("Player %s tried to place a block that was not equipped (exp %d, got %d)",
			m_Username.c_str(), Equipped.m_ItemType, a_HeldItem.m_ItemType
		);
		
		// Let's send the current world block to the client, so that it can immediately "let the user know" that they haven't placed the block
		if (a_BlockFace > -1)
		{
			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
			m_Player->GetWorld()->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
		}
		return;
	}
	
	cWorld * World = m_Player->GetWorld();

	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, BlockType, BlockMeta);
	cBlockHandler * BlockHandler = cBlockHandler::GetBlockHandler(BlockType);
	
	if (BlockHandler->IsUseable() && !m_Player->IsCrouched())
	{
		if (PlgMgr->CallHookPlayerUsingBlock(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, BlockType, BlockMeta))
		{
			// A plugin doesn't agree with using the block, abort
			return;
		}
		BlockHandler->OnUse(World, m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ);
		PlgMgr->CallHookPlayerUsedBlock(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, BlockType, BlockMeta);
		return;
	}
	
	cItemHandler * ItemHandler = cItemHandler::GetItemHandler(Equipped.m_ItemType);
	
	if (ItemHandler->IsPlaceable())
	{
		HandlePlaceBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, *ItemHandler);
	}
	else if (ItemHandler->IsFood())
	{
		if (m_Player->IsSatiated())
		{
			// The player is satiated, they cannot eat
			return;
		}
		m_Player->StartEating();
		if (PlgMgr->CallHookPlayerEating(*m_Player))
		{
			// A plugin won't let us eat, abort (send the proper packets to the client, too):
			m_Player->AbortEating();
			return;
		}
		return;
	}
	else
	{
		if (PlgMgr->CallHookPlayerUsingItem(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ))
		{
			// A plugin doesn't agree with using the item, abort
			return;
		}
		ItemHandler->OnItemUse(World, m_Player, Equipped, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
		PlgMgr->CallHookPlayerUsedItem(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ);
	}
}





void cClientHandle::HandlePlaceBlock(int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, cItemHandler & a_ItemHandler)
{
	if (a_BlockFace < 0)
	{
		// Clicked in air
		return;
	}
	
	cWorld * World = m_Player->GetWorld();

	BLOCKTYPE ClickedBlock;
	NIBBLETYPE ClickedBlockMeta;
	BLOCKTYPE EquippedBlock = (BLOCKTYPE)(m_Player->GetEquippedItem().m_ItemType);
	NIBBLETYPE EquippedBlockDamage = (NIBBLETYPE)(m_Player->GetEquippedItem().m_ItemDamage);

	if ((a_BlockY < 0) || (a_BlockY >= cChunkDef::Height))
	{
		// The block is being placed outside the world, ignore this packet altogether (#128)
		return;
	}
	
	World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, ClickedBlock, ClickedBlockMeta);

	// Special slab handling - placing a slab onto another slab produces a dblslab instead:
	if (
		cBlockSlabHandler::IsAnySlabType(ClickedBlock) &&               // Is there a slab already?
		cBlockSlabHandler::IsAnySlabType(EquippedBlock) &&              // Is the player placing another slab?
		((ClickedBlockMeta & 0x07) == (EquippedBlockDamage & 0x07)) &&  // Is it the same slab type?
		(
			(a_BlockFace == BLOCK_FACE_TOP) ||                            // Clicking the top of a bottom slab
			(a_BlockFace == BLOCK_FACE_BOTTOM)                            // Clicking the bottom of a top slab
		)
	)
	{
		// Coordinates at clicked block, which was an eligible slab, and either top or bottom faces were clicked
		// If clicked top face and slab occupies the top voxel, we want a slab to be placed above it (therefore increment Y)
		// Else if clicked bottom face and slab occupies the bottom voxel, decrement Y for the same reason
		// Don't touch coordinates if anything else because a dblslab opportunity is present
		if((ClickedBlockMeta & 0x08) && (a_BlockFace == BLOCK_FACE_TOP))
		{
			++a_BlockY;
		}
		else if (!(ClickedBlockMeta & 0x08) && (a_BlockFace == BLOCK_FACE_BOTTOM))
		{
			--a_BlockY;
		}
		World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, ClickedBlock, ClickedBlockMeta);
	}
	else
	{
		// Check if the block ignores build collision (water, grass etc.):
		if (
			BlockHandler(ClickedBlock)->DoesIgnoreBuildCollision() ||
			BlockHandler(ClickedBlock)->DoesIgnoreBuildCollision(m_Player, ClickedBlockMeta)
			)
		{
			BlockHandler(ClickedBlock)->OnDestroyedByPlayer(World, m_Player, a_BlockX, a_BlockY, a_BlockZ);
		}
		else
		{
			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
			
			if ((a_BlockY < 0) || (a_BlockY >= cChunkDef::Height))
			{
				// The block is being placed outside the world, ignore this packet altogether (#128)
				return;
			}
			
			NIBBLETYPE PlaceMeta;
			BLOCKTYPE PlaceBlock;
			World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, PlaceBlock, PlaceMeta);

			// Clicked on side of block, make sure that placement won't be cancelled if there is a slab able to be double slabbed.
			// No need to do combinability (dblslab) checks, client will do that here.
			if (cBlockSlabHandler::IsAnySlabType(PlaceBlock))
			{
				// It's a slab, don't do checks and proceed to double-slabbing
			}
			else
			{
				if (
					!BlockHandler(PlaceBlock)->DoesIgnoreBuildCollision() &&
					!BlockHandler(PlaceBlock)->DoesIgnoreBuildCollision(m_Player, PlaceMeta)
					)
				{
					// Tried to place a block *into* another?
					// Happens when you place a block aiming at side of block with a torch on it or stem beside it
					return;
				}
			}
		}
	}
	
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	if (!a_ItemHandler.GetPlacementBlockTypeMeta(World, m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, BlockType, BlockMeta))
	{
		// Handler refused the placement, send that information back to the client:
		World->SendBlockTo(a_BlockX, a_BlockY, a_BlockY, m_Player);
		return;
	}
	
	cBlockHandler * NewBlock = BlockHandler(BlockType);

	if (cRoot::Get()->GetPluginManager()->CallHookPlayerPlacingBlock(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, BlockType, BlockMeta))
	{
		// A plugin doesn't agree with placing the block, revert the block on the client:
		World->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
		return;
	}
	
	// The actual block placement:
	World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, BlockType, BlockMeta);
	if (m_Player->GetGameMode() != gmCreative)
	{
		m_Player->GetInventory().RemoveOneEquippedItem();
	}
	NewBlock->OnPlacedByPlayer(World, m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, BlockType, BlockMeta);
	
	// Step sound with 0.8f pitch is used as block placement sound
	World->BroadcastSoundEffect(NewBlock->GetStepSound(), a_BlockX * 8, a_BlockY * 8, a_BlockZ * 8, 1.0f, 0.8f);
	cRoot::Get()->GetPluginManager()->CallHookPlayerPlacedBlock(*m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, BlockType, BlockMeta);
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
	
	// Not a command, broadcast as a simple message:
	AString Msg;
	Printf(Msg, "%s<%s>%s %s",
		m_Player->GetColor().c_str(),
		m_Player->GetName().c_str(),
		cChatColor::White.c_str(),
		Message.c_str()
	);
	m_Player->GetWorld()->BroadcastChat(Msg);
}





void cClientHandle::HandlePlayerLook(float a_Rotation, float a_Pitch, bool a_IsOnGround)
{
	if ((m_Player == NULL) || (m_State != csPlaying))
	{
		return;
	}
	
	m_Player->SetRotation   (a_Rotation);
	m_Player->SetHeadYaw    (a_Rotation);
	m_Player->SetPitch      (a_Pitch);
	m_Player->SetTouchGround(a_IsOnGround);
}





void cClientHandle::HandlePlayerMoveLook(double a_PosX, double a_PosY, double a_PosZ, double a_Stance, float a_Rotation, float a_Pitch, bool a_IsOnGround)
{
	if ((m_Player == NULL) || (m_State != csPlaying))
	{
		// The client hasn't been spawned yet and sends nonsense, we know better
		return;
	}

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
	m_Player->SetHeadYaw    (a_Rotation);
	m_Player->SetRotation   (a_Rotation);
	m_Player->SetPitch      (a_Pitch);
}





void cClientHandle::HandleAnimation(char a_Animation)
{
	if (cPluginManager::Get()->CallHookPlayerAnimation(*m_Player, a_Animation))
	{
		// Plugin disagrees, bail out
		return;
	}

	// Because the animation ID sent to servers by clients are different to those sent back, we need this
	switch (a_Animation)
	{
		case 0: // No animation - wiki.vg doesn't say that client has something specific for it, so I suppose it will just become -1
		case 1:
		case 2:
		case 3:
		{
			a_Animation--; // Offset by -1
			break;
		}
		case 5: 
		case 6:
		case 7:
		{
			a_Animation -= 2; // Offset by -2
			break;
		}
		default: // Anything else is the same
			break;
	}

	m_Player->GetWorld()->BroadcastEntityAnimation(*m_Player, a_Animation, this);
}





void cClientHandle::HandleSlotSelected(short a_SlotNum)
{
	m_Player->GetInventory().SetEquippedSlotNum(a_SlotNum);
	m_Player->GetWorld()->BroadcastEntityEquipment(*m_Player, 0, m_Player->GetInventory().GetEquippedItem(), this);
}





void cClientHandle::HandleSteerVehicle(float a_Forward, float a_Sideways)
{
	m_Player->SteerVehicle(a_Forward, a_Sideways);
}





void cClientHandle::HandleWindowClose(char a_WindowID)
{
	m_Player->CloseWindowIfID(a_WindowID);
}





void cClientHandle::HandleWindowClick(char a_WindowID, short a_SlotNum, eClickAction a_ClickAction, const cItem & a_HeldItem)
{
	LOGD("WindowClick: WinID %d, SlotNum %d, action: %s, Item %s x %d",
		a_WindowID, a_SlotNum, ClickActionToString(a_ClickAction),
		ItemToString(a_HeldItem).c_str(), a_HeldItem.m_ItemCount
	);
	
	cWindow * Window = m_Player->GetWindow();
	if (Window == NULL)
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
	cWorld * World = m_Player->GetWorld();
	World->UpdateSign(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4, m_Player);
}





void cClientHandle::HandleUseEntity(int a_TargetEntityID, bool a_IsLeftClick)
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
	if (m_Player == NULL)
	{
		Destroy();
		return;
	}
	m_Player->Respawn();
	cRoot::Get()->GetPluginManager()->CallHookPlayerSpawned(*m_Player);
}





void cClientHandle::HandleDisconnect(const AString & a_Reason)
{
	LOGD("Received d/c packet from %s with reason \"%s\"", m_Username.c_str(), a_Reason.c_str());
	if (!cRoot::Get()->GetPluginManager()->CallHookDisconnect(m_Player, a_Reason))
	{
		AString DisconnectMessage;
		Printf(DisconnectMessage, "%s[LEAVE] %s%s has left the game", cChatColor::Yellow.c_str(), cChatColor::White.c_str(), m_Username.c_str());
		cRoot::Get()->BroadcastChat(DisconnectMessage);
		LOGINFO("Player %s has left the game.", m_Username.c_str());
	}
	m_HasSentDC = true;
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





bool cClientHandle::HandleHandshake(const AString & a_Username)
{
	if (!cRoot::Get()->GetPluginManager()->CallHookHandshake(this, a_Username))
	{
		if (cRoot::Get()->GetServer()->GetNumPlayers() >= cRoot::Get()->GetServer()->GetMaxPlayers())
		{
			Kick("The server is currently full :(-- Try again later");
			return false;
		}
	}
	return true;
}





void cClientHandle::HandleEntityAction(int a_EntityID, char a_ActionID)
{
	if (a_EntityID != m_Player->GetUniqueID())
	{
		// We should only receive entity actions from the entity that is performing the action
		return;
	}

	switch (a_ActionID)
	{
		case 1:  // Crouch
		{
			m_Player->SetCrouch(true);
			break;
		}
		case 2:  // Uncrouch
		{
			m_Player->SetCrouch(false);
			break;
		}
		case 3:  // Leave bed
		{
			m_Player->GetWorld()->BroadcastEntityAnimation(*m_Player, 2);
			break;
		}
		case 4:  // Start sprinting
		{
			m_Player->SetSprint(true);
			break;
		}
		case 5:  // Stop sprinting
		{
			m_Player->SetSprint(false);
			SendPlayerMaxSpeed();
			break;
		}
	}
}





void cClientHandle::HandleUnmount(void)
{
	if (m_Player == NULL)
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





void cClientHandle::SendData(const char * a_Data, int a_Size)
{
	if (m_HasSentDC)
	{
		// This could crash the client, because they've already unloaded the world etc., and suddenly a wild packet appears (#31)
		return;
	}
	
	{
		cCSLock Lock(m_CSOutgoingData);
		
		// _X 2012_09_06: We need an overflow buffer, usually when streaming the initial chunks
		if (m_OutgoingDataOverflow.empty())
		{
			// No queued overflow data; if this packet fits into the ringbuffer, put it in, otherwise put it in the overflow buffer:
			int CanFit = m_OutgoingData.GetFreeSpace();
			if (CanFit > a_Size)
			{
				CanFit = a_Size;
			}
			if (CanFit > 0)
			{
				m_OutgoingData.Write(a_Data, CanFit);
			}
			if (a_Size > CanFit)
			{
				m_OutgoingDataOverflow.append(a_Data + CanFit, a_Size - CanFit);
			}
		}
		else
		{
			// There is a queued overflow. Append to it, then send as much from its front as possible
			m_OutgoingDataOverflow.append(a_Data, a_Size);
			int CanFit = m_OutgoingData.GetFreeSpace();
			if (CanFit > 128)
			{
				// No point in moving the data over if it's not large enough - too much effort for too little an effect
				m_OutgoingData.Write(m_OutgoingDataOverflow.data(), CanFit);
				m_OutgoingDataOverflow.erase(0, CanFit);
			}
		}
	}  // Lock(m_CSOutgoingData)
	
	// Notify SocketThreads that we have something to write:
	cRoot::Get()->GetServer()->NotifyClientWrite(this);
}





void cClientHandle::MoveToWorld(cWorld & a_World, bool a_SendRespawnPacket)
{
	UNUSED(a_World);
	ASSERT(m_Player != NULL);
	
	if (a_SendRespawnPacket)
	{
		SendRespawn();
	}

	cWorld * World = m_Player->GetWorld();
		
	// Remove all associated chunks:
	cChunkCoordsList Chunks;
	{
		cCSLock Lock(m_CSChunkLists);
		std::swap(Chunks, m_LoadedChunks);
		m_ChunksToSend.clear();
	}
	for (cChunkCoordsList::iterator itr = Chunks.begin(), end = Chunks.end(); itr != end; ++itr)
	{
		World->RemoveChunkClient(itr->m_ChunkX, itr->m_ChunkZ, this);
		m_Protocol->SendUnloadChunk(itr->m_ChunkX, itr->m_ChunkZ);
	}  // for itr - Chunks[]
	
	// Do NOT stream new chunks, the new world runs its own tick thread and may deadlock
	// Instead, the chunks will be streamed when the client is moved to the new world's Tick list,
	// by setting state to csAuthenticated
	m_State = csAuthenticated;
	m_LastStreamedChunkX = 0x7fffffff;
	m_LastStreamedChunkZ = 0x7fffffff;
	m_HasSentPlayerChunk = false;
}





bool cClientHandle::CheckBlockInteractionsRate(void)
{
	ASSERT(m_Player != NULL);
	ASSERT(m_Player->GetWorld() != NULL);
	/*
	// TODO: _X 2012_11_01: This needs a total re-thinking and rewriting
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
	*/
	return true;
}





void cClientHandle::Tick(float a_Dt)
{
	// Handle clients that are waiting for final close while destroyed:
	if (m_State == csDestroyedWaiting)
	{
		m_TicksSinceDestruction += 1;  // This field is misused for the timeout counting
		if (m_TicksSinceDestruction > TICKS_BEFORE_CLOSE)
		{
			m_State = csDestroyed;
		}
		return;
	}
	
	// Process received network data:
	AString IncomingData;
	{
		cCSLock Lock(m_CSIncomingData);
		std::swap(IncomingData, m_IncomingData);
	}
	m_Protocol->DataReceived(IncomingData.data(), IncomingData.size());
	
	m_TimeSinceLastPacket += a_Dt;
	if (m_TimeSinceLastPacket > 30000.f)  // 30 seconds time-out
	{
		SendDisconnect("Nooooo!! You timed out! D: Come back!");
		Destroy();
	}
	
	if (m_Player == NULL)
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
	cTimer t1;
	if ((m_LastPingTime + cClientHandle::PING_TIME_MS <= t1.GetNowTime()))
	{
		m_PingID++;
		m_PingStartTime = t1.GetNowTime();
		m_Protocol->SendKeepAlive(m_PingID);
		m_LastPingTime = m_PingStartTime;
	}

	// Handle block break animation:
	if (m_BlockDigAnimStage > -1)
	{
		int lastAnimVal = m_BlockDigAnimStage;
		m_BlockDigAnimStage += (int)(m_BlockDigAnimSpeed * a_Dt);
		if (m_BlockDigAnimStage > 9000)
		{
			m_BlockDigAnimStage = 9000;
		}
		if (m_BlockDigAnimStage / 1000 != lastAnimVal / 1000)
		{
			m_Player->GetWorld()->BroadcastBlockBreakAnimation(m_UniqueID, m_BlockDigAnimX, m_BlockDigAnimY, m_BlockDigAnimZ, (char)(m_BlockDigAnimStage / 1000), this);
		}
	}
	
	// Update the explosion statistics:
	m_CurrentExplosionTick = (m_CurrentExplosionTick + 1) % ARRAYCOUNT(m_NumExplosionsPerTick);
	m_RunningSumExplosions -= m_NumExplosionsPerTick[m_CurrentExplosionTick];
	m_NumExplosionsPerTick[m_CurrentExplosionTick] = 0;
}





void cClientHandle::ServerTick(float a_Dt)
{
	// Handle clients that are waiting for final close while destroyed:
	if (m_State == csDestroyedWaiting)
	{
		// Do not wait while the client is not in the world, simply cut them off.
		m_State = csDestroyed;
		return;
	}
	
	// Process received network data:
	AString IncomingData;
	{
		cCSLock Lock(m_CSIncomingData);
		std::swap(IncomingData, m_IncomingData);
	}
	m_Protocol->DataReceived(IncomingData.data(), IncomingData.size());
	
	if (m_State == csAuthenticated)
	{
		StreamChunks();

		// Remove the client handle from the server, it will be ticked from its cPlayer object from now on
		cRoot::Get()->GetServer()->ClientMovedToWorld(this);
		
		// Add the player to the world (start ticking from there):
		m_State = csDownloadingWorld;
		m_Player->GetWorld()->AddPlayer(m_Player);
		return;
	}
	
	m_TimeSinceLastPacket += a_Dt;
	if (m_TimeSinceLastPacket > 30000.f)  // 30 seconds time-out
	{
		SendDisconnect("Nooooo!! You timed out! D: Come back!");
		Destroy();
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





void cClientHandle::SendBlockBreakAnim(int a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage)
{
	m_Protocol->SendBlockBreakAnim(a_EntityID, a_BlockX, a_BlockY, a_BlockZ, a_Stage);
}





void cClientHandle::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	m_Protocol->SendBlockChange(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
}





void cClientHandle::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(!a_Changes.empty());  // We don't want to be sending empty change packets!
	
	m_Protocol->SendBlockChanges(a_ChunkX, a_ChunkZ, a_Changes);
}





void cClientHandle::SendChat(const AString & a_Message)
{
	m_Protocol->SendChat(a_Message);
}





void cClientHandle::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	ASSERT(m_Player != NULL);
	
	// Check chunks being sent, erase them from m_ChunksToSend:
	bool Found = false;
	{
		cCSLock Lock(m_CSChunkLists);
		for (cChunkCoordsList::iterator itr = m_ChunksToSend.begin(); itr != m_ChunksToSend.end(); ++itr)
		{
			if ((itr->m_ChunkX == a_ChunkX) && (itr->m_ChunkZ == a_ChunkZ))
			{
				m_ChunksToSend.erase(itr);
				Found = true;
				break;
			}
		}  // for itr - m_ChunksToSend[]
	}
	if (!Found)
	{
		// This just sometimes happens. If you have a reliably replicatable situation for this, go ahead and fix it
		// It's not a big issue anyway, just means that some chunks may be compressed several times
		// LOGD("Refusing to send    chunk [%d, %d] to client \"%s\" at [%d, %d].", ChunkX, ChunkZ, m_Username.c_str(), m_Player->GetChunkX(), m_Player->GetChunkZ());
		return;
	}
	
	m_Protocol->SendChunkData(a_ChunkX, a_ChunkZ, a_Serializer);

	// If it is the chunk the player's in, make them spawn (in the tick thread):
	if ((m_State == csAuthenticated) || (m_State == csDownloadingWorld))
	{
		if ((a_ChunkX == m_Player->GetChunkX()) && (a_ChunkZ == m_Player->GetChunkZ()))
		{
			m_HasSentPlayerChunk = true;
		}
	}
}





void cClientHandle::SendCollectPickup(const cPickup & a_Pickup, const cPlayer & a_Player)
{
	m_Protocol->SendCollectPickup(a_Pickup, a_Player);
}





void cClientHandle::SendDestroyEntity(const cEntity & a_Entity)
{
	m_Protocol->SendDestroyEntity(a_Entity);
}





void cClientHandle::SendDisconnect(const AString & a_Reason)
{
	if (!m_HasSentDC)
	{
		LOGD("Sending a DC: \"%s\"", StripColorCodes(a_Reason).c_str());
		m_Protocol->SendDisconnect(a_Reason);
		m_HasSentDC = true;
	}
}





void cClientHandle::SendEditSign(int a_BlockX, int a_BlockY, int a_BlockZ)
{
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
	if (
		(m_NumExplosionsPerTick[m_CurrentExplosionTick] > MAX_EXPLOSIONS_PER_TICK) ||  // Too many explosions in this tick
		(m_RunningSumExplosions > MAX_RUNNING_SUM_EXPLOSIONS)  // Too many explosions in the recent history
	)
	{
		LOGD("Dropped %u explosions", a_BlocksAffected.size());
		return;
	}
	
	// Update the statistics:
	m_NumExplosionsPerTick[m_CurrentExplosionTick] += a_BlocksAffected.size();
	m_RunningSumExplosions += a_BlocksAffected.size();
	
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





void cClientHandle::SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	m_Protocol->SendInventorySlot(a_WindowID, a_SlotNum, a_Item);
}





void cClientHandle::SendParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmmount)
{
	m_Protocol->SendParticleEffect(a_ParticleName, a_SrcX, a_SrcY, a_SrcZ, a_OffsetX, a_OffsetY, a_OffsetZ, a_ParticleData, a_ParticleAmmount);
}





void cClientHandle::SendPickupSpawn(const cPickup & a_Pickup)
{
	m_Protocol->SendPickupSpawn(a_Pickup);
}





void cClientHandle::SendEntityAnimation(const cEntity & a_Entity, char a_Animation)
{
	m_Protocol->SendEntityAnimation(a_Entity, a_Animation);
}





void cClientHandle::SendPlayerAbilities()
{
	m_Protocol->SendPlayerAbilities();
}





void cClientHandle::SendPlayerListItem(const cPlayer & a_Player, bool a_IsOnline)
{
	m_Protocol->SendPlayerListItem(a_Player, a_IsOnline);
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





void cClientHandle::SendRespawn(void)
{
	m_Protocol->SendRespawn();
}





void cClientHandle::SendExperience(void)
{
	m_Protocol->SendExperience();
}





void cClientHandle::SendExperienceOrb(const cExpOrb & a_ExpOrb)
{
	m_Protocol->SendExperienceOrb(a_ExpOrb);
}





void cClientHandle::SendSoundEffect(const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch)
{
	m_Protocol->SendSoundEffect(a_SoundName, a_SrcX, a_SrcY, a_SrcZ, a_Volume, a_Pitch);
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





void cClientHandle::SendSpawnVehicle(const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType) // VehicleSubType is specific to Minecarts
{
	m_Protocol->SendSpawnVehicle(a_Vehicle, a_VehicleType, a_VehicleSubType);
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





void cClientHandle::SendTimeUpdate(Int64 a_WorldAge, Int64 a_TimeOfDay)
{
	m_Protocol->SendTimeUpdate(a_WorldAge, a_TimeOfDay);
}





void cClientHandle::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	m_Protocol->SendUnloadChunk(a_ChunkX, a_ChunkZ);
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





void cClientHandle::SendUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ )
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





void cClientHandle::SendWindowProperty(const cWindow & a_Window, int a_Property, int a_Value)
{
	m_Protocol->SendWindowProperty(a_Window, a_Property, a_Value);
}





const AString & cClientHandle::GetUsername(void) const
{
	return m_Username;
}





void cClientHandle::SetUsername( const AString & a_Username )
{
	m_Username = a_Username;
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
	if (m_State >= csDestroying)
	{
		return false;
	}
	
	cCSLock Lock(m_CSChunkLists);
	return (std::find(m_ChunksToSend.begin(), m_ChunksToSend.end(), cChunkCoords(a_ChunkX, a_ChunkY, a_ChunkZ)) != m_ChunksToSend.end());
}





void cClientHandle::AddWantedChunk(int a_ChunkX, int a_ChunkZ)
{
	if (m_State >= csDestroying)
	{
		return;
	}
	
	LOGD("Adding chunk [%d, %d] to wanted chunks for client %p", a_ChunkX, a_ChunkZ, this);
	cCSLock Lock(m_CSChunkLists);
	if (std::find(m_ChunksToSend.begin(), m_ChunksToSend.end(), cChunkCoords(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ)) == m_ChunksToSend.end())
	{
		m_ChunksToSend.push_back(cChunkCoords(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ));
	}
}





void cClientHandle::PacketBufferFull(void)
{
	// Too much data in the incoming queue, the server is probably too busy, kick the client:
	LOGERROR("Too much data in queue for client \"%s\" @ %s, kicking them.", m_Username.c_str(), m_IPString.c_str());
	SendDisconnect("Server busy");
	Destroy();
}





void cClientHandle::PacketUnknown(UInt32 a_PacketType)
{
	LOGERROR("Unknown packet type 0x%x from client \"%s\" @ %s", a_PacketType, m_Username.c_str(), m_IPString.c_str());

	AString Reason;
	Printf(Reason, "Unknown [C->S] PacketType: 0x%x", a_PacketType);
	SendDisconnect(Reason);
	Destroy();
}





void cClientHandle::PacketError(unsigned char a_PacketType)
{
	LOGERROR("Protocol error while parsing packet type 0x%02x; disconnecting client \"%s\"", a_PacketType, m_Username.c_str());
	SendDisconnect("Protocol error");
	Destroy();
}





void cClientHandle::DataReceived(const char * a_Data, int a_Size)
{
	// Data is received from the client, store it in the buffer to be processed by the Tick thread:
	m_TimeSinceLastPacket = 0;
	cCSLock Lock(m_CSIncomingData);
	m_IncomingData.append(a_Data, a_Size);
}





void cClientHandle::GetOutgoingData(AString & a_Data)
{
	// Data can be sent to client
	{
		cCSLock Lock(m_CSOutgoingData);
		m_OutgoingData.ReadAll(a_Data);
		m_OutgoingData.CommitRead();
		a_Data.append(m_OutgoingDataOverflow);
		m_OutgoingDataOverflow.clear();
	}

	// Disconnect player after all packets have been sent
	if (m_HasSentDC && a_Data.empty())
	{
		Destroy();
	}
}





void cClientHandle::SocketClosed(void)
{
	// The socket has been closed for any reason
	
	LOGD("Player %s @ %s disconnected", m_Username.c_str(), m_IPString.c_str());

	if (m_Username != "") // Ignore client pings
	{
		if (!cRoot::Get()->GetPluginManager()->CallHookDisconnect(m_Player, "Player disconnected"))
		{
			AString DisconnectMessage;
			Printf(DisconnectMessage, "%s[LEAVE] %s%s has left the game", cChatColor::Yellow.c_str(), cChatColor::White.c_str(), m_Username.c_str());
			cRoot::Get()->BroadcastChat(DisconnectMessage);
			LOGINFO("Player %s has left the game.", m_Username.c_str());
		}
	}

	Destroy();
}






