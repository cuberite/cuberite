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
	virtual void SendLoginSuccess() override;
	virtual void SendTags(void) override;
	virtual void SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void    SendPlayerMoveLook(const Vector3d a_Pos, const float a_Yaw, const float a_Pitch, const bool a_IsRelative) override;

	virtual void HandlePacketClientSettings(cByteBuffer & a_ByteBuffer) override;
	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;

	virtual UInt32 GetPacketID(ePacketType a_PacketType) const override;
	virtual Version GetProtocolVersion() const override;
};
