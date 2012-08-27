
// cPacket_Player.h

/* Interfaces to the player-related packets:
	- PlayerAbilities (0xca)
	- PlayerListItem  (0xc9)
	- PlayerLook      (0x0c)
	- PlayerMoveLook  (0x0d)
	- PlayerPosition  (0x0b)
*/

#pragma once





#include "cPacket.h"





// fwd:
class cPlayer;





class cPacket_PlayerAbilities : public cPacket
{
public:
	cPacket_PlayerAbilities(void) { m_PacketID = E_PLAYER_LIST_ITEM; }

	virtual int Parse(cByteBuffer & a_Buffer) override;
	virtual void Serialize(AString & a_Data) const override;

	virtual cPacket * Clone() const { return new cPacket_PlayerAbilities(*this); }

	bool m_Invulnerable;  // Speculation
	bool m_IsFlying;
	bool m_CanFly;
	bool m_InstaMine;  // Speculation
} ;





class cPacket_PlayerListItem : public cPacket
{
public:
	cPacket_PlayerListItem() { m_PacketID = E_PLAYER_LIST_ITEM; }
	cPacket_PlayerListItem(const AString & a_PlayerName, bool a_Online, short a_Ping);

	virtual int Parse(cByteBuffer & a_Buffer) override;
	virtual void Serialize(AString & a_Data) const override;

	virtual cPacket* Clone() const { return new cPacket_PlayerListItem(*this); }

	AString m_PlayerName; // Supports chat coloring, limited to 16 characters.
	bool    m_Online;
	short   m_Ping;
} ;





class cPacket_PlayerLook : public cPacket
{
public:
	cPacket_PlayerLook()
		: m_Rotation( 0 )
		, m_Pitch( 0 )
		, m_IsOnGround( false )
	{
		m_PacketID = E_PLAYERLOOK;
	}
	
	cPacket_PlayerLook( cPlayer* a_Player );
	virtual cPacket* Clone() const { return new cPacket_PlayerLook(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;
	virtual void Serialize(AString & a_Data) const override;

	float m_Rotation;
	float m_Pitch;
	bool  m_IsOnGround;
} ;





class cPacket_PlayerMoveLook : public cPacket
{
public:
	cPacket_PlayerMoveLook()
		: m_PosX( 0.0 )
		, m_PosY( 0.0 )
		, m_Stance( 0.0 )
		, m_PosZ( 0.0 )
		, m_Rotation( 0.f )
		, m_Pitch( 0.f )
		, m_IsOnGround( false )
	{
		m_PacketID = E_PLAYERMOVELOOK;
	}
	
	cPacket_PlayerMoveLook(const cPlayer & a_Player);
	virtual cPacket * Clone() const { return new cPacket_PlayerMoveLook(*this); }

	virtual int  Parse(cByteBuffer & a_Buffer) override;
	virtual void Serialize(AString & a_Data) const override;

	double m_PosX;
	double m_PosY;
	double m_Stance;
	double m_PosZ;
	float  m_Rotation;
	float  m_Pitch;
	bool   m_IsOnGround;
} ;





class cPacket_PlayerPosition : public cPacket
{
public:
	cPacket_PlayerPosition(const cPlayer & a_Player);
	cPacket_PlayerPosition()
		: m_PosX( 0.0 )
		, m_PosY( 0.0 )
		, m_PosZ( 0.0 )
		, m_Stance( 0.0 )
		, m_IsOnGround(true)
	{ m_PacketID = E_PLAYERPOS;  }
	virtual cPacket* Clone() const { return new cPacket_PlayerPosition(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;
	virtual void Serialize(AString & a_Data) const override;

	double m_PosX;
	double m_PosY;
	double m_PosZ;
	double m_Stance;
	bool   m_IsOnGround;
} ;




