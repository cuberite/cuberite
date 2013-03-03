
// Protocol125.cpp

// Implements the cProtocol125 class representing the release 1.2.5 protocol (#29)
/*
Documentation:
	- protocol: http://wiki.vg/wiki/index.php?title=Protocol&oldid=2513
	- session handling: http://wiki.vg/wiki/index.php?title=Session&oldid=2262
	- slot format: http://wiki.vg/wiki/index.php?title=Slot_Data&oldid=2152
*/

#include "Globals.h"

#include "Protocol125.h"

#include "../ClientHandle.h"
#include "ChunkDataSerializer.h"
#include "../Entity.h"
#include "../Mobs/Monster.h"
#include "../Pickup.h"
#include "../Player.h"
#include "../ChatColor.h"
#include "../UI/Window.h"
#include "../Root.h"
#include "../Server.h"
#include "../FallingBlock.h"





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
	PACKET_PLAYER_ON_GROUND          = 0x0a,
	PACKET_PLAYER_POS                = 0x0b,
	PACKET_PLAYER_LOOK               = 0x0c,
	PACKET_PLAYER_MOVE_LOOK          = 0x0d,
	PACKET_BLOCK_DIG                 = 0x0e,
	PACKET_BLOCK_PLACE               = 0x0f,
	PACKET_SLOT_SELECTED             = 0x10,
	PACKET_USE_BED                   = 0x11,
	PACKET_ANIMATION                 = 0x12,
	PACKET_PACKET_ENTITY_ACTION      = 0x13,
	PACKET_PLAYER_SPAWN              = 0x14,
	PACKET_PICKUP_SPAWN              = 0x15,
	PACKET_COLLECT_PICKUP            = 0x16,
	PACKET_SPAWN_OBJECT              = 0x17,
	PACKET_SPAWN_MOB                 = 0x18,
	PACKET_DESTROY_ENTITY            = 0x1d,
	PACKET_ENTITY                    = 0x1e,
	PACKET_ENT_REL_MOVE              = 0x1f,
	PACKET_ENT_LOOK                  = 0x20,
	PACKET_ENT_REL_MOVE_LOOK         = 0x21,
	PACKET_ENT_TELEPORT              = 0x22,
	PACKET_ENT_HEAD_LOOK             = 0x23,
	PACKET_ENT_STATUS                = 0x26,
	PACKET_ATTACH_ENTITY             = 0x27,
	PACKET_METADATA                  = 0x28,
	PACKET_PRE_CHUNK                 = 0x32,
	PACKET_MAP_CHUNK                 = 0x33,
	PACKET_MULTI_BLOCK               = 0x34,
	PACKET_BLOCK_CHANGE              = 0x35,
	PACKET_BLOCK_ACTION              = 0x36,
	PACKET_EXPLOSION                 = 0x3C,
	PACKET_SOUND_EFFECT              = 0x3e,
	PACKET_SOUND_PARTICLE_EFFECT     = 0x3d,
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





#define HANDLE_PACKET_READ(Proc, Type, Var) \
	Type Var; \
	{ \
		if (!m_ReceivedData.Proc(Var)) \
		{ \
			m_ReceivedData.CheckValid(); \
			return PARSE_INCOMPLETE; \
		} \
		m_ReceivedData.CheckValid(); \
	}




typedef unsigned char Byte;





cProtocol125::cProtocol125(cClientHandle * a_Client) :
	super(a_Client),
	m_ReceivedData(32 KiB)
{
}





void cProtocol125::SendAttachEntity(const cEntity & a_Entity, const cEntity * a_Vehicle)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_ATTACH_ENTITY);
	WriteInt(a_Entity.GetUniqueID());
	WriteInt((a_Vehicle == NULL) ? -1 : a_Vehicle->GetUniqueID());
	Flush();
}





void cProtocol125::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType)
{
	UNUSED(a_BlockType);
	
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_BLOCK_ACTION);
	WriteInt  (a_BlockX);
	WriteShort((short)a_BlockY);
	WriteInt  (a_BlockZ);
	WriteByte (a_Byte1);
	WriteByte (a_Byte2);
	Flush();
}





void cProtocol125::SendBlockBreakAnim(int a_entityID, int a_BlockX, int a_BlockY, int a_BlockZ, char stage)
{
	// Not supported in this protocol version
}





void cProtocol125::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_BLOCK_CHANGE);
	WriteInt (a_BlockX);
	WriteByte((unsigned char)a_BlockY);
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
	WriteUInt (sizeof(int) * a_Changes.size());
	for (sSetBlockVector::const_iterator itr = a_Changes.begin(), end = a_Changes.end(); itr != end; ++itr)
	{
		unsigned int Coords = itr->y | (itr->z << 8) | (itr->x << 12);
		unsigned int Blocks = itr->BlockMeta | (itr->BlockType << 4);
		WriteUInt(Coords << 16 | Blocks);
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





void cProtocol125::SendEntRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(a_Entity.GetUniqueID() != m_Client->GetPlayer()->GetUniqueID());  // Must not send for self
	
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_ENT_REL_MOVE);
	WriteInt (a_Entity.GetUniqueID());
	WriteByte(a_RelX);
	WriteByte(a_RelY);
	WriteByte(a_RelZ);
	Flush();
}





void cProtocol125::SendEntRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(a_Entity.GetUniqueID() != m_Client->GetPlayer()->GetUniqueID());  // Must not send for self
	
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_ENT_REL_MOVE_LOOK);
	WriteInt (a_Entity.GetUniqueID());
	WriteByte(a_RelX);
	WriteByte(a_RelY);
	WriteByte(a_RelZ);
	WriteByte((char)((a_Entity.GetRotation() / 360.f) * 256));
	WriteByte((char)((a_Entity.GetPitch()    / 360.f) * 256));
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





void cProtocol125::SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item)
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
	Flush();
}





void cProtocol125::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	UNUSED(a_World);
	cCSLock Lock(m_CSPacket);

	WriteByte  (PACKET_LOGIN);
	WriteInt   (a_Player.GetUniqueID());  // EntityID of the player
	WriteString("");  // Username, not used
	WriteString("default");  // Level type
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
	WriteByte   (PACKET_PICKUP_SPAWN);
	WriteInt    (a_Pickup.GetUniqueID());
	WriteShort  (a_Pickup.GetItem().m_ItemType);
	WriteByte   (a_Pickup.GetItem().m_ItemCount);
	WriteShort  (a_Pickup.GetItem().m_ItemDamage);
	WriteVectorI((Vector3i)(a_Pickup.GetPosition() * 32));
	WriteByte   ((char)(a_Pickup.GetSpeed().x * 8));
	WriteByte   ((char)(a_Pickup.GetSpeed().y * 8));
	WriteByte   ((char)(a_Pickup.GetSpeed().z * 8));
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

	WriteByte	(PACKET_PLAYER_MOVE_LOOK);
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
	WriteByte	 (PACKET_PLAYER_SPAWN);
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





void cProtocol125::SendRespawn(void)
{
	cCSLock Lock(m_CSPacket);
	WriteByte  (PACKET_RESPAWN);
	WriteInt   (0);  // TODO: Dimension;  0 = Overworld
	WriteByte  (2);  // TODO: Difficulty; 2 = Normal
	WriteByte  ((char)m_Client->GetPlayer()->GetGameMode());
	WriteShort (256);  // Current world height
	WriteString("default");
}





void cProtocol125::SendSoundEffect(const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch)
{
	// Not needed in this protocol version
}





void cProtocol125::SendSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
	// Not implemented in this protocol version
}





void cProtocol125::SendSpawnFallingBlock(const cFallingBlock & a_FallingBlock)
{
	// This protocol version implements falling blocks using the spawn object / vehicle packet:
	SendSpawnObject(a_FallingBlock, 70, a_FallingBlock.GetBlockType(), 0, 0);
}





void cProtocol125::SendSpawnMob(const cMonster & a_Mob)
{
	cCSLock Lock(m_CSPacket);
	WriteByte   (PACKET_SPAWN_MOB);
	WriteInt    (a_Mob.GetUniqueID());
	WriteByte   (a_Mob.GetMobType());
	WriteVectorI((Vector3i)(a_Mob.GetPosition() * 32));
	WriteByte   (0);
	WriteByte   (0);
	WriteByte   (0);
	AString MetaData = GetEntityMetaData(a_Mob);
	SendData (MetaData.data(), MetaData.size());
	Flush();
}





void cProtocol125::SendSpawnObject(const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch)
{
	UNUSED(a_Yaw);
	UNUSED(a_Pitch);
	
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_SPAWN_OBJECT);
	WriteInt (a_Entity.GetUniqueID());
	WriteByte(a_ObjectType);
	WriteInt ((int)(a_Entity.GetPosX() * 32));
	WriteInt ((int)(a_Entity.GetPosY() * 32));
	WriteInt ((int)(a_Entity.GetPosZ() * 32));
	WriteInt (a_ObjectData);
	if (a_ObjectData != 0)
	{
		WriteShort((short)a_Entity.GetSpeedX());
		WriteShort((short)a_Entity.GetSpeedY());
		WriteShort((short)a_Entity.GetSpeedZ());
	}
	Flush();
}





void cProtocol125::SendSpawnVehicle(const cEntity & a_Vehicle, char a_VehicleType)
{
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_SPAWN_OBJECT);
	WriteInt  (a_Vehicle.GetUniqueID());
	WriteByte (a_VehicleType);
	WriteInt  ((int)(a_Vehicle.GetPosX() * 32));
	WriteInt  ((int)(a_Vehicle.GetPosY() * 32));
	WriteInt  ((int)(a_Vehicle.GetPosZ() * 32));
	WriteInt  (1);
	WriteShort(0);  // TODO: SpeedX
	WriteShort(0);  // TODO: SpeedY
	WriteShort(0);  // TODO: SpeedZ
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





void cProtocol125::SendTimeUpdate(Int64 a_WorldAge, Int64 a_TimeOfDay)
{
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_UPDATE_TIME);
	// Use a_WorldAge for daycount, and a_TimeOfDay for the proper time of day:
	WriteInt64((24000 * (a_WorldAge / 24000)) + (a_TimeOfDay % 24000));
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





void cProtocol125::SendUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ )
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_USE_BED);
	WriteInt (a_Entity.GetUniqueID());
	WriteByte(0);	// Unknown byte only 0 has been observed
	WriteInt (a_BlockX);
	WriteByte(a_BlockY);
	WriteInt (a_BlockZ);
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
	SendWindowSlots(0, a_Inventory.c_NumSlots, a_Inventory.GetSlots());
}





void cProtocol125::SendWholeInventory(const cWindow & a_Window)
{
	cCSLock Lock(m_CSPacket);
	cItems Slots;
	a_Window.GetSlots(*(m_Client->GetPlayer()), Slots);
	SendWindowSlots(a_Window.GetWindowID(), Slots.size(), &(Slots[0]));
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
	if (a_WindowType < 0)
	{
		// Do not send for inventory windows
		return;
	}
	cCSLock Lock(m_CSPacket);
	WriteByte  (PACKET_WINDOW_OPEN);
	WriteByte  (a_WindowID);
	WriteByte  (a_WindowType);
	WriteString(a_WindowTitle);
	WriteByte  (a_NumSlots);
	Flush();
}





AString cProtocol125::GetAuthServerID(void)
{
	// http://wiki.vg/wiki/index.php?title=Session&oldid=2262
	// The server generates a random hash and that is used for all clients, unmodified
	return cRoot::Get()->GetServer()->GetServerID();
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
			case PARSE_UNKNOWN:
			{
				// An unknown packet has been received, notify the client and abort:
				m_Client->PacketUnknown(PacketType);
				return;
			}
			case PARSE_ERROR:
			{
				// An error occurred while parsing a known packet, notify the client and abort:
				m_Client->PacketError(PacketType);
				return;
			}
			case PARSE_INCOMPLETE:
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
		default: return PARSE_UNKNOWN;
		case PACKET_ANIMATION:                 return ParseArmAnim();
		case PACKET_BLOCK_DIG:                 return ParseBlockDig();
		case PACKET_BLOCK_PLACE:               return ParseBlockPlace();
		case PACKET_CHAT:                      return ParseChat();
		case PACKET_CREATIVE_INVENTORY_ACTION: return ParseCreativeInventoryAction();
		case PACKET_DISCONNECT:                return ParseDisconnect();
		case PACKET_HANDSHAKE:                 return ParseHandshake();
		case PACKET_KEEP_ALIVE:                return ParseKeepAlive();
		case PACKET_LOGIN:                     return ParseLogin();
		case PACKET_PACKET_ENTITY_ACTION:      return ParseEntityAction();
		case PACKET_PING:                      return ParsePing();
		case PACKET_PLAYER_ABILITIES:          return ParsePlayerAbilities();
		case PACKET_PLAYER_LOOK:               return ParsePlayerLook();
		case PACKET_PLAYER_MOVE_LOOK:          return ParsePlayerMoveLook();
		case PACKET_PLAYER_ON_GROUND:          return ParsePlayerOnGround();
		case PACKET_PLAYER_POS:                return ParsePlayerPosition();
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





int cProtocol125::ParseArmAnim(void)
{
	HANDLE_PACKET_READ(ReadBEInt, int,  EntityID);
	HANDLE_PACKET_READ(ReadChar,  char, Animation);
	m_Client->HandleAnimation(Animation);
	return PARSE_OK;
}





int cProtocol125::ParseBlockDig(void)
{
	HANDLE_PACKET_READ(ReadChar,	char, Status);
	HANDLE_PACKET_READ(ReadBEInt, int,  PosX);
	HANDLE_PACKET_READ(ReadByte,	Byte, PosY);
	HANDLE_PACKET_READ(ReadBEInt, int,  PosZ);
	HANDLE_PACKET_READ(ReadChar,	char, BlockFace);
	m_Client->HandleLeftClick(PosX, PosY, PosZ, BlockFace, Status);
	return PARSE_OK;
}





int cProtocol125::ParseBlockPlace(void)
{
	HANDLE_PACKET_READ(ReadBEInt, int,  PosX);
	HANDLE_PACKET_READ(ReadByte,  Byte, PosY);
	HANDLE_PACKET_READ(ReadBEInt, int,  PosZ);
	HANDLE_PACKET_READ(ReadChar,  char, BlockFace);

	cItem HeldItem;
	int res = ParseItem(HeldItem);
	if (res < 0)
	{
		return res;
	}

	// 1.2.5 didn't have any cursor position, so use 8, 8, 8, so that halfslabs and stairs work correctly and the special value is recognizable.
	m_Client->HandleRightClick(PosX, PosY, PosZ, BlockFace, 8, 8, 8, HeldItem);
	return PARSE_OK;
}





int cProtocol125::ParseChat(void)
{
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Message);
	m_Client->HandleChat(Message);
	return PARSE_OK;
}





int cProtocol125::ParseCreativeInventoryAction(void)
{
	HANDLE_PACKET_READ(ReadBEShort, short, SlotNum);
	cItem HeldItem;
	int res = ParseItem(HeldItem);
	if (res < 0)
	{
		return res;
	}
	m_Client->HandleCreativeInventory(SlotNum, HeldItem);
	return PARSE_OK;
}





int cProtocol125::ParseDisconnect(void)
{
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Reason);
	m_Client->HandleDisconnect(Reason);
	return PARSE_OK;
}





int cProtocol125::ParseEntityAction(void)
{
	HANDLE_PACKET_READ(ReadBEInt, int,  EntityID);
	HANDLE_PACKET_READ(ReadChar,  char, ActionID);
	m_Client->HandleEntityAction(EntityID, ActionID);
	return PARSE_OK;
}





int cProtocol125::ParseHandshake(void)
{
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Username);

	AStringVector UserData = StringSplit(Username, ";"); // "FakeTruth;localhost:25565"
	if (UserData.empty())
	{
		m_Client->Kick("Did not receive username");
		return PARSE_OK;
	}
	m_Username = UserData[0];

	LOGD("HANDSHAKE %s", Username.c_str());

	if (!m_Client->HandleHandshake( m_Username ))
	{
		return PARSE_OK; // Player is not allowed into the server
	}

	SendHandshake(cRoot::Get()->GetServer()->GetServerID());
	LOGD("User \"%s\" was sent a handshake response", m_Username.c_str());

	return PARSE_OK;
}





int cProtocol125::ParseKeepAlive(void)
{
	HANDLE_PACKET_READ(ReadBEInt, int, KeepAliveID);
	m_Client->HandleKeepAlive(KeepAliveID);
	return PARSE_OK;
}





int cProtocol125::ParseLogin(void)
{
	HANDLE_PACKET_READ(ReadBEInt,           int,     ProtocolVersion);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Username);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, LevelType);
	HANDLE_PACKET_READ(ReadBEInt,           int,     ServerMode);
	HANDLE_PACKET_READ(ReadBEInt,           int,     Dimension);
	HANDLE_PACKET_READ(ReadChar,            char,    Difficulty);
	HANDLE_PACKET_READ(ReadByte,            Byte,    WorldHeight);
	HANDLE_PACKET_READ(ReadByte,            Byte,    MaxPlayers);

	if (ProtocolVersion < 29)
	{
		m_Client->Kick("Your client is outdated!");
		return PARSE_OK;
	}
	else if (ProtocolVersion > 29)
	{
		m_Client->Kick("Your client version is higher than the server!");
		return PARSE_OK;
	}

	if (m_Username.compare(Username) != 0)
	{
		LOGWARNING("Login Username (\"%s\") does not match Handshake username (\"%s\") for client @ \"%s\", kicking",
			Username.c_str(),
			m_Username.c_str(),
			m_Client->GetIPString().c_str()
		);
		m_Client->Kick("Hacked client");  // Don't tell them why we don't want them
		return PARSE_OK;
	}

	m_Client->HandleLogin(ProtocolVersion, Username);
	return PARSE_OK;
}





int cProtocol125::ParsePing(void)
{
	// Packet has no more data
	m_Client->HandlePing();
	return PARSE_OK;
}






int cProtocol125::ParsePlayerAbilities(void)
{
	HANDLE_PACKET_READ(ReadBool, bool, Invulnerable);
	HANDLE_PACKET_READ(ReadBool, bool, IsFlying);
	HANDLE_PACKET_READ(ReadBool, bool, CanFly);
	HANDLE_PACKET_READ(ReadBool, bool, InstaMine);
	// TODO: m_Client->HandlePlayerAbilities(...);
	return PARSE_OK;
}





int cProtocol125::ParsePlayerLook(void)
{
	HANDLE_PACKET_READ(ReadBEFloat, float, Rotation);
	HANDLE_PACKET_READ(ReadBEFloat, float, Pitch);
	HANDLE_PACKET_READ(ReadBool,    bool,  IsOnGround);
	m_Client->HandlePlayerLook(Rotation, Pitch, IsOnGround);
	return PARSE_OK;
}





int cProtocol125::ParsePlayerMoveLook(void)
{
	HANDLE_PACKET_READ(ReadBEDouble, double, PosX);
	HANDLE_PACKET_READ(ReadBEDouble, double, PosY);
	HANDLE_PACKET_READ(ReadBEDouble, double, Stance);
	HANDLE_PACKET_READ(ReadBEDouble, double, PosZ);
	HANDLE_PACKET_READ(ReadBEFloat,  float,  Rotation);
	HANDLE_PACKET_READ(ReadBEFloat,  float,  Pitch);
	HANDLE_PACKET_READ(ReadBool,     bool,   IsOnGround);
	// LOGD("Recv PML: {%0.2f, %0.2f, %0.2f}, Stance %0.2f, Gnd: %d", PosX, PosY, PosZ, Stance, IsOnGround ? 1 : 0);
	m_Client->HandlePlayerMoveLook(PosX, PosY, PosZ, Stance, Rotation, Pitch, IsOnGround);
	return PARSE_OK;
}





int cProtocol125::ParsePlayerOnGround(void)
{
	HANDLE_PACKET_READ(ReadBool, bool, IsOnGround);
	// TODO: m_Client->HandleFlying(IsOnGround);
	return PARSE_OK;
}





int cProtocol125::ParsePlayerPosition(void)
{
	HANDLE_PACKET_READ(ReadBEDouble, double, PosX);
	HANDLE_PACKET_READ(ReadBEDouble, double, PosY);
	HANDLE_PACKET_READ(ReadBEDouble, double, Stance);
	HANDLE_PACKET_READ(ReadBEDouble, double, PosZ);
	HANDLE_PACKET_READ(ReadBool,     bool,   IsOnGround);
	m_Client->HandlePlayerPos(PosX, PosY, PosZ, Stance, IsOnGround);
	return PARSE_OK;
}





int cProtocol125::ParseRespawn(void)
{
	HANDLE_PACKET_READ(ReadBEInt,           int,     Dimension);
	HANDLE_PACKET_READ(ReadChar,            char,    Difficulty);
	HANDLE_PACKET_READ(ReadChar,            char,    CreativeMode);
	HANDLE_PACKET_READ(ReadBEShort,         short,   WorldHeight);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, LevelType);
	m_Client->HandleRespawn();
	return PARSE_OK;
}





int cProtocol125::ParseSlotSelected(void)
{
	HANDLE_PACKET_READ(ReadBEShort, short, SlotNum);
	m_Client->HandleSlotSelected(SlotNum);
	return PARSE_OK;
}





int cProtocol125::ParseUpdateSign(void)
{
	HANDLE_PACKET_READ(ReadBEInt,           int,     BlockX);
	HANDLE_PACKET_READ(ReadBEShort,         short,   BlockY);
	HANDLE_PACKET_READ(ReadBEInt,           int,     BlockZ);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Line1);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Line2);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Line3);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Line4);
	m_Client->HandleUpdateSign(BlockX, BlockY, BlockZ, Line1, Line2, Line3, Line4);
	return PARSE_OK;
}





int cProtocol125::ParseUseEntity(void)
{
	HANDLE_PACKET_READ(ReadBEInt, int,  SourceEntityID);
	HANDLE_PACKET_READ(ReadBEInt, int,  TargetEntityID);
	HANDLE_PACKET_READ(ReadBool,  bool, IsLeftClick);
	m_Client->HandleUseEntity(TargetEntityID, IsLeftClick);
	return PARSE_OK;
}





int cProtocol125::ParseWindowClick(void)
{
	HANDLE_PACKET_READ(ReadChar,    char,  WindowID);
	HANDLE_PACKET_READ(ReadBEShort, short, SlotNum);
	HANDLE_PACKET_READ(ReadBool,    bool,  IsRightClick);
	HANDLE_PACKET_READ(ReadBEShort, short, TransactionID);
	HANDLE_PACKET_READ(ReadBool,    bool,  IsShiftPressed);
	cItem HeldItem;
	int res = ParseItem(HeldItem);
	if (res < 0)
	{
		return res;
	}
	m_Client->HandleWindowClick(WindowID, SlotNum, IsRightClick, IsShiftPressed, HeldItem);
	return PARSE_OK;
}





int cProtocol125::ParseWindowClose(void)
{
	HANDLE_PACKET_READ(ReadChar, char, WindowID);
	m_Client->HandleWindowClose(WindowID);
	return PARSE_OK;
}





void cProtocol125::SendPreChunk(int a_ChunkX, int a_ChunkZ, bool a_ShouldLoad)
{
	WriteByte(PACKET_PRE_CHUNK);
	WriteInt (a_ChunkX);
	WriteInt (a_ChunkZ);
	WriteBool(a_ShouldLoad);
	Flush();
}





void cProtocol125::SendWindowSlots(char a_WindowID, int a_NumItems, const cItem * a_Items)
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





int cProtocol125::ParseItem(cItem & a_Item)
{
	HANDLE_PACKET_READ(ReadBEShort, short, ItemType);

	if (ItemType <= -1)
	{
		a_Item.Empty();
		return PARSE_OK;
	}
	a_Item.m_ItemType = ItemType;

	HANDLE_PACKET_READ(ReadChar,    char,  ItemCount);
	HANDLE_PACKET_READ(ReadBEShort, short, ItemDamage);
	a_Item.m_ItemCount  = ItemCount;
	a_Item.m_ItemDamage = ItemDamage;
	if (ItemCount <= 0)
	{
		a_Item.Empty();
	}

	if (!cItem::IsEnchantable(ItemType))
	{
		return PARSE_OK;
	}
	
	HANDLE_PACKET_READ(ReadBEShort, short, EnchantNumBytes);
	
	if (EnchantNumBytes <= 0)
	{
		return PARSE_OK;
	}
		
	// TODO: Enchantment not implemented yet!
	if (!m_ReceivedData.SkipRead(EnchantNumBytes))
	{
		return PARSE_INCOMPLETE;
	}
	
	return PARSE_OK;
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




