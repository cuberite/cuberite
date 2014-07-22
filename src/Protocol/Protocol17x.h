
// Protocol17x.h

/*
Declares the 1.7.x protocol classes:
	- cProtocol172
		- release 1.7.2 protocol (#4)
(others may be added later in the future for the 1.7 release series)
*/





#pragma once

#include "Protocol.h"
#include "../ByteBuffer.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4127)
	#pragma warning(disable:4244)
	#pragma warning(disable:4231)
	#pragma warning(disable:4189)
	#pragma warning(disable:4702)
#endif

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#include "PolarSSL++/AesCfb128Decryptor.h"
#include "PolarSSL++/AesCfb128Encryptor.h"





// fwd:
namespace Json
{
	class Value;
}





class cProtocol172 :
	public cProtocol
{
	typedef cProtocol super;
	
public:

	cProtocol172(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);
	
	/** Called when client sends some data: */
	virtual void DataReceived(const char * a_Data, size_t a_Size) override;

	/** Sending stuff to clients (alphabetically sorted): */
	virtual void SendAttachEntity        (const cEntity & a_Entity, const cEntity * a_Vehicle) override;
	virtual void SendBlockAction         (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType) override;
	virtual void SendBlockBreakAnim	     (int a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage) override;
	virtual void SendBlockChange         (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void SendBlockChanges        (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;
	virtual void SendChat                (const AString & a_Message) override;
	virtual void SendChat                (const cCompositeChat & a_Message) override;
	virtual void SendChunkData           (int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer) override;
	virtual void SendCollectEntity       (const cEntity & a_Entity, const cPlayer & a_Player) override;
	virtual void SendDestroyEntity       (const cEntity & a_Entity) override;
	virtual void SendDisconnect          (const AString & a_Reason) override;
	virtual void SendEditSign            (int a_BlockX, int a_BlockY, int a_BlockZ) override;  ///< Request the client to open up the sign editor for the sign (1.6+)
	virtual void SendEntityEffect        (const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration) override;
	virtual void SendEntityEquipment     (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendEntityHeadLook      (const cEntity & a_Entity) override;
	virtual void SendEntityLook          (const cEntity & a_Entity) override;
	virtual void SendEntityMetadata      (const cEntity & a_Entity) override;
	virtual void SendEntityProperties    (const cEntity & a_Entity) override;
	virtual void SendEntityRelMove       (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendEntityRelMoveLook   (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendEntityStatus        (const cEntity & a_Entity, char a_Status) override;
	virtual void SendEntityVelocity      (const cEntity & a_Entity) override;
	virtual void SendExplosion           (double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion) override;
	virtual void SendGameMode            (eGameMode a_GameMode) override;
	virtual void SendHealth              (void) override;
	virtual void SendInventorySlot       (char a_WindowID, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendKeepAlive           (int a_PingID) override;
	virtual void SendLogin               (const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void SendLoginSuccess        (void) override;
	virtual void SendMapColumn           (int a_ID, int a_X, int a_Y, const Byte * a_Colors, unsigned int a_Length) override;
	virtual void SendMapDecorators       (int a_ID, const cMapDecoratorList & a_Decorators) override;
	virtual void SendMapInfo             (int a_ID, unsigned int a_Scale) override;
	virtual void SendPaintingSpawn       (const cPainting & a_Painting) override;
	virtual void SendPickupSpawn         (const cPickup & a_Pickup) override;
	virtual void SendPlayerAbilities     (void) override;
	virtual void SendEntityAnimation     (const cEntity & a_Entity, char a_Animation) override;
	virtual void SendParticleEffect      (const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmmount) override;
	virtual void SendPlayerListItem      (const cPlayer & a_Player, bool a_IsOnline) override;
	virtual void SendPlayerMaxSpeed      (void) override;
	virtual void SendPlayerMoveLook      (void) override;
	virtual void SendPlayerPosition      (void) override;
	virtual void SendPlayerSpawn         (const cPlayer & a_Player) override;
	virtual void SendPluginMessage       (const AString & a_Channel, const AString & a_Message) override;
	virtual void SendRemoveEntityEffect  (const cEntity & a_Entity, int a_EffectID) override;
	virtual void SendRespawn             (const cWorld & a_World, bool a_ShouldIgnoreDimensionChecks = false) override;
	virtual void SendSoundEffect         (const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch) override;
	virtual void SendExperience          (void) override;
	virtual void SendExperienceOrb       (const cExpOrb & a_ExpOrb) override;
	virtual void SendScoreboardObjective (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) override;
	virtual void SendScoreUpdate         (const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode) override;
	virtual void SendDisplayObjective    (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) override;
	virtual void SendSoundParticleEffect (int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data) override;
	virtual void SendSpawnFallingBlock   (const cFallingBlock & a_FallingBlock) override;
	virtual void SendSpawnMob            (const cMonster & a_Mob) override;
	virtual void SendSpawnObject         (const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch) override;
	virtual void SendSpawnVehicle        (const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType) override;
	virtual void SendStatistics          (const cStatManager & a_Manager) override;
	virtual void SendTabCompletionResults(const AStringVector & a_Results) override;
	virtual void SendTeleportEntity      (const cEntity & a_Entity) override;
	virtual void SendThunderbolt         (int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void SendTimeUpdate          (Int64 a_WorldAge, Int64 a_TimeOfDay) override;
	virtual void SendUnloadChunk         (int a_ChunkX, int a_ChunkZ) override;
	virtual void SendUpdateBlockEntity   (cBlockEntity & a_BlockEntity) override;
	virtual void SendUpdateSign          (int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override;
	virtual void SendUseBed              (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void SendWeather             (eWeather a_Weather) override;
	virtual void SendWholeInventory      (const cWindow & a_Window) override;
	virtual void SendWindowClose         (const cWindow & a_Window) override;
	virtual void SendWindowOpen          (const cWindow & a_Window) override;
	virtual void SendWindowProperty      (const cWindow & a_Window, int a_Property, int a_Value) override;

	virtual AString GetAuthServerID(void) override { return m_AuthServerID; }

protected:

	/** Composes individual packets in the protocol's m_OutPacketBuffer; sends them upon being destructed */
	class cPacketizer
	{
	public:
		cPacketizer(cProtocol172 & a_Protocol, UInt32 a_PacketType) :
			m_Protocol(a_Protocol),
			m_Out(a_Protocol.m_OutPacketBuffer),
			m_Lock(a_Protocol.m_CSPacket)
		{
			m_Out.WriteVarInt(a_PacketType);
		}
		
		~cPacketizer();

		void WriteBool(bool a_Value)
		{
			m_Out.WriteBool(a_Value);
		}
		
		void WriteByte(Byte a_Value)
		{
			m_Out.WriteByte(a_Value);
		}
		
		void WriteChar(char a_Value)
		{
			m_Out.WriteChar(a_Value);
		}
		
		void WriteShort(short a_Value)
		{
			m_Out.WriteBEShort(a_Value);
		}
		
		void WriteInt(int a_Value)
		{
			m_Out.WriteBEInt(a_Value);
		}
		
		void WriteInt64(Int64 a_Value)
		{
			m_Out.WriteBEInt64(a_Value);
		}
		
		void WriteFloat(float a_Value)
		{
			m_Out.WriteBEFloat(a_Value);
		}
		
		void WriteDouble(double a_Value)
		{
			m_Out.WriteBEDouble(a_Value);
		}
		
		void WriteVarInt(UInt32 a_Value)
		{
			m_Out.WriteVarInt(a_Value);
		}
		
		void WriteString(const AString & a_Value)
		{
			m_Out.WriteVarUTF8String(a_Value);
		}
		
		void WriteBuf(const char * a_Data, size_t a_Size)
		{
			m_Out.Write(a_Data, a_Size);
		}
		
		void WriteItem(const cItem & a_Item);
		void WriteByteAngle(double a_Angle);  // Writes the specified angle using a single byte
		void WriteFPInt(double a_Value);  // Writes the double value as a 27:5 fixed-point integer
		void WriteEntityMetadata(const cEntity & a_Entity);  // Writes the metadata for the specified entity, not including the terminating 0x7f
		void WriteMobMetadata(const cMonster & a_Mob);  // Writes the mob-specific metadata for the specified mob
		void WriteEntityProperties(const cEntity & a_Entity);  // Writes the entity properties for the specified entity, including the Count field
		void WriteBlockEntity(const cBlockEntity & a_BlockEntity);
		
	protected:
		cProtocol172 & m_Protocol;
		cByteBuffer & m_Out;
		cCSLock m_Lock;
	} ;

	AString m_ServerAddress;
	
	UInt16 m_ServerPort;
	
	AString m_AuthServerID;
	
	/** State of the protocol. 1 = status, 2 = login, 3 = game */
	UInt32 m_State;

	/** Buffer for the received data */
	cByteBuffer m_ReceivedData;
	
	/** Buffer for composing the outgoing packets, through cPacketizer */
	cByteBuffer m_OutPacketBuffer;
	
	/** Buffer for composing packet length (so that each cPacketizer instance doesn't allocate a new cPacketBuffer) */
	cByteBuffer m_OutPacketLenBuffer;
	
	bool m_IsEncrypted;
	
	cAesCfb128Decryptor m_Decryptor;
	cAesCfb128Encryptor m_Encryptor;

	/** The logfile where the comm is logged, when g_ShouldLogComm is true */
	cFile m_CommLogFile;
	
	/** The dimension that was last sent to a player in a Respawn or Login packet.
	Used to avoid Respawning into the same dimension, which confuses the client. */
	eDimension m_LastSentDimension;
	
	
	/** Adds the received (unencrypted) data to m_ReceivedData, parses complete packets */
	void AddReceivedData(const char * a_Data, size_t a_Size);
	
	/** Reads and handles the packet. The packet length and type have already been read.
	Returns true if the packet was understood, false if it was an unknown packet
	*/
	bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType);
	
	// Packet handlers while in the Status state (m_State == 1):
	void HandlePacketStatusPing   (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer);
	
	// Packet handlers while in the Login state (m_State == 2):
	void HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer);
	void HandlePacketLoginStart             (cByteBuffer & a_ByteBuffer);
	
	// Packet handlers while in the Game state (m_State == 3):
	void HandlePacketAnimation              (cByteBuffer & a_ByteBuffer);
	void HandlePacketBlockDig               (cByteBuffer & a_ByteBuffer);
	void HandlePacketBlockPlace             (cByteBuffer & a_ByteBuffer);
	void HandlePacketChatMessage            (cByteBuffer & a_ByteBuffer);
	void HandlePacketClientSettings         (cByteBuffer & a_ByteBuffer);
	void HandlePacketClientStatus           (cByteBuffer & a_ByteBuffer);
	void HandlePacketCreativeInventoryAction(cByteBuffer & a_ByteBuffer);
	void HandlePacketEntityAction           (cByteBuffer & a_ByteBuffer);
	void HandlePacketKeepAlive              (cByteBuffer & a_ByteBuffer);
	void HandlePacketPlayer                 (cByteBuffer & a_ByteBuffer);
	void HandlePacketPlayerAbilities        (cByteBuffer & a_ByteBuffer);
	void HandlePacketPlayerLook             (cByteBuffer & a_ByteBuffer);
	void HandlePacketPlayerPos              (cByteBuffer & a_ByteBuffer);
	void HandlePacketPlayerPosLook          (cByteBuffer & a_ByteBuffer);
	void HandlePacketPluginMessage          (cByteBuffer & a_ByteBuffer);
	void HandlePacketSlotSelect             (cByteBuffer & a_ByteBuffer);
	void HandlePacketSteerVehicle           (cByteBuffer & a_ByteBuffer);
	void HandlePacketTabComplete            (cByteBuffer & a_ByteBuffer);
	void HandlePacketUpdateSign             (cByteBuffer & a_ByteBuffer);
	void HandlePacketUseEntity              (cByteBuffer & a_ByteBuffer);
	void HandlePacketEnchantItem            (cByteBuffer & a_ByteBuffer);
	void HandlePacketWindowClick            (cByteBuffer & a_ByteBuffer);
	void HandlePacketWindowClose            (cByteBuffer & a_ByteBuffer);
	
	
	/** Writes an entire packet into the output stream. a_Packet is expected to start with the packet type; data length is prepended here. */
	void WritePacket(cByteBuffer & a_Packet);

	/** Sends the data to the client, encrypting them if needed. */
	virtual void SendData(const char * a_Data, size_t a_Size) override;

	void SendCompass(const cWorld & a_World);
	
	/** Reads an item out of the received data, sets a_Item to the values read. Returns false if not enough received data */
	bool ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item);
	
	/** Parses item metadata as read by ReadItem(), into the item enchantments. */
	void ParseItemMetadata(cItem & a_Item, const AString & a_Metadata);
	
	void StartEncryption(const Byte * a_Key);
	
	/** Adds the chat part's style (represented by the part's stylestring) into the Json object. */
	void AddChatPartStyle(Json::Value & a_Value, const AString & a_PartStyle);
} ;





/** The version 5 lengthed protocol, used by 1.7.6 through 1.7.9. */
class cProtocol176 :
	public cProtocol172
{
	typedef cProtocol172 super;
	
public:
	cProtocol176(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);
	
	// cProtocol172 overrides:
	virtual void SendPlayerSpawn(const cPlayer & a_Player) override;
	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;

} ;




