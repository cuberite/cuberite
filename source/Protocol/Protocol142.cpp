
// Protocol142.cpp

// Implements the cProtocol142 class representing the release 1.4.2 protocol (#47)

#include "Globals.h"
#include "Protocol142.h"
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





#define HANDLE_PACKET_READ(Proc, Type, Var) \
	Type Var; \
	{ \
		if (!m_ReceivedData.Proc(Var)) \
		{ \
			return PARSE_INCOMPLETE; \
		} \
	}





typedef unsigned char Byte;





enum
{
	PACKET_UPDATE_TIME           = 0x04,
	PACKET_PICKUP_SPAWN          = 0x15,
	PACKET_SOUND_PARTICLE_EFFECT = 0x3d
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProtocol142:

cProtocol142::cProtocol142(cClientHandle * a_Client) :
	super(a_Client)
{
	LOGD("Created cProtocol142 at %p", this);
}





cProtocol142::~cProtocol142()
{
	if (!m_DataToSend.empty())
	{
		LOGD("There are %d unsent bytes while deleting cProtocol142", m_DataToSend.size());
	}
	LOGD("Deleted cProtocol142 at %p", this);
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