#pragma once
#include "Protocol_1_18.h"





class cProtocol_1_19
	: public cProtocol_1_18_2
{
	using Super = cProtocol_1_18_2;
public:

	using Super::Super;

protected:
	virtual UInt32  GetPacketID(ePacketType a_PacketType) const override;

	virtual void    SendAcknowledgeBlockChange(int a_SequenceId) override;
	virtual void    SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void    SendChatRaw(const AString & a_MessageRaw, eChatType a_Type) override;
	virtual void    SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount) override;
	virtual void    SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data) override;
	virtual void    SendEntitySpawn(const cEntity & a_Entity, const UInt8 a_ObjectType, const Int32 a_ObjectData) override;
	virtual void    SendLoginSuccess(void) override;
	virtual void    SendPlayerListAddPlayer(const cPlayer & a_Player) override;
	virtual void    SendRespawn(eDimension a_Dimension) override;
	virtual void    SendSpawnMob(const cMonster & a_Mob) override;
	virtual void    SendSoundEffect(const AString & a_SoundName, Vector3d a_Origin, float a_Volume, float a_Pitch) override;
	virtual void    SendPaintingSpawn(const cPainting & a_Painting) override;

	virtual void    HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer) override;
	virtual bool    HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void    HandlePacketLoginStart(cByteBuffer & a_ByteBuffer) override;
	virtual void    HandlePacketChatMessage(cByteBuffer & a_ByteBuffer) override;
	virtual void    HandlePacketCommandExecution(cByteBuffer & a_ByteBuffer) override;
	virtual void    HandlePacketBlockDig(cByteBuffer & a_ByteBuffer) override;
	virtual void    HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer) override;
	virtual void    HandlePacketUseItem(cByteBuffer & a_ByteBuffer) override;
	virtual void    HandlePacketWindowClick(cByteBuffer & a_ByteBuffer) override;
	virtual void    HandlePacketSetBeaconEffect(cByteBuffer & a_ByteBuffer) override;

	virtual UInt32  GetProtocolMobType(eMonsterType a_MobType) const override;
	virtual int     GetProtocolParticleID(const AString & a_ParticleName) const override;
	virtual UInt8   GetProtocolEntityType(const cEntity & a_Entity) const override;
	virtual Int32   GetProtocolCommandArgumentID(eCommandParserType a_ParserType) const override;
	virtual UInt32  GetProtocolBlockType(BlockState a_Block) const override;
	virtual UInt32  GetProtocolItemType(Item a_ItemID) const override;
	virtual Item    GetItemFromProtocolID(UInt32 a_ProtocolID) const override;


	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_19_1
	: public cProtocol_1_19
{
	using Super = cProtocol_1_19;
public:

	using Super::Super;

protected:
	virtual UInt32  GetPacketID(ePacketType a_PacketType) const override;


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
	virtual UInt32  GetPacketID(ePacketType a_PacketType) const override;
	virtual void    WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const override;
	virtual void    WriteEntityMetadata(cPacketizer & a_Pkt, const EntityMetadata a_Metadata, const EntityMetadataType a_FieldType) const override;

	virtual void    SendChatRaw(const AString & a_MessageRaw, eChatType a_Type) override;
	virtual void    SendSoundEffect(const AString & a_SoundName, Vector3d a_Origin, float a_Volume, float a_Pitch) override;
	virtual void    SendPlayerListInitChat(const cPlayer & a_Player) override;
	virtual void    SendPlayerListAddPlayer(const cPlayer & a_Player) override;
	virtual void    SendPlayerListUpdatePing() override;
	virtual void    SendPlayerListUpdateGameMode(const cPlayer & a_Player) override;
	virtual void    SendPlayerListRemovePlayer(const cPlayer & a_Player) override;

	virtual void    HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer) override;
	virtual void    HandlePacketLoginStart(cByteBuffer & a_ByteBuffer) override;
	virtual bool    HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void    HandlePacketCommandExecution(cByteBuffer & a_ByteBuffer) override;
	virtual void    HandlePacketChatMessage(cByteBuffer & a_ByteBuffer) override;

	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_19_4
	: public cProtocol_1_19_3
{
	using Super = cProtocol_1_19_3;
public:

	using Super::Super;

protected:
	virtual UInt32  GetPacketID(ePacketType a_PacketType) const override;
	virtual void    WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const override;
	virtual void    WriteEntityMetadata(cPacketizer & a_Pkt, const EntityMetadata a_Metadata, const EntityMetadataType a_FieldType) const override;

	virtual void    SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void    SendPlayerMoveLook(const Vector3d a_Pos, const float a_Yaw, const float a_Pitch, const bool a_IsRelative) override;
	virtual void    SendRespawn(eDimension a_Dimension) override;

	virtual bool    HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void    HandlePacketPlayerSession(cByteBuffer & a_ByteBuffer) override;

	virtual UInt32  GetProtocolMobType(eMonsterType a_MobType) const override;
	virtual UInt8   GetProtocolEntityType(const cEntity & a_Entity) const override;
	virtual Int32   GetProtocolCommandArgumentID(eCommandParserType a_ParserType) const override;

	virtual Version GetProtocolVersion() const override;
};
