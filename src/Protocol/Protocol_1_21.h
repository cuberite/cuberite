#pragma once
#include "Protocol_1_20.h"





class cProtocol_1_21
	: public cProtocol_1_20_5
{
	using Super = cProtocol_1_20_5;
public:

	using Super::Super;

protected:
	virtual UInt32	GetPacketID(ePacketType a_PacketType) const override;

	virtual Version GetProtocolVersion() const override;
};
