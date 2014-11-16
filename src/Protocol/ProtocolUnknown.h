
// ProtocolUnknown.h

// Declares unknown protocol versions.





#pragma once

#include "Protocol.h"
#include "../ByteBuffer.h"





class cProtocolUnknown :
	public cProtocol
{
	typedef cProtocol super;

public:

	cProtocolUnknown(cClientHandle * a_Client, UInt32 a_State);

	/** Called when client sends some data: */
	virtual void DataReceived(const char * a_Data, size_t a_Size) override;

	/** Sending stuff to clients (alphabetically sorted): */
	virtual void SendAttachEntity               (const cEntity & a_Entity, const cEntity * a_Vehicle) override {}
	virtual void SendBlockAction                (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType) override {}
	virtual void SendBlockBreakAnim	            (int a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage) override {}
	virtual void SendBlockChange                (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override {}
	virtual void SendBlockChanges               (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override {}
	virtual void SendChat                       (const AString & a_Message) override {}
	virtual void SendChat                       (const cCompositeChat & a_Message) override {}
	virtual void SendChunkData                  (int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer) override {}
	virtual void SendCollectEntity              (const cEntity & a_Entity, const cPlayer & a_Player) override {}
	virtual void SendDestroyEntity              (const cEntity & a_Entity) override {}
	virtual void SendDisconnect                 (const AString & a_Reason) override;
	virtual void SendEditSign                   (int a_BlockX, int a_BlockY, int a_BlockZ) override {}
	virtual void SendEntityEffect               (const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration) override {}
	virtual void SendEntityEquipment            (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) override {}
	virtual void SendEntityHeadLook             (const cEntity & a_Entity) override {}
	virtual void SendEntityLook                 (const cEntity & a_Entity) override {}
	virtual void SendEntityMetadata             (const cEntity & a_Entity) override {}
	virtual void SendEntityProperties           (const cEntity & a_Entity) override {}
	virtual void SendEntityRelMove              (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override {}
	virtual void SendEntityRelMoveLook          (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override {}
	virtual void SendEntityStatus               (const cEntity & a_Entity, char a_Status) override {}
	virtual void SendEntityVelocity             (const cEntity & a_Entity) override {}
	virtual void SendExplosion                  (double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion) override {}
	virtual void SendGameMode                   (eGameMode a_GameMode) override {}
	virtual void SendHealth                     (void) override {}
	virtual void SendInventorySlot              (char a_WindowID, short a_SlotNum, const cItem & a_Item) override {}
	virtual void SendKeepAlive                  (int a_PingID) override {}
	virtual void SendLogin                      (const cPlayer & a_Player, const cWorld & a_World) override {}
	virtual void SendLoginSuccess               (void) override {}
	virtual void SendMapColumn                  (int a_ID, int a_X, int a_Y, const Byte * a_Colors, unsigned int a_Length, unsigned int m_Scale) override {}
	virtual void SendMapDecorators              (int a_ID, const cMapDecoratorList & a_Decorators, unsigned int m_Scale) override {}
	virtual void SendMapInfo                    (int a_ID, unsigned int a_Scale) override {}
	virtual void SendPaintingSpawn              (const cPainting & a_Painting) override {}
	virtual void SendPickupSpawn                (const cPickup & a_Pickup) override {}
	virtual void SendPlayerAbilities            (void) override {}
	virtual void SendEntityAnimation            (const cEntity & a_Entity, char a_Animation) override {}
	virtual void SendParticleEffect             (const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount) override {}
	virtual void SendPlayerListAddPlayer        (const cPlayer & a_Player) override {}
	virtual void SendPlayerListRemovePlayer     (const cPlayer & a_Player) override {}
	virtual void SendPlayerListUpdateGameMode   (const cPlayer & a_Player) override {}
	virtual void SendPlayerListUpdatePing       (const cPlayer & a_Player) override {}
	virtual void SendPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName) override {}
	virtual void SendPlayerMaxSpeed             (void) override {}
	virtual void SendPlayerMoveLook             (void) override {}
	virtual void SendPlayerPosition             (void) override {}
	virtual void SendPlayerSpawn                (const cPlayer & a_Player) override {}
	virtual void SendPluginMessage              (const AString & a_Channel, const AString & a_Message) override {}
	virtual void SendRemoveEntityEffect         (const cEntity & a_Entity, int a_EffectID) override {}
	virtual void SendRespawn                    (eDimension a_Dimension, bool a_ShouldIgnoreDimensionChecks) override {}
	virtual void SendSoundEffect                (const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch) override {}
	virtual void SendExperience                 (void) override {}
	virtual void SendExperienceOrb              (const cExpOrb & a_ExpOrb) override {}
	virtual void SendScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) override {}
	virtual void SendScoreUpdate                (const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode) override {}
	virtual void SendDisplayObjective           (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) override {}
	virtual void SendSoundParticleEffect        (int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data) override {}
	virtual void SendSpawnFallingBlock          (const cFallingBlock & a_FallingBlock) override {}
	virtual void SendSpawnMob                   (const cMonster & a_Mob) override {}
	virtual void SendSpawnObject                (const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch) override {}
	virtual void SendSpawnVehicle               (const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType) override {}
	virtual void SendStatistics                 (const cStatManager & a_Manager) override {}
	virtual void SendTabCompletionResults       (const AStringVector & a_Results) override {}
	virtual void SendTeleportEntity             (const cEntity & a_Entity) override {}
	virtual void SendThunderbolt                (int a_BlockX, int a_BlockY, int a_BlockZ) override {}
	virtual void SendTimeUpdate                 (Int64 a_WorldAge, Int64 a_TimeOfDay, bool a_DoDaylightCycle) override {}
	virtual void SendUnloadChunk                (int a_ChunkX, int a_ChunkZ) override {}
	virtual void SendUpdateBlockEntity          (cBlockEntity & a_BlockEntity) override {}
	virtual void SendUpdateSign                 (int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override {}
	virtual void SendUseBed                     (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ) override {}
	virtual void SendWeather                    (eWeather a_Weather) override {}
	virtual void SendWholeInventory             (const cWindow & a_Window) override {}
	virtual void SendWindowClose                (const cWindow & a_Window) override {}
	virtual void SendWindowOpen                 (const cWindow & a_Window) override {}
	virtual void SendWindowProperty             (const cWindow & a_Window, short a_Property, short a_Value) override {}

	virtual AString GetAuthServerID(void) override { return "0"; }

protected:

	/** Composes individual packets in the protocol's m_OutPacketBuffer; sends them upon being destructed */
	class cPacketizer
	{
	public:
		cPacketizer(cProtocolUnknown & a_Protocol, UInt32 a_PacketType) :
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

		void WritePosition(int a_BlockX, int a_BlockY, int a_BlockZ)
		{
			m_Out.WritePosition(a_BlockX, a_BlockY, a_BlockZ);
		}
		
		void WriteBuf(const char * a_Data, size_t a_Size)
		{
			m_Out.Write(a_Data, a_Size);
		}
		
	protected:
		cProtocolUnknown & m_Protocol;
		cByteBuffer & m_Out;
		cCSLock m_Lock;
	} ;

	/** State of the protocol. 1 = status, 2 = login */
	UInt32 m_State;

	/** Buffer for the received data */
	cByteBuffer m_ReceivedData;
	
	/** Buffer for composing the outgoing packets, through cPacketizer */
	cByteBuffer m_OutPacketBuffer;
	
	/** Buffer for composing packet length (so that each cPacketizer instance doesn't allocate a new cPacketBuffer) */
	cByteBuffer m_OutPacketLenBuffer;

	/** The logfile where the comm is logged, when g_ShouldLogComm is true */
	cFile m_CommLogFile;

	/** Adds the received (unencrypted) data to m_ReceivedData, parses complete packets */
	void AddReceivedData(const char * a_Data, size_t a_Size);

	/** Reads and handles the packet. The packet length and type have already been read.
	Returns true if the packet was understood, false if it was an unknown packet
	*/
	bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType);

	// Packet handlers while in the Status state (m_State == 1):
	void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer);
	void HandlePacketStatusPing(cByteBuffer & a_ByteBuffer);

	// Packet handlers while in the Login state (m_State == 2):
	void HandlePacketLoginStart(cByteBuffer & a_ByteBuffer);

	/** Sends the data to the client, encrypting them if needed. */
	virtual void SendData(const char * a_Data, size_t a_Size) override;

};



