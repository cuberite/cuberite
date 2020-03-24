
// Protocol_1_13.h

/*
Declares the 1.13 protocol classes:
	- cProtocol_1_13
		- release 1.13 protocol (#393)
(others may be added later in the future for the 1.13 release series)
*/





#pragma once

#include "Protocol_1_12.h"





// fwd:
class BlockTypePalette;





class cProtocol_1_13 :
	public cProtocol_1_12_2
{
	typedef cProtocol_1_12_2 Super;

public:
	cProtocol_1_13(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);

	virtual void Initialize(cClientHandle & a_Client) override;


protected:

	/** The palette used to transform internal block type palette into the protocol-specific ID. */
	std::shared_ptr<const BlockTypePalette> m_BlockTypePalette;

	/** Temporary hack for initial 1.13+ support while keeping BLOCKTYPE data:
	Map of the BLOCKTYPE#META to the protocol-specific NetBlockID. */
	std::map<UInt32, UInt32> m_BlockTypeMap;


	/** Returns the string identifying the palettes' version, such as "1.13" or "1.14.4".
	The palettes for that version are loaded into m_BlockTypePalette and m_ItemTypePalette. */
	virtual AString GetPaletteVersion() const;

	// Outgoing packet type translation:
	virtual UInt32 GetPacketID(ePacketType a_PacketType) override;

	// Packet receiving:
	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketPluginMessage(cByteBuffer & a_ByteBuffer) override;

	// Packet sending:
	virtual void SendBlockChange                (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void SendBlockChanges               (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;
	virtual void SendChunkData                  (int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer) override;
	virtual void SendMapData                    (const cMap & a_Map, int a_DataStartX, int a_DataStartY) override;
	virtual void SendPaintingSpawn              (const cPainting & a_Painting) override;
	virtual void SendPluginMessage              (const AString & a_Channel, const AString & a_Message) override;
	virtual void SendScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) override;
	virtual void SendStatistics                 (const cStatManager & a_Manager) override;
	virtual void SendTabCompletionResults       (const AStringVector & a_Results) override;
	virtual void SendUpdateBlockEntity          (cBlockEntity & a_BlockEntity) override;

	virtual bool ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes) override;
	virtual void WriteItem(cPacketizer & a_Pkt, const cItem & a_Item) override;
	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) override;
};
