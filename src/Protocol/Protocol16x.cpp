
// Protocol16x.cpp

/*
Implements the 1.6.x protocol classes:
	- cProtocol161
		- release 1.6.1 protocol (#73)
	- cProtocol162
		- release 1.6.2 protocol (#74)
		- release 1.6.3 protocol (#77) - no relevant changes
		- release 1.6.4 protocol (#78) - no relevant changes
(others may be added later in the future for the 1.6 release series)
*/

#include "Globals.h"
#include "Protocol16x.h"
#include "../ClientHandle.h"
#include "../Entities/Entity.h"
#include "../Entities/Player.h"
#include "../UI/Window.h"
#include "../CompositeChat.h"





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
	PACKET_TILE_EDITOR_OPEN  = 0x85,
	PACKET_PLAYER_ABILITIES  = 0xca,
} ;





////////////////////////////////////////////////////////////////////////////////
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





void cProtocol161::SendChat(const cCompositeChat & a_Message)
{
	// This protocol version doesn't support composite messages to the full
	// Just extract each part's text and use it:
	
	super::SendChat(Printf("{\"text\":\"%s\"}", EscapeString(a_Message.ExtractText()).c_str()));
}





void cProtocol161::SendEditSign(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_TILE_EDITOR_OPEN);
	WriteByte(0);
	WriteInt(a_BlockX);
	WriteInt(a_BlockY);
	WriteInt(a_BlockZ);
	Flush();
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
	cPlayer * Player = m_Client->GetPlayer();
	WriteFloat((float)Player->GetHealth());
	WriteShort((short)Player->GetFoodLevel());
	WriteFloat((float)Player->GetFoodSaturationLevel());
	Flush();
}





void cProtocol161::SendPlayerMaxSpeed(void)
{
	cCSLock Lock(m_CSPacket);
	cPlayer * Player = m_Client->GetPlayer();
	WriteByte(PACKET_ENTITY_PROPERTIES);
	WriteInt(Player->GetUniqueID());
	WriteInt(1);
	WriteString("generic.movementSpeed");
	WriteDouble(0.1 * Player->GetMaxSpeed());
	Flush();
}





void cProtocol161::SendRespawn(const cWorld & a_World, bool a_ShouldIgnoreDimensionChecks)
{
	// Besides sending the respawn, we need to also send the player max speed, otherwise the client reverts to super-fast
	super::SendRespawn(a_World, a_ShouldIgnoreDimensionChecks);
	SendPlayerMaxSpeed();
}





void cProtocol161::SendWindowOpen(const cWindow & a_Window)
{
	if (a_Window.GetWindowType() < 0)
	{
		// Do not send for inventory windows
		return;
	}
	cCSLock Lock(m_CSPacket);
	WriteByte  (PACKET_WINDOW_OPEN);
	WriteChar  (a_Window.GetWindowID());
	WriteByte  ((Byte)a_Window.GetWindowType());
	WriteString(a_Window.GetWindowTitle());
	WriteByte  ((Byte)a_Window.GetNumNonInventorySlots());
	WriteByte  (1);  // Use title
	if (a_Window.GetWindowType() == cWindow::wtAnimalChest)
	{
		WriteInt(0);  // TODO: The animal's EntityID
	}
	Flush();
}





int cProtocol161::ParseEntityAction(void)
{
	HANDLE_PACKET_READ(ReadBEInt, int,  EntityID);
	HANDLE_PACKET_READ(ReadChar,  char, ActionID);
	HANDLE_PACKET_READ(ReadBEInt, int,  UnknownHorseVal);
	
	switch (ActionID)
	{
		case 1: m_Client->HandleEntityCrouch(EntityID, true);     break;  // Crouch
		case 2: m_Client->HandleEntityCrouch(EntityID, false);    break;  // Uncrouch
		case 3: m_Client->HandleEntityLeaveBed(EntityID);         break;  // Leave Bed
		case 4: m_Client->HandleEntitySprinting(EntityID, true);  break;  // Start sprinting
		case 5: m_Client->HandleEntitySprinting(EntityID, false); break;  // Stop sprinting
	}

	return PARSE_OK;
}





int cProtocol161::ParseLogin(void)
{
	// The login packet is sent by Forge clients only
	// Only parse the packet, do no extra processing
	// Note that the types and the names have been only guessed and are not verified at all!
	HANDLE_PACKET_READ(ReadBEInt, int, Int1);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, String1);
	HANDLE_PACKET_READ(ReadChar, char, Char1);
	HANDLE_PACKET_READ(ReadChar, char, Char2);
	HANDLE_PACKET_READ(ReadChar, char, Char3);
	HANDLE_PACKET_READ(ReadByte, Byte, Byte1);
	HANDLE_PACKET_READ(ReadByte, Byte, Byte2);
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
	if (Unmount)
	{
		m_Client->HandleUnmount();
	}
	else
	{
		m_Client->HandleSteerVehicle(Forward, Sideways);
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





////////////////////////////////////////////////////////////////////////////////
// cProtocol162:

cProtocol162::cProtocol162(cClientHandle * a_Client) :
	super(a_Client)
{
}





void cProtocol162::SendPlayerMaxSpeed(void)
{
	cCSLock Lock(m_CSPacket);
	cPlayer * Player = m_Client->GetPlayer();
	WriteByte(PACKET_ENTITY_PROPERTIES);
	WriteInt(Player->GetUniqueID());
	WriteInt(1);
	WriteString("generic.movementSpeed");
	WriteDouble(0.1 * Player->GetMaxSpeed());
	WriteShort(0);
	Flush();
}




