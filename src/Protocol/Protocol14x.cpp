
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
#include "../Item.h"
#include "ChunkDataSerializer.h"
#include "../Entities/Player.h"
#include "../Mobs/Monster.h"
#include "../UI/Window.h"
#include "../Entities/Pickup.h"
#include "../Entities/FallingBlock.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4127)
	#pragma warning(disable:4244)
	#pragma warning(disable:4231)
	#pragma warning(disable:4189)
	#pragma warning(disable:4702)
#endif

#ifdef _MSC_VER
	#pragma warning(pop)
#endif


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





enum
{
	PACKET_UPDATE_TIME           = 0x04,
	PACKET_PICKUP_SPAWN          = 0x15,
	PACKET_SPAWN_OBJECT          = 0x17,
	PACKET_ENTITY_METADATA       = 0x28,
	PACKET_SOUND_PARTICLE_EFFECT = 0x3d
} ;





////////////////////////////////////////////////////////////////////////////////
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
	m_Client->SetLocale(Locale);
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
	WriteItem   (a_Pickup.GetItem());
	WriteVectorI((Vector3i)(a_Pickup.GetPosition() * 32));
	WriteChar((char)(a_Pickup.GetSpeedX() * 8));
	WriteChar((char)(a_Pickup.GetSpeedY() * 8));
	WriteChar((char)(a_Pickup.GetSpeedZ() * 8));
	Flush();
}





void cProtocol142::SendSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_SOUND_PARTICLE_EFFECT);
	WriteInt (a_EffectID);
	WriteInt (a_SrcX);
	WriteByte((Byte)a_SrcY);
	WriteInt (a_SrcZ);
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






////////////////////////////////////////////////////////////////////////////////
// cProtocol146:

cProtocol146::cProtocol146(cClientHandle * a_Client) :
	super(a_Client)
{
}





void cProtocol146::SendPickupSpawn(const cPickup & a_Pickup)
{
	ASSERT(!a_Pickup.GetItem().IsEmpty());
	
	cCSLock Lock(m_CSPacket);

	// Send a SPAWN_OBJECT packet for the base entity:
	WriteByte(PACKET_SPAWN_OBJECT);
	WriteInt (a_Pickup.GetUniqueID());
	WriteByte(0x02);
	WriteInt ((int)(a_Pickup.GetPosX() * 32));
	WriteInt ((int)(a_Pickup.GetPosY() * 32));
	WriteInt ((int)(a_Pickup.GetPosZ() * 32));
	WriteInt (1);
	WriteShort((short)(a_Pickup.GetSpeedX() * 32));
	WriteShort((short)(a_Pickup.GetSpeedY() * 32));
	WriteShort((short)(a_Pickup.GetSpeedZ() * 32));
	WriteByte(0);
	WriteByte(0);
	
	// Send a ENTITY_METADATA packet with the slot info:
	WriteByte(PACKET_ENTITY_METADATA);
	WriteInt(a_Pickup.GetUniqueID());
	WriteByte(0xaa);  // a slot value at index 10
	WriteItem(a_Pickup.GetItem());
	WriteByte(0x7f);  // End of metadata
	Flush();
}





void cProtocol146::SendSpawnFallingBlock(const cFallingBlock & a_FallingBlock)
{
	// Send a spawn object / vehicle packet
	cCSLock Lock(m_CSPacket);

	WriteByte(PACKET_SPAWN_OBJECT);
	WriteInt (a_FallingBlock.GetUniqueID());
	WriteByte(70);
	WriteInt ((int)(a_FallingBlock.GetPosX() * 32));
	WriteInt ((int)(a_FallingBlock.GetPosY() * 32));
	WriteInt ((int)(a_FallingBlock.GetPosZ() * 32));
	WriteByte (0);  // Pitch
	WriteByte (0);  // Yaw
	WriteInt (a_FallingBlock.GetBlockType());  // data indicator = blocktype
	WriteShort((short)(a_FallingBlock.GetSpeedX() * 400));
	WriteShort((short)(a_FallingBlock.GetSpeedY() * 400));
	WriteShort((short)(a_FallingBlock.GetSpeedZ() * 400));
	Flush();
}





void cProtocol146::SendSpawnObject(const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_SPAWN_OBJECT);
	WriteInt (a_Entity.GetUniqueID());
	WriteChar(a_ObjectType);
	WriteInt ((int)(a_Entity.GetPosX() * 32));
	WriteInt ((int)(a_Entity.GetPosY() * 32));
	WriteInt ((int)(a_Entity.GetPosZ() * 32));
	WriteByte(a_Pitch);
	WriteByte(a_Yaw);
	WriteInt (a_ObjectData);
	if (a_ObjectData != 0)
	{
		WriteShort((short)(a_Entity.GetSpeedX() * 400));
		WriteShort((short)(a_Entity.GetSpeedY() * 400));
		WriteShort((short)(a_Entity.GetSpeedZ() * 400));
	}
	Flush();
}





void cProtocol146::SendSpawnVehicle(const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType)
{
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_SPAWN_OBJECT);
	WriteInt  (a_Vehicle.GetUniqueID());
	WriteChar (a_VehicleType);
	WriteInt  ((int)(a_Vehicle.GetPosX() * 32));
	WriteInt  ((int)(a_Vehicle.GetPosY() * 32));
	WriteInt  ((int)(a_Vehicle.GetPosZ() * 32));
	WriteByte ((Byte)((a_Vehicle.GetPitch() / 360.f) * 256));
	WriteByte ((Byte)((a_Vehicle.GetYaw()   / 360.f) * 256));
	WriteInt  (a_VehicleSubType);
	if (a_VehicleSubType != 0)
	{
		WriteShort((short)(a_Vehicle.GetSpeedX() * 400));
		WriteShort((short)(a_Vehicle.GetSpeedY() * 400));
		WriteShort((short)(a_Vehicle.GetSpeedZ() * 400));
	}
	Flush();
}





