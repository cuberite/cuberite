
// Protocol18x.h

/*
Declares the 1.8.x protocol classes:
	- cProtocol180
		- release 1.8 protocol (#47)
(others may be added later in the future for the 1.8 release series)
*/





#pragma once

#include "Protocol.h"
#include "Protocol17x.h"
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





// fwd:
namespace Json
{
	class Value;
}





class cProtocol180 :
	public cProtocol176
{
	typedef cProtocol176 super;
	
public:

	cProtocol180(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);

	virtual void SendSoundParticleEffect (int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data) override;
	virtual void SendLoginSuccess        (void) override;
	virtual void SendPickupSpawn         (const cPickup & a_Pickup) override {}
	virtual void SendSpawnObject         (const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch) override {}
	virtual void SendInventorySlot       (char a_WindowID, short a_SlotNum, const cItem & a_Item) override {}
	virtual void SendWholeInventory      (const cWindow & a_Window) override {}
	virtual void SendParticleEffect      (const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmmount) override;
	virtual void SendPlayerMoveLook      (void) override;
	virtual void SendUseBed              (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void SendCollectEntity       (const cEntity & a_Entity, const cPlayer & a_Player) override;
	virtual void SendEntityVelocity      (const cEntity & a_Entity) override;
	virtual void SendDestroyEntity       (const cEntity & a_Entity) override;
	virtual void SendEntityRelMove       (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendEntityRelMoveLook   (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendEntityLook(const cEntity & a_Entity) override;
	virtual void SendTeleportEntity(const cEntity & a_Entity) override;
	virtual void SendEntityHeadLook(const cEntity & a_Entity) override;
	virtual void SendEntityMetadata(const cEntity & a_Entity) override;
	virtual void SendEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration) override;
	virtual void SendRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID) override;
	virtual void SendEntityProperties(const cEntity & a_Entity) override;
	virtual void SendKeepAlive(int a_PingID) override;
	virtual void SendHealth(void) override;
	virtual void SendExperience(void) override;
	virtual void SendPaintingSpawn(const cPainting & a_Painting) override;
	virtual void SendPluginMessage(const AString & a_Channel, const AString & a_Message) override;
	virtual void SendUnloadChunk(int a_ChunkX, int a_ChunkZ) override;
	virtual void SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer) override;
	virtual void SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;
	virtual void SendChat(const AString & a_Message) override;
	virtual void SendChat(const cCompositeChat & a_Message) override;
	virtual void SendPlayerListItem(const cPlayer & a_Player, bool a_IsOnline) override {}
	virtual void SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;

protected:

	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;

	// Packet handlers while in the Login state (m_State == 2):
	virtual void HandlePacketLoginStart(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer) override;

	// Packet handlers while in the Game state (m_State == 3):
	virtual void HandlePacketPluginMessage(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketClientStatus(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketSteerVehicle(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketEntityAction(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketUseEntity(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketKeepAlive(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketPlayerPos(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketPlayerPosLook(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketClientSettings(cByteBuffer & a_ByteBuffer) override;
} ;




