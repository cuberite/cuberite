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
	virtual void    SendRespawn(eDimension a_Dimension) override;
	virtual void    SendEditSign(Vector3i a_BlockPos) override;

	virtual void    HandlePacketUpdateSign(cByteBuffer & a_ByteBuffer) override;

	virtual void    WriteBlockEntity(cFastNBTWriter & a_Writer, const cBlockEntity & a_BlockEntity) const override;

	virtual UInt32  GetProtocolBlockType(BlockState a_Block) const override;
	virtual UInt32  GetProtocolItemType(Item a_ItemID) const override;
	virtual Item    GetItemFromProtocolID(UInt32 a_ProtocolID) const override;

	virtual int     GetProtocolParticleID(const AString & a_ParticleName) const override;
	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_20_2
	: public cProtocol_1_20
{
	using Super = cProtocol_1_20;
public:

	using Super::Super;

protected:
	virtual UInt32  GetPacketID(ePacketType a_PacketType) const override;

	virtual void    SendLoginSuccess(void) override;
	virtual void    SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void    SendPluginMessage(const AString & a_Channel, const ContiguousByteBufferView a_Message) override;
	virtual void    SendPlayerSpawn(const cPlayer & a_Player) override;
	virtual void    SendRespawn(eDimension a_Dimension) override;
	virtual void    SendUnloadChunk(int a_ChunkX, int a_ChunkZ) override;
	virtual void    SendFinishConfiguration() override;
	virtual void    SendDynamicRegistries() override;

	virtual bool    HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void    HandlePacketEnterConfiguration(cByteBuffer & a_ByteBuffer) override;
	virtual void    HandlePacketReady(cByteBuffer & a_ByteBuffer) override;
	virtual void    HandlePacketLoginStart(cByteBuffer & a_ByteBuffer) override;

	virtual void    WriteItem(cPacketizer & a_Pkt, const cItem & a_Item) const override;
	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_20_3
	: public cProtocol_1_20_2
{
	using Super = cProtocol_1_20_2;
public:

	using Super::Super;

protected:
	virtual UInt32 GetPacketID(ePacketType a_PacketType) const override;

	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;

	virtual void SendDisconnect(const AString & a_Reason) override;
	virtual void SendChat(const AString & a_Message, eChatType a_Type) override;
	virtual void SendChat(const cCompositeChat & a_Message, eChatType a_Type, bool a_ShouldUseChatPrefixes) override;
	virtual void SendChatRaw(const AString & a_MessageRaw, eChatType a_Type) override;
	virtual void SendWindowOpen(const cWindow & a_Window) override;
	virtual void SendInitialChunksComing() override;

	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const EntityMetadata a_Metadata, const EntityMetadataType a_FieldType) const override;
	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const override;
	virtual Int32   GetProtocolCommandArgumentID(eCommandParserType a_ParserType) const override;

	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_20_5
	: public cProtocol_1_20_3
{
	using Super = cProtocol_1_20_3;
public:

	using Super::Super;

protected:
	virtual UInt32 GetPacketID(ePacketType a_PacketType) const override;

	virtual void SendDynamicRegistries() override;
	virtual void SendSelectKnownPacks() override;
	virtual void SendLoginSuccess(void) override;
	virtual void SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void SendRespawn(eDimension a_Dimension) override;

	virtual void HandlePacketCommandExecution(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketLoginStart(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketEnterConfiguration(cByteBuffer & a_ByteBuffer) override;
	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;

	virtual void WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity) const override;
	virtual void WriteItem(cPacketizer & a_Pkt, const cItem & a_Item) const override;
	virtual bool ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes) const override;
	virtual Version GetProtocolVersion() const override;

	virtual UInt32    GetProtocolMobType(eMonsterType a_MobType) const override;
	virtual int       GetProtocolParticleID(const AString & a_ParticleName) const override;
	virtual UInt8     GetProtocolEntityType(const cEntity & a_Entity) const override;
	virtual Int32     GetProtocolCommandArgumentID(eCommandParserType a_ParserType) const override;
};
