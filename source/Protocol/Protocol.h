
// Protocol.h

// Interfaces to the cProtocol class representing the generic interface that a protocol
// parser and serializer must implement





#pragma once

#include "../Defines.h"
#include "../Endianness.h"




class cPlayer;
class cEntity;
class cWindow;
class cInventory;
class cPawn;
class cPickup;
class cMonster;
class cChunkDataSerializer;
class cWorld;





class cProtocol
{
public:
	cProtocol(cClientHandle * a_Client) :
		m_Client(a_Client)
	{
	}
	virtual ~cProtocol() {}
	
	/// Called when client sends some data
	virtual void DataReceived(const char * a_Data, int a_Size) = 0;
	
	// Sending stuff to clients (alphabetically sorted):
	virtual void SendBlockAction        (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType) = 0;
	virtual void SendBlockBreakAnim     (int a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage) = 0;
	virtual void SendBlockChange        (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) = 0;
	virtual void SendBlockChanges       (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) = 0;
	virtual void SendChat               (const AString & a_Message) = 0;
	virtual void SendChunkData          (int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer) = 0;
	virtual void SendCollectPickup      (const cPickup & a_Pickup, const cPlayer & a_Player) = 0;
	virtual void SendDestroyEntity      (const cEntity & a_Entity) = 0;
	virtual void SendDisconnect         (const AString & a_Reason) = 0;
	virtual void SendEntHeadLook        (const cEntity & a_Entity) = 0;
	virtual void SendEntLook            (const cEntity & a_Entity) = 0;
	virtual void SendEntRelMove         (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) = 0;
	virtual void SendEntRelMoveLook     (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) = 0;
	virtual void SendEntityEquipment    (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) = 0;
	virtual void SendEntityStatus       (const cEntity & a_Entity, char a_Status) = 0;
	virtual void SendGameMode           (eGameMode a_GameMode) = 0;
	virtual void SendHealth             (void) = 0;
	virtual void SendInventoryProgress  (char a_WindowID, short a_Progressbar, short a_Value) = 0;
	virtual void SendInventorySlot      (char a_WindowID, short a_SlotNum, const cItem & a_Item) = 0;
	virtual void SendKeepAlive          (int a_PingID) = 0;
	virtual void SendLogin              (const cPlayer & a_Player, const cWorld & a_World) = 0;
	virtual void SendMetadata           (const cEntity & a_Entity) = 0;
	virtual void SendPickupSpawn        (const cPickup & a_Pickup) = 0;
	virtual void SendPlayerAnimation    (const cPlayer & a_Player, char a_Animation) = 0;
	virtual void SendPlayerListItem     (const cPlayer & a_Player, bool a_IsOnline) = 0;
	virtual void SendPlayerMoveLook     (void) = 0;
	virtual void SendPlayerPosition     (void) = 0;
	virtual void SendPlayerSpawn        (const cPlayer & a_Player) = 0;
	virtual void SendRespawn            (void) = 0;
	virtual void SendSoundEffect        (const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch) = 0;  // a_Src coords are Block * 8
	virtual void SendSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data) = 0;
	virtual void SendSpawnMob           (const cMonster & a_Mob) = 0;
	virtual void SendSpawnObject        (const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, short a_SpeedX, short a_SpeedY, short a_SpeedZ) = 0;
	virtual void SendTeleportEntity     (const cEntity & a_Entity) = 0;
	virtual void SendThunderbolt        (int a_BlockX, int a_BlockY, int a_BlockZ) = 0;
	virtual void SendTimeUpdate         (Int64 a_WorldTime) = 0;
	virtual void SendUnloadChunk        (int a_ChunkX, int a_ChunkZ) = 0;
	virtual void SendUpdateSign         (int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) = 0;
	virtual void SendUseBed             (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ ) = 0;
	virtual void SendWeather            (eWeather a_Weather) = 0;
	virtual void SendWholeInventory     (const cInventory & a_Inventory) = 0;
	virtual void SendWholeInventory     (const cWindow    & a_Window) = 0;
	virtual void SendWindowClose        (char a_WindowID) = 0;
	virtual void SendWindowOpen         (char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots) = 0;

	/// Returns the ServerID used for authentication through session.minecraft.net
	virtual AString GetAuthServerID(void) = 0;

protected:
	cClientHandle * m_Client;
	cCriticalSection m_CSPacket;  //< Each SendXYZ() function must acquire this CS in order to send the whole packet at once
	
	/// A generic data-sending routine, all outgoing packet data needs to be routed through this so that descendants may override it
	virtual void SendData(const char * a_Data, int a_Size) = 0;
	
	/// Called after writing each packet, enables descendants to flush their buffers
	virtual void Flush(void) {};
	
	// Helpers for writing partial packet data, write using SendData()
	void WriteByte(char a_Value)
	{
		SendData((const char *)&a_Value, 1);
	}
	
	void WriteShort(short a_Value)
	{
		a_Value = htons(a_Value);
		SendData((const char *)&a_Value, 2);
	}
	
	/*
	void WriteShort(unsigned short a_Value)
	{
		a_Value = htons(a_Value);
		SendData((const char *)&a_Value, 2);
	}
	*/
	
	void WriteInt(int a_Value)
	{
		a_Value = htonl(a_Value);
		SendData((const char *)&a_Value, 4);
	}
	
	void WriteUInt(unsigned int a_Value)
	{
		a_Value = htonl(a_Value);
		SendData((const char *)&a_Value, 4);
	}
	
	void WriteInt64 (Int64 a_Value)
	{
		a_Value = HostToNetwork8(&a_Value);
		SendData((const char *)&a_Value, 8);
	}
	
	void WriteFloat (float a_Value)
	{
		unsigned int val = HostToNetwork4(&a_Value);
		SendData((const char *)&val, 4);
	}
	
	void WriteDouble(double a_Value)
	{
		unsigned long long val = HostToNetwork8(&a_Value);
		SendData((const char *)&val, 8);
	}
	
	void WriteString(const AString & a_Value)
	{
		AString UTF16;
		UTF8ToRawBEUTF16(a_Value.c_str(), a_Value.length(), UTF16);
		WriteShort((unsigned short)(UTF16.size() / 2));
		SendData(UTF16.data(), UTF16.size());
	}
	
	void WriteBool(bool a_Value)
	{
		WriteByte(a_Value ? 1 : 0);
	}
	
	void WriteVectorI(const Vector3i & a_Vector)
	{
		WriteInt(a_Vector.x);
		WriteInt(a_Vector.y);
		WriteInt(a_Vector.z);
	}
} ;





