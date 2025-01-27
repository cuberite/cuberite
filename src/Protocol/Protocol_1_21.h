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
	virtual UInt32 GetProtocolBlockType(BlockState a_Block) const override;
	virtual UInt32 GetProtocolItemType(Item a_ItemID) const override;
	virtual Item GetItemFromProtocolID(UInt32 a_ProtocolID) const override;
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

	virtual void HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketClientSettings(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketSteerVehicle(cByteBuffer & a_ByteBuffer) override;
	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;

	virtual UInt32 GetProtocolBlockType(BlockState a_Block) const override;
	virtual UInt32 GetProtocolItemType(Item a_ItemID) const override;
	virtual Item GetItemFromProtocolID(UInt32 a_ProtocolID) const override;
	virtual UInt32 GetProtocolMobType(eMonsterType a_MobType) const override;
	virtual UInt8  GetProtocolEntityType(const cEntity & a_Entity) const override;

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
	// virtual void SendDynamicRegistries() override;

	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;

	// virtual UInt32 GetProtocolBlockType(BlockState a_Block) const override;
	// virtual UInt32 GetProtocolItemType(Item a_ItemID) const override;
	// virtual Item GetItemFromProtocolID(UInt32 a_ProtocolID) const override;
	virtual UInt32 GetProtocolMobType(eMonsterType a_MobType) const override;
	virtual UInt8 GetProtocolEntityType(const cEntity & a_Entity) const override;

	virtual Version GetProtocolVersion() const override;
};
