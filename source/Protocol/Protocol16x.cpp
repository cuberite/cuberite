
// Protocol16x.cpp

/*
Implements the 1.6.x protocol classes:
	- cProtocol161
		- release 1.6.1 protocol (#73)
	- cProtocol162
		- release 1.6.2 protocol (#74)
(others may be added later in the future for the 1.6 release series)
*/

#include "Globals.h"
#include "Protocol16x.h"
#include "../ClientHandle.h"
#include "../Entity.h"
#include "../Player.h"





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
	PACKET_CHAT              = 0x03,
	PACKET_UPDATE_HEALTH     = 0x08,
	PACKET_STEER_VEHICLE     = 0x1b,
	PACKET_ATTACH_ENTITY     = 0x27,
	PACKET_ENTITY_PROPERTIES = 0x2c,
	PACKET_WINDOW_OPEN       = 0x64,
	PACKET_PLAYER_ABILITIES  = 0xca,
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProtocol161:

cProtocol161::cProtocol161(cClientHandle * a_Client) :
	super(a_Client)
{
}





void cProtocol161::SendAttachEntity(const cEntity & a_Entity, const cEntity * a_Vehicle)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_ATTACH_ENTITY);
	WriteInt(a_Entity.GetUniqueID());
	WriteInt((a_Vehicle == NULL) ? -1 : a_Vehicle->GetUniqueID());
	WriteBool(false);  // TODO: "Should use leash?" -> no
	Flush();
}





void cProtocol161::SendChat(const AString & a_Message)
{
	super::SendChat(Printf("{\"text\":\"%s\"}", EscapeString(a_Message).c_str()));
}




void cProtocol161::SendGameMode(eGameMode a_GameMode)
{
	super::SendGameMode(a_GameMode);
	SendPlayerMaxSpeed();
}





void cProtocol161::SendHealth(void)
{
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_UPDATE_HEALTH);
	WriteFloat((float)m_Client->GetPlayer()->GetHealth());
	WriteShort(m_Client->GetPlayer()->GetFoodLevel());
	WriteFloat((float)m_Client->GetPlayer()->GetFoodSaturationLevel());
	Flush();
}





void cProtocol161::SendPlayerMaxSpeed(void)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_ENTITY_PROPERTIES);
	WriteInt(m_Client->GetPlayer()->GetUniqueID());
	WriteInt(1);
	WriteString("generic.movementSpeed");
	WriteDouble(m_Client->GetPlayer()->GetMaxSpeed());
	Flush();
}





void cProtocol161::SendRespawn(void)
{
	// Besides sending the respawn, we need to also send the player max speed, otherwise the client reverts to super-fast
	super::SendRespawn();
	SendPlayerMaxSpeed();
}





void cProtocol161::SendWindowOpen(char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots)
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
	WriteByte  (1);  // Use title
	if (a_WindowType == 11)  // horse / donkey
	{
		WriteInt(0);  // Unknown value sent only when window type is 11 (horse / donkey)
	}
	Flush();
}





int cProtocol161::ParseEntityAction(void)
{
	HANDLE_PACKET_READ(ReadBEInt, int,  EntityID);
	HANDLE_PACKET_READ(ReadChar,  char, ActionID);
	HANDLE_PACKET_READ(ReadBEInt, int,  UnknownHorseVal);
	m_Client->HandleEntityAction(EntityID, ActionID);
	return PARSE_OK;
}





int cProtocol161::ParsePlayerAbilities(void)
{
	HANDLE_PACKET_READ(ReadByte,    Byte,  Flags);
	HANDLE_PACKET_READ(ReadBEFloat, float, FlyingSpeed);
	HANDLE_PACKET_READ(ReadBEFloat, float, WalkingSpeed);
	// TODO: m_Client->HandlePlayerAbilities(...);
	return PARSE_OK;
}





int cProtocol161::ParseSteerVehicle(void)
{
	HANDLE_PACKET_READ(ReadBEFloat, float, Sideways);
	HANDLE_PACKET_READ(ReadBEFloat, float, Forward);
	HANDLE_PACKET_READ(ReadBool,    bool,  Jump);
	HANDLE_PACKET_READ(ReadBool,    bool,  Unmount);
	// TODO: m_Client->HandleSteerVehicle(...);
	if (Unmount)
	{
		m_Client->HandleUnmount();
	}
	return PARSE_OK;
}





int cProtocol161::ParsePacket(unsigned char a_PacketType)
{
	switch (a_PacketType)
	{
		case PACKET_STEER_VEHICLE: return ParseSteerVehicle();
		default:                   return super::ParsePacket(a_PacketType);
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProtocol162:

cProtocol162::cProtocol162(cClientHandle * a_Client) :
	super(a_Client)
{
}





void cProtocol162::SendPlayerMaxSpeed(void)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_ENTITY_PROPERTIES);
	WriteInt(m_Client->GetPlayer()->GetUniqueID());
	WriteInt(1);
	WriteString("generic.movementSpeed");
	WriteDouble(m_Client->GetPlayer()->GetMaxSpeed());
	WriteShort(0);
	Flush();
}




