#pragma once
#include "Protocol_1_20.h"





class cProtocol_1_21
	: public cProtocol_1_20_5
{
	using Super = cProtocol_1_20_5;
public:

	using Super::Super;

protected:
	virtual void HandlePacketUseItem(cByteBuffer & a_ByteBuffer) override;

	virtual void SendDynamicRegistries() override;
	virtual void SendSelectKnownPacks() override;
	virtual void SendUpdateBlockEntity(cBlockEntity & a_BlockEntity) override;

	virtual UInt32 GetPacketID(ePacketType a_PacketType) const override;

	virtual Version GetProtocolVersion() const override;
	virtual UInt32 GetProtocolSoundID(const AString & a_SoundName) const override;
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
	virtual void SendTimeUpdate(cTickTimeLong a_WorldAge, cTickTimeLong a_WorldDate, bool a_DoDaylightCycle) override;
	virtual void SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void SendPlayerMoveLook(const Vector3d a_Pos, const float a_Yaw, const float a_Pitch, const bool a_IsRelative) override;
	virtual void SendEntityPosition(const cEntity & a_Entity) override;
	virtual void SendDynamicRegistries() override;
	virtual void SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendRespawn(eDimension a_Dimension) override;
	virtual void SendUpdateBlockEntity(cBlockEntity & a_BlockEntity) override;
	virtual void SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount) override;
	virtual void SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data) override;

	virtual void HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketClientSettings(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketSteerVehicle(cByteBuffer & a_ByteBuffer) override;
	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;

	virtual bool ReadComponent(cByteBuffer & a_ByteBuffer, DataComponents::DataComponent & a_Result) const override;
	virtual void WriteComponent(cPacketizer & a_Pkt, const DataComponents::DataComponent & a_Component) const override;


	virtual UInt8  GetProtocolEntityType(eEntityType a_Type) const override;
	virtual UInt32 GetBlockEntityID(const cBlockEntity & a_BlockEntity) const override;

	virtual UInt32 GetPacketID(ePacketType a_PacketType) const override;
	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_21_4
	: public cProtocol_1_21_2
{
	using Super = cProtocol_1_21_2;
public:

	using Super::Super;

protected:

	virtual void SendSelectKnownPacks() override;

	virtual void SendTags(void) override;

	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void HandlePacketVehicleMove(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketClientTickEnd(cByteBuffer & a_ByteBuffer);

	virtual UInt8  GetProtocolEntityType(eEntityType a_Type) const override;
	virtual UInt8  GetEntityMetadataID(EntityMetadata a_Metadata) const override;

	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_21_5
	: public cProtocol_1_21_4
{
	using Super = cProtocol_1_21_4;
public:

	using Super::Super;

protected:

	virtual void SendSelectKnownPacks() override;
	virtual void SendExperienceOrb(const cExpOrb & a_ExpOrb) override;
	virtual void SendDynamicRegistries() override;

	virtual void SendTags(void) override;

	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;

	virtual UInt8  GetProtocolEntityType(eEntityType a_Type) const override;
	virtual UInt8  GetEntityMetadataID(EntityMetadata a_Metadata) const override;

	virtual UInt32 GetPacketID(ePacketType a_PacketType) const override;
	virtual Version GetProtocolVersion() const override;

	virtual bool ReadComponent(cByteBuffer & a_ByteBuffer, DataComponents::DataComponent & a_Result) const override;
	virtual void WriteComponent(cPacketizer & a_Pkt, const DataComponents::DataComponent & a_Component) const override;
};





class cProtocol_1_21_6
	: public cProtocol_1_21_5
{
	using Super = cProtocol_1_21_5;
public:

	using Super::Super;

protected:

	virtual void SendSelectKnownPacks() override;
	// virtual void SendDynamicRegistries() override;

	virtual void SendTags(void) override;

	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;

	virtual UInt8  GetProtocolEntityType(eEntityType a_Type) const override;
	// virtual UInt8  GetEntityMetadataID(EntityMetadata a_Metadata) const override;

	virtual UInt32 GetPacketID(ePacketType a_PacketType) const override;
	virtual Version GetProtocolVersion() const override;
	virtual Int32  GetProtocolCommandArgumentID(eCommandParserType a_ParserType) const override;
};





class cProtocol_1_21_7
	: public cProtocol_1_21_6
{
	using Super = cProtocol_1_21_6;
public:

	using Super::Super;

protected:
	virtual void SendTags(void) override;

	virtual void SendSelectKnownPacks() override;
	virtual Version GetProtocolVersion() const override;
};
