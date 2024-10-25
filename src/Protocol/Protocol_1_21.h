#pragma once
#include "Protocol_1_20.h"





class cProtocol_1_21
	: public cProtocol_1_20_5
{
	using Super = cProtocol_1_20_5;
public:

	using Super::Super;

protected:
	virtual void SendDynamicRegistries() override;
	virtual void SendSelectKnownPacks() override;

	virtual UInt32	GetPacketID(ePacketType a_PacketType) const override;

	virtual Version GetProtocolVersion() const override;
	virtual UInt32 GetProtocolBlockType(BlockState a_Block) const override;
	virtual UInt32 GetProtocolItemType(Item a_ItemID) const override;
	virtual Item GetItemFromProtocolID(UInt32 a_ProtocolID) const override;
};

class cProtocol_1_21_2
	: public cProtocol_1_21
{
	using Super = cProtocol_1_21;
public:

	using Super::Super;

protected:

	virtual void SendSelectKnownPacks() override;
	virtual Version GetProtocolVersion() const override;
};
