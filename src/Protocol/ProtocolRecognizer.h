
// ProtocolRecognizer.h

// Interfaces to the cProtocolRecognizer class representing the meta-protocol that recognizes possibly multiple
// protocol versions and redirects everything to them





#pragma once

#include "Protocol.h"
#include "../ByteBuffer.h"





// Adjust these if a new protocol is added or an old one is removed:
#define MCS_CLIENT_VERSIONS "1.7.x, 1.8.x"
#define MCS_PROTOCOL_VERSIONS "4, 5, 47"





class cProtocolRecognizer :
	public cProtocol
{
	typedef cProtocol super;

public:
	enum
	{
		PROTO_VERSION_1_7_2 = 4,
		PROTO_VERSION_1_7_6 = 5,
		PROTO_VERSION_1_8_0 = 47,
		PROTO_VERSION_TESTING = 59,  // 15w34b
		PROTO_VERSION_1_9_0 = 107,
		PROTO_VERSION_1_9_1 = 108,
		PROTO_VERSION_1_9_2 = 109
	} ;

	cProtocolRecognizer(cClientHandle * a_Client);
	virtual ~cProtocolRecognizer();

	/** Translates protocol version number into protocol version text: 49 -> "1.4.4" */
	static AString GetVersionTextFromInt(int a_ProtocolVersion);

	/** Called when client sends some data: */
	virtual void DataReceived(const char * a_Data, size_t a_Size) override;

	/** Sending stuff to clients (alphabetically sorted): */
	virtual void SendAttachEntity               (const cEntity & a_Entity, const cEntity * a_Vehicle) override;
	virtual void SendBlockAction                (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType) override;
	virtual void SendBlockBreakAnim             (UInt32 a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage) override;
	virtual void SendBlockChange                (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void SendBlockChanges               (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;
	virtual void SendChat                       (const AString & a_Message, eChatType a_Type) override;
	virtual void SendChat                       (const cCompositeChat & a_Message, eChatType a_Type, bool a_ShouldUseChatPrefixes) override;
	virtual void SendChunkData                  (int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer) override;
	virtual void SendCollectEntity              (const cEntity & a_Entity, const cPlayer & a_Player) override;
	virtual void SendDestroyEntity              (const cEntity & a_Entity) override;
	virtual void SendDisconnect                 (const AString & a_Reason) override;
	virtual void SendEditSign                   (int a_BlockX, int a_BlockY, int a_BlockZ) override;  ///< Request the client to open up the sign editor for the sign (1.6+)
	virtual void SendEntityEffect               (const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration) override;
	virtual void SendEntityEquipment            (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendEntityHeadLook             (const cEntity & a_Entity) override;
	virtual void SendEntityLook                 (const cEntity & a_Entity) override;
	virtual void SendEntityMetadata             (const cEntity & a_Entity) override;
	virtual void SendEntityProperties           (const cEntity & a_Entity) override;
	virtual void SendEntityRelMove              (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendEntityRelMoveLook          (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendEntityStatus               (const cEntity & a_Entity, char a_Status) override;
	virtual void SendEntityVelocity             (const cEntity & a_Entity) override;
	virtual void SendExplosion                  (double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion) override;
	virtual void SendGameMode                   (eGameMode a_GameMode) override;
	virtual void SendHealth                     (void) override;
	virtual void SendHideTitle                  (void) override;
	virtual void SendInventorySlot              (char a_WindowID, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendKeepAlive                  (UInt32 a_PingID) override;
	virtual void SendLogin                      (const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void SendLoginSuccess               (void) override;
	virtual void SendMapData                    (const cMap & a_Map, int a_DataStartX, int a_DataStartY) override;
	virtual void SendParticleEffect             (const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount) override;
	virtual void SendParticleEffect             (const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data) override;
	virtual void SendPaintingSpawn              (const cPainting & a_Painting) override;
	virtual void SendPickupSpawn                (const cPickup & a_Pickup) override;
	virtual void SendPlayerAbilities            (void) override;
	virtual void SendEntityAnimation            (const cEntity & a_Entity, char a_Animation) override;
	virtual void SendPlayerListAddPlayer        (const cPlayer & a_Player) override;
	virtual void SendPlayerListRemovePlayer     (const cPlayer & a_Player) override;
	virtual void SendPlayerListUpdateGameMode   (const cPlayer & a_Player) override;
	virtual void SendPlayerListUpdatePing       (const cPlayer & a_Player) override;
	virtual void SendPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName) override;
	virtual void SendPlayerMaxSpeed             (void) override;
	virtual void SendPlayerMoveLook             (void) override;
	virtual void SendPlayerPosition             (void) override;
	virtual void SendPlayerSpawn                (const cPlayer & a_Player) override;
	virtual void SendPluginMessage              (const AString & a_Channel, const AString & a_Message) override;
	virtual void SendRemoveEntityEffect         (const cEntity & a_Entity, int a_EffectID) override;
	virtual void SendResetTitle                 (void) override;
	virtual void SendRespawn                    (eDimension a_Dimension, bool a_ShouldIgnoreDimensionChecks) override;
	virtual void SendExperience                 (void) override;
	virtual void SendExperienceOrb              (const cExpOrb & a_ExpOrb) override;
	virtual void SendScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) override;
	virtual void SendScoreUpdate                (const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode) override;
	virtual void SendDisplayObjective           (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) override;
	virtual void SendSetSubTitle                (const cCompositeChat & a_SubTitle) override;
	virtual void SendSetRawSubTitle             (const AString & a_SubTitle) override;
	virtual void SendSetTitle                   (const cCompositeChat & a_Title) override;
	virtual void SendSetRawTitle                (const AString & a_Title) override;
	virtual void SendSoundEffect                (const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch) override;
	virtual void SendSoundParticleEffect        (const EffectID a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data) override;
	virtual void SendSpawnFallingBlock          (const cFallingBlock & a_FallingBlock) override;
	virtual void SendSpawnMob                   (const cMonster & a_Mob) override;
	virtual void SendSpawnObject                (const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch) override;
	virtual void SendSpawnVehicle               (const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType) override;
	virtual void SendStatistics                 (const cStatManager & a_Manager) override;
	virtual void SendTabCompletionResults       (const AStringVector & a_Results) override;
	virtual void SendTeleportEntity             (const cEntity & a_Entity) override;
	virtual void SendThunderbolt                (int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void SendTitleTimes                 (int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks) override;
	virtual void SendTimeUpdate                 (Int64 a_WorldAge, Int64 a_TimeOfDay, bool a_DoDaylightCycle) override;
	virtual void SendUnloadChunk                (int a_ChunkX, int a_ChunkZ) override;
	virtual void SendUpdateBlockEntity          (cBlockEntity & a_BlockEntity) override;
	virtual void SendUpdateSign                 (int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override;
	virtual void SendUseBed                     (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void SendWeather                    (eWeather a_Weather) override;
	virtual void SendWholeInventory             (const cWindow & a_Window) override;
	virtual void SendWindowClose                (const cWindow & a_Window) override;
	virtual void SendWindowOpen                 (const cWindow & a_Window) override;
	virtual void SendWindowProperty             (const cWindow & a_Window, short a_Property, short a_Value) override;

	virtual AString GetAuthServerID(void) override;

	virtual void SendData(const char * a_Data, size_t a_Size) override;

protected:
	/** The recognized protocol */
	cProtocol * m_Protocol;

	/** Buffer for the incoming data until we recognize the protocol */
	cByteBuffer m_Buffer;


	/** Tries to recognize protocol based on m_Buffer contents; returns true if recognized */
	bool TryRecognizeProtocol(void);

	/** Tries to recognize a protocol in the lengthed family (1.7+), based on m_Buffer; returns true if recognized.
	The packet length and type have already been read, type is 0
	The number of bytes remaining in the packet is passed as a_PacketLengthRemaining. */
	bool TryRecognizeLengthedProtocol(UInt32 a_PacketLengthRemaining);

	/** Sends a single packet contained within the cPacketizer class.
	The cPacketizer's destructor calls this to send the contained packet; protocol may transform the data (compression in 1.8 etc). */
	virtual void SendPacket(cPacketizer & a_Pkt) override;
} ;





