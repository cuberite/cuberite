
// Protocol_1_12.h

/*
Declares the 1.12 protocol classes:
	- cProtocol_1_12
		- release 1.12 protocol (#335)
	- cProtocol_1_12_1
		- release 1.12.1 protocol (#338)
	- cProtocol_1_12_2
		- release 1.12.2 protocol (#340)
(others may be added later in the future for the 1.12 release series)
*/





#pragma once

#include "Protocol_1_11.h"





class cProtocol_1_12 :
	public cProtocol_1_11_1
{
	typedef cProtocol_1_11_1 super;

public:
	cProtocol_1_12(cClientHandle * a_Client, const AString &a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);

	virtual void SendCollectEntity(const cEntity & a_Entity, const cPlayer & a_Player, int a_Count) override;
	virtual void SendHideTitle(void) override;
	virtual void SendResetTitle(void) override;
	virtual void SendSpawnMob(const cMonster & a_Mob) override;
	virtual void SendTitleTimes(int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks) override;
protected:
	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void HandlePacketAdvancementTab(cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketCraftingBookData(cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;
	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) override;
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) override;

protected:

	virtual UInt32 GetPacketId(eOutgoingPackets a_Packet) override;
};





class cProtocol_1_12_1 :
	public cProtocol_1_12
{
	typedef cProtocol_1_12 super;

public:
	cProtocol_1_12_1(cClientHandle * a_Client, const AString &a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);

protected:
	virtual UInt32 GetPacketId(eOutgoingPackets a_Packet) override;

	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;
};





class cProtocol_1_12_2:
	public cProtocol_1_12_1
{
	typedef cProtocol_1_12_1 Super;

public:
	cProtocol_1_12_2(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State):
		Super(a_Client, a_ServerAddress, a_ServerPort, a_State)
	{
	}

protected:
	virtual void HandlePacketKeepAlive(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;
	virtual void SendKeepAlive(UInt32 a_PingID) override;
};




