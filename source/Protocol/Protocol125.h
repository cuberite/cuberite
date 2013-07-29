
// Protocol125.h

// Interfaces to the cProtocol125 class representing the release 1.2.5 protocol (#29)





#pragma once

#include "Protocol.h"
#include "../ByteBuffer.h"





class cProtocol125 :
	public cProtocol
{
	typedef cProtocol super;
public:
	cProtocol125(cClientHandle * a_Client);
	
	/// Called when client sends some data:
	virtual void DataReceived(const char * a_Data, int a_Size) override;
	
	/// Sending stuff to clients (alphabetically sorted):
	virtual void SendAttachEntity       (const cEntity & a_Entity, const cEntity * a_Vehicle) override;
	virtual void SendBlockAction        (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType) override;
	virtual void SendBlockBreakAnim	    (int a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage) override;
	virtual void SendBlockChange        (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void SendBlockChanges       (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;
	virtual void SendChat               (const AString & a_Message) override;
	virtual void SendChunkData          (int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer) override;
	virtual void SendCollectPickup      (const cPickup & a_Pickup, const cPlayer & a_Player) override;
	virtual void SendDestroyEntity      (const cEntity & a_Entity) override;
	virtual void SendDisconnect         (const AString & a_Reason) override;
	virtual void SendEntityEquipment    (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendEntityHeadLook     (const cEntity & a_Entity) override;
	virtual void SendEntityLook         (const cEntity & a_Entity) override;
	virtual void SendEntityMetadata     (const cEntity & a_Entity) override;
	virtual void SendEntityProperties   (const cEntity & a_Entity) override;
	virtual void SendEntityRelMove      (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendEntityRelMoveLook  (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendEntityStatus       (const cEntity & a_Entity, char a_Status) override;
	virtual void SendEntityVelocity     (const cEntity & a_Entity) override;
	virtual void SendExplosion          (double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion) override;
	virtual void SendGameMode           (eGameMode a_GameMode) override;
	virtual void SendHealth             (void) override;
	virtual void SendInventoryProgress  (char a_WindowID, short a_Progressbar, short a_Value) override;
	virtual void SendInventorySlot      (char a_WindowID, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendKeepAlive          (int a_PingID) override;
	virtual void SendLogin              (const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void SendPickupSpawn        (const cPickup & a_Pickup) override;
	virtual void SendPlayerAnimation    (const cPlayer & a_Player, char a_Animation) override;
	virtual void SendPlayerListItem     (const cPlayer & a_Player, bool a_IsOnline) override;
	virtual void SendPlayerMaxSpeed     (void) override;
	virtual void SendPlayerMoveLook     (void) override;
	virtual void SendPlayerPosition     (void) override;
	virtual void SendPlayerSpawn        (const cPlayer & a_Player) override;
	virtual void SendRespawn            (void) override;
	virtual void SendSoundEffect        (const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch) override;  // a_Src coords are Block * 8
	virtual void SendSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data) override;
	virtual void SendSpawnFallingBlock  (const cFallingBlock & a_FallingBlock) override;
	virtual void SendSpawnMob           (const cMonster & a_Mob) override;
	virtual void SendSpawnObject        (const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch) override;
	virtual void SendSpawnVehicle       (const cEntity & a_Vehicle, char a_VehicleType) override;
	virtual void SendTeleportEntity     (const cEntity & a_Entity) override;
	virtual void SendThunderbolt        (int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void SendTimeUpdate         (Int64 a_WorldAge, Int64 a_TimeOfDay) override;
	virtual void SendUnloadChunk        (int a_ChunkX, int a_ChunkZ) override;
	virtual void SendUpdateSign         (int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override;
	virtual void SendUseBed             (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ ) override;
	virtual void SendWeather            (eWeather a_Weather) override;
	virtual void SendWholeInventory     (const cInventory & a_Inventory) override;
	virtual void SendWholeInventory     (const cWindow    & a_Window) override;
	virtual void SendWindowClose        (const cWindow    & a_Window) override;
	virtual void SendWindowOpen         (char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots) override;
	
	virtual AString GetAuthServerID(void) override;
	
protected:
	/// Results of packet-parsing:
	enum {
		PARSE_OK         =  1,
		PARSE_ERROR      = -1,
		PARSE_UNKNOWN    = -2,
		PARSE_INCOMPLETE = -3,
	} ;
	
	cByteBuffer m_ReceivedData;  //< Buffer for the received data
	
	AString m_Username;  //< Stored in ParseHandshake(), compared to Login username
	
	virtual void SendData(const char * a_Data, int a_Size) override;
	
	/// Sends the Handshake packet
	void SendHandshake(const AString & a_ConnectionHash);

	/// Parse the packet of the specified type from m_ReceivedData (switch into ParseXYZ() )
	virtual int ParsePacket(unsigned char a_PacketType);

	// Specific packet parsers:
	virtual int ParseArmAnim                (void);
	virtual int ParseBlockDig               (void);
	virtual int ParseBlockPlace             (void);
	virtual int ParseChat                   (void);
	virtual int ParseCreativeInventoryAction(void);
	virtual int ParseDisconnect             (void);
	virtual int ParseEntityAction           (void);
	virtual int ParseHandshake              (void);
	virtual int ParseKeepAlive              (void);
	virtual int ParseLogin                  (void);
	virtual int ParsePing                   (void);
	virtual int ParsePlayerAbilities        (void);
	virtual int ParsePlayerLook             (void);
	virtual int ParsePlayerMoveLook         (void);
	virtual int ParsePlayerOnGround         (void);
	virtual int ParsePlayerPosition         (void);
	virtual int ParsePluginMessage          (void);
	virtual int ParseRespawn                (void);
	virtual int ParseSlotSelected           (void);
	virtual int ParseUpdateSign             (void);
	virtual int ParseUseEntity              (void);
	virtual int ParseWindowClick            (void);
	virtual int ParseWindowClose            (void);
	
	// Utility functions:
	/// Writes a "pre-chunk" packet
	void SendPreChunk(int a_ChunkX, int a_ChunkZ, bool a_ShouldLoad);
	
	/// Writes a "set window items" packet with the specified params
	void SendWindowSlots(char a_WindowID, int a_NumItems, const cItem * a_Items);
	
	/// Writes one item, "slot" as the protocol wiki calls it
	virtual void WriteItem(const cItem & a_Item);
	
	/// Parses one item, "slot" as the protocol wiki calls it, from m_ReceivedData; returns the usual ParsePacket() codes
	virtual int  ParseItem(cItem & a_Item);
	
	/// Returns the entity metadata representation
	AString GetEntityMetaData(const cEntity & a_Entity);
	
	/// Returns the entity common metadata, index 0 (generic flags)
	char GetEntityMetadataFlags(const cEntity & a_Entity);
} ;




