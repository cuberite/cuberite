
// Protocol_1_10.h

/*
Declares the 1.10 protocol classes:
	- cProtocol_1_10
		- release 1.10 protocol (#210), also used by 1.10.1 and 1.10.2
*/





#pragma once

#include "Protocol_1_9.h"





class cProtocol_1_10_0:
	public cProtocol_1_9_4
{
	using Super = cProtocol_1_9_4;

public:

	using Super::Super;

protected:

	virtual void SendSoundEffect(const AString & a_SoundName, Vector3d a_Origin, float a_Volume, float a_Pitch) override;
	virtual void SendSoundEffect(const eSoundEvent a_SoundEvent, Vector3d a_Origin, float a_Volume, float a_Pitch) override;

	virtual UInt32 GetProtocolMobType(eMonsterType a_MobType) const override;
	virtual Version GetProtocolVersion() const override;

	virtual void HandlePacketResourcePackStatus(cByteBuffer & a_ByteBuffer) override;

	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const override;
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) const override;
};
