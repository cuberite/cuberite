
// ProtocolRecognizer.h

// Interfaces to the cProtocolRecognizer class representing the meta-protocol that recognizes possibly multiple 
// protocol versions and redirects everything to them





#pragma once

#include "Protocol.h"
#include "ByteBuffer.h"





class cProtocolRecognizer :
	public cProtocol
{
	typedef cProtocol super;
	
public:
	cProtocolRecognizer(cClientHandle * a_Client);
	
	/// Called when client sends some data:
	virtual void DataReceived(const char * a_Data, int a_Size) override;
	
	/// Sending stuff to clients:
	virtual void SendBlockAction      (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType) override;
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
	virtual void SendEntRelMove       (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendEntRelMoveLook   (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendGameMode         (eGameMode a_GameMode) override;
	virtual void SendHealth           (void) override;
	virtual void SendInventoryProgress(char a_WindowID, short a_Progressbar, short a_Value) override;
	virtual void SendInventorySlot    (char a_WindowID, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendKeepAlive        (int a_PingID) override;
	virtual void SendLogin            (const cPlayer & a_Player) override;
	virtual void SendMetadata         (const cEntity & a_Entity) override;
	virtual void SendPickupSpawn      (const cPickup & a_Pickup) override;
	virtual void SendPlayerAnimation  (const cPlayer & a_Player, char a_Animation) override;
	virtual void SendPlayerListItem   (const cPlayer & a_Player, bool a_IsOnline) override;
	virtual void SendPlayerMoveLook   (void) override;
	virtual void SendPlayerPosition   (void) override;
	virtual void SendPlayerSpawn      (const cPlayer & a_Player) override;
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

	virtual void SendData(const char * a_Data, int a_Size) override;

protected:
	cProtocol * m_Protocol;  //< The recognized protocol
	cByteBuffer m_Buffer;    //< Buffer for the incoming data until we recognize the protocol
	
	/// Tries to recognize protocol based on m_Buffer contents; returns true if recognized
	bool TryRecognizeProtocol(void);
	
	/// Called when the recognizer gets a server ping packet; responds with server stats and destroys the client
	void HandleServerPing(void);
} ;





