
// Protocol_1_13.h

/*
Declares the 1.13 protocol classes:
	- cProtocol_1_13
		- release 1.13 protocol (#393)
	- cProtocol_1_13_1
		- release 1.13.1 protocol (#401)
	- cProtocol_1_13_2
		- release 1.13.2 protocol (#404)
*/





#pragma once

#include "Protocol_1_12.h"
#include "Packetizer.h"
#include "Palettes/Upgrade.h"





class cProtocol_1_13:
	public cProtocol_1_12_2
{
	using Super = cProtocol_1_12_2;

public:

	using Super::Super;

protected:

	virtual void SendBlockChange                (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void SendBlockChanges               (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;
	virtual void SendMapData                    (const cMap & a_Map, int a_DataStartX, int a_DataStartY) override;
	virtual void SendPaintingSpawn              (const cPainting & a_Painting) override;
	virtual void SendParticleEffect             (const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data) override;
	virtual void SendScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) override;
	virtual void SendStatistics                 (const StatisticsManager & a_Manager) override;
	virtual void SendTabCompletionResults       (const AStringVector & a_Results) override;
	virtual void SendUpdateBlockEntity          (cBlockEntity & a_BlockEntity) override;
	virtual UInt8 GetEntityMetadataID(EntityMetadata a_Metadata) const;
	virtual UInt8 GetEntityMetadataID(EntityMetadataType a_FieldType) const;
	virtual std::pair<short, short> GetItemFromProtocolID(UInt32 a_ProtocolID) const;
	virtual UInt32 GetPacketID(ePacketType a_PacketType) const override;
	virtual UInt32 GetProtocolBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const;
	virtual signed char GetProtocolEntityStatus(EntityAnimation a_Animation) const override;
	virtual UInt32 GetProtocolItemType(short a_ItemID, short a_ItemDamage) const;
	virtual UInt32 GetProtocolMobType(eMonsterType a_MobType) const override;
	virtual UInt32 GetProtocolStatisticType(CustomStatistic a_Statistic) const;
	virtual Version GetProtocolVersion() const override;

	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void HandlePacketPluginMessage(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketSetBeaconEffect(cByteBuffer & a_ByteBuffer);

	virtual bool ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes) const override;
	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, EntityMetadata a_Metadata, EntityMetadataType a_FieldType) const;
	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const override;
	virtual void WriteItem(cPacketizer & a_Pkt, const cItem & a_Item) const override;
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) const override;
};





class cProtocol_1_13_1 :
	public cProtocol_1_13
{
	using Super = cProtocol_1_13;

public:

	using Super::Super;

protected:

	virtual void SendBossBarAdd(UInt32 a_UniqueID, const cCompositeChat & a_Title, float a_FractionFilled, BossBarColor a_Color, BossBarDivisionType a_DivisionType, bool a_DarkenSky, bool a_PlayEndMusic, bool a_CreateFog) override;
	virtual void SendBossBarUpdateFlags(UInt32 a_UniqueID, bool a_DarkenSky, bool a_PlayEndMusic, bool a_CreateFog) override;

	virtual std::pair<short, short> GetItemFromProtocolID(UInt32 a_ProtocolID) const override;
	virtual UInt32 GetProtocolBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override;
	virtual UInt32 GetProtocolItemType(short a_ItemID, short a_ItemDamage) const override;
	virtual UInt32 GetProtocolStatisticType(CustomStatistic a_Statistic) const override;
	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_13_2 :
	public cProtocol_1_13_1
{
	using Super = cProtocol_1_13_1;

public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
	virtual bool ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes) const override;
	virtual void WriteItem(cPacketizer & a_Pkt, const cItem & a_Item) const override;
};
