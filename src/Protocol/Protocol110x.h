
// Protocol110x.h

/*
Declares the 1.10.x protocol classes:
	- cProtocol1100
		- release 1.10.0 protocol (#210)
(others may be added later in the future for the 1.10 release series)
*/





#pragma once

#include "Protocol19x.h"

class cProtocol1100 :
	public cProtocol194
{
	typedef cProtocol194 super;

public:
	cProtocol1100(cClientHandle * a_Client, const AString &a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);

	virtual void SendSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch) override;

	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;

protected:
	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) override;
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) override;
};
