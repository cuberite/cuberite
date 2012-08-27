
// Protocol125.h

// Interfaces to the cProtocol125 class representing the release 1.2.5 protocol (#29)





#pragma once

#include "Protocol.h"
#include "ByteBuffer.h"





// fwd:
class cPacket;





class cProtocol125 :
	public cProtocol
{
	typedef cProtocol super;
public:
	cProtocol125(cClientHandle * a_Client);
	
	/// Called when client sends some data:
	virtual void DataReceived(const char * a_Data, int a_Size) override;
	
	/// Sending stuff to clients:
	virtual void SendBlockAction      (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2) override;
	virtual void SendBlockChange      (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void SendBlockChanges     (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;
	virtual void SendChat             (const AString & a_Message) override;
	virtual void SendChunkData        (int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer) override;
	virtual void SendCollectPickup    (const cPickup & a_Pickup, const cPlayer & a_Player) override;
	virtual void SendDestroyEntity    (const cEntity & a_Entity) override;
	virtual void SendDisconnect       (const AString & a_Reason) override;
	virtual void SendEntHeadLook      (const cEntity & a_Entity) override;
	virtual void SendEntLook          (const cEntity & a_Entity) override;
	virtual void SendEntityEquipment  (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendEntityStatus     (const cEntity & a_Entity, char a_Status) override;
	virtual void SendGameMode         (eGameMode a_GameMode) override;
	virtual void SendHandshake        (const AString & a_ServerName) override;
	virtual void SendHealth           (void) override;
	virtual void SendInventoryProgress(char a_WindowID, short a_Progressbar, short a_Value) override;
	virtual void SendInventorySlot    (int a_WindowID, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendKeepAlive        (int a_PingID) override;
	virtual void SendLogin            (const cPlayer & a_Player) override;
	virtual void SendMetadata         (const cPawn & a_Entity) override;
	virtual void SendPickupSpawn      (const cPickup & a_Pickup) override;
	virtual void SendPlayerAnimation  (const cPlayer & a_Player, char a_Animation) override;
	virtual void SendPlayerListItem   (const cPlayer & a_Player, bool a_IsOnline) override;
	virtual void SendPlayerMoveLook   (void) override;
	virtual void SendPlayerPosition   (void) override;
	virtual void SendPlayerSpawn      (const cPlayer & a_Player) override;
	virtual void SendRelEntMove       (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendRelEntMoveLook   (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendRespawn          (void) override;
	virtual void SendSpawnMob         (const cMonster & a_Mob) override;
	virtual void SendTeleportEntity   (const cEntity & a_Entity) override;
	virtual void SendThunderbolt      (int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void SendTimeUpdate       (Int64 a_WorldTime) override;
	virtual void SendUnloadChunk      (int a_ChunkX, int a_ChunkZ) override;
	virtual void SendUpdateSign       (int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override;
	virtual void SendWeather          (eWeather a_Weather) override;
	virtual void SendWholeInventory   (const cInventory & a_Inventory) override;
	virtual void SendWholeInventory   (const cWindow    & a_Window) override;
	virtual void SendWindowClose      (char a_WindowID) override;
	virtual void SendWindowOpen       (char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots) override;
	
protected:
	/// Results of packet-parsing:
	enum {
		PACKET_OK         =  1,
		PACKET_ERROR      = -1,
		PACKET_UNKNOWN    = -2,
		PACKET_INCOMPLETE = -3,
	} ;
	
	cByteBuffer m_ReceivedData;  //< Buffer for the received data
	
	void Send(const cPacket & a_Packet);
	
	virtual void SendData(const char * a_Data, int a_Size) override;
	
	/// Parse the packet of the specified type from m_ReceivedData (switch into ParseXYZ() )
	virtual int ParsePacket(unsigned char a_PacketType);

	// Specific packet parsers:
	virtual int ParseKeepAlive              (void);
	virtual int ParseHandshake              (void);
	virtual int ParseLogin                  (void);
	virtual int ParsePlayerPosition         (void);
	virtual int ParsePlayerLook             (void);
	virtual int ParsePlayerMoveLook         (void);
	virtual int ParsePlayerAbilities        (void);
	virtual int ParseChat                   (void);
	virtual int ParseArmAnim                (void);
	virtual int ParseFlying                 (void);
	virtual int ParseBlockDig               (void);
	virtual int ParseBlockPlace             (void);
	virtual int ParseDisconnect             (void);
	virtual int ParseItemSwitch             (void);
	virtual int ParseEntityEquipment        (void);
	virtual int ParseCreativeInventoryAction(void);
	virtual int ParseNewInvalidState        (void);
	virtual int ParsePickupSpawn            (void);
	virtual int ParseUseEntity              (void);
	virtual int ParseWindowClose            (void);
	virtual int ParseWindowClick            (void);
	virtual int ParseEntityAction           (void);
	virtual int ParseUpdateSign             (void);
	virtual int ParseRespawn                (void);
	virtual int ParsePing                   (void);
} ;




