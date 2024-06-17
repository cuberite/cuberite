#pragma once
#include "Protocol_1_16.h"





class cProtocol_1_17
	: public cProtocol_1_16
{
	using Super = cProtocol_1_16;
public:

	using Super::Super;

protected:
	virtual UInt32    GetPacketID(ePacketType a_PacketType) const override;
	virtual bool      HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void      SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void      SendPlayerMoveLook(const Vector3d a_Pos, const float a_Yaw, const float a_Pitch,const bool a_IsRelative) override;
	virtual void      HandlePacketClientSettings(cByteBuffer & a_ByteBuffer) override;

	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_17_1
	: public cProtocol_1_17
{
	using Super = cProtocol_1_17;
public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
	virtual void SendWholeInventory(const cWindow & a_Window) override;
};
