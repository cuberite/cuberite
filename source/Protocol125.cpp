
// Protocol125.cpp

// Implements the cProtocol125 class representing the release 1.2.5 protocol (#29)

#include "Globals.h"

#include "Protocol125.h"

#include "packets/cPacket.h"
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
#include "packets/cPacket_Thunderbolt.h"
#include "packets/cPacket_TimeUpdate.h"
#include "packets/cPacket_UpdateHealth.h"
#include "packets/cPacket_UpdateSign.h"
#include "packets/cPacket_UseEntity.h"
#include "packets/cPacket_WholeInventory.h"
#include "packets/cPacket_WindowClick.h"
#include "packets/cPacket_WindowClose.h"
#include "packets/cPacket_WindowOpen.h"

#include "cClientHandle.h"
#include "ChunkDataSerializer.h"
#include "cEntity.h"
#include "cMonster.h"
#include "cPickup.h"
#include "cPlayer.h"





cProtocol125::cProtocol125(cClientHandle * a_Client) :
	super(a_Client),
	m_ReceivedData(64 KiB)
{
}





void cProtocol125::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2)
{
	cCSLock Lock(m_CSPacket);
	cPacket_BlockAction ba;
	ba.m_BlockX = a_BlockX;
	ba.m_BlockY = (short)a_BlockY;
	ba.m_BlockZ = a_BlockZ;
	ba.m_Byte1 = a_Byte1;
	ba.m_Byte2 = a_Byte2;
	Send(ba);
}





void cProtocol125::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CSPacket);
	cPacket_BlockChange BlockChange;
	BlockChange.m_PosX = a_BlockX;
	BlockChange.m_PosY = (unsigned char)a_BlockY;
	BlockChange.m_PosZ = a_BlockZ;
	BlockChange.m_BlockType = a_BlockType;
	BlockChange.m_BlockMeta = a_BlockMeta;
	Send(BlockChange);
}





void cProtocol125::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	cCSLock Lock(m_CSPacket);
	if (a_Changes.size() == 1)
	{
		// Special packet for single-block changes
		const sSetBlock & blk = a_Changes.front();
		SendBlockChange(a_ChunkX * cChunkDef::Width + blk.x, blk.y, a_ChunkZ * cChunkDef::Width + blk.z, blk.BlockType, blk.BlockMeta);
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





void cProtocol125::SendChat(const AString & a_Message)
{
	cCSLock Lock(m_CSPacket);
	cPacket_Chat Chat(a_Message);
	Send(Chat);
}





void cProtocol125::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	cCSLock Lock(m_CSPacket);

	// Send the pre-chunk:
	cPacket_PreChunk pre(a_ChunkX, a_ChunkZ, true);
	Send(pre);
	
	// Send the data:
	cPacket_MapChunk mc(a_ChunkX, a_ChunkZ, a_Serializer.Serialize(cChunkDataSerializer::RELEASE_1_2_5));
	Send(mc);
}





void cProtocol125::SendCollectPickup(const cPickup & a_Pickup, const cPlayer & a_Player)
{
	cCSLock Lock(m_CSPacket);
	cPacket_CollectItem ci;
	ci.m_CollectedID = a_Pickup.GetUniqueID();
	ci.m_CollectorID = a_Player.GetUniqueID();
	Send(ci);
}





void cProtocol125::SendDestroyEntity(const cEntity & a_Entity)
{
	cCSLock Lock(m_CSPacket);
	cPacket_DestroyEntity de;
	de.m_UniqueID = a_Entity.GetUniqueID();
	Send(de);
}





void cProtocol125::SendDisconnect(const AString & a_Reason)
{
	cCSLock Lock(m_CSPacket);
	cPacket_Disconnect DC(a_Reason);
	Send(DC);  // TODO: Send it immediately to the socket, bypassing any packet buffers (? is it safe? packet boundaries...)
}





void cProtocol125::SendEntHeadLook(const cEntity & a_Entity)
{
	ASSERT(a_Entity.GetUniqueID() != m_Client->GetPlayer()->GetUniqueID());  // Must not send for self
	
	cCSLock Lock(m_CSPacket);
	cPacket_EntityHeadLook ehl(a_Entity);
	Send(ehl);
}





void cProtocol125::SendEntLook(const cEntity & a_Entity)
{
	ASSERT(a_Entity.GetUniqueID() != m_Client->GetPlayer()->GetUniqueID());  // Must not send for self
	
	cCSLock Lock(m_CSPacket);
	cPacket_EntityLook el;
	el.m_UniqueID = a_Entity.GetUniqueID();
	el.m_Rotation = (char)((a_Entity.GetRotation() / 360.f) * 256);
	el.m_Pitch    = (char)((a_Entity.GetPitch()    / 360.f) * 256);
	Send(el);
}





void cProtocol125::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	cCSLock Lock(m_CSPacket);
	cPacket_EntityEquipment ee;
	ee.m_UniqueID   = a_Entity.GetUniqueID();
	ee.m_SlotNum    = a_SlotNum;
	ee.m_ItemType   = a_Item.m_ItemType;
	ee.m_ItemDamage = a_Item.m_ItemDamage;
	Send(ee);
}





void cProtocol125::SendEntityStatus(const cEntity & a_Entity, char a_Status)
{
	cCSLock Lock(m_CSPacket);
	cPacket_EntityStatus es;
	es.m_Status   = a_Status;
	es.m_UniqueID = a_Entity.GetUniqueID();
	Send(es);
}





void cProtocol125::SendGameMode(eGameMode a_GameMode)
{
	cCSLock Lock(m_CSPacket);
	cPacket_NewInvalidState nis;
	nis.m_Reason = 3;
	nis.m_GameMode = (char)a_GameMode;
	Send(nis);
}





void cProtocol125::SendHandshake(const AString & a_ServerName)
{
	cCSLock Lock(m_CSPacket);
	cPacket_Handshake Handshake;
	Handshake.m_Username = a_ServerName;
	Send(Handshake);
}





void cProtocol125::SendHealth(void)
{
	cCSLock Lock(m_CSPacket);
	cPacket_UpdateHealth Health;
	Health.m_Health     = (short)m_Client->GetPlayer()->GetHealth();
	Health.m_Food       = m_Client->GetPlayer()->GetFoodLevel();
	Health.m_Saturation = m_Client->GetPlayer()->GetFoodSaturationLevel();
	Send(Health);
}





void cProtocol125::SendInventoryProgress(char a_WindowID, short a_ProgressBar, short a_Value)
{
	cCSLock Lock(m_CSPacket);
	cPacket_InventoryProgressBar Progress;
	Progress.m_WindowID    = a_WindowID;
	Progress.m_ProgressBar = a_ProgressBar;
	Progress.m_Value       = a_Value;
	Progress.m_WindowID    = a_WindowID;
	Send(Progress);
}





void cProtocol125::SendInventorySlot(int a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	cCSLock Lock(m_CSPacket);
	cPacket_InventorySlot Packet;
	Packet.m_WindowID  = (char)a_WindowID;
	Packet.m_SlotNum   = a_SlotNum;
	Packet.m_ItemID    = (short)(a_Item.m_ItemID);
	Packet.m_ItemCount = a_Item.m_ItemCount;
	Packet.m_ItemUses  = a_Item.m_ItemHealth;
	Send(Packet);
}





void cProtocol125::SendKeepAlive(int a_PingID)
{
	cCSLock Lock(m_CSPacket);
	cPacket_KeepAlive ka(a_PingID);
	Send(ka);
}





void cProtocol125::SendLogin(const cPlayer & a_Player)
{
	cCSLock Lock(m_CSPacket);
	cPacket_Login LoginResponse;
	LoginResponse.m_ProtocolVersion = a_Player.GetUniqueID();
	LoginResponse.m_Username = "";  // Not used
	LoginResponse.m_ServerMode = a_Player.GetGameMode();  // set gamemode from player.
	LoginResponse.m_Dimension = 0;  // TODO: Dimension (Nether / Overworld / End)
	LoginResponse.m_MaxPlayers = 60;  // Client list width or something
	LoginResponse.m_Difficulty = 2;  // TODO: Difficulty
	Send(LoginResponse);
}





void cProtocol125::SendMetadata(const cPawn & a_Pawn)
{
	cCSLock Lock(m_CSPacket);
	cPacket_Metadata md(a_Pawn.GetMetaData(), a_Pawn.GetUniqueID());
	Send(md);
}





void cProtocol125::SendPickupSpawn(const cPickup & a_Pickup)
{
	cCSLock Lock(m_CSPacket);
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





void cProtocol125::SendPlayerAnimation(const cPlayer & a_Player, char a_Animation)
{
	cCSLock Lock(m_CSPacket);
	cPacket_ArmAnim Anim;
	Anim.m_EntityID  = a_Player.GetUniqueID();
	Anim.m_Animation = a_Animation;
	Send(Anim);
}





void cProtocol125::SendPlayerListItem(const cPlayer & a_Player, bool a_IsOnline)
{
	cCSLock Lock(m_CSPacket);
	cPacket_PlayerListItem pli(a_Player.GetColor() + a_Player.GetName(), a_IsOnline, a_Player.GetClientHandle()->GetPing());
	Send(pli);
}





void cProtocol125::SendPlayerMoveLook(void)
{
	cCSLock Lock(m_CSPacket);
	cPacket_PlayerMoveLook pml(*m_Client->GetPlayer());
	/*
	LOGD("Sending PlayerMoveLook: {%0.2f, %0.2f, %0.2f}, stance %0.2f, OnGround: %d",
		m_Player->GetPosX(), m_Player->GetPosY(), m_Player->GetPosZ(), m_Player->GetStance(), m_Player->IsOnGround() ? 1 : 0
	);
	*/
	Send(pml);
}





void cProtocol125::SendPlayerPosition(void)
{
	cCSLock Lock(m_CSPacket);
	cPacket_PlayerPosition pp(*(m_Client->GetPlayer()));
	Send(pp);
}





void cProtocol125::SendPlayerSpawn(const cPlayer & a_Player)
{
	cCSLock Lock(m_CSPacket);
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





void cProtocol125::SendRelEntMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(a_Entity.GetUniqueID() != m_Client->GetPlayer()->GetUniqueID());  // Must not send for self
	
	cCSLock Lock(m_CSPacket);
	cPacket_RelativeEntityMove rem;
	rem.m_UniqueID = a_Entity.GetUniqueID();
	rem.m_MoveX    = a_RelX;
	rem.m_MoveY    = a_RelY;
	rem.m_MoveZ    = a_RelZ;
	Send(rem);
}





void cProtocol125::SendRelEntMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(a_Entity.GetUniqueID() != m_Client->GetPlayer()->GetUniqueID());  // Must not send for self
	
	cCSLock Lock(m_CSPacket);
	cPacket_RelativeEntityMoveLook reml;
	reml.m_UniqueID = a_Entity.GetUniqueID();
	reml.m_MoveX    = a_RelX;
	reml.m_MoveY    = a_RelY;
	reml.m_MoveZ    = a_RelZ;
	reml.m_Yaw      = (char)((a_Entity.GetRotation() / 360.f) * 256);
	reml.m_Pitch    = (char)((a_Entity.GetPitch()    / 360.f) * 256);
	Send(reml);
}





void cProtocol125::SendRespawn(void)
{
	cCSLock Lock(m_CSPacket);
	cPacket_Respawn Packet;
	Packet.m_CreativeMode = (char)m_Client->GetPlayer()->GetGameMode();  // Set GameMode packet based on Player's GameMode;
	Send(Packet);
}





void cProtocol125::SendSpawnMob(const cMonster & a_Mob)
{
	cCSLock Lock(m_CSPacket);
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





void cProtocol125::SendTeleportEntity(const cEntity & a_Entity)
{
	cCSLock Lock(m_CSPacket);
	cPacket_TeleportEntity te(a_Entity);
	Send(te);
}





void cProtocol125::SendThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cCSLock Lock(m_CSPacket);
	cPacket_Thunderbolt ThunderboltPacket;
	ThunderboltPacket.m_xLBPos = a_BlockX;
	ThunderboltPacket.m_yLBPos = a_BlockY;
	ThunderboltPacket.m_zLBPos = a_BlockZ;
	Send(ThunderboltPacket);
}





void cProtocol125::SendTimeUpdate(Int64 a_WorldTime)
{
	cCSLock Lock(m_CSPacket);
	cPacket_TimeUpdate tu;
	tu.m_Time = a_WorldTime;
	Send(tu);
}





void cProtocol125::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSPacket);
	cPacket_PreChunk UnloadPacket;
	UnloadPacket.m_PosX = a_ChunkX;
	UnloadPacket.m_PosZ = a_ChunkZ;
	UnloadPacket.m_bLoad = false;  // Unload
	Send(UnloadPacket);
}





void cProtocol125::SendUpdateSign(
	int a_BlockX, int a_BlockY, int a_BlockZ,
	const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4
)
{
	cCSLock Lock(m_CSPacket);
	cPacket_UpdateSign us;
	us.m_BlockX = a_BlockX;
	us.m_BlockY = (short)a_BlockY;
	us.m_BlockZ = a_BlockZ;
	us.m_Line1  = a_Line1;
	us.m_Line2  = a_Line2;
	us.m_Line3  = a_Line3;
	us.m_Line4  = a_Line4;
	Send(us);
}





void cProtocol125::SendWeather(eWeather a_Weather)
{
	cCSLock Lock(m_CSPacket);
	switch( a_Weather )
	{
		case eWeather_Sunny:
		{
			cPacket_NewInvalidState WeatherPacket;
			WeatherPacket.m_Reason = 2;  // stop rain
			Send(WeatherPacket);
			break;
		}
		
		case eWeather_Rain:
		{
			cPacket_NewInvalidState WeatherPacket;
			WeatherPacket.m_Reason = 1;  // begin rain
			Send(WeatherPacket);
			break;
		}
		
		case eWeather_ThunderStorm:
		{
			cPacket_NewInvalidState WeatherPacket;
			WeatherPacket.m_Reason = 1;  // begin rain
			Send(WeatherPacket);
			break;
		}
	}
}





void cProtocol125::SendWholeInventory(const cInventory & a_Inventory)
{
	cCSLock Lock(m_CSPacket);
	cPacket_WholeInventory wi(a_Inventory);
	Send(wi);
}





void cProtocol125::SendWholeInventory(const cWindow & a_Window)
{
	cCSLock Lock(m_CSPacket);
	cPacket_WholeInventory wi(a_Window);
	Send(wi);
}





void cProtocol125::SendWindowClose(char a_WindowID)
{
	cCSLock Lock(m_CSPacket);
	cPacket_WindowClose wc;
	wc.m_WindowID = a_WindowID;
	Send(wc);
}





void cProtocol125::SendWindowOpen(char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots)
{
	cCSLock Lock(m_CSPacket);
	cPacket_WindowOpen WindowOpen;
	WindowOpen.m_WindowID      = a_WindowID;
	WindowOpen.m_InventoryType = a_WindowType;
	WindowOpen.m_WindowTitle   = a_WindowTitle;
	WindowOpen.m_NumSlots      = a_NumSlots;
	Send(WindowOpen);
}





void cProtocol125::Send(const cPacket & a_Packet)
{
	AString str;
	a_Packet.Serialize(str);
	SendData(str.data(), str.size());
}





void cProtocol125::SendData(const char * a_Data, int a_Size)
{
	m_Client->SendData(a_Data, a_Size);
}





void cProtocol125::DataReceived(const char * a_Data, int a_Size)
{
	if (!m_ReceivedData.Write(a_Data, a_Size))
	{
		// Too much data in the incoming queue, report to caller:
		m_Client->PacketBufferFull();
		return;
	}

	// Parse and handle all complete packets in m_ReceivedData:
	while (m_ReceivedData.CanReadBytes(1))
	{
		unsigned char PacketType;
		m_ReceivedData.ReadByte(PacketType);
		switch (ParsePacket(PacketType))
		{
			case PACKET_UNKNOWN:
			{
				// An unknown packet has been received, notify the client and abort:
				m_Client->PacketUnknown(PacketType);
				return;
			}
			case PACKET_ERROR:
			{
				// An error occurred while parsing a known packet, notify the client and abort:
				m_Client->PacketError(PacketType);
				return;
			}
			case PACKET_INCOMPLETE:
			{
				// Incomplete packet, bail out and process with the next batch of data
				m_ReceivedData.ResetRead();
				return;
			}
			default:
			{
				// Packet successfully parsed, commit the read data and try again one more packet
				m_ReceivedData.CommitRead();
				break;
			}
		}
	}
}





int cProtocol125::ParsePacket(unsigned char a_PacketType)
{
	switch (a_PacketType)
	{
		default: return PACKET_UNKNOWN;
		case E_KEEP_ALIVE:                return ParseKeepAlive();
		case E_HANDSHAKE:                 return ParseHandshake();
		case E_LOGIN:                     return ParseLogin();
		case E_PLAYERPOS:                 return ParsePlayerPosition();
		case E_PLAYERLOOK:                return ParsePlayerLook();
		case E_PLAYERMOVELOOK:            return ParsePlayerMoveLook();
		case E_PLAYER_ABILITIES:          return ParsePlayerAbilities();
		case E_CHAT:                      return ParseChat();
		case E_ANIMATION:                 return ParseArmAnim();
		case E_FLYING:                    return ParseFlying();
		case E_BLOCK_DIG:                 return ParseBlockDig();
		case E_BLOCK_PLACE:               return ParseBlockPlace();
		case E_DISCONNECT:                return ParseDisconnect();
		case E_ITEM_SWITCH:               return ParseItemSwitch();
		case E_ENTITY_EQUIPMENT:          return ParseEntityEquipment();
		case E_CREATIVE_INVENTORY_ACTION: return ParseCreativeInventoryAction();
		case E_NEW_INVALID_STATE:         return ParseNewInvalidState();
		case E_PICKUP_SPAWN:              return ParsePickupSpawn();
		case E_USE_ENTITY:                return ParseUseEntity();
		case E_WINDOW_CLOSE:              return ParseWindowClose();
		case E_WINDOW_CLICK:              return ParseWindowClick();
		case E_PACKET_ENTITY_ACTION:      return ParseEntityAction();
		case E_UPDATE_SIGN:               return ParseUpdateSign();
		case E_RESPAWN:                   return ParseRespawn();
		case E_PING:                      return ParsePing();
	}
}





#define HANDLE_PACKET_PARSE(Packet) \
	{ \
		int res = Packet.Parse(m_ReceivedData); \
		if (res < 0) \
		{ \
			return res; \
		} \
	}





int cProtocol125::ParseKeepAlive(void)
{
	cPacket_KeepAlive KeepAlive;
	HANDLE_PACKET_PARSE(KeepAlive);
	m_Client->HandleKeepAlive(KeepAlive.m_KeepAliveID);
	return PACKET_OK;
}





int cProtocol125::ParseHandshake(void)
{
	cPacket_Handshake Handshake;
	HANDLE_PACKET_PARSE(Handshake);
	m_Client->HandleHandshake(Handshake.m_Username);
	return PACKET_OK;
}





int cProtocol125::ParseLogin(void)
{
	cPacket_Login Login;
	HANDLE_PACKET_PARSE(Login);
	m_Client->HandleLogin(Login.m_ProtocolVersion, Login.m_Username);
	return PACKET_OK;
}





int cProtocol125::ParsePlayerPosition(void)
{
	cPacket_PlayerPosition pp;
	HANDLE_PACKET_PARSE(pp);
	m_Client->HandlePlayerPos(pp.m_PosX, pp.m_PosY, pp.m_PosZ, pp.m_Stance, pp.m_IsOnGround);
	return PACKET_OK;
}





int cProtocol125::ParsePlayerLook(void)
{
	cPacket_PlayerLook pl;
	HANDLE_PACKET_PARSE(pl);
	m_Client->HandlePlayerLook(pl.m_Rotation, pl.m_Pitch, pl.m_IsOnGround);
	return PACKET_OK;
}





int cProtocol125::ParsePlayerMoveLook(void)
{
	cPacket_PlayerMoveLook pml;
	HANDLE_PACKET_PARSE(pml);
	m_Client->HandlePlayerMoveLook(pml.m_PosX, pml.m_PosY, pml.m_PosZ, pml.m_Stance, pml.m_Rotation, pml.m_Pitch, pml.m_IsOnGround);
	return PACKET_OK;
}





int cProtocol125::ParsePlayerAbilities(void)
{
	cPacket_PlayerAbilities pa;
	HANDLE_PACKET_PARSE(pa);
	// TODO: m_Client->HandlePlayerAbilities(...);
	return PACKET_OK;
}





int cProtocol125::ParseChat(void)
{
	cPacket_Chat ch;
	HANDLE_PACKET_PARSE(ch);
	m_Client->HandleChat(ch.m_Message);
	return PACKET_OK;
}





int cProtocol125::ParseArmAnim(void)
{
	cPacket_ArmAnim aa;
	HANDLE_PACKET_PARSE(aa);
	m_Client->HandleAnimation(aa.m_Animation);
	return PACKET_OK;
}





int cProtocol125::ParseFlying(void)
{
	cPacket_Flying fl;
	HANDLE_PACKET_PARSE(fl);
	// TODO: m_Client->HandleFlying(fl.m_bFlying);
	return PACKET_OK;
}





int cProtocol125::ParseBlockDig(void)
{
	cPacket_BlockDig bd;
	HANDLE_PACKET_PARSE(bd);
	m_Client->HandleBlockDig(bd.m_PosX, bd.m_PosY, bd.m_PosZ, bd.m_Direction, bd.m_Status);
	return PACKET_OK;
}





int cProtocol125::ParseBlockPlace(void)
{
	cPacket_BlockPlace bp;
	HANDLE_PACKET_PARSE(bp);
	m_Client->HandleBlockPlace(bp.m_PosX, bp.m_PosY, bp.m_PosZ, bp.m_Direction, bp.m_HeldItem);
	return PACKET_OK;
}





int cProtocol125::ParseDisconnect(void)
{
	cPacket_Disconnect dc;
	HANDLE_PACKET_PARSE(dc);
	m_Client->HandleDisconnect(dc.m_Reason);
	return PACKET_OK;
}





int cProtocol125::ParseItemSwitch(void)
{
	cPacket_ItemSwitch its;
	HANDLE_PACKET_PARSE(its);
	m_Client->HandleSlotSelected(its.m_SlotNum);
	return PACKET_OK;
}





int cProtocol125::ParseEntityEquipment(void)
{
	cPacket_EntityEquipment ee;
	HANDLE_PACKET_PARSE(ee);
	// TODO: m_Client->HandleEntityEquipment(...);
	return PACKET_OK;
}





int cProtocol125::ParseCreativeInventoryAction(void)
{
	cPacket_CreativeInventoryAction cia;
	HANDLE_PACKET_PARSE(cia);
	m_Client->HandleCreativeInventory(cia.m_SlotNum, cia.m_ClickedItem);
	return PACKET_OK;
}





int cProtocol125::ParseNewInvalidState(void)
{
	cPacket_NewInvalidState nis;
	HANDLE_PACKET_PARSE(nis);
	// TODO: m_Client->Handle(...);
	return PACKET_OK;
}





int cProtocol125::ParsePickupSpawn(void)
{
	cPacket_PickupSpawn ps;
	HANDLE_PACKET_PARSE(ps);
	// TODO: m_Client->HandlePickupSpawn(...);
	return PACKET_OK;
}





int cProtocol125::ParseUseEntity(void)
{
	cPacket_UseEntity ue;
	HANDLE_PACKET_PARSE(ue);
	m_Client->HandleUseEntity(ue.m_TargetEntityID, ue.m_IsLeftClick);
	return PACKET_OK;
}





int cProtocol125::ParseWindowClose(void)
{
	cPacket_WindowClose wc;
	HANDLE_PACKET_PARSE(wc);
	m_Client->HandleWindowClose(wc.m_WindowID);
	return PACKET_OK;
}





int cProtocol125::ParseWindowClick(void)
{
	cPacket_WindowClick wc;
	HANDLE_PACKET_PARSE(wc);
	m_Client->HandleWindowClick(wc.m_WindowID, wc.m_SlotNum, wc.m_IsRightClick, wc.m_IsShiftPressed, wc.m_HeldItem);
	return PACKET_OK;
}





int cProtocol125::ParseEntityAction(void)
{
	cPacket_13 ea;
	HANDLE_PACKET_PARSE(ea);
	// TODO: m_Client->HandleEntityAction(...);
	return PACKET_OK;
}





int cProtocol125::ParseUpdateSign(void)
{
	cPacket_UpdateSign us;
	HANDLE_PACKET_PARSE(us);
	m_Client->HandleUpdateSign(
		us.m_BlockX, us.m_BlockY, us.m_BlockZ,
		us.m_Line1, us.m_Line2, us.m_Line3, us.m_Line4
	);
	return PACKET_OK;
}





int cProtocol125::ParseRespawn(void)
{
	cPacket_Respawn rsp;
	HANDLE_PACKET_PARSE(rsp);
	m_Client->HandleRespawn();
	return PACKET_OK;
}





int cProtocol125::ParsePing(void)
{
	cPacket_Ping ping;
	HANDLE_PACKET_PARSE(ping);
	m_Client->HandlePing();
	return PACKET_OK;
}





