#pragma once
#include "Protocol_1_16.h"





class cProtocol_1_17
	: public cProtocol_1_16_4
{
	using Super = cProtocol_1_16_4;
public:

	using Super::Super;

protected:
	virtual UInt32    GetPacketID(ePacketType a_PacketType) const override;
	virtual bool      HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void      HandlePacketClientSettings(cByteBuffer & a_ByteBuffer) override;
	virtual void      HandlePacketWindowClick(cByteBuffer & a_ByteBuffer) override;

	virtual void      SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void      SendPlayerMoveLook(const Vector3d a_Pos, const float a_Yaw, const float a_Pitch, const bool a_IsRelative) override;
	virtual void      SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;
	virtual void      SendRespawn(eDimension a_Dimension) override;
	virtual void      SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item) override;
	virtual void      SendMapData(const cMap & a_Map, int a_DataStartX, int a_DataStartY) override;

	virtual UInt32    GetProtocolMobType(eMonsterType a_MobType) const override;
	virtual int       GetProtocolParticleID(const AString & a_ParticleName) const override;
	virtual UInt8     GetProtocolEntityType(const cEntity & a_Entity) const override;

	virtual UInt8     GetEntityMetadataID(EntityMetadata a_Metadata) const override;

	virtual void      WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity) const override;

	virtual Version   GetProtocolVersion() const override;
};





class cProtocol_1_17_1
	: public cProtocol_1_17
{
	using Super = cProtocol_1_17;
public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
	virtual void SendWholeInventory(const cWindow & a_Window, const cItem & a_CursorStack) override;
};
