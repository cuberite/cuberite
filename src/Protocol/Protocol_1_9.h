
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





class cProtocol_1_9_0:
	public cProtocol_1_8_0
{
	using Super = cProtocol_1_8_0;

public:

	cProtocol_1_9_0(cClientHandle * a_Client, const AString & a_ServerAddress, State a_State);

	virtual void SendAttachEntity         (const cEntity & a_Entity, const cEntity & a_Vehicle) override;
	virtual void SendBossBarAdd           (UInt32 a_UniqueID, const cCompositeChat & a_Title, float a_FractionFilled, BossBarColor a_Color, BossBarDivisionType a_DivisionType, bool a_DarkenSky, bool a_PlayEndMusic, bool a_CreateFog) override;
	virtual void SendBossBarRemove        (UInt32 a_UniqueID) override;
	virtual void SendBossBarUpdateFlags   (UInt32 a_UniqueID, bool a_DarkenSky, bool a_PlayEndMusic, bool a_CreateFog) override;
	virtual void SendBossBarUpdateHealth  (UInt32 a_UniqueID, float a_FractionFilled) override;
	virtual void SendBossBarUpdateStyle   (UInt32 a_UniqueID, BossBarColor a_Color, BossBarDivisionType a_DivisionType) override;
	virtual void SendBossBarUpdateTitle   (UInt32 a_UniqueID, const cCompositeChat & a_Title) override;
	virtual void SendDetachEntity         (const cEntity & a_Entity, const cEntity & a_PreviousVehicle) override;
	virtual void SendEntityEquipment      (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendEntityMetadata       (const cEntity & a_Entity) override;
	virtual void SendEntityPosition       (const cEntity & a_Entity) override;
	virtual void SendExperienceOrb        (const cExpOrb & a_ExpOrb) override;
	virtual void SendKeepAlive            (UInt32 a_PingID) override;
	virtual void SendLeashEntity          (const cEntity & a_Entity, const cEntity & a_EntityLeashedTo) override;
	virtual void SendMapData              (const cMap & a_Map, int a_DataStartX, int a_DataStartY) override;
	virtual void SendPaintingSpawn        (const cPainting & a_Painting) override;
	virtual void SendPlayerMoveLook       (Vector3d a_Pos, float a_Yaw, float a_Pitch, bool a_IsRelative) override;
	virtual void SendPlayerMoveLook       (void) override;
	virtual void SendPlayerPermissionLevel() override;
	virtual void SendPlayerSpawn          (const cPlayer & a_Player) override;
	virtual void SendSoundEffect          (const AString & a_SoundName, Vector3d a_Origin, float a_Volume, float a_Pitch) override;
	virtual void SendSoundEffect          (const eSoundEvent a_SoundEvent, Vector3d a_Origin, float a_Volume, float a_Pitch) override;
	virtual void SendSpawnMob             (const cMonster & a_Mob) override;
	virtual void SendThunderbolt          (Vector3i a_Origin) override;
	virtual void SendUnleashEntity        (const cEntity & a_Entity) override;
	virtual void SendUnloadChunk          (int a_ChunkX, int a_ChunkZ) override;

protected:

	/** The current teleport ID. */
	bool m_IsTeleportIdConfirmed;

	/** Whether the current teleport ID has been confirmed by the client. */
	UInt32 m_OutstandingTeleportId;

	virtual UInt32 GetPacketID(ePacketType a_Packet) const override;
	virtual unsigned char GetProtocolEntityAnimation(EntityAnimation a_Animation) const override;
	virtual signed char GetProtocolEntityStatus(EntityAnimation a_Animation) const override;
	virtual UInt32 GetProtocolMobType(eMonsterType a_MobType) const override;
	virtual AString GetProtocolSoundEffectAsString(eSoundEvent a_SoundEvent) const override;
	virtual Version GetProtocolVersion() const override;

	virtual bool HandlePacket                       (cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
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
	virtual void HandleVanillaPluginMessage         (cByteBuffer & a_ByteBuffer, std::string_view a_Channel) override;

	virtual void ParseItemMetadata(cItem & a_Item, ContiguousByteBufferView a_Metadata) const override;
	virtual void SendEntitySpawn(const cEntity & a_Entity, const UInt8 a_ObjectType, const Int32 a_ObjectData) override;
	virtual void WriteBlockEntity(cFastNBTWriter & a_Writer, const cBlockEntity & a_BlockEntity) const override;
	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const override;
	virtual void WriteItem(cPacketizer & a_Pkt, const cItem & a_Item) const override;
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) const override;

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

	using Super::Super;

protected:

	virtual void SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;

	virtual Version GetProtocolVersion() const override;
} ;





/** The version 109 protocol, used by 1.9.2.  Same as 1.9.1, except the server list ping version number changed with the protocol number. */
class cProtocol_1_9_2:
	public cProtocol_1_9_1
{
	using Super = cProtocol_1_9_1;

public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
} ;





/** The version 110 protocol, used by 1.9.3 and 1.9.4. */
class cProtocol_1_9_4:
	public cProtocol_1_9_2
{
	using Super = cProtocol_1_9_2;

public:

	using Super::Super;

protected:

	virtual void SendUpdateSign(Vector3i a_BlockPos, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override;

	virtual UInt32 GetPacketID(ePacketType a_Packet) const override;
	virtual Version GetProtocolVersion() const override;
} ;
