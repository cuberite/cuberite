#pragma once
#include "Protocol_1_17.h"




class cProtocol_1_18
	: public cProtocol_1_17_1
{
	using Super = cProtocol_1_17_1;
public:

	using Super::Super;

protected:
	virtual UInt32    GetPacketID(ePacketType a_PacketType) const override;
	virtual void      SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void      HandlePacketClientSettings(cByteBuffer & a_ByteBuffer) override;

	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_18_2
	: public cProtocol_1_18
{
	using Super = cProtocol_1_18;
public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
};
