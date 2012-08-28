
// Protocol125.cpp

// Implements the cProtocol125 class representing the release 1.2.5 protocol (#29)

#include "Globals.h"

#include "Protocol125.h"

#include "packets/cPacket.h"
#include "packets/cPacket_13.h"
#include "packets/cPacket_ArmAnim.h"
#include "packets/cPacket_BlockDig.h"
#include "packets/cPacket_BlockPlace.h"
#include "packets/cPacket_Chat.h"
#include "packets/cPacket_CreativeInventoryAction.h"
#include "packets/cPacket_Disconnect.h"
#include "packets/cPacket_Flying.h"
#include "packets/cPacket_Handshake.h"
#include "packets/cPacket_ItemSwitch.h"
#include "packets/cPacket_KeepAlive.h"
#include "packets/cPacket_Login.h"
#include "packets/cPacket_Ping.h"
#include "packets/cPacket_Player.h"
#include "packets/cPacket_Respawn.h"
#include "packets/cPacket_UpdateSign.h"
#include "packets/cPacket_UseEntity.h"
#include "packets/cPacket_WindowClick.h"
#include "packets/cPacket_WindowClose.h"

#include "cClientHandle.h"
#include "ChunkDataSerializer.h"
#include "cEntity.h"
#include "cMonster.h"
#include "cPickup.h"
#include "cPlayer.h"
#include "cChatColor.h"
#include "cWindow.h"





enum
{
	PACKET_KEEP_ALIVE                = 0x00,
	PACKET_LOGIN                     = 0x01,
	PACKET_HANDSHAKE                 = 0x02,
	PACKET_CHAT                      = 0x03,
	PACKET_UPDATE_TIME               = 0x04,
	PACKET_ENTITY_EQUIPMENT          = 0x05,
	PACKET_USE_ENTITY                = 0x07,
	PACKET_UPDATE_HEALTH             = 0x08,
	PACKET_RESPAWN                   = 0x09,
	PACKET_FLYING                    = 0x0a,
	PACKET_PLAYERPOS                 = 0x0b,
	PACKET_PLAYERLOOK                = 0x0c,
	PACKET_PLAYERMOVELOOK            = 0x0d,
	PACKET_BLOCK_DIG                 = 0x0e,
	PACKET_BLOCK_PLACE               = 0x0f,
	PACKET_SLOT_SELECTED             = 0x10,
	PACKET_ADD_TO_INV                = 0x11,  // TODO: Sure this is not Use Bed??
	PACKET_ANIMATION                 = 0x12,
	PACKET_PACKET_ENTITY_ACTION      = 0x13,
	PACKET_PLAYER_SPAWN              = 0x14,
	PACKET_PICKUP_SPAWN              = 0x15,
	PACKET_COLLECT_PICKUP            = 0x16,
	PACKET_ADD_VEHICLE               = 0x17,
	PACKET_SPAWN_MOB                 = 0x18,
	PACKET_DESTROY_ENTITY            = 0x1d,
	PACKET_ENTITY                    = 0x1e,
	PACKET_REL_ENT_MOVE              = 0x1f,
	PACKET_ENT_LOOK                  = 0x20,
	PACKET_REL_ENT_MOVE_LOOK         = 0x21,
	PACKET_ENT_TELEPORT              = 0x22,
	PACKET_ENT_HEAD_LOOK             = 0x23,
	PACKET_ENT_STATUS                = 0x26,
	PACKET_METADATA                  = 0x28,
	PACKET_PRE_CHUNK                 = 0x32,
	PACKET_MAP_CHUNK                 = 0x33,
	PACKET_MULTI_BLOCK               = 0x34,
	PACKET_BLOCK_CHANGE              = 0x35,
	PACKET_BLOCK_ACTION              = 0x36,
	PACKET_EXPLOSION                 = 0x3C,
	PACKET_SOUND_EFFECT              = 0x3D,
	PACKET_CHANGE_GAME_STATE         = 0x46,
	PACKET_THUNDERBOLT               = 0x47,
	PACKET_WINDOW_OPEN               = 0x64,
	PACKET_WINDOW_CLOSE              = 0x65,
	PACKET_WINDOW_CLICK              = 0x66,
	PACKET_INVENTORY_SLOT            = 0x67,
	PACKET_INVENTORY_WHOLE           = 0x68,
	PACKET_INVENTORY_PROGRESS        = 0x69,
	PACKET_CREATIVE_INVENTORY_ACTION = 0x6B,
	PACKET_UPDATE_SIGN               = 0x82,
	PACKET_PLAYER_LIST_ITEM          = 0xC9,
	PACKET_PLAYER_ABILITIES          = 0xca,
	PACKET_PING                      = 0xfe,
	PACKET_DISCONNECT                = 0xff
} ;





cProtocol125::cProtocol125(cClientHandle * a_Client) :
	super(a_Client),
	m_ReceivedData(64 KiB)
{
}





void cProtocol125::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2)
{
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_BLOCK_ACTION);
	WriteInt  (a_BlockX);
	WriteShort((short)a_BlockY);
	WriteInt  (a_BlockZ);
	WriteByte (a_Byte1);
	WriteByte (a_Byte2);
	Flush();
}





void cProtocol125::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_BLOCK_CHANGE);
	WriteInt (a_BlockX);
	WriteByte(a_BlockY);
	WriteInt (a_BlockZ);
	WriteByte(a_BlockType);
	WriteByte(a_BlockMeta);
	Flush();
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
	
	WriteByte (PACKET_MULTI_BLOCK);
	WriteInt  (a_ChunkX);
	WriteInt  (a_ChunkZ);
	WriteShort((unsigned short)a_Changes.size());
	WriteInt  (sizeof(int) * a_Changes.size());
	for (sSetBlockVector::const_iterator itr = a_Changes.begin(), end = a_Changes.end(); itr != end; ++itr)
	{
		unsigned int Coords = itr->y | (itr->z << 8) | (itr->x << 12);
		unsigned int Blocks = itr->BlockMeta | (itr->BlockType << 4);
		WriteInt(Coords << 16 | Blocks);
	}
	Flush();
}





void cProtocol125::SendChat(const AString & a_Message)
{
	cCSLock Lock(m_CSPacket);
	WriteByte  (PACKET_CHAT);
	WriteString(a_Message);
	Flush();
}





void cProtocol125::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	cCSLock Lock(m_CSPacket);

	// Send the pre-chunk:
	SendPreChunk(a_ChunkX, a_ChunkZ, true);
	
	// Send the chunk data:
	AString Serialized = a_Serializer.Serialize(cChunkDataSerializer::RELEASE_1_2_5);
	WriteByte(PACKET_MAP_CHUNK);
	WriteInt (a_ChunkX);
	WriteInt (a_ChunkZ);
	SendData(Serialized.data(), Serialized.size());
	Flush();
}





void cProtocol125::SendCollectPickup(const cPickup & a_Pickup, const cPlayer & a_Player)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_COLLECT_PICKUP);
	WriteInt (a_Pickup.GetUniqueID());
	WriteInt (a_Player.GetUniqueID());
	Flush();
}





void cProtocol125::SendDestroyEntity(const cEntity & a_Entity)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_DESTROY_ENTITY);
	WriteInt (a_Entity.GetUniqueID());
	Flush();
}





void cProtocol125::SendDisconnect(const AString & a_Reason)
{
	cCSLock Lock(m_CSPacket);
	WriteByte  ((unsigned char)PACKET_DISCONNECT);
	WriteString(a_Reason);
	Flush();
}





void cProtocol125::SendEntHeadLook(const cEntity & a_Entity)
{
	ASSERT(a_Entity.GetUniqueID() != m_Client->GetPlayer()->GetUniqueID());  // Must not send for self
	
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_ENT_HEAD_LOOK);
	WriteInt (a_Entity.GetUniqueID());
	WriteByte((char)((a_Entity.GetRotation() / 360.f) * 256));
	Flush();
}





void cProtocol125::SendEntLook(const cEntity & a_Entity)
{
	ASSERT(a_Entity.GetUniqueID() != m_Client->GetPlayer()->GetUniqueID());  // Must not send for self
	
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_ENT_LOOK);
	WriteInt (a_Entity.GetUniqueID());
	WriteByte((char)((a_Entity.GetRotation() / 360.f) * 256));
	WriteByte((char)((a_Entity.GetPitch()    / 360.f) * 256));
	Flush();
}





void cProtocol125::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_ENTITY_EQUIPMENT);
	WriteInt  (a_Entity.GetUniqueID());
	WriteShort(a_SlotNum);
	WriteShort(a_Item.m_ItemType);
	WriteShort(a_Item.m_ItemDamage);
	Flush();
}





void cProtocol125::SendEntityStatus(const cEntity & a_Entity, char a_Status)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_ENT_STATUS);
	WriteInt (a_Entity.GetUniqueID());
	WriteByte(a_Status);
	Flush();
}





void cProtocol125::SendGameMode(eGameMode a_GameMode)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_CHANGE_GAME_STATE);
	WriteByte(3);
	WriteByte((char)a_GameMode);
	Flush();
}





void cProtocol125::SendHandshake(const AString & a_ConnectionHash)
{
	cCSLock Lock(m_CSPacket);
	WriteByte  (PACKET_HANDSHAKE);
	WriteString(a_ConnectionHash);
	Flush();
}





void cProtocol125::SendHealth(void)
{
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_UPDATE_HEALTH);
	WriteShort((short)m_Client->GetPlayer()->GetHealth());
	WriteShort(m_Client->GetPlayer()->GetFoodLevel());
	WriteFloat(m_Client->GetPlayer()->GetFoodSaturationLevel());
	Flush();
}





void cProtocol125::SendInventoryProgress(char a_WindowID, short a_ProgressBar, short a_Value)
{
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_INVENTORY_PROGRESS);
	WriteByte (a_WindowID);
	WriteShort(a_ProgressBar);
	WriteShort(a_Value);
	Flush();
}





void cProtocol125::SendInventorySlot(int a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_INVENTORY_SLOT);
	WriteByte (a_WindowID);
	WriteShort(a_SlotNum);
	WriteItem (a_Item);
	Flush();
}





void cProtocol125::SendKeepAlive(int a_PingID)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_KEEP_ALIVE);
	WriteInt (a_PingID);
}





void cProtocol125::SendLogin(const cPlayer & a_Player)
{
	cCSLock Lock(m_CSPacket);

	WriteByte  (E_LOGIN);
	WriteInt   (a_Player.GetUniqueID());  // EntityID of the player
	WriteString("");  // Username, not used
	WriteString("DEFAULT");  // Level type
	WriteInt   ((int)a_Player.GetGameMode());
	WriteInt   (0);  // TODO: Dimension (Nether / Overworld / End)
	WriteByte  (2);  // TODO: Difficulty
	WriteByte  (0);  // Unused
	WriteByte  (60);  // Client list width or something
	Flush();
}





void cProtocol125::SendMetadata(const cEntity & a_Entity)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_METADATA);
	WriteInt (a_Entity.GetUniqueID());
	AString MetaData = GetEntityMetaData(a_Entity);
	SendData(MetaData.data(), MetaData.size());
	Flush();
}





void cProtocol125::SendPickupSpawn(const cPickup & a_Pickup)
{
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_PICKUP_SPAWN);
	WriteInt  (a_Pickup.GetUniqueID());
	WriteShort(a_Pickup.GetItem()->m_ItemType);
	WriteByte (a_Pickup.GetItem()->m_ItemCount);
	WriteShort(a_Pickup.GetItem()->m_ItemDamage);
	WriteInt  ((int)(a_Pickup.GetPosX() * 32));
	WriteInt  ((int)(a_Pickup.GetPosY() * 32));
	WriteInt  ((int)(a_Pickup.GetPosZ() * 32));
	WriteByte ((char)(a_Pickup.GetSpeed().x * 8));
	WriteByte ((char)(a_Pickup.GetSpeed().y * 8));
	WriteByte ((char)(a_Pickup.GetSpeed().z * 8));
	Flush();
}





void cProtocol125::SendPlayerAnimation(const cPlayer & a_Player, char a_Animation)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_ANIMATION);
	WriteInt (a_Player.GetUniqueID());
	WriteByte(a_Animation);
	Flush();
}





void cProtocol125::SendPlayerListItem(const cPlayer & a_Player, bool a_IsOnline)
{
	cCSLock Lock(m_CSPacket);
	AString PlayerName(a_Player.GetColor());
	PlayerName.append(a_Player.GetName());
	if (PlayerName.length() > 14)
	{
		PlayerName.erase(14);
	}
	PlayerName += cChatColor::White;

	WriteByte  ((unsigned char)PACKET_PLAYER_LIST_ITEM);
	WriteString(PlayerName);
	WriteBool  (a_IsOnline);
	WriteShort (a_Player.GetClientHandle()->GetPing());
	Flush();
}





void cProtocol125::SendPlayerMoveLook(void)
{
	cCSLock Lock(m_CSPacket);

	/*
	LOGD("Sending PlayerMoveLook: {%0.2f, %0.2f, %0.2f}, stance %0.2f, OnGround: %d",
		m_Player->GetPosX(), m_Player->GetPosY(), m_Player->GetPosZ(), m_Player->GetStance(), m_Player->IsOnGround() ? 1 : 0
	);
	*/

	WriteByte	(PACKET_PLAYERMOVELOOK);
	cPlayer * Player = m_Client->GetPlayer();
	WriteDouble(Player->GetPosX());
	WriteDouble(Player->GetStance() + 0.03);  // Add a small amount so that the player doesn't start inside a block
	WriteDouble(Player->GetPosY()   + 0.03);  // Add a small amount so that the player doesn't start inside a block
	WriteDouble(Player->GetPosZ());
	WriteFloat (Player->GetRotation());
	WriteFloat (Player->GetPitch());
	WriteBool  (Player->IsOnGround());
	Flush();
}





void cProtocol125::SendPlayerPosition(void)
{
	cCSLock Lock(m_CSPacket);
	LOGD("Ignore send PlayerPos");  // PlayerPos is a C->S packet only now
}





void cProtocol125::SendPlayerSpawn(const cPlayer & a_Player)
{
	const cItem & HeldItem = a_Player.GetEquippedItem();
	cCSLock Lock(m_CSPacket);
	WriteByte	(PACKET_PLAYER_SPAWN);
	WriteInt   (a_Player.GetUniqueID());
	WriteString(a_Player.GetName());
	WriteInt   ((int)(a_Player.GetPosX() * 32));
	WriteInt   ((int)(a_Player.GetPosY() * 32));
	WriteInt   ((int)(a_Player.GetPosZ() * 32));
	WriteByte  ((char)((a_Player.GetRot().x / 360.f) * 256));
	WriteByte  ((char)((a_Player.GetRot().y / 360.f) * 256));
	WriteShort (HeldItem.IsEmpty() ? 0 : HeldItem.m_ItemType);
	Flush();
}





void cProtocol125::SendRelEntMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(a_Entity.GetUniqueID() != m_Client->GetPlayer()->GetUniqueID());  // Must not send for self
	
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_REL_ENT_MOVE);
	WriteInt (a_Entity.GetUniqueID());
	WriteByte(a_RelX);
	WriteByte(a_RelY);
	WriteByte(a_RelZ);
	Flush();
}





void cProtocol125::SendRelEntMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(a_Entity.GetUniqueID() != m_Client->GetPlayer()->GetUniqueID());  // Must not send for self
	
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_REL_ENT_MOVE_LOOK);
	WriteInt (a_Entity.GetUniqueID());
	WriteByte(a_RelX);
	WriteByte(a_RelY);
	WriteByte(a_RelZ);
	WriteByte((char)((a_Entity.GetRotation() / 360.f) * 256));
	WriteByte((char)((a_Entity.GetPitch()    / 360.f) * 256));
	Flush();
}





void cProtocol125::SendRespawn(void)
{
	cCSLock Lock(m_CSPacket);
	WriteByte  (PACKET_RESPAWN);
	WriteInt   (0);  // TODO: Dimension;  0 = Overworld
	WriteByte  (2);  // TODO: Difficulty; 2 = Normal
	WriteByte  ((char)m_Client->GetPlayer()->GetGameMode());
	WriteShort (256);  // Current world height
	WriteString("DEFAULT");
}





void cProtocol125::SendSpawnMob(const cMonster & a_Mob)
{
	cCSLock Lock(m_CSPacket);
	Vector3i Pos = (Vector3i)(a_Mob.GetPosition() * 32);
	WriteByte(PACKET_SPAWN_MOB);
	WriteInt (a_Mob.GetUniqueID());
	WriteByte(a_Mob.GetMobType());
	WriteInt (Pos.x);
	WriteInt (Pos.y);
	WriteInt (Pos.z);
	WriteByte(0);
	WriteByte(0);
	WriteByte(0);
	AString MetaData = GetEntityMetaData(a_Mob);
	SendData (MetaData.data(), MetaData.size());
	Flush();
}





void cProtocol125::SendTeleportEntity(const cEntity & a_Entity)
{
	cCSLock Lock(m_CSPacket);
	WriteByte   (PACKET_ENT_TELEPORT);
	WriteInt    (a_Entity.GetUniqueID());
	WriteVectorI((Vector3i)(a_Entity.GetPosition() * 32));
	WriteByte   ((char)((a_Entity.GetRotation() / 360.f) * 256));
	WriteByte   ((char)((a_Entity.GetPitch() / 360.f) * 256));
	Flush();
}





void cProtocol125::SendThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_THUNDERBOLT);
	WriteInt (0x7fffffff);  // Entity ID of the thunderbolt; we use a constant one
	WriteBool(true);  // Unknown bool
	WriteInt (a_BlockX * 32);
	WriteInt (a_BlockY * 32);
	WriteInt (a_BlockZ * 32);
	Flush();
}





void cProtocol125::SendTimeUpdate(Int64 a_WorldTime)
{
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_UPDATE_TIME);
	WriteInt64(a_WorldTime);
	Flush();
}





void cProtocol125::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSPacket);
	SendPreChunk(a_ChunkX, a_ChunkZ, false);
}





void cProtocol125::SendUpdateSign(
	int a_BlockX, int a_BlockY, int a_BlockZ,
	const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4
)
{
	cCSLock Lock(m_CSPacket);
	WriteByte  ((unsigned char)PACKET_UPDATE_SIGN);
	WriteInt   (a_BlockX);
	WriteShort ((short)a_BlockY);
	WriteInt   (a_BlockZ);
	WriteString(a_Line1);
	WriteString(a_Line2);
	WriteString(a_Line3);
	WriteString(a_Line4);
	Flush();
}





void cProtocol125::SendWeather(eWeather a_Weather)
{
	cCSLock Lock(m_CSPacket);
	switch( a_Weather )
	{
		case eWeather_Sunny:
		{
			WriteByte(PACKET_CHANGE_GAME_STATE);
			WriteByte(2);  // Stop rain
			WriteByte(0);  // Unused
			Flush();
			break;
		}
		
		case eWeather_Rain:
		case eWeather_ThunderStorm:
		{
			WriteByte(PACKET_CHANGE_GAME_STATE);
			WriteByte(1);  // Begin rain
			WriteByte(0);  // Unused
			Flush();
			break;
		}
	}
}





void cProtocol125::SendWholeInventory(const cInventory & a_Inventory)
{
	cCSLock Lock(m_CSPacket);
	SendWholeInventory(0, a_Inventory.c_NumSlots, a_Inventory.GetSlots());
}





void cProtocol125::SendWholeInventory(const cWindow & a_Window)
{
	cCSLock Lock(m_CSPacket);
	SendWholeInventory(
		a_Window.GetWindowID(), 
		a_Window.GetNumSlots(),
		a_Window.GetSlots()
	);
}





void cProtocol125::SendWindowClose(char a_WindowID)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_WINDOW_CLOSE);
	WriteByte(a_WindowID);
	Flush();
}





void cProtocol125::SendWindowOpen(char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots)
{
	cCSLock Lock(m_CSPacket);
	WriteByte  (PACKET_WINDOW_OPEN);
	WriteByte  (a_WindowID);
	WriteByte  (a_WindowType);
	WriteString(a_WindowTitle);
	WriteByte  (a_NumSlots);
	Flush();
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
		case PACKET_ANIMATION:                 return ParseArmAnim();
		case PACKET_BLOCK_DIG:                 return ParseBlockDig();
		case PACKET_BLOCK_PLACE:               return ParseBlockPlace();
		case PACKET_CHAT:                      return ParseChat();
		case PACKET_CREATIVE_INVENTORY_ACTION: return ParseCreativeInventoryAction();
		case PACKET_DISCONNECT:                return ParseDisconnect();
		case PACKET_FLYING:                    return ParseFlying();
		case PACKET_HANDSHAKE:                 return ParseHandshake();
		case PACKET_KEEP_ALIVE:                return ParseKeepAlive();
		case PACKET_LOGIN:                     return ParseLogin();
		case PACKET_PACKET_ENTITY_ACTION:      return ParseEntityAction();
		case PACKET_PING:                      return ParsePing();
		case PACKET_PLAYERLOOK:                return ParsePlayerLook();
		case PACKET_PLAYERMOVELOOK:            return ParsePlayerMoveLook();
		case PACKET_PLAYERPOS:                 return ParsePlayerPosition();
		case PACKET_PLAYER_ABILITIES:          return ParsePlayerAbilities();
		case PACKET_RESPAWN:                   return ParseRespawn();
		case PACKET_SLOT_SELECTED:             return ParseSlotSelected();
		case PACKET_UPDATE_SIGN:               return ParseUpdateSign();
		case PACKET_USE_ENTITY:                return ParseUseEntity();
		case PACKET_WINDOW_CLICK:              return ParseWindowClick();
		case PACKET_WINDOW_CLOSE:              return ParseWindowClose();
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





int cProtocol125::ParseSlotSelected(void)
{
	cPacket_ItemSwitch its;
	HANDLE_PACKET_PARSE(its);
	m_Client->HandleSlotSelected(its.m_SlotNum);
	return PACKET_OK;
}





int cProtocol125::ParseCreativeInventoryAction(void)
{
	cPacket_CreativeInventoryAction cia;
	HANDLE_PACKET_PARSE(cia);
	m_Client->HandleCreativeInventory(cia.m_SlotNum, cia.m_ClickedItem);
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






void cProtocol125::SendPreChunk(int a_ChunkX, int a_ChunkZ, bool a_ShouldLoad)
{
	WriteByte(E_PRE_CHUNK);
	WriteInt (a_ChunkX);
	WriteInt (a_ChunkZ);
	WriteBool(a_ShouldLoad);
	Flush();
}





void cProtocol125::SendWholeInventory(char a_WindowID, int a_NumItems, const cItem * a_Items)
{
	WriteByte (PACKET_INVENTORY_WHOLE);
	WriteByte (a_WindowID);
	WriteShort((short)a_NumItems);

	for (int j = 0; j < a_NumItems; j++)
	{
		WriteItem(a_Items[j]);
	}
	Flush();
}





void cProtocol125::WriteItem(const cItem & a_Item)
{
	short ItemType = a_Item.m_ItemType;
	ASSERT(ItemType >= -1);  // Check validity of packets in debug runtime
	if (ItemType <= 0)
	{
		// Fix, to make sure no invalid values are sent.
		ItemType = -1;
	}
	
	WriteShort(ItemType);
	if (a_Item.IsEmpty())
	{
		return;
	}
	
	WriteByte (a_Item.m_ItemCount);
	WriteShort(a_Item.m_ItemDamage);
	
	if (cItem::IsEnchantable(a_Item.m_ItemType))
	{
		// TODO: Implement enchantments
		WriteShort(-1);
	}
}





AString cProtocol125::GetEntityMetaData(const cEntity & a_Entity)
{
	// We should send all the metadata here
	AString MetaData;
	// Common metadata (index 0, byte):
	MetaData.push_back(0);
	MetaData.push_back(GetEntityMetadataFlags(a_Entity));
	
	// TODO: Add more entity-specific metadata
	
	MetaData.push_back(0x7f);  // End metadata
	return MetaData;
}





char cProtocol125::GetEntityMetadataFlags(const cEntity & a_Entity)
{
	char Flags = 0;
	if (a_Entity.IsOnFire())
	{
		Flags |= 1;
	}
	if (a_Entity.IsCrouched())
	{
		Flags |= 2;
	}
	if (a_Entity.IsRiding())
	{
		Flags |= 4;
	}
	if (a_Entity.IsSprinting())
	{
		Flags |= 8;
	}
	if (a_Entity.IsRclking())
	{
		Flags |= 16;
	}
	return Flags;
}




