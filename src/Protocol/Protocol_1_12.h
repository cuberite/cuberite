
// Protocol_1_12.h

/*
Declares the 1.12 protocol classes:
	- cProtocol_1_12
		- release 1.12 protocol (#335)
	- cProtocol_1_12_1
		- release 1.12.1 protocol (#338)
	- cProtocol_1_12_2
		- release 1.12.2 protocol (#340)
*/





#pragma once

#include "Protocol_1_11.h"

#include "RecipeMapper.h"



class cProtocol_1_12:
	public cProtocol_1_11_1
{
	using Super = cProtocol_1_11_1;

public:

	using Super::Super;

protected:

	virtual UInt32 GetPacketID(ePacketType a_Packet) const override;
	virtual signed char GetProtocolEntityStatus(EntityAnimation a_Animation) const override;
	virtual UInt32 GetProtocolMobType(eMonsterType a_MobType) const override;
	virtual AString GetProtocolSoundEffectAsString(eSoundEvent a_SoundEvent) const override;
	virtual Version GetProtocolVersion() const override;

	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void HandlePacketAdvancementTab(cByteBuffer & a_ByteBuffer);
	virtual void HandleCraftRecipe(cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketCraftingBookData(cByteBuffer & a_ByteBuffer);

	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const override;
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) const override;
};





class cProtocol_1_12_1:
	public cProtocol_1_12
{
	using Super = cProtocol_1_12;

public:

	using Super::Super;

protected:

	virtual UInt32 GetPacketID(ePacketType a_Packet) const override;
	virtual Version GetProtocolVersion() const override;

	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
};





class cProtocol_1_12_2:
	public cProtocol_1_12_1
{
	using Super = cProtocol_1_12_1;

public:

	using Super::Super;

protected:

	virtual Version GetProtocolVersion() const override;

	virtual void HandlePacketKeepAlive(cByteBuffer & a_ByteBuffer) override;

	virtual void SendKeepAlive(UInt32 a_PingID) override;
	virtual void SendUnlockRecipe(UInt32 a_RecipeID) override;
	virtual void SendInitRecipes(UInt32 a_RecipeID) override;
};
