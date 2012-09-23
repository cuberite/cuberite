
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
#include "UI/Window.h"
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
#include "ChunkDataSerializer.h"

#include "cTracer.h"
#include "Vector3f.h"
#include "Vector3d.h"

#include "cSleep.h"
#include "cRoot.h"

#include "cBlockingTCPLink.h"
#include "cAuthenticator.h"
#include "MersenneTwister.h"

#include "Protocol/ProtocolRecognizer.h"





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
	, m_Socket(a_Socket)
	, m_OutgoingData(64 KiB)
	, m_bDestroyed(false)
	, m_Player(NULL)
	, m_bKicking(false)
	, m_TimeLastPacket(cWorld::GetTime())
	, m_bKeepThreadGoing(true)
	, m_Ping(1000)
	, m_PingID(1)
	, m_State(csConnected)
	, m_LastStreamedChunkX(0x7fffffff)  // bogus chunk coords to force streaming upon login
	, m_LastStreamedChunkZ(0x7fffffff)
	, m_ShouldCheckDownloaded(false)
	, m_UniqueID(0)
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

			// Send the Chat packet:
			AString Left(m_Username + " left the game!");
			World->BroadcastChat(Left, this);
		}
		if (World != NULL)
		{
			World->RemovePlayer(m_Player);
		}
	}

	if (m_Socket.IsValid())
	{
		if (!m_bKicking)
		{
			SendDisconnect("Server shut down? Kthnxbai");
		}
	}
	
	if (m_Player != NULL)
	{
		m_Player->Destroy();
		m_Player = NULL;
	}

	// Queue all remaining outgoing packets to cSocketThreads:
	{
		cCSLock Lock(m_CSOutgoingData);
		AString Data;
		m_OutgoingData.ReadAll(Data);
		m_OutgoingData.CommitRead();
		cRoot::Get()->GetServer()->WriteToClient(&m_Socket, Data);
	}
	
	// Queue the socket to close as soon as it sends all outgoing data:
	cRoot::Get()->GetServer()->QueueClientClose(&m_Socket);
	
	// We need to remove the socket from SocketThreads because we own it and it gets destroyed after this destructor finishes
	// TODO: The socket needs to stay alive, someone else has to own it
	cRoot::Get()->GetServer()->RemoveClient(&m_Socket);
	
	delete m_Protocol;
	m_Protocol = NULL;
	
	LOGD("ClientHandle at %p deleted", this);
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
	SendDisconnect(a_Reason);
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
	
	if (m_Player->GetGameMode() == eGameMode_NotSet)
	{
		m_Player->LoginSetGameMode(World->GetGameMode());
	}

	m_Player->SetIP (m_Socket.GetIPString());

	cRoot::Get()->GetPluginManager()->CallHook(cPluginManager::HOOK_PLAYER_SPAWN, 1, m_Player);
	
	m_ConfirmPosition = m_Player->GetPosition();

	// Return a server login packet
	m_Protocol->SendLogin(*m_Player, *World);

	// Send Weather if raining:
	if ((World->GetWeather() == 1) || (World->GetWeather() == 2))
	{
		m_Protocol->SendWeather(World->GetWeather());
	}

	// Send time
	m_Protocol->SendTimeUpdate(World->GetWorldTime());

	// Send inventory
	m_Player->GetInventory().SendWholeInventory(*this);

	// Send health
	m_Player->SendHealth();
	
	m_Player->Initialize(World);
	StreamChunks();
	m_State = csDownloadingWorld;
	
	// Broadcast this player's spawning to all other players in the same chunk
	m_Player->GetWorld()->BroadcastSpawn(*m_Player, this);
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
		m_Protocol->SendUnloadChunk(itr->m_ChunkX, itr->m_ChunkZ);
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
	if (m_Player->GetGameMode() != eGameMode_Creative)
	{
		LOGWARNING("Got a CreativeInventoryAction packet from user \"%s\" while not in creative mode. Ignoring.", m_Username.c_str());
		return;
	}
	if (m_Player->GetWindow()->GetWindowType() != cWindow::Inventory)
	{
		LOGWARNING("Got a CreativeInventoryAction packet from user \"%s\" while not in the inventory window. Ignoring.", m_Username.c_str());
		return;
	}
	
	m_Player->GetWindow()->Clicked(*m_Player, 0, a_SlotNum, false, false, a_HeldItem);
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
		int pY = a_BlockY;
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
	LOGD("HandleBlockPlace: {%d, %d, %d}, face %d, itemtype: %d",
		a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_HeldItem.m_ItemType
	);
	
	if (!CheckBlockInteractionsRate())
	{
		return;
	}
	
	cItem & Equipped = m_Player->GetInventory().GetEquippedItem();

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
			AddDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
			m_Player->GetWorld()->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, m_Player);
		}
		return;
	}

	if (cRoot::Get()->GetPluginManager()->CallHookBlockPlace(m_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, Equipped))
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
				// World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
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


				BLOCKTYPE PlaceBlock = m_Player->GetWorld()->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
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
				// Step sound with 0.8f pitch is used as block placement sound
				World->BroadcastSoundEffect(NewBlock->GetStepSound(),a_BlockX * 8, a_BlockY * 8, a_BlockZ * 8, 1.0f, 0.8f);
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
	switch (m_State)
	{
		case csPlaying:
		{
			m_Player->MoveTo(Vector3d(a_PosX, a_PosY, a_PosZ));
			m_Player->SetStance     (a_Stance);
			m_Player->SetTouchGround(a_IsOnGround);
			m_Player->SetRotation   (a_Rotation);
			m_Player->SetPitch      (a_Pitch);
			m_Player->WrapRotation();
			break;
		}
		
		case csDownloadingWorld:
		{
			Vector3d ReceivedPosition = Vector3d(a_PosX, a_PosY, a_PosZ);
			// LOGD("Received MoveLook confirmation: {%0.2f %0.2f %0.2f}", a_PosX, a_PosY, a_PosZ);
			
			// Test the distance between points with a small/large enough value instead of comparing directly. Floating point inaccuracies might screw stuff up
			double Dist = (ReceivedPosition - m_ConfirmPosition).SqrLength();
			if (Dist < 1.0)
			{
				if (ReceivedPosition.Equals(m_ConfirmPosition))
				{
					LOGINFO("Exact position confirmed by client!");
				}
				m_State = csPlaying;
			}
			else
			{
				LOGWARNING("Player \"%s\" sent a weird position confirmation %.2f blocks away, retrying", m_Username.c_str(), sqrt(Dist));
				LOGD("  Expected pos: {%0.2f, %0.2f, %0.2f}", m_ConfirmPosition.x, m_ConfirmPosition.y, m_ConfirmPosition.z);
				LOGD("  Received pos: {%0.2f, %0.2f, %0.2f}", a_PosX, a_PosY, a_PosZ);
				m_ConfirmPosition = m_Player->GetPosition();
				SendPlayerMoveLook();
			}
			break;
		}
	}
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
	LOGD("WindowClick: WinID %d, SlotNum %d, IsRclk %d, IsShift %d, Item %s x %d",
		a_WindowID, a_SlotNum, a_IsRightClick, a_IsShiftPressed,
		ItemToString(a_HeldItem).c_str(), a_HeldItem.m_ItemCount
	);
	
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
	World->UpdateSign(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4, m_Player);
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





bool cClientHandle::HandleHandshake(const AString & a_Username)
{
	if (!cRoot::Get()->GetPluginManager()->CallHookHandshake(this, a_Username))
	{
		if (cRoot::Get()->GetDefaultWorld()->GetNumPlayers() >= cRoot::Get()->GetDefaultWorld()->GetMaxPlayers())
		{
			Kick("The server is currently full :(-- Try again later");
			return false;
		}
	}
	return true;
}





void cClientHandle::SendData(const char * a_Data, int a_Size)
{
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
		SendDisconnect("Nooooo!! You timed out! D: Come back!");

		// TODO: Cannot sleep in the tick thread!
		cSleep::MilliSleep(1000);  // Give packet some time to be received

		Destroy();
	}
	
	if ((m_State == csDownloadingWorld) && m_ShouldCheckDownloaded)
	{
		CheckIfWorldDownloaded();
		m_ShouldCheckDownloaded = false;
	}
	
	cTimer t1;
	// Send ping packet
	if (
		(m_Player != NULL) &&  // Is logged in?
		(m_LastPingTime + cClientHandle::PING_TIME_MS <= t1.GetNowTime())
	)
	{
		m_PingID++;
		m_PingStartTime = t1.GetNowTime();
		m_Protocol->SendKeepAlive(m_PingID);
		m_LastPingTime = m_PingStartTime;
	}
}





void cClientHandle::SendDisconnect(const AString & a_Reason)
{
	LOGD("Sending a DC: \"%s\"", a_Reason.c_str());
	m_Protocol->SendDisconnect(a_Reason);
}





void cClientHandle::SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	m_Protocol->SendInventorySlot(a_WindowID, a_SlotNum, a_Item);
}





void cClientHandle::SendChat(const AString & a_Message)
{
	m_Protocol->SendChat(a_Message);
}





void cClientHandle::SendPlayerAnimation(const cPlayer & a_Player, char a_Animation)
{
	m_Protocol->SendPlayerAnimation(a_Player, a_Animation);
}





void cClientHandle::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	m_Protocol->SendEntityEquipment(a_Entity, a_SlotNum, a_Item);
}





void cClientHandle::SendWindowOpen(char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots)
{
	m_Protocol->SendWindowOpen(a_WindowID, a_WindowType, a_WindowTitle, a_NumSlots);
}





void cClientHandle::SendWindowClose(char a_WindowID)
{
	m_Protocol->SendWindowClose(a_WindowID);
}





void cClientHandle::SendWholeInventory(const cInventory & a_Inventory)
{
	m_Protocol->SendWholeInventory(a_Inventory);
}





void cClientHandle::SendWholeInventory(const cWindow & a_Window)
{
	m_Protocol->SendWholeInventory(a_Window);
}





void cClientHandle::SendTeleportEntity(const cEntity & a_Entity)
{
	m_Protocol->SendTeleportEntity(a_Entity);
}





void cClientHandle::SendPlayerListItem(const cPlayer & a_Player, bool a_IsOnline)
{
	m_Protocol->SendPlayerListItem(a_Player, a_IsOnline);
}





void cClientHandle::SendPlayerPosition(void)
{
	m_Protocol->SendPlayerPosition();
}





void cClientHandle::SendEntRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(a_Entity.GetUniqueID() != m_Player->GetUniqueID());  // Must not send for self
	
	m_Protocol->SendEntRelMoveLook(a_Entity, a_RelX, a_RelY, a_RelZ);
}





void cClientHandle::SendEntRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(a_Entity.GetUniqueID() != m_Player->GetUniqueID());  // Must not send for self
	
	m_Protocol->SendEntRelMove(a_Entity, a_RelX, a_RelY, a_RelZ);
}





void cClientHandle::SendEntLook(const cEntity & a_Entity)
{
	ASSERT(a_Entity.GetUniqueID() != m_Player->GetUniqueID());  // Must not send for self
	
	m_Protocol->SendEntLook(a_Entity);
}





void cClientHandle::SendEntHeadLook(const cEntity & a_Entity)
{
	ASSERT(a_Entity.GetUniqueID() != m_Player->GetUniqueID());  // Must not send for self
	
	m_Protocol->SendEntHeadLook(a_Entity);
}





void cClientHandle::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType)
{
	m_Protocol->SendBlockAction(a_BlockX, a_BlockY, a_BlockZ, a_Byte1, a_Byte2, a_BlockType);
}





void cClientHandle::SendHealth(void)
{
	m_Protocol->SendHealth();
}





void cClientHandle::SendRespawn(void)
{
	m_Protocol->SendRespawn();
}





void cClientHandle::SendGameMode(eGameMode a_GameMode)
{
	m_Protocol->SendGameMode(a_GameMode);
}





void cClientHandle::SendDestroyEntity(const cEntity & a_Entity)
{
	m_Protocol->SendDestroyEntity(a_Entity);
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





void cClientHandle::SendEntityStatus(const cEntity & a_Entity, char a_Status)
{
	m_Protocol->SendEntityStatus(a_Entity, a_Status);
}





void cClientHandle::SendMetadata(const cPawn & a_Pawn)
{
	m_Protocol->SendMetadata(a_Pawn);
}





void cClientHandle::SendInventoryProgress(char a_WindowID, short a_ProgressBar, short a_Value)
{
	m_Protocol->SendInventoryProgress(a_WindowID, a_ProgressBar, a_Value);
}





void cClientHandle::SendPlayerSpawn(const cPlayer & a_Player)
{
	if (a_Player.GetUniqueID() == m_Player->GetUniqueID())
	{
		// Do NOT send this packet to myself
		return;
	}
	
	LOG("Spawning player \"%s\" on client \"%s\" @ %s", 
		a_Player.GetName().c_str(), GetPlayer()->GetName().c_str(), GetIPString().c_str()
	);
	
	m_Protocol->SendPlayerSpawn(a_Player);
}





void cClientHandle::SendPickupSpawn(const cPickup & a_Pickup)
{
	m_Protocol->SendPickupSpawn(a_Pickup);
}





void cClientHandle::SendSpawnMob(const cMonster & a_Mob)
{
	m_Protocol->SendSpawnMob(a_Mob);
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





void cClientHandle::SendCollectPickup(const cPickup & a_Pickup, const cPlayer & a_Player)
{
	m_Protocol->SendCollectPickup(a_Pickup, a_Player);
}





void cClientHandle::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	m_Protocol->SendBlockChange(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
}





void cClientHandle::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	m_Protocol->SendBlockChanges(a_ChunkX, a_ChunkZ, a_Changes);
}





void cClientHandle::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	m_Protocol->SendUnloadChunk(a_ChunkX, a_ChunkZ);
}





void cClientHandle::SendWeather(eWeather a_Weather)
{
	m_Protocol->SendWeather(a_Weather);
}





void cClientHandle::SendTimeUpdate(Int64 a_WorldTime)
{
	m_Protocol->SendTimeUpdate(a_WorldTime);
}





void cClientHandle::SendThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	m_Protocol->SendThunderbolt(a_BlockX, a_BlockY, a_BlockZ);
}





void cClientHandle::SendSoundEffect(const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch)
{
	m_Protocol->SendSoundEffect(a_SoundName, a_SrcX, a_SrcY, a_SrcZ, a_Volume, a_Pitch);
}





void cClientHandle::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	// Check chunks being sent, erase them from m_ChunksToSend:
	bool Found = false;
	{
		cCSLock Lock(m_CSChunkLists);
		for (cChunkCoordsList::iterator itr = m_ChunksToSend.begin(); itr != m_ChunksToSend.end(); ++itr)
		{
			if ((itr->m_ChunkX == a_ChunkX) && (itr->m_ChunkZ == a_ChunkZ))
			{
				m_ChunksToSend.erase(itr);
				
				// Make the tick thread check if all the needed chunks have been downloaded
				//   -- needed to offload this from here due to a deadlock possibility
				m_ShouldCheckDownloaded = true;
				
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
		cRoot::Get()->GetServer()->BroadcastChat(m_Username + " joined the game!", this);
	}

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





void cClientHandle::PacketBufferFull(void)
{
	// Too much data in the incoming queue, the server is probably too busy, kick the client:
	LOGERROR("Too much data in queue for client \"%s\" @ %s, kicking them.", m_Username.c_str(), m_Socket.GetIPString().c_str());
	SendDisconnect("Server busy");
	// TODO: QueueDestroy();
	cSleep::MilliSleep(1000); // Give packet some time to be received
	Destroy();
}





void cClientHandle::PacketUnknown(unsigned char a_PacketType)
{
	LOGERROR("Unknown packet type 0x%02x from client \"%s\" @ %s", a_PacketType, m_Username.c_str(), m_Socket.GetIPString().c_str());

	AString Reason;
	Printf(Reason, "[C->S] Unknown PacketID: 0x%02x", a_PacketType);
	SendDisconnect(Reason);
	// TODO: QueueDestroy();
	cSleep::MilliSleep(1000); // Give packet some time to be received
	Destroy();
}





void cClientHandle::PacketError(unsigned char a_PacketType)
{
	LOGERROR("Protocol error while parsing packet type 0x%02x; disconnecting client \"%s\"", a_PacketType, m_Username.c_str());
	SendDisconnect("Protocol error");
	// TODO: QueueDestroy();
	cSleep::MilliSleep(1000); // Give packet some time to be received
	Destroy();
}





void cClientHandle::DataReceived(const char * a_Data, int a_Size)
{
	// Data is received from the client, hand it off to the protocol:
	m_Protocol->DataReceived(a_Data, a_Size);
	m_TimeLastPacket = cWorld::GetTime();
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
	if (m_bKicking && a_Data.empty())
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






