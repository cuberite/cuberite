
// cPacket_Player.cpp

/* Implements the player-related packets:
	- PlayerAbilities (0xca)
	- PlayerListItem  (0xc9)
	- PlayerLook      (0x0c)
	- PlayerMoveLook  (0x0d)
	- PlayerPosition  (0x0b)
*/

#include "Globals.h"

#include "cPacket_Player.h"
#include "../cPlayer.h"
#include "../cChatColor.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cPacket_PlayerAbilities:

int cPacket_PlayerAbilities::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBool, m_Invulnerable, TotalBytes);
	HANDLE_PACKET_READ(ReadBool, m_IsFlying,     TotalBytes);
	HANDLE_PACKET_READ(ReadBool, m_CanFly,       TotalBytes);
	HANDLE_PACKET_READ(ReadBool, m_InstaMine,    TotalBytes);
	return TotalBytes;
}





void cPacket_PlayerAbilities::Serialize(AString & a_Data) const
{
	char Data[5];
	Data[0] = m_PacketID;
	Data[1] = (char)m_Invulnerable;
	Data[2] = (char)m_IsFlying;
	Data[3] = (char)m_CanFly;
	Data[4] = (char)m_InstaMine;
	a_Data.append(Data, 5);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cPacket_PlayerListItem:

cPacket_PlayerListItem::cPacket_PlayerListItem(const AString & a_PlayerName, bool a_Online, short a_Ping)
{
	m_PacketID = E_PLAYER_LIST_ITEM;
	m_PlayerName = a_PlayerName;
	m_Online = a_Online;
	m_Ping = a_Ping;
}





int cPacket_PlayerListItem::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEUTF16String16, m_PlayerName, TotalBytes);
	HANDLE_PACKET_READ(ReadBool,            m_Online,     TotalBytes);
	HANDLE_PACKET_READ(ReadBEShort,         m_Ping,       TotalBytes);
	return TotalBytes;
}





void cPacket_PlayerListItem::Serialize(AString & a_Data) const
{
	AString PlayerName(m_PlayerName);
	if (PlayerName.length() > 16)
	{
		PlayerName.erase(16);
	}
	else if (PlayerName.length() <= 14)
	{
		PlayerName += cChatColor::White;
	}

	AppendByte    (a_Data, m_PacketID);
	AppendString16(a_Data, PlayerName);
	AppendBool    (a_Data, m_Online);
	AppendShort   (a_Data, m_Ping);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cPacket_PlayerLook:

cPacket_PlayerLook::cPacket_PlayerLook( cPlayer* a_Player )
{
	m_PacketID   = E_PLAYERLOOK;
	m_Rotation   = a_Player->GetRotation();
	m_Pitch      = a_Player->GetPitch();
	m_IsOnGround = a_Player->IsOnGround();
}





int cPacket_PlayerLook::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEFloat, m_Rotation,   TotalBytes);
	HANDLE_PACKET_READ(ReadBEFloat, m_Pitch,      TotalBytes);
	HANDLE_PACKET_READ(ReadBool,    m_IsOnGround, TotalBytes);
	return TotalBytes;
}





void cPacket_PlayerLook::Serialize(AString & a_Data) const
{
	AppendByte	(a_Data, m_PacketID);
	AppendFloat	(a_Data, m_Rotation);
	AppendFloat	(a_Data, m_Pitch);
	AppendBool	(a_Data, m_IsOnGround);
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cPacket_PlayerMoveLook:

cPacket_PlayerMoveLook::cPacket_PlayerMoveLook(const cPlayer & a_Player)
{
	m_PacketID   = E_PLAYERMOVELOOK;
	m_PosX       = a_Player.GetPosX();
	m_PosY       = a_Player.GetPosY() + 0.03;  // Add a small amount so that the player doesn't start inside a block
	m_PosZ       = a_Player.GetPosZ();
	m_Stance     = a_Player.GetStance() + 0.03;  // Add a small amount so that the player doesn't start inside a block
	m_Rotation   = a_Player.GetRotation();
	m_Pitch      = a_Player.GetPitch();
	m_IsOnGround = a_Player.IsOnGround();
}





int cPacket_PlayerMoveLook::Parse(cByteBuffer & a_Buffer)
{
	// NOTE that Stance and Y are swapped when sent C->S vs S->C
	// This is the C->S case:
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEDouble, m_PosX,       TotalBytes);
	HANDLE_PACKET_READ(ReadBEDouble, m_PosY,       TotalBytes);
	HANDLE_PACKET_READ(ReadBEDouble, m_Stance,     TotalBytes);
	HANDLE_PACKET_READ(ReadBEDouble, m_PosZ,       TotalBytes);
	HANDLE_PACKET_READ(ReadBEFloat,  m_Rotation,   TotalBytes);
	HANDLE_PACKET_READ(ReadBEFloat,  m_Pitch,      TotalBytes);
	HANDLE_PACKET_READ(ReadBool,     m_IsOnGround, TotalBytes);
	// LOGD("Recv PML: {%0.2f, %0.2f, %0.2f}, Stance %0.2f, Gnd: %d", m_PosX, m_PosY, m_PosZ, m_Stance, m_IsOnGround ? 1 : 0);
	return TotalBytes;
}





void cPacket_PlayerMoveLook::Serialize(AString & a_Data) const
{
	// NOTE that Stance and Y are swapped when sent C->S vs S->C
	// This is the S->C case:
	// LOGD("Send PML: {%0.2f, %0.2f, %0.2f}, Stance: %0.2f, Gnd: %d", m_PosX, m_PosY, m_PosZ, m_Stance, m_IsOnGround ? 1 : 0);
	
	AppendByte	(a_Data, m_PacketID);
	AppendDouble(a_Data, m_PosX);
	AppendDouble(a_Data, m_Stance);
	AppendDouble(a_Data, m_PosY);
	AppendDouble(a_Data, m_PosZ);
	AppendFloat	(a_Data, m_Rotation);
	AppendFloat	(a_Data, m_Pitch);
	AppendBool	(a_Data, m_IsOnGround);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cPacket_PlayerPosition:

cPacket_PlayerPosition::cPacket_PlayerPosition(const cPlayer & a_Player)
{
	m_PacketID = E_PLAYERPOS;

	m_PosX       = a_Player.GetPosX();
	m_PosY       = a_Player.GetPosY();
	m_PosZ       = a_Player.GetPosZ();
	m_Stance     = a_Player.GetStance();
	m_IsOnGround = a_Player.IsOnGround();
}





int cPacket_PlayerPosition::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEDouble, m_PosX,       TotalBytes);
	HANDLE_PACKET_READ(ReadBEDouble, m_PosY,       TotalBytes);
	HANDLE_PACKET_READ(ReadBEDouble, m_Stance,     TotalBytes);
	HANDLE_PACKET_READ(ReadBEDouble, m_PosZ,       TotalBytes);
	HANDLE_PACKET_READ(ReadBool,     m_IsOnGround, TotalBytes);
	// LOGD("Recv PlayerPos: {%0.2f %0.2f %0.2f}, Stance %0.2f, Gnd: %d", m_PosX, m_PosY, m_PosZ, m_Stance, m_IsOnGround ? 1 : 0);
	return TotalBytes;
}





void cPacket_PlayerPosition::Serialize(AString & a_Data) const
{
	LOGD("Ignore send PlayerPos");
	/*
	LOGD("Send PlayerPos: {%0.2f %0.2f %0.2f}, Stance %0.2f, Gnd: %d", m_PosX, m_PosY, m_PosZ, m_Stance, m_IsOnGround ? 1 : 0);
	// _X: This should not get sent to the client at all - http://wiki.vg/wiki/index.php?title=Protocol&oldid=2513#Player_Position_.280x0B.29
	AppendByte	 (a_Data, m_PacketID);
	AppendDouble (a_Data, m_PosX);
	AppendDouble (a_Data, m_PosY);
	AppendDouble (a_Data, m_Stance);
	AppendDouble (a_Data, m_PosZ);
	AppendBool	 (a_Data, m_IsOnGround);
	*/
}




