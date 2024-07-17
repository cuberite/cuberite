#pragma once
#include "Protocol_1_18.h"





class cProtocol_1_19
	: public cProtocol_1_18_2
{
	using Super = cProtocol_1_18_2;
public:

	using Super::Super;

protected:
	virtual UInt32	GetPacketID(ePacketType a_PacketType) const override;

	virtual void    SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void    SendChatRaw(const AString & a_MessageRaw, eChatType a_Type) override;
	virtual void    SendEntitySpawn(const cEntity & a_Entity, const UInt8 a_ObjectType, const Int32 a_ObjectData) override;
	virtual void    SendLoginSuccess(void) override;
	virtual void    SendPlayerListAddPlayer(const cPlayer & a_Player) override;
	virtual void    SendRespawn(eDimension a_Dimension) override;

	virtual void    HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer) override;
    virtual bool    HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void    HandlePacketLoginStart(cByteBuffer & a_ByteBuffer) override;
	virtual void    HandlePacketChatMessage(cByteBuffer & a_ByteBuffer) override;
	virtual void    HandlePacketCommandExecution(cByteBuffer & a_ByteBuffer) override;

	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_19_1
	: public cProtocol_1_19
{
	using Super = cProtocol_1_19;
public:

	using Super::Super;
	

  protected:
	virtual UInt32	GetPacketID(ePacketType a_PacketType) const override;


	virtual void    SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;

	virtual void    HandlePacketLoginStart(cByteBuffer & a_ByteBuffer) override;
	virtual bool    HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void    HandlePacketCommandExecution(cByteBuffer & a_ByteBuffer) override;

	virtual Version GetProtocolVersion() const override;
};






class cProtocol_1_19_3
	: public cProtocol_1_19_1
{
	using Super = cProtocol_1_19_1;
public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
};






class cProtocol_1_19_4
	: public cProtocol_1_19_3
{
	using Super = cProtocol_1_19_3;
public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
};
