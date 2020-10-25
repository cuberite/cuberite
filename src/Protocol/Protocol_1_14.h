
// Protocol_1_14.h

/*
Declares the 1.14 protocol classes:
	- cProtocol_1_14
		- release 1.14 protocol (#477)
*/





#pragma once

#include "Protocol_1_13.h"





class cProtocol_1_14:
	public cProtocol_1_13_2
{
	using Super = cProtocol_1_13_2;

public:

	using Super::cProtocol_1_13_2;

protected:

	virtual void SendBlockAction                (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType) override;
	virtual void SendBlockBreakAnim             (UInt32 a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage) override;
	virtual void SendEditSign                   (int a_BlockX, int a_BlockY, int a_BlockZ) override;  ///< Request the client to open up the sign editor for the sign (1.6+)
	virtual void SendLogin                      (const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void SendPaintingSpawn              (const cPainting & a_Painting) override;
	virtual void SendSoundParticleEffect        (const EffectID a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data) override;
	virtual void SendUpdateBlockEntity          (cBlockEntity & a_BlockEntity) override;
	virtual void SendUpdateSign                 (int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override;
	virtual void SendUseBed                     (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ) override;

	virtual UInt32 GetPacketID(ePacketType a_PacketType) override;
	virtual Version GetProtocolVersion() override;
	virtual std::pair<short, short> GetItemFromProtocolID(UInt32 a_ProtocolID) override;
	virtual UInt32 GetProtocolBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override;
	virtual UInt32 GetProtocolItemType(short a_ItemID, short a_ItemDamage) override;
	virtual UInt32 GetProtocolStatisticType(Statistic a_Statistic) override;

	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void HandlePacketBlockDig(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketUpdateSign(cByteBuffer & a_ByteBuffer) override;

	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) override {}
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) override {}
};
