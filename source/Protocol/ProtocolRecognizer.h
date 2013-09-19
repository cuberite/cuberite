
// ProtocolRecognizer.h

// Interfaces to the cProtocolRecognizer class representing the meta-protocol that recognizes possibly multiple 
// protocol versions and redirects everything to them





#pragma once

#include "Protocol.h"
#include "../ByteBuffer.h"





// Adjust these if a new protocol is added or an old one is removed:
#define MCS_CLIENT_VERSIONS "1.2.4, 1.2.5, 1.3.1, 1.3.2, 1.4.2, 1.4.4, 1.4.5, 1.4.6, 1.4.7, 1.5, 1.5.1, 1.5.2, 1.6.1, 1.6.2, 1.6.3, 1.6.4"
#define MCS_PROTOCOL_VERSIONS "29, 39, 47, 49, 51, 60, 61, 73, 74, 77, 78"





class cProtocolRecognizer :
	public cProtocol
{
	typedef cProtocol super;
	
public:
	enum
	{
		PROTO_VERSION_1_2_5 = 29,
		PROTO_VERSION_1_3_2 = 39,
		PROTO_VERSION_1_4_2 = 47,
		PROTO_VERSION_1_4_4 = 49,
		PROTO_VERSION_1_4_6 = 51,
		PROTO_VERSION_1_5_0 = 60,
		PROTO_VERSION_1_5_2 = 61,
		PROTO_VERSION_1_6_1 = 73,
		PROTO_VERSION_1_6_2 = 74,
		PROTO_VERSION_1_6_3 = 77,
		PROTO_VERSION_1_6_4 = 78,
		
		PROTO_VERSION_NEXT,
		PROTO_VERSION_LATEST = PROTO_VERSION_NEXT - 1,  ///< Automatically assigned to the last protocol version, this serves as the default for PrimaryServerVersion
	} ;

	cProtocolRecognizer(cClientHandle * a_Client);
	virtual ~cProtocolRecognizer();
	
	/// Translates protocol version number into protocol version text: 49 -> "1.4.4"
	static AString GetVersionTextFromInt(int a_ProtocolVersion);
	
	/// Called when client sends some data:
	virtual void DataReceived(const char * a_Data, int a_Size) override;
	
	/// Sending stuff to clients (alphabetically sorted):
	virtual void SendAttachEntity        (const cEntity & a_Entity, const cEntity * a_Vehicle) override;
	virtual void SendBlockAction         (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType) override;
	virtual void SendBlockBreakAnim      (int a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage) override;
	virtual void SendBlockChange         (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void SendBlockChanges        (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;
	virtual void SendChat                (const AString & a_Message) override;
	virtual void SendChunkData           (int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer) override;
	virtual void SendCollectPickup       (const cPickup & a_Pickup, const cPlayer & a_Player) override;
	virtual void SendDestroyEntity       (const cEntity & a_Entity) override;
	virtual void SendDisconnect          (const AString & a_Reason) override;
	virtual void SendEditSign            (int a_BlockX, int a_BlockY, int a_BlockZ) override;  ///< Request the client to open up the sign editor for the sign (1.6+)
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
	virtual void SendPickupSpawn         (const cPickup & a_Pickup) override;
	virtual void SendPlayerAnimation     (const cPlayer & a_Player, char a_Animation) override;
	virtual void SendPlayerListItem      (const cPlayer & a_Player, bool a_IsOnline) override;
	virtual void SendPlayerMaxSpeed      (void) override;
	virtual void SendPlayerMoveLook      (void) override;
	virtual void SendPlayerPosition      (void) override;
	virtual void SendPlayerSpawn         (const cPlayer & a_Player) override;
	virtual void SendRespawn             (void) override;
	virtual void SendSoundEffect         (const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch) override;
	virtual void SendSoundParticleEffect (int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data) override;
	virtual void SendSpawnFallingBlock   (const cFallingBlock & a_FallingBlock) override;
	virtual void SendSpawnMob            (const cMonster & a_Mob) override;
	virtual void SendSpawnObject         (const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch) override;
	virtual void SendSpawnVehicle        (const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType) override;
	virtual void SendTabCompletionResults(const AStringVector & a_Results) override;
	virtual void SendTeleportEntity      (const cEntity & a_Entity) override;
	virtual void SendThunderbolt         (int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void SendTimeUpdate          (Int64 a_WorldAge, Int64 a_TimeOfDay) override;
	virtual void SendUnloadChunk         (int a_ChunkX, int a_ChunkZ) override;
	virtual void SendUpdateSign          (int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override;
	virtual void SendUseBed              (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ ) override;
	virtual void SendWeather             (eWeather a_Weather) override;
	virtual void SendWholeInventory      (const cInventory & a_Inventory) override;
	virtual void SendWholeInventory      (const cWindow    & a_Window) override;
	virtual void SendWindowClose         (const cWindow    & a_Window) override;
	virtual void SendWindowOpen          (char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots) override;
	virtual void SendWindowProperty      (const cWindow & a_Window, short a_Property, short a_Value) override;
	
	virtual AString GetAuthServerID(void) override;

	virtual void SendData(const char * a_Data, int a_Size) override;

protected:
	cProtocol * m_Protocol;  //< The recognized protocol
	cByteBuffer m_Buffer;    //< Buffer for the incoming data until we recognize the protocol
	
	/// Tries to recognize protocol based on m_Buffer contents; returns true if recognized
	bool TryRecognizeProtocol(void);
	
	/// Called when the recognizer gets a server ping packet; responds with server stats and destroys the client
	void HandleServerPing(void);
} ;





