
// Protocol_1_9.h

/*
Declares the 1.9 protocol classes:
	- cProtocol_1_9_0
		- release 1.9 protocol (#107)
	- cProtocol_1_9_1
		- release 1.9.1 protocol (#108)
	- cProtocol_1_9_2
		- release 1.9.2 protocol (#109)
	- cProtocol_1_9_4
		- release 1.9.4 protocol (#110)
*/





#pragma once

#include "Protocol.h"
#include "Protocol_1_8.h"
#include "../ByteBuffer.h"

#include "../mbedTLS++/AesCfb128Decryptor.h"
#include "../mbedTLS++/AesCfb128Encryptor.h"





class cProtocol_1_9_0:
	public cProtocol_1_8_0
{
	using Super = cProtocol_1_8_0;

public:

	cProtocol_1_9_0(cClientHandle * a_Client, const AString & a_ServerAddress, State a_State);

	/** Sending stuff to clients (alphabetically sorted): */
	virtual void SendAttachEntity               (const cEntity & a_Entity, const cEntity & a_Vehicle) override;
	virtual void SendDetachEntity               (const cEntity & a_Entity, const cEntity & a_PreviousVehicle) override;
	virtual void SendEntityEquipment            (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendEntityMetadata             (const cEntity & a_Entity) override;
	virtual void SendEntityPosition             (const cEntity & a_Entity) override;
	virtual void SendEntityStatus               (const cEntity & a_Entity, char a_Status) override;
	virtual void SendExperienceOrb              (const cExpOrb & a_ExpOrb) override;
	virtual void SendKeepAlive                  (UInt32 a_PingID) override;
	virtual void SendLeashEntity                (const cEntity & a_Entity, const cEntity & a_EntityLeashedTo) override;
	virtual void SendMapData                    (const cMap & a_Map, int a_DataStartX, int a_DataStartY) override;
	virtual void SendPaintingSpawn              (const cPainting & a_Painting) override;
	virtual void SendPlayerMaxSpeed             (void) override;
	virtual void SendPlayerMoveLook             (void) override;
	virtual void SendPlayerSpawn                (const cPlayer & a_Player) override;
	virtual void SendSoundEffect                (const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch) override;
	virtual void SendSpawnMob                   (const cMonster & a_Mob) override;
	virtual void SendThunderbolt                (int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void SendUnleashEntity              (const cEntity & a_Entity) override;
	virtual void SendUnloadChunk                (int a_ChunkX, int a_ChunkZ) override;

protected:

	/** The current teleport ID, and whether it has been confirmed by the client */
	bool m_IsTeleportIdConfirmed;
	UInt32 m_OutstandingTeleportId;

	/** Get the packet ID for a given packet. */
	virtual UInt32 GetPacketID(ePacketType a_Packet) override;

	/** Returns 1.9. */
	virtual Version GetProtocolVersion() override;

	/** Reads and handles the packet. The packet length and type have already been read.
	Returns true if the packet was understood, false if it was an unknown packet. */
	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;

	// Packet handlers while in the Game state (m_State == 3):
	virtual void HandlePacketAnimation              (cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketBlockDig               (cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketBlockPlace             (cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketBoatSteer              (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketClientSettings         (cByteBuffer & a_ByteBuffer) override;
	virtual void HandleConfirmTeleport              (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketEntityAction           (cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketPlayerPos              (cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketPlayerPosLook          (cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketSteerVehicle           (cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketTabComplete            (cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketUpdateSign             (cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketUseEntity              (cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketUseItem                (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketVehicleMove            (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketWindowClick            (cByteBuffer & a_ByteBuffer) override;

	/** Parses item metadata as read by ReadItem(), into the item enchantments. */
	virtual void ParseItemMetadata(cItem & a_Item, const AString & a_Metadata) override;

	/** Converts the hand parameter received by the protocol into eHand constants.
	If the received value doesn't match any of the know value, raise an assertion fail or return hMain. */
	static eHand HandIntToEnum(Int32 a_Hand);

	/** Sends the entity type and entity-dependent data required for the entity to initially spawn. */
	virtual void SendEntitySpawn(const cEntity & a_Entity, const UInt8 a_ObjectType, const Int32 a_ObjectData) override;

	/** Writes the item data into a packet. */
	virtual void WriteItem(cPacketizer & a_Pkt, const cItem & a_Item) override;

	/** Writes the metadata for the specified entity, not including the terminating 0xff. */
	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) override;

	/** Writes the mob-specific metadata for the specified mob */
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) override;

	/** Writes the entity properties for the specified entity, including the Count field. */
	virtual void WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity) override;

	/** Writes the block entity data for the specified block entity into the packet. */
	virtual void WriteBlockEntity(cPacketizer & a_Pkt, const cBlockEntity & a_BlockEntity) override;

	/** Types used within metadata */
	enum eMetadataType
	{
		METADATA_TYPE_BYTE              = 0,
		METADATA_TYPE_VARINT            = 1,
		METADATA_TYPE_FLOAT             = 2,
		METADATA_TYPE_STRING            = 3,
		METADATA_TYPE_CHAT              = 4,
		METADATA_TYPE_ITEM              = 5,
		METADATA_TYPE_BOOL              = 6,
		METADATA_TYPE_ROTATION          = 7,
		METADATA_TYPE_POSITION          = 8,
		METADATA_TYPE_OPTIONAL_POSITION = 9,
		METADATA_TYPE_DIRECTION         = 10,
		METADATA_TYPE_OPTIONAL_UUID     = 11,
		METADATA_TYPE_BLOCKID           = 12
	} ;
} ;





/** The version 108 protocol, used by 1.9.1.  Uses an int rather than a byte for dimension in join game. */
class cProtocol_1_9_1:
	public cProtocol_1_9_0
{
	using Super = cProtocol_1_9_0;

public:

	using Super::cProtocol_1_9_0;

protected:

	virtual void SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;

	/** Returns 1.9.1. */
	virtual Version GetProtocolVersion() override;
} ;





/** The version 109 protocol, used by 1.9.2.  Same as 1.9.1, except the server list ping version number changed with the protocol number. */
class cProtocol_1_9_2:
	public cProtocol_1_9_1
{
	using Super = cProtocol_1_9_1;

public:

	using Super::cProtocol_1_9_1;

protected:

	/** Returns 1.9.2. */
	virtual Version GetProtocolVersion() override;
} ;





/** The version 110 protocol, used by 1.9.3 and 1.9.4. */
class cProtocol_1_9_4:
	public cProtocol_1_9_2
{
	using Super = cProtocol_1_9_2;

public:

	using Super::cProtocol_1_9_2;

protected:

	virtual void SendUpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override;

	/** Returns 1.9.4. */
	virtual Version GetProtocolVersion() override;
	virtual UInt32 GetPacketID(ePacketType a_Packet) override;
} ;
