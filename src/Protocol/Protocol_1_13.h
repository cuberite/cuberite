
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

	using Super::cProtocol_1_12_2;

protected:

	virtual void SendBlockChange                (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	template <auto Palette> void SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);  // Template to avoid virtual calls in tight loops
	virtual void SendBlockChanges               (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;

	/** Common implementation of multiblock change sending, templated to avoid virtual calls in tight loops. */
	template <auto Palette>void SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
	{
		ASSERT(m_State == 3);  // In game mode?

		cPacketizer Pkt(*this, pktBlockChanges);
		Pkt.WriteBEInt32(a_ChunkX);
		Pkt.WriteBEInt32(a_ChunkZ);
		Pkt.WriteVarInt32(static_cast<UInt32>(a_Changes.size()));
		for (const auto & Change : a_Changes)
		{
			Int16 Coords = static_cast<Int16>(Change.m_RelY | (Change.m_RelZ << 8) | (Change.m_RelX << 12));
			Pkt.WriteBEInt16(Coords);
			Pkt.WriteVarInt32(Palette(PaletteUpgrade::FromBlock(Change.m_BlockType, Change.m_BlockMeta)));
		}  // for itr - a_Changes[]
	}

	virtual void SendMapData                    (const cMap & a_Map, int a_DataStartX, int a_DataStartY) override;
	virtual void SendPaintingSpawn              (const cPainting & a_Painting) override;
	virtual void SendParticleEffect             (const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data) override;
	virtual void SendPluginMessage              (const AString & a_Channel, const AString & a_Message) override;
	virtual void SendScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) override;
	virtual void SendStatistics                 (const cStatManager & a_Manager) override;
	virtual void SendTabCompletionResults       (const AStringVector & a_Results) override;
	virtual void SendUpdateBlockEntity          (cBlockEntity & a_BlockEntity) override;

	/** Translates outgoing packet types. */
	virtual UInt32 GetPacketID(ePacketType a_PacketType) override;

	/** Returns 1.13. */
	virtual Version GetProtocolVersion() override;

	/** Converts eMonsterType to protocol-specific mob types */
	virtual UInt32 GetProtocolMobType(eMonsterType a_MobType) override;

	virtual UInt8 GetEntityMetadataID(eEntityMetadata a_Metadata);
	virtual UInt8 GetEntityMetadataID(eEntityMetadataType a_FieldType);
	virtual std::pair<short, short> GetItemFromProtocolID(UInt32 a_ProtocolID);
	virtual UInt32 GetProtocolIDFromItem(short a_ItemID, short a_ItemDamage);

	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void HandlePacketPluginMessage(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketSetBeaconEffect(cByteBuffer & a_ByteBuffer);

	virtual bool ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes) override;
	virtual void WriteItem(cPacketizer & a_Pkt, const cItem & a_Item) override;
	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const eEntityMetadata a_Metadata, const eEntityMetadataType a_FieldType);
	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) override;
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) override;
};





class cProtocol_1_13_1 :
	public cProtocol_1_13
{
	using Super = cProtocol_1_13;

public:

	using Super::cProtocol_1_13;

protected:

	virtual void SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;

	virtual Version GetProtocolVersion() override;
	virtual std::pair<short, short> GetItemFromProtocolID(UInt32 a_ProtocolID) override;
	virtual UInt32 GetProtocolIDFromItem(short a_ItemID, short a_ItemDamage) override;
};





class cProtocol_1_13_2 :
	public cProtocol_1_13_1
{
	using Super = cProtocol_1_13_1;

public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() override;
	virtual bool ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes) override;
	virtual void WriteItem(cPacketizer & a_Pkt, const cItem & a_Item) override;
};
