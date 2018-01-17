
// Protocol_1_11.h

/*
Declares the 1.11 protocol classes:
	- cProtocol_1_11_0
		- release 1.11 protocol (#315)
(others may be added later in the future for the 1.11 release series)
*/





#pragma once

#include "Protocol_1_10.h"





class cProtocol_1_11_0 :
	public cProtocol_1_10_0
{
	typedef cProtocol_1_10_0 super;

public:
	cProtocol_1_11_0(cClientHandle * a_Client, const AString &a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);

	virtual void SendCollectEntity(const cEntity & a_Entity, const cPlayer & a_Player, int a_Count) override;
	virtual void SendHideTitle    (void) override;
	virtual void SendResetTitle   (void) override;
	virtual void SendSpawnMob     (const cMonster & a_Mob) override;
	virtual void SendTitleTimes   (int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks) override;

protected:

	virtual void HandlePacketBlockPlace   (cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;

	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) override;
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) override;
	virtual void WriteBlockEntity(cPacketizer & a_Pkt, const cBlockEntity & a_BlockEntity) override;
};





class cProtocol_1_11_1 :
	public cProtocol_1_11_0
{
	typedef cProtocol_1_11_0 super;

public:
	cProtocol_1_11_1(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);

	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;
};
