#pragma once
#include "Protocol_1_19.h"





class cProtocol_1_20
	: public cProtocol_1_19_4
{
	using Super = cProtocol_1_19_4;
public:

	using Super::Super;

protected:
	virtual void    SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void    SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;

	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_20_2
	: public cProtocol_1_20
{
	using Super = cProtocol_1_20;
public:

	using Super::Super;

protected:
	virtual UInt32	GetPacketID(ePacketType a_PacketType) const override;

	virtual void    SendLoginSuccess(void) override;
	virtual void    SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void    SendPluginMessage(const AString & a_Channel, const ContiguousByteBufferView a_Message) override;

	virtual bool    HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void    HandlePacketEnterConfiguration(cByteBuffer & a_ByteBuffer);
	virtual void    HandlePacketReady(cByteBuffer & a_ByteBuffer);
	virtual void    HandlePacketLoginStart(cByteBuffer & a_ByteBuffer) override;

	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_20_3
	: public cProtocol_1_20_2
{
	using Super = cProtocol_1_20_2;
public:

	using Super::Super;

protected:
	virtual UInt32	GetPacketID(ePacketType a_PacketType) const override;

	virtual bool    HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;

	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_20_5
	: public cProtocol_1_20_3
{
	using Super = cProtocol_1_20_3;
public:

	using Super::Super;

protected:
	virtual UInt32	GetPacketID(ePacketType a_PacketType) const override;

	virtual bool    HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;

	virtual Version GetProtocolVersion() const override;
};
