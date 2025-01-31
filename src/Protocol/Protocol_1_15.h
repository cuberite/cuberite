
// Protocol_1_15.h

/*
Declares the 1.15 protocol classes:
	- cProtocol_1_15
		- release 1.15 protocol (#573)
		- release 1.15.1 protocol (#574)
		- release 1.15.2 protocol (#577)
*/





#pragma once

#include "Protocol_1_14.h"





class cProtocol_1_15:
	public cProtocol_1_14_4
{
	using Super = cProtocol_1_14_4;

public:

	using Super::Super;

protected:

	virtual void SendBlockAction                (Vector3i a_BlockPos, char a_Byte1, char a_Byte2, BlockState a_Block) override;
	virtual void SendBlockBreakAnim             (UInt32 a_EntityID, Vector3i a_BlockPos, char a_Stage) override;
	virtual void SendRenderDistanceCenter       (cChunkCoords a_chunk) override;
	virtual void SendEntityAnimation            (const cEntity & a_Entity, EntityAnimation a_Animation) override;
	virtual void SendEntitySpawn                (const cEntity & a_Entity, const UInt8 a_ObjectType, const Int32 a_ObjectData) override;
	virtual void SendLogin                      (const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void SendPlayerActionResponse       (Vector3i a_blockpos, int a_state_id, cProtocol::PlayerActionResponses a_action, bool a_IsApproved) override;
	virtual void SendMapData                    (const cMap & a_Map, int a_DataStartX, int a_DataStartY) override;
	virtual void SendPaintingSpawn              (const cPainting & a_Painting) override;
	virtual void SendParticleEffect             (const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount) override;
	virtual void SendParticleEffect             (const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data) override;
	virtual void SendRespawn                    (eDimension a_Dimension) override;
	virtual void SendSoundParticleEffect        (const EffectID a_EffectID, Vector3i a_Origin, int a_Data) override;
	virtual void SendUpdateBlockEntity          (cBlockEntity & a_BlockEntity) override;
	virtual void SendUpdateSign                 (Vector3i a_BlockPos, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override;
	virtual void SendWindowOpen                 (const cWindow & a_Window) override;
	virtual void SendEntityMetadata             (const cEntity & a_Entity) override;
	virtual void SendPlayerSpawn                (const cPlayer & a_Player) override;
	virtual void SendSoundEffect                (const AString & a_SoundName, Vector3d a_Origin, float a_Volume, float a_Pitch) override;
	virtual void SendSpawnMob                   (const cMonster & a_Mob) override;
	virtual void SendBlockChanges               (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;


	virtual UInt8 GetEntityMetadataID(EntityMetadata a_Metadata) const override;
	virtual UInt32 GetPacketID(ePacketType a_PacketType) const override;

	virtual signed char GetProtocolEntityStatus(EntityAnimation a_Animation) const override;
	virtual UInt8 GetProtocolEntityType(const cEntity & a_Entity) const override;
	virtual UInt32 GetProtocolMobType(eMonsterType a_MobType) const override;
	virtual int GetProtocolParticleID(const AString & a_ParticleName) const override;
	virtual UInt32 GetProtocolStatisticType(CustomStatistic a_Statistic) const override;
	virtual Version GetProtocolVersion() const override;

	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void HandlePacketBlockDig(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketUpdateSign(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketBookUpdate(cByteBuffer & a_ByteBuffer) override;

	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, EntityMetadata a_Metadata, EntityMetadataType a_FieldType) const override;
	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const override;
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) const override;
};





class cProtocol_1_15_1:
	public cProtocol_1_15
{
	using Super = cProtocol_1_15;

public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
};





class cProtocol_1_15_2:
	public cProtocol_1_15_1
{
	using Super = cProtocol_1_15_1;

public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;
};
