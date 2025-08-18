
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

	using Super::Super;

protected:

	virtual void SendCollectEntity    (const cEntity & a_Collected, const cPawn & a_Collector, unsigned a_Count) override;
	virtual void SendEntityAnimation  (const cEntity & a_Entity, EntityAnimation a_Animation) override;
	virtual void SendHideTitle        (void) override;
	virtual void SendResetTitle       (void) override;
	virtual void SendSpawnMob         (const cMonster & a_Mob) override;
	virtual void SendTitleTimes       (int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks) override;
	virtual void SendUpdateBlockEntity(cBlockEntity & a_BlockEntity) override;

	virtual signed char GetProtocolEntityStatus(EntityAnimation a_Animation) const override;
	virtual UInt32 GetProtocolMobType(eMonsterType a_MobType) const override;
	virtual Version GetProtocolVersion() const override;

	virtual void HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer) override;

	virtual void WriteBlockEntity(cFastNBTWriter & a_Writer, const cBlockEntity & a_BlockEntity) const override;
	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const override;
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) const override;
};





class cProtocol_1_11_1:
	public cProtocol_1_11_0
{
	using Super = cProtocol_1_11_0;

public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
};
