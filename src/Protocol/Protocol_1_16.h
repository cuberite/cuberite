#pragma once

#include "Protocol_1_15.h"


class cProtocol_1_16:
	public cProtocol_1_15
{
	using Super = cProtocol_1_15;

public:

	using Super::Super;

protected:

	virtual UInt32 GetPacketID(ePacketType a_PacketType) const override;
	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;

	virtual void SendLoginSuccess(void) override;
	virtual void SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void SendRespawn(eDimension a_Dimension) override;
	virtual void SendChatRaw(const AString & a_MessageRaw, eChatType a_Type) override;
	virtual void SendThunderbolt(Vector3i a_Origin) override;
	virtual void HandlePacketPlayerAbilities(cByteBuffer & a_ByteBuffer) override;


	virtual int GetProtocolParticleID(const AString & a_ParticleName) const override;
	virtual UInt32 GetProtocolMobType(eMonsterType a_MobType) const override;
	virtual UInt8 GetProtocolEntityType(const cEntity & a_Entity) const override;

	virtual void WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity) const override;

	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_16_1:
	public cProtocol_1_16
{
	using Super = cProtocol_1_16;

public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
	virtual void HandlePacketUseEntity(cByteBuffer & a_ByteBuffer) override;
};





class cProtocol_1_16_2:
	public cProtocol_1_16_1
{
	using Super = cProtocol_1_16_1;

public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
	virtual UInt32 GetPacketID(ePacketType a_PacketType) const override;
	virtual void SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
};





class cProtocol_1_16_3:
	public cProtocol_1_16_2
{
	using Super = cProtocol_1_16_2;

public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_16_4:
	public cProtocol_1_16_3
{
	using Super = cProtocol_1_16_3;

public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
};
