
// Protocol14x.cpp

/*
Implements the 1.4.x protocol classes representing these protocols:
- cProtocol142:
	- release 1.4.2 protocol (#47)
	- release 1.4.4 protocol (#49) - the same protocol class is used, because the only difference is in a packet that MCServer doesn't implement yet (ITEM_DATA)
	- release 1.4.5 protocol (same as 1.4.4)
- cProtocol146:
	- release 1.4.6 protocol (#51)
*/

#include "Globals.h"
#include "Protocol14x.h"
#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"
#include "../../CryptoPP/randpool.h"
#include "../Item.h"
#include "ChunkDataSerializer.h"
#include "../Player.h"
#include "../Mobs/Monster.h"
#include "../UI/Window.h"
#include "../Pickup.h"
#include "../FallingBlock.h"





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





enum
{
	PACKET_UPDATE_TIME           = 0x04,
	PACKET_PICKUP_SPAWN          = 0x15,
	PACKET_SPAWN_OBJECT          = 0x17,
	PACKET_ENTITY_METADATA       = 0x28,
	PACKET_SOUND_PARTICLE_EFFECT = 0x3d
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProtocol142:

cProtocol142::cProtocol142(cClientHandle * a_Client) :
	super(a_Client)
{
}





int cProtocol142::ParseLocaleViewDistance(void)
{
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Locale);
	HANDLE_PACKET_READ(ReadChar,            char,    ViewDistance);
	HANDLE_PACKET_READ(ReadChar,            char,    ChatFlags);
	HANDLE_PACKET_READ(ReadChar,            char,    ClientDifficulty);
	HANDLE_PACKET_READ(ReadChar,            char,    ShouldShowCape);  // <-- new in 1.4.2
	// TODO: m_Client->HandleLocale(Locale);
	// TODO: m_Client->HandleViewDistance(ViewDistance);
	// TODO: m_Client->HandleChatFlags(ChatFlags);
	// Ignoring client difficulty
	return PARSE_OK;
}





void cProtocol142::SendPickupSpawn(const cPickup & a_Pickup)
{
	cCSLock Lock(m_CSPacket);
	WriteByte   (PACKET_PICKUP_SPAWN);
	WriteInt    (a_Pickup.GetUniqueID());
	WriteItem   (*(a_Pickup.GetItem()));
	WriteVectorI((Vector3i)(a_Pickup.GetPosition() * 32));
	WriteByte   ((char)(a_Pickup.GetSpeed().x * 8));
	WriteByte   ((char)(a_Pickup.GetSpeed().y * 8));
	WriteByte   ((char)(a_Pickup.GetSpeed().z * 8));
	Flush();
}





void cProtocol142::SendSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_SOUND_PARTICLE_EFFECT);
	WriteInt (a_EffectID);
	WriteInt (a_SrcX / 8);
	WriteByte(a_SrcY / 8);
	WriteInt (a_SrcZ / 8);
	WriteInt (a_Data);
	WriteBool(0);
	Flush();
}





void cProtocol142::SendTimeUpdate(Int64 a_WorldAge, Int64 a_TimeOfDay)
{
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_UPDATE_TIME);
	WriteInt64(a_WorldAge);
	WriteInt64(a_TimeOfDay);
	Flush();
}






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProtocol146:

cProtocol146::cProtocol146(cClientHandle * a_Client) :
	super(a_Client)
{
}





void cProtocol146::SendPickupSpawn(const cPickup & a_Pickup)
{
	ASSERT(!a_Pickup.GetItem()->IsEmpty());
	
	cCSLock Lock(m_CSPacket);

	// Send a SPAWN_OBJECT packet for the base entity:
	WriteByte(PACKET_SPAWN_OBJECT);
	WriteInt (a_Pickup.GetUniqueID());
	WriteByte(0x02);
	WriteInt ((int)(a_Pickup.GetPosX() * 32));
	WriteInt ((int)(a_Pickup.GetPosY() * 32));
	WriteInt ((int)(a_Pickup.GetPosZ() * 32));
	WriteInt (1);
	WriteShort((short)(a_Pickup.GetSpeed().x * 32));
	WriteShort((short)(a_Pickup.GetSpeed().y * 32));
	WriteShort((short)(a_Pickup.GetSpeed().z * 32));
	WriteByte(0);
	WriteByte(0);
	
	// Send a ENTITY_METADATA packet with the slot info:
	WriteByte(PACKET_ENTITY_METADATA);
	WriteInt(a_Pickup.GetUniqueID());
	WriteByte(0xaa);  // a slot value at index 10
	WriteItem(*a_Pickup.GetItem());
	WriteByte(0x7f);  // End of metadata
	Flush();
}





void cProtocol146::SendSpawnFallingBlock(const cFallingBlock & a_FallingBlock)
{
	// Send two packets - spawn object, then entity metadata
	cCSLock Lock(m_CSPacket);

	WriteByte(PACKET_SPAWN_OBJECT);
	WriteInt (a_FallingBlock.GetUniqueID());
	WriteByte(70);
	WriteInt ((int)(a_FallingBlock.GetPosX() * 32));
	WriteInt ((int)(a_FallingBlock.GetPosY() * 32));
	WriteInt ((int)(a_FallingBlock.GetPosZ() * 32));
	WriteInt (0x800000);
	WriteShort(0);
	WriteShort(0);
	WriteShort(0);
	WriteByte (0);
	WriteByte (0);
	
	// TODO: This still doesn't work, although it is exactly the same that the vanilla server sends. WTF?
	WriteByte(PACKET_ENTITY_METADATA);
	WriteInt(a_FallingBlock.GetUniqueID());
	WriteByte(0xaa);  // a slot value at index 10
	cItem Item(a_FallingBlock.GetBlockType(), 1);
	WriteItem(Item);
	WriteByte(0x7f);  // End of metadata
	Flush();
}





void cProtocol146::SendSpawnObject(const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, short a_SpeedX, short a_SpeedY, short a_SpeedZ, Byte a_Yaw, Byte a_Pitch)
{
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
		WriteShort(a_SpeedX);
		WriteShort(a_SpeedY);
		WriteShort(a_SpeedZ);
		WriteByte(a_Yaw);
		WriteByte(a_Pitch);
	}
	Flush();
}





