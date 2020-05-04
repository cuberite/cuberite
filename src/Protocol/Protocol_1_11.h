
// Protocol_1_11.h

/*
Declares the 1.11 protocol classes:
	- cProtocol_1_11_0
		- release 1.11 protocol (#315)
	- cProtocol_1_11_1
		- release 1.11.1 protocol (#316)
*/





#pragma once

#include "Protocol_1_10.h"





class cProtocol_1_11_0:
	public cProtocol_1_10_0
{
	using Super = cProtocol_1_10_0;

public:

	cProtocol_1_11_0(cClientHandle * a_Client, const AString &a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);

	virtual void SendCollectEntity(const cEntity & a_Collected, const cEntity & a_Collector, unsigned a_Count) override;
	virtual void SendHideTitle    (void) override;
	virtual void SendResetTitle   (void) override;
	virtual void SendSpawnMob     (const cMonster & a_Mob) override;
	virtual void SendTitleTimes   (int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks) override;

protected:

	/** Converts eMonsterType to protocol-specific mob IDs */
	virtual UInt32 GetProtocolMobType(eMonsterType a_MobType) override;

	virtual void HandlePacketBlockPlace   (cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;

	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) override;
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) override;
	virtual void WriteBlockEntity(cPacketizer & a_Pkt, const cBlockEntity & a_BlockEntity) override;
};





class cProtocol_1_11_1:
	public cProtocol_1_11_0
{
	using Super = cProtocol_1_11_0;

public:

	cProtocol_1_11_1(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);

	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;
};
