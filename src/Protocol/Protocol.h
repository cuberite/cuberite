
// Protocol.h

// Interfaces to the cProtocol class representing the generic interface that a protocol
// parser and serializer must implement





#pragma once

#include "../Defines.h"
#include "../Endianness.h"
#include "../Scoreboard.h"
#include "../Map.h"




class cExpOrb;
class cPlayer;
class cEntity;
class cWindow;
class cInventory;
class cPawn;
class cPickup;
class cPainting;
class cWorld;
class cMonster;
class cChunkDataSerializer;
class cFallingBlock;
class cCompositeChat;
class cStatManager;





typedef unsigned char Byte;





class cProtocol
{
public:
	cProtocol(cClientHandle * a_Client) :
		m_Client(a_Client)
	{
	}
	virtual ~cProtocol() {}
	
	/// Called when client sends some data
	virtual void DataReceived(const char * a_Data, size_t a_Size) = 0;
	
	// Sending stuff to clients (alphabetically sorted):
	virtual void SendAttachEntity        (const cEntity & a_Entity, const cEntity * a_Vehicle) = 0;
	virtual void SendBlockAction         (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType) = 0;
	virtual void SendBlockBreakAnim      (int a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage) = 0;
	virtual void SendBlockChange         (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) = 0;
	virtual void SendBlockChanges        (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) = 0;
	virtual void SendChat                (const AString & a_Message) = 0;
	virtual void SendChat                (const cCompositeChat & a_Message) = 0;
	virtual void SendChunkData           (int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer) = 0;
	virtual void SendCollectEntity       (const cEntity & a_Entity, const cPlayer & a_Player) = 0;
	virtual void SendDestroyEntity       (const cEntity & a_Entity) = 0;
	virtual void SendDisconnect          (const AString & a_Reason) = 0;
	virtual void SendEditSign            (int a_BlockX, int a_BlockY, int a_BlockZ) = 0;  ///< Request the client to open up the sign editor for the sign (1.6+)
	virtual void SendEntityEffect        (const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration) = 0;
	virtual void SendEntityEquipment     (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) = 0;
	virtual void SendEntityHeadLook      (const cEntity & a_Entity) = 0;
	virtual void SendEntityLook          (const cEntity & a_Entity) = 0;
	virtual void SendEntityMetadata      (const cEntity & a_Entity) = 0;
	virtual void SendEntityProperties    (const cEntity & a_Entity) = 0;
	virtual void SendEntityRelMove       (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) = 0;
	virtual void SendEntityRelMoveLook   (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) = 0;
	virtual void SendEntityStatus        (const cEntity & a_Entity, char a_Status) = 0;
	virtual void SendEntityVelocity      (const cEntity & a_Entity) = 0;
	virtual void SendExplosion           (double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion) = 0;
	virtual void SendGameMode            (eGameMode a_GameMode) = 0;
	virtual void SendHealth              (void) = 0;
	virtual void SendInventorySlot       (char a_WindowID, short a_SlotNum, const cItem & a_Item) = 0;
	virtual void SendKeepAlive           (int a_PingID) = 0;
	virtual void SendLogin               (const cPlayer & a_Player, const cWorld & a_World) = 0;
	virtual void SendLoginSuccess        (void) = 0;
	virtual void SendMapColumn           (int a_ID, int a_X, int a_Y, const Byte * a_Colors, unsigned int a_Length) = 0;
	virtual void SendMapDecorators       (int a_ID, const cMapDecoratorList & a_Decorators) = 0;
	virtual void SendMapInfo             (int a_ID, unsigned int a_Scale) = 0;
	virtual void SendPaintingSpawn       (const cPainting & a_Painting) = 0;
	virtual void SendPickupSpawn         (const cPickup & a_Pickup) = 0;
	virtual void SendPlayerAbilities     (void) = 0;
	virtual void SendEntityAnimation     (const cEntity & a_Entity, char a_Animation) = 0;
	virtual void SendParticleEffect      (const AString & a_SoundName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmmount) = 0;
	virtual void SendPlayerListItem      (const cPlayer & a_Player, bool a_IsOnline) = 0;
	virtual void SendPlayerMaxSpeed      (void) = 0;  ///< Informs the client of the maximum player speed (1.6.1+)
	virtual void SendPlayerMoveLook      (void) = 0;
	virtual void SendPlayerPosition      (void) = 0;
	virtual void SendPlayerSpawn         (const cPlayer & a_Player) = 0;
	virtual void SendPluginMessage       (const AString & a_Channel, const AString & a_Message) = 0;
	virtual void SendRemoveEntityEffect  (const cEntity & a_Entity, int a_EffectID) = 0;
	virtual void SendRespawn             (const cWorld & a_World, bool a_ShouldIgnoreDimensionChecks = false) = 0;
	virtual void SendExperience          (void) = 0;
	virtual void SendExperienceOrb       (const cExpOrb & a_ExpOrb) = 0;
	virtual void SendScoreboardObjective (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) = 0;
	virtual void SendScoreUpdate         (const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode) = 0;
	virtual void SendDisplayObjective    (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) = 0;
	virtual void SendSoundEffect         (const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch) = 0;
	virtual void SendSoundParticleEffect (int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data) = 0;
	virtual void SendSpawnFallingBlock   (const cFallingBlock & a_FallingBlock) = 0;
	virtual void SendSpawnMob            (const cMonster & a_Mob) = 0;
	virtual void SendSpawnObject         (const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch) = 0;
	virtual void SendSpawnVehicle        (const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType) = 0;
	virtual void SendStatistics          (const cStatManager & a_Manager) = 0;
	virtual void SendTabCompletionResults(const AStringVector & a_Results) = 0;
	virtual void SendTeleportEntity      (const cEntity & a_Entity) = 0;
	virtual void SendThunderbolt         (int a_BlockX, int a_BlockY, int a_BlockZ) = 0;
	virtual void SendTimeUpdate          (Int64 a_WorldAge, Int64 a_TimeOfDay) = 0;
	virtual void SendUnloadChunk         (int a_ChunkX, int a_ChunkZ) = 0;
	virtual void SendUpdateBlockEntity   (cBlockEntity & a_BlockEntity) = 0;
	virtual void SendUpdateSign          (int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) = 0;
	virtual void SendUseBed              (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ) = 0;
	virtual void SendWeather             (eWeather a_Weather) = 0;
	virtual void SendWholeInventory      (const cWindow    & a_Window) = 0;
	virtual void SendWindowClose         (const cWindow    & a_Window) = 0;
	virtual void SendWindowOpen          (const cWindow & a_Window) = 0;
	virtual void SendWindowProperty      (const cWindow & a_Window, int a_Property, int a_Value) = 0;

	/// Returns the ServerID used for authentication through session.minecraft.net
	virtual AString GetAuthServerID(void) = 0;

protected:
	cClientHandle * m_Client;
	cCriticalSection m_CSPacket;  // Each SendXYZ() function must acquire this CS in order to send the whole packet at once
	
	/// A generic data-sending routine, all outgoing packet data needs to be routed through this so that descendants may override it
	virtual void SendData(const char * a_Data, size_t a_Size) = 0;
	
	/// Called after writing each packet, enables descendants to flush their buffers
	virtual void Flush(void) {};
	
	// Helpers for writing partial packet data, write using SendData()
	void WriteByte(Byte a_Value)
	{
		SendData((const char *)&a_Value, 1);
	}
	
	void WriteChar(char a_Value)
	{
		SendData(&a_Value, 1);
	}
	
	void WriteShort(short a_Value)
	{
		u_short Value = htons((u_short)a_Value);
		SendData((const char *)&Value, 2);
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
		u_long Value = htonl((u_long)a_Value);
		SendData((const char *)&Value, 4);
	}
	
	void WriteUInt(unsigned int a_Value)
	{
		a_Value = htonl(a_Value);
		SendData((const char *)&a_Value, 4);
	}
	
	void WriteInt64 (Int64 a_Value)
	{
		UInt64 Value = HostToNetwork8(&a_Value);
		SendData((const char *)&Value, 8);
	}
	
	void WriteFloat (float a_Value)
	{
		UInt32 val = HostToNetwork4(&a_Value);
		SendData((const char *)&val, 4);
	}
	
	void WriteDouble(double a_Value)
	{
		UInt64 val = HostToNetwork8(&a_Value);
		SendData((const char *)&val, 8);
	}
	
	void WriteString(const AString & a_Value)
	{
		AString UTF16;
		UTF8ToRawBEUTF16(a_Value.c_str(), a_Value.length(), UTF16);
		WriteShort((short)(UTF16.size() / 2));
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
	
	void WriteVarInt(UInt32 a_Value)
	{
		// A 32-bit integer can be encoded by at most 5 bytes:
		unsigned char b[5];
		size_t idx = 0;
		do
		{
			b[idx] = (a_Value & 0x7f) | ((a_Value > 0x7f) ? 0x80 : 0x00);
			a_Value = a_Value >> 7;
			idx++;
		} while (a_Value > 0);

		SendData((const char *)b, idx);
	}
	
	void WriteVarUTF8String(const AString & a_String)
	{
		WriteVarInt((UInt32)a_String.size());
		SendData(a_String.data(), a_String.size());
	}
} ;





