#include "cClientHandle.h"
#include "cServer.h"
#include "cWorld.h"
#include "cChunk.h"
#include "cPickup.h"
#include "cPluginManager.h"
#include "cPlayer.h"
#include "cInventory.h"
#include "cChestEntity.h"
#include "cSignEntity.h"
#include "cMCLogger.h"
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
#include "cThread.h"
#include "cSocket.h"

#include "cTracer.h"
#include "Vector3f.h"
#include "Vector3d.h"

#include "cCriticalSection.h"
#include "cSemaphore.h"
#include "cEvent.h"
#include "cSleep.h"
#include "cRoot.h"

#include "cBlockingTCPLink.h"
#include "cAuthenticator.h"

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
#include "packets/cPacket_CreateInventoryAction.h"
#include "packets/cPacket_NewInvalidState.h"
#include "packets/cPacket_UseEntity.h"
#include "packets/cPacket_WindowClose.h"
#include "packets/cPacket_13.h"
#include "packets/cPacket_UpdateSign.h"
#include "packets/cPacket_Ping.h"
#include "packets/cPacket_PlayerListItem.h"


#ifndef _WIN32
#define sprintf_s(dst, size, format, ...) sprintf(dst, format, __VA_ARGS__ )
#include <stdlib.h> // rand()
#endif

#define AddPistonDir( x, y, z, dir, amount ) switch(dir) { case 0: (y)-=(amount); break; case 1: (y)+=(amount); break;\
													 case 2: (z)-=(amount); break; case 3: (z)+=(amount); break;\
													 case 4: (x)-=(amount); break; case 5: (x)+=(amount); break; }

#define MAX_SEMAPHORES (2000)

typedef std::list<cPacket*> PacketList;

struct cClientHandle::sClientHandleState
{
	sClientHandleState()
		: ProtocolVersion( 0 )
		, pReceiveThread( 0 )
		, pSendThread( 0 )
		, pAuthenticateThread( 0 )
		, pSemaphore( 0 )
	{
		for( int i = 0; i < 256; ++i )
			PacketMap[i] = 0;
	}
	int ProtocolVersion;
	std::string Username;
	std::string Password;

	PacketList PendingParsePackets;
	PacketList PendingNrmSendPackets;
	PacketList PendingLowSendPackets;

	cThread* pReceiveThread;
	cThread* pSendThread;
	cThread* pAuthenticateThread;

	cSocket Socket;

	cCriticalSection CriticalSection;
	cCriticalSection SendCriticalSection;
	cCriticalSection SocketCriticalSection;
	cSemaphore* pSemaphore;

	cPacket* PacketMap[256];
};

cClientHandle::cClientHandle(const cSocket & a_Socket)
	: m_bDestroyed( false )
	, m_Player( 0 )
	, m_bKicking( false )
	, m_TimeLastPacket( cWorld::GetTime() )
	, m_bLoggedIn( false )
	, m_bKeepThreadGoing( true )
	, m_bSendLoginResponse( false )
	, m_pState( new sClientHandleState )
{
    LOG("cClientHandle::cClientHandle");

	m_pState->Socket = a_Socket;

	m_pState->pSemaphore = new cSemaphore( MAX_SEMAPHORES, 0 );

	// All the packets that can be received from the client
	m_pState->PacketMap[E_KEEP_ALIVE]		= new cPacket_KeepAlive;
	m_pState->PacketMap[E_HANDSHAKE]		= new cPacket_Handshake;
	m_pState->PacketMap[E_LOGIN]			= new cPacket_Login;
	m_pState->PacketMap[E_PLAYERPOS]		= new cPacket_PlayerPosition;
	m_pState->PacketMap[E_PLAYERLOOK]		= new cPacket_PlayerLook;
	m_pState->PacketMap[E_PLAYERMOVELOOK]		= new cPacket_PlayerMoveLook;
	m_pState->PacketMap[E_CHAT]			= new cPacket_Chat;
	m_pState->PacketMap[E_ANIMATION]		= new cPacket_ArmAnim;
	m_pState->PacketMap[E_FLYING]			= new cPacket_Flying;
	m_pState->PacketMap[E_BLOCK_DIG]		= new cPacket_BlockDig;
	m_pState->PacketMap[E_BLOCK_PLACE]		= new cPacket_BlockPlace;
	m_pState->PacketMap[E_DISCONNECT]		= new cPacket_Disconnect;
	m_pState->PacketMap[E_ITEM_SWITCH]		= new cPacket_ItemSwitch;
	m_pState->PacketMap[E_ENTITY_EQUIPMENT]		= new cPacket_EntityEquipment;
	m_pState->PacketMap[E_CREATE_INVENTORY_ACTION] 	= new cPacket_CreateInventoryAction;
	m_pState->PacketMap[E_NEW_INVALID_STATE] 	= new cPacket_NewInvalidState;
	m_pState->PacketMap[E_PICKUP_SPAWN]		= new cPacket_PickupSpawn;
	m_pState->PacketMap[E_USE_ENTITY]		= new cPacket_UseEntity;
	m_pState->PacketMap[E_WINDOW_CLOSE]		= new cPacket_WindowClose;
	m_pState->PacketMap[E_WINDOW_CLICK]		= new cPacket_WindowClick;
	m_pState->PacketMap[E_PACKET_13]		= new cPacket_13;
	m_pState->PacketMap[E_UPDATE_SIGN]		= new cPacket_UpdateSign;
	m_pState->PacketMap[E_RESPAWN]			= new cPacket_Respawn;
	m_pState->PacketMap[E_PING]			= new cPacket_Ping;

	memset( m_LoadedChunks, 0x00, sizeof(cChunk*)*VIEWDISTANCE*VIEWDISTANCE );

	//////////////////////////////////////////////////////////////////////////
	m_pState->pReceiveThread = new cThread( ReceiveThread, this, "cClientHandle::ReceiveThread" );
	m_pState->pSendThread = new cThread( SendThread, this, "cClientHandle::SendThread" );
	m_pState->pReceiveThread->Start( true );
	m_pState->pSendThread->Start( true );
	//////////////////////////////////////////////////////////////////////////

	LOG("New ClientHandle" );
}

cClientHandle::~cClientHandle()
{
	LOG("Deleting client %s", GetUsername() );

	for(unsigned int i = 0; i < VIEWDISTANCE*VIEWDISTANCE; i++)
	{
		if( m_LoadedChunks[i] ) m_LoadedChunks[i]->RemoveClient( this );
	}

	// First stop sending thread
	m_bKeepThreadGoing = false;

	m_pState->SocketCriticalSection.Lock();
	if( m_pState->Socket )
	{
		cPacket_Disconnect Disconnect;
		Disconnect.m_Reason = "Server shut down? Kthnxbai";
		Disconnect.Send( m_pState->Socket );

		m_pState->Socket.CloseSocket();
		m_pState->Socket = 0;
	}
	m_pState->SocketCriticalSection.Unlock();

	m_pState->pSemaphore->Signal();
	delete m_pState->pAuthenticateThread;
	delete m_pState->pReceiveThread;
	delete m_pState->pSendThread;
	delete m_pState->pSemaphore;

	while( !m_pState->PendingParsePackets.empty() )
	{
		delete *m_pState->PendingParsePackets.begin();
		m_pState->PendingParsePackets.erase( m_pState->PendingParsePackets.begin() );
	}
	while( !m_pState->PendingNrmSendPackets.empty() )
	{
		delete *m_pState->PendingNrmSendPackets.begin();
		m_pState->PendingNrmSendPackets.erase( m_pState->PendingNrmSendPackets.begin() );
	}
	if(m_Player)
	{
		m_Player->SetClientHandle( 0 );
 		m_Player->Destroy();
 		m_Player = 0;
 	}
	for(int i = 0; i < 256; i++)
	{
		if( m_pState->PacketMap[i] )
			delete m_pState->PacketMap[i];
	}

	delete m_pState;
}

void cClientHandle::Destroy()
{
	 m_bDestroyed = true;
	 m_pState->SocketCriticalSection.Lock();
	 if( m_pState->Socket )
	 {
		 m_pState->Socket.CloseSocket();
		 m_pState->Socket = 0;
	 }
	 m_pState->SocketCriticalSection.Unlock();
}

void cClientHandle::Kick( const char* a_Reason )
{
	Send( cPacket_Disconnect( a_Reason ) );
	m_bKicking = true;
}

void cClientHandle::StreamChunks()
{
	if( !m_bLoggedIn )
		return;

	int ChunkPosX = (int)floor(m_Player->GetPosX() / 16);
	int ChunkPosZ = (int)floor(m_Player->GetPosZ() / 16);

	cWorld* World = m_Player->GetWorld();

	cChunk* NeededChunks[VIEWDISTANCE*VIEWDISTANCE] = { 0 };
	const int MaxDist = VIEWDISTANCE+GENERATEDISTANCE*2;
	for(int x = 0; x < MaxDist; x++)
	{
		for(int z = 0; z < MaxDist; z++)
		{
			int RelX = x - (MaxDist-1)/2;
			int RelZ = z - (MaxDist-1)/2;
			cChunk* Chunk = World->GetChunk( ChunkPosX + RelX, 0, ChunkPosZ + RelZ );	// Touch all chunks in wide range, so they get generated
			if( x >= GENERATEDISTANCE && x < VIEWDISTANCE+GENERATEDISTANCE && z >= GENERATEDISTANCE && z < VIEWDISTANCE+GENERATEDISTANCE ) // but player only needs chunks in view distance
				NeededChunks[(x-GENERATEDISTANCE) + (z-GENERATEDISTANCE)*VIEWDISTANCE] = Chunk;
		}
	}

	cChunk* MissingChunks[VIEWDISTANCE*VIEWDISTANCE];
	memset( MissingChunks, 0, VIEWDISTANCE*VIEWDISTANCE*sizeof(cChunk*) );
	unsigned int MissIndex = 0;
	for(int i = 0; i < VIEWDISTANCE*VIEWDISTANCE; i++)	// Handshake loop - touch each chunk once
	{
		if( NeededChunks[i] == 0 ) continue;	// Chunk is not yet loaded, so ignore
												// This can cause MissIndex to be 0 and thus chunks will not be unloaded while they are actually out of range,
												// which might actually be a good thing, otherwise it would keep trying to unload chunks until the new chunks are fully loaded
		bool bChunkMissing = true;
		for(int j = 0; j < VIEWDISTANCE*VIEWDISTANCE; j++)
		{
			if( m_LoadedChunks[j] == NeededChunks[i] )
			{
				bChunkMissing = false;
				break;
			}
		}
		if(bChunkMissing)
		{
			MissingChunks[MissIndex] = NeededChunks[i];
			MissIndex++;
		}
	}

	if( MissIndex > 0 )
	{	// Chunks are gonna be streamed in, so chunks probably also need to be streamed out <- optimization
		for(int x = 0; x < VIEWDISTANCE; x++)
		{
			for(int z = 0; z < VIEWDISTANCE; z++)
			{
				cChunk* Chunk = m_LoadedChunks[x + z*VIEWDISTANCE];
				if( Chunk )
				{
					if( Chunk->GetPosX() < ChunkPosX-(VIEWDISTANCE-1)/2
						|| Chunk->GetPosX() > ChunkPosX+(VIEWDISTANCE-1)/2
						|| Chunk->GetPosZ() < ChunkPosZ-(VIEWDISTANCE-1)/2
						|| Chunk->GetPosZ() > ChunkPosZ+(VIEWDISTANCE-1)/2 )
					{
						Chunk->RemoveClient( this );
						Chunk->AsyncUnload( this );		// TODO - I think it's possible to unload the chunk immediately instead of next tick
														// I forgot why I made it happen next tick

						m_LoadedChunks[x + z*VIEWDISTANCE] = 0;
					}
				}
			}
		}

		StreamChunksSmart( MissingChunks, MissIndex );

		memcpy( m_LoadedChunks, NeededChunks, VIEWDISTANCE*VIEWDISTANCE*sizeof(cChunk*) );
	}
}

// Sends chunks to the player from the player position outward
void cClientHandle::StreamChunksSmart( cChunk** a_Chunks, unsigned int a_NumChunks )
{
	int X = (int)floor(m_Player->GetPosX() / 16);
	int Y = (int)floor(m_Player->GetPosY() / 128);
	int Z = (int)floor(m_Player->GetPosZ() / 16);

	bool bAllDone = false;
	while( !bAllDone )
	{
		bAllDone = true;
		int ClosestIdx = -1;
		unsigned int ClosestSqrDist = (unsigned int)-1; // wraps around, becomes biggest number possible
		for(unsigned int i = 0; i < a_NumChunks; ++i)
		{
			if( a_Chunks[i] )
			{
				bAllDone = false;
				int DistX = a_Chunks[i]->GetPosX()-X;
				int DistY = a_Chunks[i]->GetPosY()-Y;
				int DistZ = a_Chunks[i]->GetPosZ()-Z;
				unsigned int SqrDist = (DistX*DistX)+(DistY*DistY)+(DistZ*DistZ);
				if( SqrDist < ClosestSqrDist )
				{
					ClosestSqrDist = SqrDist;
					ClosestIdx = i;
				}
			}
		}
		if(ClosestIdx > -1)
		{
			a_Chunks[ClosestIdx]->Send( this );
			a_Chunks[ClosestIdx]->AddClient( this );
			a_Chunks[ClosestIdx] = 0;
		}
	}
}

void cClientHandle::AddPacket(cPacket * a_Packet)
{
	m_pState->CriticalSection.Lock();
	m_pState->PendingParsePackets.push_back( a_Packet->Clone() );
	m_pState->CriticalSection.Unlock();
}

void cClientHandle::RemovePacket( cPacket * a_Packet )
{
	delete a_Packet;
	m_pState->PendingParsePackets.remove( a_Packet );
}

void cClientHandle::HandlePendingPackets()
{
	m_pState->CriticalSection.Lock();
	while( m_pState->PendingParsePackets.begin() != m_pState->PendingParsePackets.end() )
	{
		HandlePacket( *m_pState->PendingParsePackets.begin() );
		RemovePacket( *m_pState->PendingParsePackets.begin() );
	}
	m_pState->CriticalSection.Unlock();
}

void cClientHandle::HandlePacket( cPacket* a_Packet )
{
	m_TimeLastPacket = cWorld::GetTime();

// 	cPacket* CopiedPacket = a_Packet->Clone();
// 	a_Packet = CopiedPacket;

	//LOG("Packet: 0x%02x", a_Packet->m_PacketID );

	if( m_bKicking ) return;

	if(!m_bLoggedIn)
	{
		switch( a_Packet->m_PacketID )
		{
		case E_NEW_INVALID_STATE: // New/Invalid State packet received. I'm guessing the client only sends it when there's a problem with the bed?
                        {
                                LOGINFO("Got New Invalid State packet");
                        }
                        break;
		case E_CREATE_INVENTORY_ACTION: // I don't think we need to do anything with this packet, but justin case...
                        {
                                LOGINFO("Got Create Inventory Action packet");
                        }
                        break;
		case E_PING: // Somebody tries to retrieve information about the server
			{
				LOGINFO("Got ping");
				char NumPlayers[8], cMaxPlayers[8];
				sprintf_s(NumPlayers, 8, "%i", cRoot::Get()->GetWorld()->GetNumPlayers());
				sprintf_s(cMaxPlayers, 8, "%i", cRoot::Get()->GetWorld()->GetMaxPlayers());
				std::string response = std::string(cRoot::Get()->GetWorld()->GetDescription() + cChatColor::Delimiter + NumPlayers + cChatColor::Delimiter + cMaxPlayers );
				Kick( response.c_str() );
			}
			break;
		case E_HANDSHAKE:
			{
				cPacket_Handshake* PacketData = reinterpret_cast<cPacket_Handshake*>(a_Packet);
				m_pState->Username = PacketData->m_Username;
				LOG("HANDSHAKE %s", GetUsername() );
				cPacket_Chat Connecting(m_pState->Username + " is connecting.");

				if (cRoot::Get()->GetWorld()->GetNumPlayers() == cRoot::Get()->GetWorld()->GetMaxPlayers()) {
					Kick("The server is currently full :( -- Try again later");
					break;
				}
				cRoot::Get()->GetServer()->Broadcast( Connecting, this );

				// Give a server handshake thingy back
				cPacket_Handshake Handshake;
				Handshake.m_Username = cRoot::Get()->GetServer()->GetServerID();//ServerID;//"2e66f1dc032ab5f0";
				Send( Handshake );
			}
			break;
		case E_LOGIN:
			{
				LOG("LOGIN %s", GetUsername() );
				cPacket_Login* PacketData = reinterpret_cast<cPacket_Login*>(a_Packet);
				if( m_pState->Username.compare( PacketData->m_Username ) != 0 )
				{
					Kick("Login Username does not match Handshake username!");
					return;
				}
				//m_Password = PacketData->m_Password;

				if( cRoot::Get()->GetPluginManager()->CallHook( cPluginManager::E_PLUGIN_LOGIN, 1, PacketData ) )
				{
					Destroy();
					return;
				}

				if( m_pState->pAuthenticateThread ) delete m_pState->pAuthenticateThread;
				m_pState->pAuthenticateThread = new cThread( AuthenticateThread, this, "cClientHandle::AuthenticateThread" );
				m_pState->pAuthenticateThread->Start( true );
			}
			break;
		case E_PLAYERMOVELOOK:	// After this is received we're safe to send anything
			{
				if( !m_Player )
				{
					Kick("Received wrong packet! Check your login sequence!");
					return;
				}
				if( !cRoot::Get()->GetPluginManager()->CallHook( cPluginManager::E_PLUGIN_PLAYER_JOIN, 1, m_Player ) )
				{
					// Broadcast that this player has joined the game! Yay~
					cPacket_Chat Joined( m_pState->Username + " joined the game!");
					cRoot::Get()->GetServer()->Broadcast( Joined, this );
				}
				int posx = (int) m_Player->GetPosX();
				int posy = (int) m_Player->GetPosY();
				int posz = (int) m_Player->GetPosZ();
				// Now initialize player (adds to entity list etc.)
				cWorld* PlayerWorld = cRoot::Get()->GetWorld( m_Player->GetLoadedWorldName() );
				if( !PlayerWorld ) PlayerWorld = cRoot::Get()->GetDefaultWorld();
				m_Player->Initialize( PlayerWorld ); // TODO - Get correct world for player

				// Broadcasts to all but this ( this is actually handled in cChunk.cpp, after entity is added to the chunk )
				//m_Player->SpawnOn( 0 );

				// Send all already connected players to new player
				//cRoot::Get()->GetServer()->SendAllEntitiesTo( this );

				// Then we can start doing more stuffs! :D
				m_bLoggedIn = true;
				LOG("%s completely logged in", GetUsername() );
				m_Player->TeleportTo( posx, posy, posz );
				StreamChunks();
			}
			break;
		case E_KEEP_ALIVE:
			break;
		default:
			{
				LOG("INVALID RESPONSE FOR LOGIN: needed 0x%02x got 0x%02x", E_PLAYERMOVELOOK, a_Packet->m_PacketID );
				Kick("INVALID RESPONSE FOR LOGIN: needed 0x0d!");
			}
			break;
		}
	}
	else // m_bLoggedIn == true
	{
		switch( a_Packet->m_PacketID )
		{
		case E_PLAYERPOS:
			{
				cPacket_PlayerPosition* PacketData = reinterpret_cast<cPacket_PlayerPosition*>(a_Packet);
				//LOG("recv player pos: %0.2f %0.2f %0.2f", PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ );
				m_Player->MoveTo( Vector3d( PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ ) );
				m_Player->SetStance( PacketData->m_Stance );
				m_Player->SetTouchGround( PacketData->m_bFlying );
			}
			break;
		case E_BLOCK_DIG:
			{
				
				int LastActionCnt = m_Player->GetLastBlockActionCnt();
				if ( cRoot::Get()->GetWorld()->GetTime() - m_Player->GetLastBlockActionTime() < 0.1 ) { //only allow block interactions every 0.1 seconds
					m_Player->SetLastBlockActionTime(); //Player tried to interact with a block. Reset last block interation time.
					m_Player->SetLastBlockActionCnt(LastActionCnt+1);
					if (LastActionCnt > 3) { //kick if more than 3 interactions per .1 seconds
						LOGWARN("Player %s tried to interact with a block too quickly! (could indicate bot) Was Kicked.", GetUsername() );
						//TODO Too many false-positives :s for example on a minimal server lagg :s should be re checked
						Kick("You're a baaaaaad boy!");
						break;
					}
				} else {
					m_Player->SetLastBlockActionCnt(0); //reset count 
					m_Player->SetLastBlockActionTime(); //Player tried to interact with a block. Reset last block interation time.
				}

				cPacket_BlockDig* PacketData = reinterpret_cast<cPacket_BlockDig*>(a_Packet);
				LOG("OnBlockDig: %i %i %i Dir: %i Stat: %i", PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ, PacketData->m_Direction, PacketData->m_Status );
				if( PacketData->m_Status == 0x04 )	// Drop block
				{
					m_Player->TossItem( false );
				}
				else
				{
					cWorld* World = m_Player->GetWorld();
					char OldBlock = World->GetBlock(PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ);
					char MetaData = World->GetBlockMeta(PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ);
					bool bBroken = (PacketData->m_Status == 0x02) || g_BlockOneHitDig[(int)OldBlock] || ( (PacketData->m_Status == 0x00) && (m_Player->GetGameMode() == 1) );
					if(bBroken == false) bBroken = (m_Player->GetInventory().GetEquippedItem().m_ItemID == E_ITEM_SHEARS && OldBlock == E_BLOCK_LEAVES);

					if(OldBlock == E_BLOCK_WOODEN_DOOR && !bBroken)
					{
						cDoors::ChangeDoor(m_Player->GetWorld(), PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ);
					}

					cItem PickupItem;
					if( bBroken && !(m_Player->GetGameMode() == 1) ) // broken
					{
						
						ENUM_ITEM_ID PickupID = cBlockToPickup::ToPickup( (ENUM_BLOCK_ID)OldBlock, m_Player->GetInventory().GetEquippedItem().m_ItemID );
						PickupItem.m_ItemID = PickupID;
						PickupItem.m_ItemHealth = MetaData;
						PickupItem.m_ItemCount = cBlockToPickup::PickupCount(OldBlock);
						if( OldBlock == E_BLOCK_LAPIS_ORE )
						{
							PickupItem.m_ItemHealth = 4;
						}
						if(cDoors::IsDoor(OldBlock))
						{
							PickupItem.m_ItemHealth = 1;	//For a complete door this works :D
						}
					}
					if(!cRoot::Get()->GetPluginManager()->CallHook( cPluginManager::E_PLUGIN_BLOCK_DIG, 2, PacketData, m_Player, &PickupItem ) )
					{
						if( bBroken ) // Block broken
						{
							if( World->DigBlock( PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ, PickupItem ) )
							{
								if (OldBlock == E_BLOCK_REDSTONE_TORCH_ON) {
									cRedstone Redstone(World);
									Redstone.ChangeRedstone( PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ, false );
								}
								if (OldBlock == E_BLOCK_REDSTONE_TORCH_OFF) {
									cRedstone Redstone(World);
									Redstone.ChangeRedstone( PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ, false );
								}
								if (OldBlock == E_BLOCK_REDSTONE_WIRE) {
									cRedstone Redstone(World);
									Redstone.ChangeRedstone( PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ, false );
								}
								if (( OldBlock == E_BLOCK_PISTON) || (OldBlock == E_BLOCK_STICKY_PISTON) ) {
									int newX = PacketData->m_PosX;
									int newY = PacketData->m_PosY;
									int newZ = PacketData->m_PosZ;
									AddPistonDir( newX, newY, newZ, MetaData & ~(8), 1 )
									if (World->GetBlock(newX, newY, newZ) == E_BLOCK_PISTON_EXTENSION) {
										World->SetBlock( newX, newY, newZ, E_BLOCK_AIR, 0 );
									}
								}

								if(cDoors::IsDoor(OldBlock))	//Special actions for destroyed door (Destroy second part)
								{
									if(MetaData & 8)	//was Upper part of door
									{
										if(cDoors::IsDoor(World->GetBlock(PacketData->m_PosX, PacketData->m_PosY - 1, PacketData->m_PosZ)))
											World->SetBlock(PacketData->m_PosX, PacketData->m_PosY - 1, PacketData->m_PosZ, E_BLOCK_AIR, 0);
									}else{	//Was lower part
										if(cDoors::IsDoor(World->GetBlock(PacketData->m_PosX, PacketData->m_PosY + 1, PacketData->m_PosZ)))
											World->SetBlock(PacketData->m_PosX, PacketData->m_PosY + 1, PacketData->m_PosZ, E_BLOCK_AIR, 0);
									}
								}

								int helditem = m_Player->GetInventory().GetEquippedItem().m_ItemID;
								bool itemhasdur = false;
								switch(helditem)
								{
									case 256 : itemhasdur = true; break;
									case 257 : itemhasdur = true; break;
									case 258 : itemhasdur = true; break;
									case 267 : itemhasdur = true; break;
									case 268 : itemhasdur = true; break;
									case 269 : itemhasdur = true; break;
									case 270 : itemhasdur = true; break;
									case 271 : itemhasdur = true; break;
									case 272 : itemhasdur = true; break;
									case 273 : itemhasdur = true; break;
									case 274 : itemhasdur = true; break;
									case 275 : itemhasdur = true; break;
									case 276 : itemhasdur = true; break;
									case 277 : itemhasdur = true; break;
									case 278 : itemhasdur = true; break;
									case 279 : itemhasdur = true; break;
									case 283 : itemhasdur = true; break;
									case 284 : itemhasdur = true; break;
									case 285 : itemhasdur = true; break;
									case 286 : itemhasdur = true; break;
									case 290 : itemhasdur = true; break;
									case 291 : itemhasdur = true; break;
									case 292 : itemhasdur = true; break;
									case 293 : itemhasdur = true; break;
									case 294 : itemhasdur = true; break;
									case 359 : itemhasdur = true; break;
								}
								if (itemhasdur) 
								{
									int maxhelditemdur = 1563;
									switch(helditem)
									{
										case 256 : maxhelditemdur = 251; break;
										case 257 : maxhelditemdur = 251; break;
										case 258 : maxhelditemdur = 251; break;
										case 267 : maxhelditemdur = 251; break;
										case 268 : maxhelditemdur = 60; break;
										case 269 : maxhelditemdur = 60; break;
										case 270 : maxhelditemdur = 60; break;
										case 271 : maxhelditemdur = 60; break;
										case 272 : maxhelditemdur = 132; break;
										case 273 : maxhelditemdur = 132; break;
										case 274 : maxhelditemdur = 132; break;
										case 275 : maxhelditemdur = 132; break;
										case 276 : maxhelditemdur = 1563; break;
										case 277 : maxhelditemdur = 1563; break;
										case 278 : maxhelditemdur = 1563; break;
										case 279 : maxhelditemdur = 1563; break;
										case 283 : maxhelditemdur = 32; break;
										case 284 : maxhelditemdur = 32; break;
										case 285 : maxhelditemdur = 32; break;
										case 286 : maxhelditemdur = 32; break;
										case 290 : maxhelditemdur = 60; break;
										case 291 : maxhelditemdur = 132; break;
										case 292 : maxhelditemdur = 251; break;
										case 293 : maxhelditemdur = 1563; break;
										case 294 : maxhelditemdur = 32; break;
										case 359 : maxhelditemdur = 251; break;
									}
									m_Player->GetInventory().GetEquippedItem().m_ItemHealth ++;
									LOG("Health: %i", m_Player->GetInventory().GetEquippedItem().m_ItemHealth);
									if (m_Player->GetInventory().GetEquippedItem().m_ItemHealth >= maxhelditemdur)
									{
										LOG("Player %s Broke ID: %i", GetUsername(), m_Player->GetInventory().GetEquippedItem().m_ItemID);
										m_Player->GetInventory().RemoveItem( m_Player->GetInventory().GetEquippedItem());
									}
								}
							}
						}
					}
					else
					{
						World->SendBlockTo( PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ, m_Player );
					}
				}
			}
			break;
		case E_BLOCK_PLACE:
			{

				int LastActionCnt = m_Player->GetLastBlockActionCnt();
				if ( cRoot::Get()->GetWorld()->GetTime() - m_Player->GetLastBlockActionTime() < 0.1 ) { //only allow block interactions every 0.1 seconds
					m_Player->SetLastBlockActionTime(); //Player tried to interact with a block. Reset last block interation time.
					m_Player->SetLastBlockActionCnt(LastActionCnt+1);
					if (LastActionCnt > 3) { //kick if more than 3 interactions per .1 seconds
						LOGWARN("Player %s tried to interact with a block too quickly! (could indicate bot) Was Kicked.", GetUsername() );
						Kick("You're a baaaaaad boy!");
						break;
					}
				} else {
					m_Player->SetLastBlockActionCnt(0); //reset count 
					m_Player->SetLastBlockActionTime(); //Player tried to interact with a block. Reset last block interation time.
				}

				cPacket_BlockPlace* PacketData = reinterpret_cast<cPacket_BlockPlace*>(a_Packet);
				cItem & Equipped = m_Player->GetInventory().GetEquippedItem();
				//if( (Equipped.m_ItemID != PacketData->m_ItemType) )	// Not valid

				if( (Equipped.m_ItemID != PacketData->m_ItemType) && (m_Player->GetGameMode() != 1) )	// Not valid
				{
					LOGWARN("Player %s tried to place a block that was not selected! (could indicate bot)", GetUsername() );
					break;
				}

				if(cRoot::Get()->GetPluginManager()->CallHook( cPluginManager::E_PLUGIN_BLOCK_PLACE, 2, PacketData, m_Player ) )
				{
					if( PacketData->m_Direction > -1 )
					{
						AddDirection( PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ, PacketData->m_Direction );
						m_Player->GetWorld()->SendBlockTo( PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ, m_Player );
					}
					break;
				}

				//LOG("%i %i %i %i %i %i", PacketData->m_Count, PacketData->m_Direction, PacketData->m_ItemType, PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ );

				//printf("Place Dir:%i %i %i %i : %i\n", PacketData->m_Direction, PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ, PacketData->m_ItemType);
				// 'use' useable items instead of placing blocks
				bool bPlaceBlock = true;
				bool UpdateRedstone = false;
				bool AddedCurrent = false;

				if( PacketData->m_Direction >= 0 )
				{

					ENUM_BLOCK_ID BlockID = (ENUM_BLOCK_ID)m_Player->GetWorld()->GetBlock( PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ );
					switch( BlockID )
					{
					case E_BLOCK_REDSTONE_REPEATER_ON:
					case E_BLOCK_REDSTONE_REPEATER_OFF:
						{
							//no need to update redstone current with a repeater
							//todo: Find meta value of repeater and change it to one step more.
						}
						break;
					case E_BLOCK_WORKBENCH:
						{
							bPlaceBlock = false;
							cWindow* Window = new cCraftingWindow( 0, true );
							m_Player->OpenWindow( Window );
						}
						break;
					case E_BLOCK_FURNACE:
					case E_BLOCK_CHEST:
						{
							bPlaceBlock = false;
							cBlockEntity* BlockEntity = m_Player->GetWorld()->GetBlockEntity( PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ );
							if( BlockEntity )
							{
								BlockEntity->UsedBy( *m_Player );
							}
						}
						break;
					case E_BLOCK_WOODEN_DOOR:
						{
							bPlaceBlock = false;
							cDoors::ChangeDoor(m_Player->GetWorld(), PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ);
						}
						break;
					default:
						break;
					};
				}

				// Some checks to see if it's a placeable item :P
				if( bPlaceBlock )
				{
					cItem Item;
					Item.m_ItemID = Equipped.m_ItemID;
					Item.m_ItemCount = 1;
					LOG("PacketData->m_ItemType: %i", (int)PacketData->m_ItemType);
					// Hacked in edible items go!~
					bool bEat = false;
					bool isDoor = false;
					switch( Item.m_ItemID )
					{
					case E_ITEM_APPLE:
						m_Player->Heal( 4 ); // 2 hearts
						bEat = true;
						break;
					case E_ITEM_GOLDEN_APPLE:
						m_Player->Heal( 20 ); // 10 hearts
						bEat = true;
						break;
					case E_ITEM_MUSHROOM_SOUP:
						m_Player->Heal( 10 ); // 5 hearts
						bEat = true;
						break;
					case E_ITEM_BREAD:
						m_Player->Heal( 5 ); // 2.5 hearts
						bEat = true;
						break;
					case E_ITEM_RAW_MEAT:
						m_Player->Heal( 3 ); // 1.5 hearts
						bEat = true;
						break;
					case E_ITEM_COOKED_MEAT:
						m_Player->Heal( 8 ); // 4 hearts
						bEat = true;
						break;
					case E_ITEM_RAW_FISH:
						m_Player->Heal( 2 ); // 1 heart
						bEat = true;
						break;
					case E_ITEM_COOKED_FISH:
						m_Player->Heal( 5 ); // 2.5 hearts
						bEat = true;
						break;
					default:
						break;
					};

					if( bEat )
					{
						m_Player->GetInventory().RemoveItem( Item );
						break;
					}

					if( PacketData->m_Direction < 0 ) // clicked in air
						break;

					//TODO: Wrong Blocks!
					int clickedBlock = (int)m_Player->GetWorld()->GetBlock( PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ );
					char MetaData = (char)Equipped.m_ItemHealth;
					bool LavaBucket = false;
					bool WaterBucket = false;

					switch( PacketData->m_ItemType )	// Special handling for special items
					{
					case E_ITEM_BUCKET:
						//TODO: Change this, it is just a small hack to get it working a little bit. seems like the Client sends the position from the first hitable block :s
						clickedBlock = (int)m_Player->GetWorld()->GetBlock( PacketData->m_PosX, PacketData->m_PosY + 1, PacketData->m_PosZ );
						LOG("Bucket Clicked BlockID: %d", clickedBlock);
						switch (clickedBlock)
						{
							case E_BLOCK_WATER:
							case E_BLOCK_STATIONARY_WATER:
								WaterBucket = true;
								break;
							case E_BLOCK_LAVA:
							case E_BLOCK_STATIONARY_LAVA:
								LavaBucket = true;
								break;
						}
						break;
					case E_ITEM_LAVA_BUCKET:
						if((m_Player->GetGameMode() == 1) || (m_Player->GetInventory().RemoveItem( Item )))
						{
							cItem NewItem;
							NewItem.m_ItemID = E_ITEM_BUCKET;
							NewItem.m_ItemCount = 1;
							m_Player->GetInventory().AddItem( NewItem );
							PacketData->m_ItemType = E_BLOCK_LAVA;
						}
						break;
					case E_ITEM_WATER_BUCKET:
						if((m_Player->GetGameMode() == 1) || (m_Player->GetInventory().RemoveItem( Item )))
						{
							cItem NewItem;
							NewItem.m_ItemID = E_ITEM_BUCKET;
							NewItem.m_ItemCount = 1;
							m_Player->GetInventory().AddItem( NewItem );
							PacketData->m_ItemType = E_BLOCK_WATER;
						}
						break;
					case E_BLOCK_WHITE_CLOTH:
						MetaData = (char)PacketData->m_Uses;
						break;
					case E_BLOCK_TORCH:
						MetaData = cTorch::DirectionToMetaData( PacketData->m_Direction );
						break;
					case E_BLOCK_REDSTONE_TORCH_OFF:
						{
						MetaData = cTorch::DirectionToMetaData( PacketData->m_Direction );
						if( g_BlockTransparent[ (int)m_Player->GetWorld()->GetBlock( PacketData->m_PosX, PacketData->m_PosY+2, PacketData->m_PosZ ) ] == true ) {//if block above is transparent
							//printf("transparent above me\n");
						} else {
							//printf("transparent not above me\n");
						}
						UpdateRedstone = true;
						AddedCurrent = false;
						break;
						}
					case E_BLOCK_REDSTONE_TORCH_ON:
						{
						MetaData = cTorch::DirectionToMetaData( PacketData->m_Direction );
						UpdateRedstone = true;
						AddedCurrent = true;
						break;
						}
					case E_ITEM_REDSTONE_DUST:
						MetaData = 0;
						PacketData->m_ItemType = E_BLOCK_REDSTONE_WIRE;
						UpdateRedstone = true;
						AddedCurrent = false;
						break;
					case E_ITEM_REDSTONE_REPEATER:
						MetaData = cRedstone::RepeaterRotationToMetaData( m_Player->GetRotation() );
						PacketData->m_ItemType = E_BLOCK_REDSTONE_REPEATER_OFF;
						UpdateRedstone = true;
						AddedCurrent = false;
						break;
					case E_BLOCK_PISTON:
					case E_BLOCK_STICKY_PISTON:
						MetaData = cPiston::RotationPitchToMetaData( m_Player->GetRotation(), m_Player->GetPitch() );
						UpdateRedstone = true;
						AddedCurrent = false;
						break;
					case E_ITEM_IRON_DOOR:
						{
							PacketData->m_ItemType = E_BLOCK_IRON_DOOR;
							MetaData = cDoors::RotationToMetaData( m_Player->GetRotation() );
							isDoor = true;
						}
						break;
					case E_ITEM_WOODEN_DOOR:
						{
							PacketData->m_ItemType = E_BLOCK_WOODEN_DOOR;
							MetaData = cDoors::RotationToMetaData( m_Player->GetRotation() );
							isDoor = true;
						}
						break;
					case E_BLOCK_COBBLESTONE_STAIRS:
					case E_BLOCK_BRICK_STAIRS:
					case E_BLOCK_STONE_BRICK_STAIRS:
					case E_BLOCK_NETHER_BRICK_STAIRS:
					case E_BLOCK_WOODEN_STAIRS:
						MetaData = cStairs::RotationToMetaData( m_Player->GetRotation() );
						break;
					case E_BLOCK_LADDER:
						MetaData = cLadder::DirectionToMetaData( PacketData->m_Direction );
						break;
					case E_ITEM_SIGN:
						LOG("Dir: %i", PacketData->m_Direction);
						if( PacketData->m_Direction == 1 )
						{
						    LOG("Player Rotation: %f", m_Player->GetRotation() );
							MetaData = cSign::RotationToMetaData( m_Player->GetRotation() );
							LOG("Sign rotation %i", MetaData);
							PacketData->m_ItemType = E_BLOCK_SIGN_POST;
						}
						else
						{
							MetaData = cSign::DirectionToMetaData( PacketData->m_Direction );
							PacketData->m_ItemType = E_BLOCK_WALLSIGN;
						}
						break;
					default:
						break;
					};


					if (LavaBucket) {

						if( (m_Player->GetGameMode() == 1) || (m_Player->GetInventory().RemoveItem( Item )) ) {
							cItem NewItem;
							NewItem.m_ItemID = E_ITEM_LAVA_BUCKET;
							NewItem.m_ItemCount = 1;
							m_Player->GetInventory().AddItem( NewItem );
							m_Player->GetWorld()->SetBlock( PacketData->m_PosX, PacketData->m_PosY + 1, PacketData->m_PosZ, E_BLOCK_AIR, 0 );
						}

					} else if (WaterBucket) {

						if( (m_Player->GetGameMode() == 1) || (m_Player->GetInventory().RemoveItem( Item )) ) {
							cItem NewItem;
							NewItem.m_ItemID = E_ITEM_WATER_BUCKET;
							NewItem.m_ItemCount = 1;
							m_Player->GetInventory().AddItem( NewItem );
							m_Player->GetWorld()->SetBlock( PacketData->m_PosX, PacketData->m_PosY + 1, PacketData->m_PosZ, E_BLOCK_AIR, 0 );
						}

					} else if( IsValidBlock( PacketData->m_ItemType) ) {
						int X = PacketData->m_PosX;
						char Y = PacketData->m_PosY;
						int Z = PacketData->m_PosZ;
						AddDirection( X, Y, Z, PacketData->m_Direction );

						int PlaceBlock = m_Player->GetWorld()->GetBlock( X, Y, Z );
						if (!( ( PlaceBlock == E_BLOCK_AIR ) || ( PlaceBlock == E_BLOCK_WATER ) || ( PlaceBlock == E_BLOCK_STATIONARY_WATER ) || ( PlaceBlock == E_BLOCK_LAVA ) || ( PlaceBlock == E_BLOCK_STATIONARY_LAVA ) ) ) { //tried to place a block *into* another?
							break;	//happens when you place a block aiming at side of block like torch or stem
						}

						if( (m_Player->GetGameMode() == 1) || (m_Player->GetInventory().RemoveItem( Item )) )
						{
							if (isDoor) {
								if ( ( m_Player->GetWorld()->GetBlock( X, Y+1, Z ) == E_BLOCK_AIR ) || ( m_Player->GetWorld()->GetBlock( X, Y+1, Z ) == E_BLOCK_AIR ) ) {
									m_Player->GetWorld()->SetBlock( X, Y+1, Z, (char)PacketData->m_ItemType, MetaData + 8 );

									m_Player->GetWorld()->SetBlock( X, Y, Z, (char)PacketData->m_ItemType, MetaData );
								}
							} else {
								m_Player->GetWorld()->SetBlock( X, Y, Z, (char)PacketData->m_ItemType, MetaData );
							}
							if (UpdateRedstone) {
								cRedstone Redstone(m_Player->GetWorld());
								Redstone.ChangeRedstone( PacketData->m_PosX, PacketData->m_PosY+1, PacketData->m_PosZ, AddedCurrent );
							}

						}
					}
				}
				/*
				// Remove stuff with stick! :D
				if( m_Username.compare("FakeTruth") == 0 )
				{	// It's me! :D
					if( PacketData->m_ItemType == 280 )
					{
						cRoot::Get()->GetWorld()->SetBlock( PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ, 0, 0 );
					}
				}
				*/
			}
			break;
		case E_PICKUP_SPAWN:
			{
				LOG("Received packet E_PICKUP_SPAWN");
				cPacket_PickupSpawn* PacketData = reinterpret_cast<cPacket_PickupSpawn*>(a_Packet);

				cItem DroppedItem;
				DroppedItem.m_ItemID = (ENUM_ITEM_ID)PacketData->m_Item;
				DroppedItem.m_ItemCount = PacketData->m_Count;
				DroppedItem.m_ItemHealth = 0x0; // TODO: Somehow figure out what item was dropped, and apply correct health
				if( m_Player->GetInventory().RemoveItem( DroppedItem ) )
				{
					cPickup* Pickup = new cPickup( PacketData );
					Pickup->Initialize( m_Player->GetWorld() );
				}
			}
			break;
		case E_CHAT:
			{
				cPacket_Chat* PacketData = reinterpret_cast<cPacket_Chat*>(a_Packet);
				if( !cRoot::Get()->GetServer()->Command( *this, PacketData->m_Message.c_str() ) )
				{
					PacketData->m_Message.insert( 0, "<"+m_Player->GetColor() + m_pState->Username + cChatColor::White + "> " );
					cRoot::Get()->GetServer()->Broadcast( *PacketData );
				}
			}
			break;
		case E_PLAYERLOOK:
			{
				cPacket_PlayerLook* PacketData = reinterpret_cast<cPacket_PlayerLook*>(a_Packet);
				m_Player->SetRotation( PacketData->m_Rotation );
				m_Player->SetPitch( PacketData->m_Pitch );
				m_Player->SetTouchGround( PacketData->m_bFlying );
				m_Player->WrapRotation();
			}
			break;
		case E_PLAYERMOVELOOK:
			{
				cPacket_PlayerMoveLook* PacketData = reinterpret_cast<cPacket_PlayerMoveLook*>(a_Packet);
				m_Player->MoveTo( Vector3d( PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ ) );
				m_Player->SetStance( PacketData->m_Stance );
				m_Player->SetTouchGround( PacketData->m_bFlying );
				m_Player->SetRotation( PacketData->m_Rotation );
				m_Player->SetPitch( PacketData->m_Pitch );
				m_Player->WrapRotation();
			}
			break;
		case E_ANIMATION:
			{
				cPacket_ArmAnim* PacketData = reinterpret_cast<cPacket_ArmAnim*>(a_Packet);
				PacketData->m_EntityID = m_Player->GetUniqueID();
				cRoot::Get()->GetServer()->Broadcast( *PacketData, this );
			}
			break;
		case E_ITEM_SWITCH:
			{
				cPacket_ItemSwitch* PacketData = reinterpret_cast<cPacket_ItemSwitch*>(a_Packet);

				m_Player->GetInventory().SetEquippedSlot( PacketData->m_SlotNum );

				cPacket_EntityEquipment Equipment;
				Equipment.m_ItemID = (short)m_Player->GetInventory().GetEquippedItem().m_ItemID;
				Equipment.m_Slot = 0;
				Equipment.m_UniqueID = m_Player->GetUniqueID();
				cRoot::Get()->GetServer()->Broadcast( Equipment, this );
			}
			break;
		case E_WINDOW_CLOSE:
			{
				cPacket_WindowClose* PacketData = reinterpret_cast<cPacket_WindowClose*>(a_Packet);
				m_Player->CloseWindow(PacketData->m_Close);
				/*
				if( PacketData->m_Close > 0 ) // Don't care about closing inventory
				{
					m_Player->CloseWindow();
				}
				*/
			}
			break;
		case E_WINDOW_CLICK:
			{
				cPacket_WindowClick* PacketData = reinterpret_cast<cPacket_WindowClick*>(a_Packet);
				if( PacketData->m_WindowID == 0 )
				{
					m_Player->GetInventory().Clicked( PacketData );
				}
				else
				{
					cWindow* Window = m_Player->GetWindow();
					if( Window ) Window->Clicked( PacketData, *m_Player );
					else LOG("No 'other' window! WTF");
				}
			}
			break;
		case E_UPDATE_SIGN:
			{
				cPacket_UpdateSign* PacketData = reinterpret_cast<cPacket_UpdateSign*>(a_Packet);
				cWorld* World = m_Player->GetWorld();
				cChunk* Chunk = World->GetChunkOfBlock( PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ );
				cBlockEntity* BlockEntity = Chunk->GetBlockEntity( PacketData->m_PosX, PacketData->m_PosY, PacketData->m_PosZ );
				if( BlockEntity && (BlockEntity->GetBlockType() == E_BLOCK_SIGN_POST || BlockEntity->GetBlockType() == E_BLOCK_WALLSIGN ) )
				{
					cSignEntity* Sign = reinterpret_cast< cSignEntity* >(BlockEntity);
					Sign->SetLines( PacketData->m_Line1, PacketData->m_Line2, PacketData->m_Line3, PacketData->m_Line4 );
					Sign->SendTo( 0 ); // Broadcast to all players in chunk
				}
			}
			break;
		case E_USE_ENTITY:
			{
				cPacket_UseEntity* PacketData = reinterpret_cast<cPacket_UseEntity*>(a_Packet);
				if( PacketData->m_bLeftClick )
				{
					cWorld* World = m_Player->GetWorld();
					cEntity* Entity = World->GetEntity( PacketData->m_TargetID );
					if( Entity && Entity->IsA("cPawn") )
					{
						cPawn* Pawn = (cPawn*)Entity;
						Pawn->TakeDamage( 1, m_Player );
					}
				}
			}
			break;
		case E_RESPAWN:
			{
				m_Player->Respawn();
			}
			break;
		case E_DISCONNECT:
			{
			    LOG("Received d/c packet from %s", GetUsername() );
				cPacket_Disconnect* PacketData = reinterpret_cast<cPacket_Disconnect*>(a_Packet);
				if( !cRoot::Get()->GetPluginManager()->CallHook( cPluginManager::E_PLUGIN_DISCONNECT, 2, PacketData->m_Reason.c_str(), m_Player ) )
				{
					cPacket_Chat DisconnectMessage( m_pState->Username + " disconnected: " + PacketData->m_Reason );
					cRoot::Get()->GetServer()->Broadcast( DisconnectMessage );
					cWorld::PlayerList PlayerList = cRoot::Get()->GetWorld()->GetAllPlayers();
					for( cWorld::PlayerList::iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr )
					{
						cPacket_PlayerListItem *PlayerList = new cPacket_PlayerListItem(m_Player->GetColor() + GetUsername(), false, (short)9999);
						(*itr)->GetClientHandle()->Send( *PlayerList );
					}
				}
				Destroy();
				return;
			}
			break;
		case E_KEEP_ALIVE:
				// TODO: Handle player ping per minecraft
				//cPacket_KeepAlive* PacketData = reinterpret_cast<cPacket_KeepAlive*>(a_Packet);
			break;
			default:
                break;
		}
	}
}

void cClientHandle::AuthenticateThread( void* a_Param )
{
	cClientHandle* self = (cClientHandle*)a_Param;

	cAuthenticator Authenticator;
	if( !Authenticator.Authenticate( self->GetUsername(), cRoot::Get()->GetServer()->GetServerID() ) )
	{
		self->Kick("Failed to verify username!");
		return;
	}
	self->m_bSendLoginResponse = true;
}


void cClientHandle::Tick(float a_Dt)
{
	(void)a_Dt;
	if( cWorld::GetTime() - m_TimeLastPacket > 30.f ) // 30 seconds time-out
	{
		cPacket_Disconnect DC("Nooooo!! You timed out! D: Come back!");
		DC.Send( m_pState->Socket );

		cSleep::MilliSleep( 1000 ); // Give packet some time to be received

		Destroy();
	}

	if( m_bSendLoginResponse )
	{
		m_bSendLoginResponse = false;

		// Spawn player (only serversided, so data is loaded)
		m_Player = new cPlayer( this, GetUsername() );	// !!DO NOT INITIALIZE!! <- is done after receiving MoveLook Packet

		cWorld* World = cRoot::Get()->GetWorld( m_Player->GetLoadedWorldName() ); // TODO - Get the correct world or better yet, move this to the main thread so we don't have to lock anything
		if( !World ) World = cRoot::Get()->GetDefaultWorld();
		World->LockEntities();
		m_Player->LoginSetGameMode ( World->GetGameMode() ); //set player's gamemode to server's gamemode at login.

		m_Player->SetIP ( m_pState->Socket.GetIPString() );

		cRoot::Get()->GetPluginManager()->CallHook( cPluginManager::E_PLUGIN_PLAYER_SPAWN, 1, m_Player );

		// Return a server login packet
		cPacket_Login LoginResponse;
		LoginResponse.m_ProtocolVersion = m_Player->GetUniqueID();
		//LoginResponse.m_Username = "";
		LoginResponse.m_ServerMode = m_Player->GetGameMode(); //set gamemode from player.
		LoginResponse.m_MapSeed = 0;
		LoginResponse.m_Dimension = 0;
		LoginResponse.m_MaxPlayers = (unsigned char)cRoot::Get()->GetWorld()->GetMaxPlayers();
		Send( LoginResponse );

		// Send Weather if raining:
		if ( (World->GetWeather() == 1) || (World->GetWeather() == 2) ) {
			cPacket_NewInvalidState RainPacket;
			RainPacket.m_Reason = 1; //begin rain
			Send( RainPacket );
		}

		// Send position
		Send( cPacket_PlayerMoveLook( m_Player ) );

		// Send time
		Send( cPacket_TimeUpdate( World->GetWorldTime() ) );

		// Send inventory
		m_Player->GetInventory().SendWholeInventory( this );

		// Send health
		Send( cPacket_UpdateHealth( (short)m_Player->GetHealth() ) );

		World->UnlockEntities();
	}
}

void cClientHandle::Send( const cPacket & a_Packet, ENUM_PRIORITY a_Priority /* = E_PRIORITY_NORMAL */ )
{
	if( m_bKicking ) return; // Don't add more packets if player is getting kicked anyway

	bool bSignalSemaphore = true;
	m_pState->SendCriticalSection.Lock();
	if( a_Priority == E_PRIORITY_NORMAL )
	{
		if( a_Packet.m_PacketID == E_REL_ENT_MOVE_LOOK )
		{
			PacketList & Packets = m_pState->PendingNrmSendPackets;
			for( std::list<cPacket*>::iterator itr = Packets.begin(); itr != Packets.end(); ++itr )
			{
				bool bBreak = false;
				switch( (*itr)->m_PacketID )
				{
				case E_REL_ENT_MOVE_LOOK:
					{
						const cPacket_RelativeEntityMoveLook* ThisPacketData = reinterpret_cast< const cPacket_RelativeEntityMoveLook* >(&a_Packet);
						cPacket_RelativeEntityMoveLook* PacketData = reinterpret_cast< cPacket_RelativeEntityMoveLook* >(*itr);
						if( ThisPacketData->m_UniqueID == PacketData->m_UniqueID )
						{
							//LOGINFO("Optimized by removing double packet");
							Packets.erase( itr );
							bBreak = true;
							bSignalSemaphore = false; // Because 1 packet is removed, semaphore count is the same
							delete PacketData;
							break;
						}
					}
					break;
                default:
					break;
				}
				if( bBreak )
					break;
			}
		}
		m_pState->PendingNrmSendPackets.push_back( a_Packet.Clone() );
	}
	else if( a_Priority == E_PRIORITY_LOW ) m_pState->PendingLowSendPackets.push_back( a_Packet.Clone() );
	m_pState->SendCriticalSection.Unlock();
	if( bSignalSemaphore )
		m_pState->pSemaphore->Signal();
}

void cClientHandle::SendThread( void *lpParam )
{
	cClientHandle* self = (cClientHandle*)lpParam;
	sClientHandleState* m_pState = self->m_pState;
	PacketList & NrmSendPackets = m_pState->PendingNrmSendPackets;
	PacketList & LowSendPackets = m_pState->PendingLowSendPackets;


	while( self->m_bKeepThreadGoing && m_pState->Socket )
	{
		m_pState->pSemaphore->Wait();
		m_pState->SendCriticalSection.Lock();
		//LOG("Pending packets: %i", m_PendingPackets.size() );
		if( NrmSendPackets.size() + LowSendPackets.size() > MAX_SEMAPHORES )
		{
			LOGERROR("ERROR: Too many packets in queue for player %s !!", self->GetUsername() );
			cPacket_Disconnect DC("Too many packets in queue.");
			DC.Send( m_pState->Socket );

			cSleep::MilliSleep( 1000 ); // Give packet some time to be received

			self->Destroy();
			m_pState->SendCriticalSection.Unlock();
			break;
		}
		if( NrmSendPackets.size() == 0 && LowSendPackets.size() == 0 )
		{
			if( self->m_bKeepThreadGoing ) LOGERROR("ERROR: Semaphore was signaled while PendingSendPackets.size == 0");
			m_pState->SendCriticalSection.Unlock();
			continue;
		}
		if( NrmSendPackets.size() > MAX_SEMAPHORES/2 )
		{
			LOGINFO("Pending packets: %i Last: 0x%02x", NrmSendPackets.size(), (*NrmSendPackets.rbegin())->m_PacketID );
		}

		cPacket* Packet = 0;
		if( NrmSendPackets.size() > 0 )
		{
			Packet = *NrmSendPackets.begin();
			NrmSendPackets.erase( NrmSendPackets.begin() );
		}
		else if( LowSendPackets.size() > 0 )
		{
			Packet = *LowSendPackets.begin();
			LowSendPackets.erase( LowSendPackets.begin() );
		}
		m_pState->SendCriticalSection.Unlock();

		m_pState->SocketCriticalSection.Lock();
		if( !m_pState->Socket )
		{
			m_pState->SocketCriticalSection.Unlock();
			break;
		}
		bool bSuccess = Packet->Send( m_pState->Socket );
		m_pState->SocketCriticalSection.Unlock();
		if( !bSuccess )
		{
			LOGERROR("ERROR: While sending packet 0x%02x", Packet->m_PacketID );
			delete Packet;
			self->Destroy();
			break;
		}
		delete Packet;

		if( self->m_bKicking && (NrmSendPackets.size() + LowSendPackets.size() == 0) ) // Disconnect player after all packets have been sent
		{
			cSleep::MilliSleep( 1000 ); // Give all packets some time to be received
			self->Destroy();
			break;
		}
	}

	return;
}


extern std::string GetWSAError();

void cClientHandle::ReceiveThread( void *lpParam )
{
	LOG("ReceiveThread");

	cClientHandle* self = (cClientHandle*)lpParam;


	char temp = 0;
	int iStat = 0;

	cSocket socket = self->GetSocket();

	while( self->m_bKeepThreadGoing )
	{
		iStat = socket.Receive( &temp, 1, 0 );
		if( cSocket::IsSocketError(iStat) || iStat == 0 )
		{
			LOG("CLIENT DISCONNECTED (%i bytes):%s", iStat, GetWSAError().c_str() );
			break;
		}
		else
		{
			cPacket* pPacket = self->m_pState->PacketMap[ (unsigned char)temp ];
			if( pPacket )
			{
				if( pPacket->Parse( socket ) )
				{
					self->AddPacket( pPacket );
					//self->HandlePendingPackets();
				}
				else
				{
					LOGERROR("Something went wrong during PacketID 0x%02x (%s)", temp, cSocket::GetLastErrorString() );
					LOG("CLIENT %s DISCONNECTED", self->GetUsername() );
					break;
				}
			}
			else
			{
				LOG("Unknown packet: 0x%2x %c %i", (unsigned char)temp, (unsigned char)temp, (unsigned char)temp );


				char c_Str[128];
				sprintf_s( c_Str, 128, "[C->S] Unknown PacketID: 0x%2x", (unsigned char)temp );
				cPacket_Disconnect DC(c_Str);
				DC.Send( socket );

				cSleep::MilliSleep( 1000 ); // Give packet some time to be received
				break;
			}
		}
	}

	self->Destroy();

	LOG("ReceiveThread STOPPED");
	return;
}


const char* cClientHandle::GetUsername()
{
	return m_pState->Username.c_str();
}

const cSocket & cClientHandle::GetSocket()
{
	return m_pState->Socket;
}
