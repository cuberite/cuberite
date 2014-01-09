
// Protocol17x.cpp

/*
Implements the 1.7.x protocol classes:
	- cProtocol172
		- release 1.7.2 protocol (#4)
(others may be added later in the future for the 1.7 release series)
*/

#include "Globals.h"
#include "Protocol17x.h"
#include "ChunkDataSerializer.h"
#include "../ClientHandle.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"
#include "../WorldStorage/FastNBT.h"
#include "../StringCompression.h"
#include "../Entities/ExpOrb.h"
#include "../Entities/Minecart.h"
#include "../Entities/FallingBlock.h"
#include "../Entities/Pickup.h"
#include "../Entities/Player.h"
#include "../Mobs/IncludeAllMonsters.h"
#include "../UI/Window.h"





#define HANDLE_READ(ByteBuf, Proc, Type, Var) \
	Type Var; \
	ByteBuf.Proc(Var);





#define HANDLE_PACKET_READ(ByteBuf, Proc, Type, Var) \
	Type Var; \
	{ \
		if (!ByteBuf.Proc(Var)) \
		{ \
			ByteBuf.CheckValid(); \
			return false; \
		} \
		ByteBuf.CheckValid(); \
	}





cProtocol172::cProtocol172(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State) :
	super(a_Client),
	m_ServerAddress(a_ServerAddress),
	m_ServerPort(a_ServerPort),
	m_State(a_State),
	m_ReceivedData(32 KiB),
	m_OutPacketBuffer(64 KiB),
	m_OutPacketLenBuffer(20),  // 20 bytes is more than enough for one VarInt
	m_IsEncrypted(false)
{
}





void cProtocol172::DataReceived(const char * a_Data, int a_Size)
{
	if (m_IsEncrypted)
	{
		byte Decrypted[512];
		while (a_Size > 0)
		{
			int NumBytes = (a_Size > sizeof(Decrypted)) ? sizeof(Decrypted) : a_Size;
			m_Decryptor.ProcessData(Decrypted, (byte *)a_Data, NumBytes);
			AddReceivedData((const char *)Decrypted, NumBytes);
			a_Size -= NumBytes;
			a_Data += NumBytes;
		}
	}
	else
	{
		AddReceivedData(a_Data, a_Size);
	}
}





void cProtocol172::SendAttachEntity(const cEntity & a_Entity, const cEntity * a_Vehicle)
{
	cPacketizer Pkt(*this, 0x1b);  // Attach Entity packet
	Pkt.WriteInt(a_Entity.GetUniqueID());
	Pkt.WriteInt((a_Vehicle != NULL) ? a_Vehicle->GetUniqueID() : 0);
	Pkt.WriteBool(false);
}





void cProtocol172::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType)
{
	cPacketizer Pkt(*this, 0x24);  // Block Action packet
	Pkt.WriteInt(a_BlockX);
	Pkt.WriteShort(a_BlockY);
	Pkt.WriteInt(a_BlockZ);
	Pkt.WriteByte(a_Byte1);
	Pkt.WriteByte(a_Byte2);
	Pkt.WriteVarInt(a_BlockType);
}





void cProtocol172::SendBlockBreakAnim(int a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage)
{
	cPacketizer Pkt(*this, 0x24);  // Block Break Animation packet
	Pkt.WriteInt(a_EntityID);
	Pkt.WriteInt(a_BlockX);
	Pkt.WriteInt(a_BlockY);
	Pkt.WriteInt(a_BlockZ);
	Pkt.WriteChar(a_Stage);
}





void cProtocol172::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cPacketizer Pkt(*this, 0x23);  // Block Change packet
	Pkt.WriteInt(a_BlockX);
	Pkt.WriteByte(a_BlockY);
	Pkt.WriteInt(a_BlockZ);
	Pkt.WriteVarInt(a_BlockType);
	Pkt.WriteByte(a_BlockMeta);
}





void cProtocol172::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	cPacketizer Pkt(*this, 0x22);  // Multi Block Change packet
	Pkt.WriteInt(a_ChunkX);
	Pkt.WriteInt(a_ChunkZ);
	Pkt.WriteShort((short)a_Changes.size());
	Pkt.WriteInt(a_Changes.size() * 4);
	for (sSetBlockVector::const_iterator itr = a_Changes.begin(), end = a_Changes.end(); itr != end; ++itr)
	{
		unsigned int Coords = itr->y | (itr->z << 8) | (itr->x << 12);
		unsigned int Blocks = itr->BlockMeta | (itr->BlockType << 4);
		Pkt.WriteInt((Coords << 16) | Blocks);
	}  // for itr - a_Changes[]
}





void cProtocol172::SendChat(const AString & a_Message)
{
	cPacketizer Pkt(*this, 0x02);  // Chat Message packet
	Pkt.WriteString(Printf("{\"text\":\"%s\"}", EscapeString(a_Message).c_str()));
}





void cProtocol172::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	// Serialize first, before creating the Packetizer (the packetizer locks a CS)
	// This contains the flags and bitmasks, too
	const AString & ChunkData = a_Serializer.Serialize(cChunkDataSerializer::RELEASE_1_3_2);
	
	cPacketizer Pkt(*this, 0x21);  // Chunk Data packet
	Pkt.WriteInt(a_ChunkX);
	Pkt.WriteInt(a_ChunkZ);
	Pkt.WriteBuf(ChunkData.data(), ChunkData.size());
}





void cProtocol172::SendCollectPickup(const cPickup & a_Pickup, const cPlayer & a_Player)
{
	cPacketizer Pkt(*this, 0x0d);  // Collect Item packet
	Pkt.WriteInt(a_Pickup.GetUniqueID());
	Pkt.WriteInt(a_Player.GetUniqueID());
}





void cProtocol172::SendDestroyEntity(const cEntity & a_Entity)
{
	cPacketizer Pkt(*this, 0x13);  // Destroy Entities packet
	Pkt.WriteByte(1);
	Pkt.WriteInt(a_Entity.GetUniqueID());
}





void cProtocol172::SendDisconnect(const AString & a_Reason)
{
	cPacketizer Pkt(*this, 0x40);
	Pkt.WriteString(Printf("{\"text\":\"%s\"}", EscapeString(a_Reason).c_str()));
}





void cProtocol172::SendEditSign(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cPacketizer Pkt(*this, 0x36);  // Sign Editor Open packet
	Pkt.WriteInt(a_BlockX);
	Pkt.WriteInt(a_BlockY);
	Pkt.WriteInt(a_BlockZ);
}





void cProtocol172::SendEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration)
{
	cPacketizer Pkt(*this, 0x1D);  // Entity Effect packet
	Pkt.WriteInt(a_Entity.GetUniqueID());
	Pkt.WriteByte(a_EffectID);
	Pkt.WriteByte(a_Amplifier);
	Pkt.WriteShort(a_Duration);
}





void cProtocol172::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	cPacketizer Pkt(*this, 0x04);  // Entity Equipment packet
	Pkt.WriteInt(a_Entity.GetUniqueID());
	Pkt.WriteShort(a_SlotNum);
	Pkt.WriteItem(a_Item);
}





void cProtocol172::SendEntityHeadLook(const cEntity & a_Entity)
{
	cPacketizer Pkt(*this, 0x19);  // Entity Head Look packet
	Pkt.WriteInt(a_Entity.GetUniqueID());
	Pkt.WriteByteAngle(a_Entity.GetHeadYaw());
}





void cProtocol172::SendEntityLook(const cEntity & a_Entity)
{
	cPacketizer Pkt(*this, 0x16);  // Entity Look packet
	Pkt.WriteInt(a_Entity.GetUniqueID());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
}





void cProtocol172::SendEntityMetadata(const cEntity & a_Entity)
{
	cPacketizer Pkt(*this, 0x1c);  // Entity Metadata packet
	Pkt.WriteInt(a_Entity.GetUniqueID());
	Pkt.WriteEntityMetadata(a_Entity);
	Pkt.WriteByte(0x7f);  // The termination byte
}





void cProtocol172::SendEntityProperties(const cEntity & a_Entity)
{
	cPacketizer Pkt(*this, 0x20);  // Entity Properties packet
	Pkt.WriteInt(a_Entity.GetUniqueID());
	Pkt.WriteEntityProperties(a_Entity);
}





void cProtocol172::SendEntityRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	cPacketizer Pkt(*this, 0x15);  // Entity Relative Move packet
	Pkt.WriteInt(a_Entity.GetUniqueID());
	Pkt.WriteByte(a_RelX);
	Pkt.WriteByte(a_RelY);
	Pkt.WriteByte(a_RelZ);
}





void cProtocol172::SendEntityRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	cPacketizer Pkt(*this, 0x17);  // Entity Look And Relative Move packet
	Pkt.WriteInt(a_Entity.GetUniqueID());
	Pkt.WriteByte(a_RelX);
	Pkt.WriteByte(a_RelY);
	Pkt.WriteByte(a_RelZ);
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
}





void cProtocol172::SendEntityStatus(const cEntity & a_Entity, char a_Status)
{
	cPacketizer Pkt(*this, 0x1a);  // Entity Status packet
	Pkt.WriteInt(a_Entity.GetUniqueID());
	Pkt.WriteChar(a_Status);
}





void cProtocol172::SendEntityVelocity(const cEntity & a_Entity)
{
	cPacketizer Pkt(*this, 0x12);  // Entity Velocity packet
	Pkt.WriteInt(a_Entity.GetUniqueID());
	// 400 = 8000 / 20 ... Conversion from our speed in m/s to 8000 m/tick
	Pkt.WriteShort((short)(a_Entity.GetSpeedX() * 400));
	Pkt.WriteShort((short)(a_Entity.GetSpeedY() * 400));
	Pkt.WriteShort((short)(a_Entity.GetSpeedZ() * 400));
}





void cProtocol172::SendExplosion(double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion)
{
	cPacketizer Pkt(*this, 0x27);  // Explosion packet
	Pkt.WriteFloat((float)a_BlockX);
	Pkt.WriteFloat((float)a_BlockY);
	Pkt.WriteFloat((float)a_BlockZ);
	Pkt.WriteFloat((float)a_Radius);
	Pkt.WriteInt(a_BlocksAffected.size());
	for (cVector3iArray::const_iterator itr = a_BlocksAffected.begin(), end = a_BlocksAffected.end(); itr != end; ++itr)
	{
		Pkt.WriteChar((char)itr->x);
		Pkt.WriteChar((char)itr->y);
		Pkt.WriteChar((char)itr->z);
	}  // for itr - a_BlockAffected[]
	Pkt.WriteFloat((float)a_PlayerMotion.x);
	Pkt.WriteFloat((float)a_PlayerMotion.y);
	Pkt.WriteFloat((float)a_PlayerMotion.z);
}





void cProtocol172::SendGameMode(eGameMode a_GameMode)
{
	cPacketizer Pkt(*this, 0x2b);  // Change Game State packet
	Pkt.WriteByte(3);  // Reason: Change game mode
	Pkt.WriteFloat((float)a_GameMode);
}





void cProtocol172::SendHealth(void)
{
	cPacketizer Pkt(*this, 0x06);  // Update Health packet
	Pkt.WriteFloat((float)m_Client->GetPlayer()->GetHealth());
	Pkt.WriteShort(m_Client->GetPlayer()->GetFoodLevel());
	Pkt.WriteFloat((float)m_Client->GetPlayer()->GetFoodSaturationLevel());
}





void cProtocol172::SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	cPacketizer Pkt(*this, 0x2f);  // Set Slot packet
	Pkt.WriteChar(a_WindowID);
	Pkt.WriteShort(a_SlotNum);
	Pkt.WriteItem(a_Item);
}





void cProtocol172::SendKeepAlive(int a_PingID)
{
	cPacketizer Pkt(*this, 0x00);  // Keep Alive packet
	Pkt.WriteInt(a_PingID);
}





void cProtocol172::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	// Send the Join Game packet:
	{
		cPacketizer Pkt(*this, 0x01);  // Join Game packet
		Pkt.WriteInt(a_Player.GetUniqueID());
		Pkt.WriteByte((Byte)a_Player.GetEffectiveGameMode() | (cRoot::Get()->GetServer()->IsHardcore() ? 0x08 : 0)); // Hardcore flag bit 4
		Pkt.WriteChar((char)a_World.GetDimension());
		Pkt.WriteByte(2);  // TODO: Difficulty (set to Normal)
		Pkt.WriteByte(cRoot::Get()->GetServer()->GetMaxPlayers());
		Pkt.WriteString("default");  // Level type - wtf?
	}
	
	// Send the spawn position:
	{
		cPacketizer Pkt(*this, 0x05);  // Spawn Position packet
		Pkt.WriteInt((int)a_World.GetSpawnX());
		Pkt.WriteInt((int)a_World.GetSpawnY());
		Pkt.WriteInt((int)a_World.GetSpawnZ());
	}
	
	// Send player abilities:
	SendPlayerAbilities();
}





void cProtocol172::SendPickupSpawn(const cPickup & a_Pickup)
{
	{
		cPacketizer Pkt(*this, 0x0e);  // Spawn Object packet
		Pkt.WriteVarInt(a_Pickup.GetUniqueID());
		Pkt.WriteByte(2);  // Type = Pickup
		Pkt.WriteFPInt(a_Pickup.GetPosX());
		Pkt.WriteFPInt(a_Pickup.GetPosY());
		Pkt.WriteFPInt(a_Pickup.GetPosZ());
		Pkt.WriteByteAngle(a_Pickup.GetYaw());
		Pkt.WriteByteAngle(a_Pickup.GetPitch());
		Pkt.WriteInt(0);  // No object data
	}
	{
		cPacketizer Pkt(*this, 0x1c);  // Entity Metadata packet
		Pkt.WriteInt(a_Pickup.GetUniqueID());
		Pkt.WriteByte((0x05 << 5) | 10);  // Slot type + index 10
		Pkt.WriteItem(a_Pickup.GetItem());
		Pkt.WriteByte(0x7f);  // End of metadata
	}
}





void cProtocol172::SendPlayerAbilities(void)
{
	cPacketizer Pkt(*this, 0x39);  // Player Abilities packet
	Byte Flags = 0;
	if (m_Client->GetPlayer()->IsGameModeCreative())
	{
		Flags |= 0x01;
		Flags |= 0x08; // Godmode, used for creative
	}
	if (m_Client->GetPlayer()->IsFlying())
	{
		Flags |= 0x02;
	}
	if (m_Client->GetPlayer()->CanFly())
	{
		Flags |= 0x04;
	}
	Pkt.WriteByte(Flags);
	// TODO: Pkt.WriteFloat(m_Client->GetPlayer()->GetMaxFlyingSpeed());
	Pkt.WriteFloat(0.05f);
	Pkt.WriteFloat((float)m_Client->GetPlayer()->GetMaxSpeed());
}





void cProtocol172::SendEntityAnimation(const cEntity & a_Entity, char a_Animation)
{
	cPacketizer Pkt(*this, 0x0b);  // Animation packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteChar(a_Animation);
}





void cProtocol172::SendParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmmount)
{
	cPacketizer Pkt(*this, 0x2A);
	Pkt.WriteString(a_ParticleName);
	Pkt.WriteFloat(a_SrcX);
	Pkt.WriteFloat(a_SrcY);
	Pkt.WriteFloat(a_SrcZ);
	Pkt.WriteFloat(a_OffsetX);
	Pkt.WriteFloat(a_OffsetY);
	Pkt.WriteFloat(a_OffsetZ);
	Pkt.WriteFloat(a_ParticleData);
	Pkt.WriteInt(a_ParticleAmmount);
}





void cProtocol172::SendPlayerListItem(const cPlayer & a_Player, bool a_IsOnline)
{
	cPacketizer Pkt(*this, 0x38);  // Playerlist Item packet
	Pkt.WriteString(a_Player.GetName());
	Pkt.WriteBool(a_IsOnline);
	Pkt.WriteShort(a_IsOnline ? a_Player.GetClientHandle()->GetPing() : 0);
}





void cProtocol172::SendPlayerMaxSpeed(void)
{
	cPacketizer Pkt(*this, 0x20);  // Entity Properties
	Pkt.WriteInt(m_Client->GetPlayer()->GetUniqueID());
	Pkt.WriteInt(1);  // Count
	Pkt.WriteString("generic.movementSpeed");
	Pkt.WriteDouble(0.1);
	if (m_Client->GetPlayer()->IsSprinting())
	{
		Pkt.WriteShort(1);  // Modifier count
		Pkt.WriteInt64(0x662a6b8dda3e4c1c);
		Pkt.WriteInt64(0x881396ea6097278d);  // UUID of the modifier
		Pkt.WriteDouble(0.3);
		Pkt.WriteByte(2);
	}
	else
	{
		Pkt.WriteShort(0);  // Modifier count
	}
}





void cProtocol172::SendPlayerMoveLook(void)
{
	cPacketizer Pkt(*this, 0x08);  // Player Position And Look packet
	Pkt.WriteDouble(m_Client->GetPlayer()->GetPosX());
	
	// Protocol docs say this is PosY, but #323 says this is eye-pos
	// Moreover, the "+ 0.001" is there because otherwise the player falls through the block they were standing on.
	Pkt.WriteDouble(m_Client->GetPlayer()->GetStance() + 0.001);
	
	Pkt.WriteDouble(m_Client->GetPlayer()->GetPosZ());
	Pkt.WriteFloat((float)m_Client->GetPlayer()->GetYaw());
	Pkt.WriteFloat((float)m_Client->GetPlayer()->GetPitch());
	Pkt.WriteBool(m_Client->GetPlayer()->IsOnGround());
}





void cProtocol172::SendPlayerPosition(void)
{
	// There is no dedicated packet for this, send the whole thing:
	SendPlayerMoveLook();
}





void cProtocol172::SendPlayerSpawn(const cPlayer & a_Player)
{
	// Called to spawn another player for the client
	cPacketizer Pkt(*this, 0x0c);  // Spawn Player packet
	Pkt.WriteVarInt(a_Player.GetUniqueID());
	Pkt.WriteString(Printf("%d", a_Player.GetUniqueID()));  // TODO: Proper UUID
	Pkt.WriteString(a_Player.GetName());
	Pkt.WriteFPInt(a_Player.GetPosX());
	Pkt.WriteFPInt(a_Player.GetPosY());
	Pkt.WriteFPInt(a_Player.GetPosZ());
	Pkt.WriteByteAngle(a_Player.GetYaw());
	Pkt.WriteByteAngle(a_Player.GetPitch());
	short ItemType = a_Player.GetEquippedItem().IsEmpty() ? 0 : a_Player.GetEquippedItem().m_ItemType;
	Pkt.WriteShort(ItemType);
	Pkt.WriteByte((3 << 5) | 6);  // Metadata: float + index 6
	Pkt.WriteFloat((float)a_Player.GetHealth());
	Pkt.WriteByte(0x7f);  // Metadata: end
}





void cProtocol172::SendPluginMessage(const AString & a_Channel, const AString & a_Message)
{
	cPacketizer Pkt(*this, 0x3f);
	Pkt.WriteString(a_Channel);
	Pkt.WriteShort((short)a_Message.size());
	Pkt.WriteBuf(a_Message.data(), a_Message.size());
}





void cProtocol172::SendRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID)
{
	cPacketizer Pkt(*this, 0x1E);
	Pkt.WriteInt(a_Entity.GetUniqueID());
	Pkt.WriteByte(a_EffectID);
}





void cProtocol172::SendRespawn(void)
{
	cPacketizer Pkt(*this, 0x07);  // Respawn packet
	Pkt.WriteInt(m_Client->GetPlayer()->GetWorld()->GetDimension());
	Pkt.WriteByte(2);  // TODO: Difficulty (set to Normal)
	Pkt.WriteByte((Byte)m_Client->GetPlayer()->GetEffectiveGameMode());
	Pkt.WriteString("default");
}





void cProtocol172::SendExperience (void)
{
	cPacketizer Pkt(*this, 0x1F); //Experience Packet
	Pkt.WriteFloat(m_Client->GetPlayer()->GetXpPercentage());
	Pkt.WriteShort(m_Client->GetPlayer()->GetXpLevel());
	Pkt.WriteShort(m_Client->GetPlayer()->GetCurrentXp());
}





void cProtocol172::SendExperienceOrb(const cExpOrb & a_ExpOrb)
{
	cPacketizer Pkt(*this, 0x11);	
	Pkt.WriteVarInt(a_ExpOrb.GetUniqueID());
	Pkt.WriteInt((int) a_ExpOrb.GetPosX());
	Pkt.WriteInt((int) a_ExpOrb.GetPosY());
	Pkt.WriteInt((int) a_ExpOrb.GetPosZ());
	Pkt.WriteShort(a_ExpOrb.GetReward());
}





void cProtocol172::SendSoundEffect(const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch)  // a_Src coords are Block * 8
{
	cPacketizer Pkt(*this, 0x29);  // Sound Effect packet
	Pkt.WriteString(a_SoundName);
	Pkt.WriteInt(a_SrcX);
	Pkt.WriteInt(a_SrcY);
	Pkt.WriteInt(a_SrcZ);
	Pkt.WriteFloat(a_Volume);
	Pkt.WriteByte((Byte)(a_Pitch * 63));
}





void cProtocol172::SendSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
	cPacketizer Pkt(*this, 0x28);  // Effect packet
	Pkt.WriteInt(a_EffectID);
	Pkt.WriteInt(a_SrcX);
	Pkt.WriteByte(a_SrcY);
	Pkt.WriteInt(a_SrcZ);
	Pkt.WriteInt(a_Data);
	Pkt.WriteBool(false);
}





void cProtocol172::SendSpawnFallingBlock(const cFallingBlock & a_FallingBlock)
{
	cPacketizer Pkt(*this, 0x0e);  // Spawn Object packet
	Pkt.WriteVarInt(a_FallingBlock.GetUniqueID());
	Pkt.WriteByte(70);  // Falling block
	Pkt.WriteFPInt(a_FallingBlock.GetPosX());
	Pkt.WriteFPInt(a_FallingBlock.GetPosY());
	Pkt.WriteFPInt(a_FallingBlock.GetPosZ());
	Pkt.WriteByteAngle(a_FallingBlock.GetYaw());
	Pkt.WriteByteAngle(a_FallingBlock.GetPitch());
	Pkt.WriteInt(((int)a_FallingBlock.GetBlockType()) | (((int)a_FallingBlock.GetBlockMeta()) << 16)); // Or 0x10
	Pkt.WriteShort((short)(a_FallingBlock.GetSpeedX() * 400));
	Pkt.WriteShort((short)(a_FallingBlock.GetSpeedY() * 400));
	Pkt.WriteShort((short)(a_FallingBlock.GetSpeedZ() * 400));
}





void cProtocol172::SendSpawnMob(const cMonster & a_Mob)
{
	cPacketizer Pkt(*this, 0x0f);  // Spawn Mob packet
	Pkt.WriteVarInt(a_Mob.GetUniqueID());
	Pkt.WriteByte((Byte)a_Mob.GetMobType());
	Pkt.WriteFPInt(a_Mob.GetPosX());
	Pkt.WriteFPInt(a_Mob.GetPosY());
	Pkt.WriteFPInt(a_Mob.GetPosZ());
	Pkt.WriteByteAngle(a_Mob.GetPitch());
	Pkt.WriteByteAngle(a_Mob.GetHeadYaw());
	Pkt.WriteByteAngle(a_Mob.GetYaw());
	Pkt.WriteShort((short)(a_Mob.GetSpeedX() * 400));
	Pkt.WriteShort((short)(a_Mob.GetSpeedY() * 400));
	Pkt.WriteShort((short)(a_Mob.GetSpeedZ() * 400));
	Pkt.WriteEntityMetadata(a_Mob);
	Pkt.WriteByte(0x7f);  // Metadata terminator
}





void cProtocol172::SendSpawnObject(const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch)
{
	cPacketizer Pkt(*this, 0xe);  // Spawn Object packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteByte(a_ObjectType);
	Pkt.WriteFPInt(a_Entity.GetPosX());
	Pkt.WriteFPInt(a_Entity.GetPosY());
	Pkt.WriteFPInt(a_Entity.GetPosZ());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteInt(a_ObjectData);
	if (a_ObjectData != 0)
	{
		Pkt.WriteShort((short)(a_Entity.GetSpeedX() * 400));
		Pkt.WriteShort((short)(a_Entity.GetSpeedY() * 400));
		Pkt.WriteShort((short)(a_Entity.GetSpeedZ() * 400));
	}
}





void cProtocol172::SendSpawnVehicle(const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType)
{
	cPacketizer Pkt(*this, 0xe);  // Spawn Object packet
	Pkt.WriteVarInt(a_Vehicle.GetUniqueID());
	Pkt.WriteByte(a_VehicleType);
	Pkt.WriteFPInt(a_Vehicle.GetPosX());
	Pkt.WriteFPInt(a_Vehicle.GetPosY());
	Pkt.WriteFPInt(a_Vehicle.GetPosZ());
	Pkt.WriteByteAngle(a_Vehicle.GetYaw());
	Pkt.WriteByteAngle(a_Vehicle.GetPitch());
	Pkt.WriteInt(a_VehicleSubType);
	if (a_VehicleSubType != 0)
	{
		Pkt.WriteShort((short)(a_Vehicle.GetSpeedX() * 400));
		Pkt.WriteShort((short)(a_Vehicle.GetSpeedY() * 400));
		Pkt.WriteShort((short)(a_Vehicle.GetSpeedZ() * 400));
	}
}





void cProtocol172::SendTabCompletionResults(const AStringVector & a_Results)
{
	cPacketizer Pkt(*this, 0x3a);  // Tab-Complete packet
	Pkt.WriteVarInt(a_Results.size());

	for (AStringVector::const_iterator itr = a_Results.begin(), end = a_Results.end(); itr != end; ++itr)
	{
		Pkt.WriteString(*itr);
	}
}





void cProtocol172::SendTeleportEntity(const cEntity & a_Entity)
{
	cPacketizer Pkt(*this, 0x18);
	Pkt.WriteInt(a_Entity.GetUniqueID());
	Pkt.WriteFPInt(a_Entity.GetPosX());
	Pkt.WriteFPInt(a_Entity.GetPosY());
	Pkt.WriteFPInt(a_Entity.GetPosZ());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
}





void cProtocol172::SendThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cPacketizer Pkt(*this, 0x2c);  // Spawn Global Entity packet
	Pkt.WriteVarInt(0);  // EntityID = 0, always
	Pkt.WriteByte(1);  // Type = Thunderbolt
	Pkt.WriteFPInt(a_BlockX);
	Pkt.WriteFPInt(a_BlockY);
	Pkt.WriteFPInt(a_BlockZ);
}





void cProtocol172::SendTimeUpdate(Int64 a_WorldAge, Int64 a_TimeOfDay)
{
	cPacketizer Pkt(*this, 0x03);
	Pkt.WriteInt64(a_WorldAge);
	Pkt.WriteInt64(a_TimeOfDay);
}





void cProtocol172::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	cPacketizer Pkt(*this, 0x21);  // Chunk Data packet
	Pkt.WriteInt(a_ChunkX);
	Pkt.WriteInt(a_ChunkZ);
	Pkt.WriteBool(true);
	Pkt.WriteShort(0);  // Primary bitmap
	Pkt.WriteShort(0);  // Add bitmap
	Pkt.WriteInt(0);  // Compressed data size
}





void cProtocol172::SendUpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	cPacketizer Pkt(*this, 0x33);
	Pkt.WriteInt(a_BlockX);
	Pkt.WriteShort((short)a_BlockY);
	Pkt.WriteInt(a_BlockZ);
	Pkt.WriteString(a_Line1);
	Pkt.WriteString(a_Line2);
	Pkt.WriteString(a_Line3);
	Pkt.WriteString(a_Line4);
}





void cProtocol172::SendUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cPacketizer Pkt(*this, 0x0a);
	Pkt.WriteInt(a_Entity.GetUniqueID());
	Pkt.WriteInt(a_BlockX);
	Pkt.WriteByte((Byte)a_BlockY);
	Pkt.WriteInt(a_BlockZ);
}





void cProtocol172::SendWeather(eWeather a_Weather)
{
	{
		cPacketizer Pkt(*this, 0x2b);  // Change Game State packet
		Pkt.WriteByte((a_Weather == wSunny) ? 1 : 2);  // End rain / begin rain
		Pkt.WriteFloat(0);  // Unused for weather
	}

	// TODO: Fade effect, somehow
}





void cProtocol172::SendWholeInventory(const cWindow & a_Window)
{
	cPacketizer Pkt(*this, 0x30);  // Window Items packet
	Pkt.WriteChar(a_Window.GetWindowID());
	Pkt.WriteShort(a_Window.GetNumSlots());
	cItems Slots;
	a_Window.GetSlots(*(m_Client->GetPlayer()), Slots);
	for (cItems::const_iterator itr = Slots.begin(), end = Slots.end(); itr != end; ++itr)
	{
		Pkt.WriteItem(*itr);
	}  // for itr - Slots[]
}





void cProtocol172::SendWindowClose(const cWindow & a_Window)
{
	cPacketizer Pkt(*this, 0x2e);
	Pkt.WriteChar(a_Window.GetWindowID());
}





void cProtocol172::SendWindowOpen(const cWindow & a_Window)
{
	if (a_Window.GetWindowType() < 0)
	{
		// Do not send this packet for player inventory windows
		return;
	}
	
	cPacketizer Pkt(*this, 0x2d);
	Pkt.WriteChar(a_Window.GetWindowID());
	Pkt.WriteChar(a_Window.GetWindowType());
	Pkt.WriteString(a_Window.GetWindowTitle());
	Pkt.WriteChar(a_Window.GetNumNonInventorySlots());
	Pkt.WriteBool(true);
	if (a_Window.GetWindowType() == cWindow::wtAnimalChest)
	{
		Pkt.WriteInt(0);  // TODO: The animal's EntityID
	}
}





void cProtocol172::SendWindowProperty(const cWindow & a_Window, short a_Property, short a_Value)
{
	cPacketizer Pkt(*this, 0x31);  // Window Property packet
	Pkt.WriteChar(a_Window.GetWindowID());
	Pkt.WriteShort(a_Property);
	Pkt.WriteShort(a_Value);
}





void cProtocol172::AddReceivedData(const char * a_Data, int a_Size)
{
	if (!m_ReceivedData.Write(a_Data, a_Size))
	{
		// Too much data in the incoming queue, report to caller:
		m_Client->PacketBufferFull();
		return;
	}
	
	// Handle all complete packets:
	for (;;)
	{
		UInt32 PacketLen;
		if (!m_ReceivedData.ReadVarInt(PacketLen))
		{
			// Not enough data
			return;
		}
		if (!m_ReceivedData.CanReadBytes(PacketLen))
		{
			// The full packet hasn't been received yet
			return;
		}
		cByteBuffer bb(PacketLen + 1);
		VERIFY(m_ReceivedData.ReadToByteBuffer(bb, (int)PacketLen));
		m_ReceivedData.CommitRead();
		
		// Write one NUL extra, so that we can detect over-reads
		bb.Write("\0", 1);
		
		UInt32 PacketType;
		if (!bb.ReadVarInt(PacketType))
		{
			// Not enough data
			return;
		}

		HandlePacket(bb, PacketType);
		
		if (bb.GetReadableSpace() != 1)
		{
			// Read more or less than packet length, report as error
			ASSERT(!"Read wrong number of bytes!");
			m_Client->PacketError(PacketType);
		}
	}  // while (true)
}




void cProtocol172::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	switch (m_State)
	{
		case 1:
		{
			// Status
			switch (a_PacketType)
			{
				case 0x00: HandlePacketStatusRequest(a_ByteBuffer); return;
				case 0x01: HandlePacketStatusPing   (a_ByteBuffer); return;
			}
			break;
		}
		
		case 2:
		{
			// Login
			switch (a_PacketType)
			{
				case 0x00: HandlePacketLoginStart             (a_ByteBuffer); return;
				case 0x01: HandlePacketLoginEncryptionResponse(a_ByteBuffer); return;
			}
			break;
		}
		
		case 3:
		{
			// Game
			switch (a_PacketType)
			{
				case 0x00: HandlePacketKeepAlive              (a_ByteBuffer); return;
				case 0x01: HandlePacketChatMessage            (a_ByteBuffer); return;
				case 0x02: HandlePacketUseEntity              (a_ByteBuffer); return;
				case 0x03: HandlePacketPlayer                 (a_ByteBuffer); return;
				case 0x04: HandlePacketPlayerPos              (a_ByteBuffer); return;
				case 0x05: HandlePacketPlayerLook             (a_ByteBuffer); return;
				case 0x06: HandlePacketPlayerPosLook          (a_ByteBuffer); return;
				case 0x07: HandlePacketBlockDig               (a_ByteBuffer); return;
				case 0x08: HandlePacketBlockPlace             (a_ByteBuffer); return;
				case 0x09: HandlePacketSlotSelect             (a_ByteBuffer); return;
				case 0x0a: HandlePacketAnimation              (a_ByteBuffer); return;
				case 0x0b: HandlePacketEntityAction           (a_ByteBuffer); return;
				case 0x0c: HandlePacketSteerVehicle           (a_ByteBuffer); return;
				case 0x0d: HandlePacketWindowClose            (a_ByteBuffer); return;
				case 0x0e: HandlePacketWindowClick            (a_ByteBuffer); return;
				case 0x0f: // Confirm transaction - not used in MCS
				case 0x10: HandlePacketCreativeInventoryAction(a_ByteBuffer); return;
				case 0x12: HandlePacketUpdateSign             (a_ByteBuffer); return;
				case 0x13: HandlePacketPlayerAbilities        (a_ByteBuffer); return;
				case 0x14: HandlePacketTabComplete            (a_ByteBuffer); return;
				case 0x15: HandlePacketClientSettings         (a_ByteBuffer); return;
				case 0x16: HandlePacketClientStatus           (a_ByteBuffer); return;
				case 0x17: HandlePacketPluginMessage          (a_ByteBuffer); return;
			}
			break;
		}
	}  // switch (m_State)
	
	// Unknown packet type, report to the client:
	m_Client->PacketUnknown(a_PacketType);
}





void cProtocol172::HandlePacketStatusPing(cByteBuffer & a_ByteBuffer)
{
	Int64 Timestamp;
	a_ByteBuffer.ReadBEInt64(Timestamp);
	
	cPacketizer Pkt(*this, 0x01);  // Ping packet
	Pkt.WriteInt64(Timestamp);
}





void cProtocol172::HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer)
{
	// Send the response:
	AString Response = "{\"version\":{\"name\":\"1.7.2\",\"protocol\":4},\"players\":{";
	AppendPrintf(Response, "\"max\":%u,\"online\":%u,\"sample\":[]},",
		cRoot::Get()->GetServer()->GetMaxPlayers(),
		cRoot::Get()->GetServer()->GetNumPlayers()
	);
	AppendPrintf(Response, "\"description\":{\"text\":\"%s\"},",
		cRoot::Get()->GetServer()->GetDescription().c_str()
	);
	AppendPrintf(Response, "\"favicon\":\"data:image/png;base64,%s\"",
		cRoot::Get()->GetServer()->GetFaviconData().c_str()
	);
	Response.append("}");
	
	cPacketizer Pkt(*this, 0x00);  // Response packet
	Pkt.WriteString(Response);
}





void cProtocol172::HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer)
{
	// TODO: Add protocol encryption
}





void cProtocol172::HandlePacketLoginStart(cByteBuffer & a_ByteBuffer)
{
	AString Username;
	a_ByteBuffer.ReadVarUTF8String(Username);
	
	// TODO: Protocol encryption should be set up here if not localhost / auth
	
	// Send login success:
	{
		cPacketizer Pkt(*this, 0x02);  // Login success packet
		Pkt.WriteString(Printf("%d", m_Client->GetUniqueID()));  // TODO: proper UUID
		Pkt.WriteString(Username);
	}

	m_State = 3;  // State = Game
	m_Client->HandleLogin(4, Username);
}





void cProtocol172::HandlePacketAnimation(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt, int,  EntityID);
	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, Animation);
	m_Client->HandleAnimation(Animation);
}





void cProtocol172::HandlePacketBlockDig(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, Status);
	HANDLE_READ(a_ByteBuffer, ReadBEInt, int,  BlockX);
	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, BlockY);
	HANDLE_READ(a_ByteBuffer, ReadBEInt, int,  BlockZ);
	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, Face);
	m_Client->HandleLeftClick(BlockX, BlockY, BlockZ, Face, Status);
}





void cProtocol172::HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt, int,  BlockX);
	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, BlockY);
	HANDLE_READ(a_ByteBuffer, ReadBEInt, int,  BlockZ);
	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, Face);
	cItem Item;
	ReadItem(a_ByteBuffer, Item);

	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, CursorX);
	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, CursorY);
	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, CursorZ);
	m_Client->HandleRightClick(BlockX, BlockY, BlockZ, Face, CursorX, CursorY, CursorZ, m_Client->GetPlayer()->GetEquippedItem());
}





void cProtocol172::HandlePacketChatMessage(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Message);
	m_Client->HandleChat(Message);
}





void cProtocol172::HandlePacketClientSettings(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Locale);
	HANDLE_READ(a_ByteBuffer, ReadByte,          Byte,    ViewDistance);
	HANDLE_READ(a_ByteBuffer, ReadByte,          Byte,    ChatFlags);
	HANDLE_READ(a_ByteBuffer, ReadByte,          Byte,    ChatColors);
	HANDLE_READ(a_ByteBuffer, ReadByte,          Byte,    Difficulty);
	HANDLE_READ(a_ByteBuffer, ReadByte,          Byte,    ShowCape);
	// TODO: handle in m_Client
}





void cProtocol172::HandlePacketClientStatus(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadByte, Byte, ActionID);
	switch (ActionID)
	{
		case 0:
		{
			// Respawn
			m_Client->HandleRespawn();
			break;
		}
		case 1:
		{
			// Request stats
			// TODO
			break;
		}
		case 2:
		{
			// Open Inventory achievement
			// TODO
			break;
		}
	}
}





void cProtocol172::HandlePacketCreativeInventoryAction(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEShort, short, SlotNum);
	cItem Item;
	if (!ReadItem(a_ByteBuffer, Item))
	{
		return;
	}
	m_Client->HandleCreativeInventory(SlotNum, Item);
}





void cProtocol172::HandlePacketEntityAction(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt, int,  PlayerID);
	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, Action);
	HANDLE_READ(a_ByteBuffer, ReadBEInt, int,  JumpBoost);
	m_Client->HandleEntityAction(PlayerID, Action);
}





void cProtocol172::HandlePacketKeepAlive(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt, int, KeepAliveID);
	m_Client->HandleKeepAlive(KeepAliveID);
}





void cProtocol172::HandlePacketPlayer(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, IsOnGround);
	// TODO: m_Client->HandlePlayerOnGround(IsOnGround);
}





void cProtocol172::HandlePacketPlayerAbilities(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadByte,    Byte,  Flags);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, FlyingSpeed);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, WalkingSpeed);

	bool IsFlying = false, CanFly = false;
	if ((Flags & 2) != 0)
	{
		IsFlying = true;
	}
	if ((Flags & 4) != 0)
	{
		CanFly = true;
	}

	m_Client->HandlePlayerAbilities(CanFly, IsFlying, FlyingSpeed, WalkingSpeed);
}





void cProtocol172::HandlePacketPlayerLook(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Yaw);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Pitch);
	HANDLE_READ(a_ByteBuffer, ReadBool,    bool,  IsOnGround);
	m_Client->HandlePlayerLook(Yaw, Pitch, IsOnGround);
}





void cProtocol172::HandlePacketPlayerPos(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosX);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosY);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, Stance);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosZ);
	HANDLE_READ(a_ByteBuffer, ReadBool,     bool,   IsOnGround);
	m_Client->HandlePlayerPos(PosX, PosY, PosZ, Stance, IsOnGround);
}





void cProtocol172::HandlePacketPlayerPosLook(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosX);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosY);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, Stance);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosZ);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Yaw);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Pitch);
	HANDLE_READ(a_ByteBuffer, ReadBool,     bool,   IsOnGround);
	m_Client->HandlePlayerMoveLook(PosX, PosY, PosZ, Stance, Yaw, Pitch, IsOnGround);
}





void cProtocol172::HandlePacketPluginMessage(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Channel);
	HANDLE_READ(a_ByteBuffer, ReadBEShort,       short,   Length);
	AString Data;
	a_ByteBuffer.ReadString(Data, Length);
	m_Client->HandlePluginMessage(Channel, Data);
}





void cProtocol172::HandlePacketSlotSelect(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEShort, short, SlotNum);
	m_Client->HandleSlotSelected(SlotNum);
}





void cProtocol172::HandlePacketSteerVehicle(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Forward);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Sideways);
	HANDLE_READ(a_ByteBuffer, ReadBool,    bool,  ShouldJump);
	HANDLE_READ(a_ByteBuffer, ReadBool,    bool,  ShouldUnmount);
	if (ShouldUnmount)
	{
		m_Client->HandleUnmount();
	}
	else
	{
		m_Client->HandleSteerVehicle(Forward, Sideways);
	}
}





void cProtocol172::HandlePacketTabComplete(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Text);
	m_Client->HandleTabCompletion(Text);
}





void cProtocol172::HandlePacketUpdateSign(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt,         int,     BlockX);
	HANDLE_READ(a_ByteBuffer, ReadBEShort,       short,   BlockY);
	HANDLE_READ(a_ByteBuffer, ReadBEInt,         int,     BlockZ);
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Line1);
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Line2);
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Line3);
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Line4);
	m_Client->HandleUpdateSign(BlockX, BlockY, BlockZ, Line1, Line2, Line3, Line4);
}





void cProtocol172::HandlePacketUseEntity(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt, int,  EntityID);
	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, MouseButton);
	m_Client->HandleUseEntity(EntityID, (MouseButton == 1));
}





void cProtocol172::HandlePacketWindowClick(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadChar,    char,  WindowID);
	HANDLE_READ(a_ByteBuffer, ReadBEShort, short, SlotNum);
	HANDLE_READ(a_ByteBuffer, ReadByte,    Byte,  Button);
	HANDLE_READ(a_ByteBuffer, ReadBEShort, short, TransactionID);
	HANDLE_READ(a_ByteBuffer, ReadByte,    Byte,  Mode);
	cItem Item;
	ReadItem(a_ByteBuffer, Item);

	// Convert Button, Mode, SlotNum and HeldItem into eClickAction:
	eClickAction Action;
	switch ((Mode << 8) | Button)
	{
		case 0x0000: Action = (SlotNum != -999) ? caLeftClick  : caLeftClickOutside;  break;
		case 0x0001: Action = (SlotNum != -999) ? caRightClick : caRightClickOutside; break;
		case 0x0100: Action = caShiftLeftClick;  break;
		case 0x0101: Action = caShiftRightClick; break;
		case 0x0200: Action = caNumber1;         break;
		case 0x0201: Action = caNumber2;         break;
		case 0x0202: Action = caNumber3;         break;
		case 0x0203: Action = caNumber4;         break;
		case 0x0204: Action = caNumber5;         break;
		case 0x0205: Action = caNumber6;         break;
		case 0x0206: Action = caNumber7;         break;
		case 0x0207: Action = caNumber8;         break;
		case 0x0208: Action = caNumber9;         break;
		case 0x0300: Action = caMiddleClick;     break;
		case 0x0400: Action = (SlotNum == -999) ? caLeftClickOutsideHoldNothing  : caDropKey;     break;
		case 0x0401: Action = (SlotNum == -999) ? caRightClickOutsideHoldNothing : caCtrlDropKey; break;
		case 0x0500: Action = (SlotNum == -999) ? caLeftPaintBegin               : caUnknown;     break;
		case 0x0501: Action = (SlotNum != -999) ? caLeftPaintProgress            : caUnknown;     break;
		case 0x0502: Action = (SlotNum == -999) ? caLeftPaintEnd                 : caUnknown;     break;
		case 0x0504: Action = (SlotNum == -999) ? caRightPaintBegin              : caUnknown;     break;
		case 0x0505: Action = (SlotNum != -999) ? caRightPaintProgress           : caUnknown;     break;
		case 0x0506: Action = (SlotNum == -999) ? caRightPaintEnd                : caUnknown;     break;
		case 0x0600: Action = caDblClick; break;
		default:
		{
			LOGWARNING("Unhandled window click mode / button combination: %d (0x%x)", (Mode << 8) | Button, (Mode << 8) | Button);
			Action = caUnknown;
			break;
		}
	}

	m_Client->HandleWindowClick(WindowID, SlotNum, Action, Item);
}





void cProtocol172::HandlePacketWindowClose(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadChar, char, WindowID);
	m_Client->HandleWindowClose(WindowID);
}





void cProtocol172::WritePacket(cByteBuffer & a_Packet)
{
	cCSLock Lock(m_CSPacket);
	AString Pkt;
	a_Packet.ReadAll(Pkt);
	WriteVarInt(Pkt.size());
	SendData(Pkt.data(), Pkt.size());
	Flush();
}





void cProtocol172::SendData(const char * a_Data, int a_Size)
{
	if (m_IsEncrypted)
	{
		byte Encrypted[8192];  // Larger buffer, we may be sending lots of data (chunks)
		while (a_Size > 0)
		{
			int NumBytes = (a_Size > sizeof(Encrypted)) ? sizeof(Encrypted) : a_Size;
			m_Encryptor.ProcessData(Encrypted, (byte *)a_Data, NumBytes);
			m_Client->SendData((const char *)Encrypted, NumBytes);
			a_Size -= NumBytes;
			a_Data += NumBytes;
		}
	}
	else
	{
		m_Client->SendData(a_Data, a_Size);
	}
}






bool cProtocol172::ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item)
{
	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEShort, short, ItemType);
	if (ItemType == -1)
	{
		// The item is empty, no more data follows
		a_Item.Empty();
		return true;
	}
	a_Item.m_ItemType = ItemType;
	
	HANDLE_PACKET_READ(a_ByteBuffer, ReadChar,    char,  ItemCount);
	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEShort, short, ItemDamage);
	a_Item.m_ItemCount  = ItemCount;
	a_Item.m_ItemDamage = ItemDamage;
	if (ItemCount <= 0)
	{
		a_Item.Empty();
	}

	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEShort, short, MetadataLength);
	if (MetadataLength <= 0)
	{
		return true;
	}
	
	// Read the metadata
	AString Metadata;
	if (!a_ByteBuffer.ReadString(Metadata, MetadataLength))
	{
		return false;
	}
	
	ParseItemMetadata(a_Item, Metadata);
	return true;
}





void cProtocol172::ParseItemMetadata(cItem & a_Item, const AString & a_Metadata)
{
	// Uncompress the GZIPped data:
	AString Uncompressed;
	if (UncompressStringGZIP(a_Metadata.data(), a_Metadata.size(), Uncompressed) != Z_OK)
	{
		AString HexDump;
		CreateHexDump(HexDump, a_Metadata.data(), a_Metadata.size(), 16);
		LOGWARNING("Cannot unGZIP item metadata (%u bytes):\n%s", a_Metadata.size(), HexDump.c_str());
		return;
	}
	
	// Parse into NBT:
	cParsedNBT NBT(Uncompressed.data(), Uncompressed.size());
	if (!NBT.IsValid())
	{
		AString HexDump;
		CreateHexDump(HexDump, Uncompressed.data(), Uncompressed.size(), 16);
		LOGWARNING("Cannot parse NBT item metadata: (%u bytes)\n%s", Uncompressed.size(), HexDump.c_str());
		return;
	}
	
	// Load enchantments from the NBT:
	for (int tag = NBT.GetFirstChild(NBT.GetRoot()); tag >= 0; tag = NBT.GetNextSibling(tag))
	{
		if (
			(NBT.GetType(tag) == TAG_List) &&
			(
				(NBT.GetName(tag) == "ench") ||
				(NBT.GetName(tag) == "StoredEnchantments")
			)
		)
		{
			a_Item.m_Enchantments.ParseFromNBT(NBT, tag);
		}
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProtocol172::cPacketizer:

cProtocol172::cPacketizer::~cPacketizer()
{
	AString DataToSend;

	// Send the packet length
	UInt32 PacketLen = m_Out.GetUsedSpace();
	m_Protocol.m_OutPacketLenBuffer.WriteVarInt(PacketLen);
	m_Protocol.m_OutPacketLenBuffer.ReadAll(DataToSend);
	m_Protocol.SendData(DataToSend.data(), DataToSend.size());
	m_Protocol.m_OutPacketLenBuffer.CommitRead();
	
	// Send the packet data:
	m_Out.ReadAll(DataToSend);
	m_Protocol.SendData(DataToSend.data(), DataToSend.size());
	m_Out.CommitRead();
}





void cProtocol172::cPacketizer::WriteItem(const cItem & a_Item)
{
	short ItemType = a_Item.m_ItemType;
	ASSERT(ItemType >= -1);  // Check validity of packets in debug runtime
	if (ItemType <= 0)
	{
		// Fix, to make sure no invalid values are sent.
		ItemType = -1;
	}
	
	if (a_Item.IsEmpty())
	{
		WriteShort(-1);
		return;
	}
	
	WriteShort(ItemType);
	WriteByte (a_Item.m_ItemCount);
	WriteShort(a_Item.m_ItemDamage);
	
	if (a_Item.m_Enchantments.IsEmpty())
	{
		WriteShort(-1);
		return;
	}

	// Send the enchantments:
	cFastNBTWriter Writer;
	const char * TagName = (a_Item.m_ItemType == E_ITEM_BOOK) ? "StoredEnchantments" : "ench";
	a_Item.m_Enchantments.WriteToNBTCompound(Writer, TagName);
	Writer.Finish();
	AString Compressed;
	CompressStringGZIP(Writer.GetResult().data(), Writer.GetResult().size(), Compressed);
	WriteShort(Compressed.size());
	WriteBuf(Compressed.data(), Compressed.size());
}





void cProtocol172::cPacketizer::WriteByteAngle(double a_Angle)
{
	WriteByte((char)(255 * a_Angle / 360));
}





void cProtocol172::cPacketizer::WriteFPInt(double a_Value)
{
	int Value = (int)(a_Value * 32);
	WriteInt(Value);
}





void cProtocol172::cPacketizer::WriteEntityMetadata(const cEntity & a_Entity)
{
	// Common metadata:
	Byte Flags = 0;
	if (a_Entity.IsOnFire())
	{
		Flags |= 0x01;
	}
	if (a_Entity.IsCrouched())
	{
		Flags |= 0x02;
	}
	if (a_Entity.IsSprinting())
	{
		Flags |= 0x08;
	}
	if (a_Entity.IsRclking())
	{
		Flags |= 0x10;
	}
	if (a_Entity.IsInvisible())
	{
		Flags |= 0x20;
	}
	WriteByte(0);  // Byte(0) + index 0
	WriteByte(Flags);
	
	switch (a_Entity.GetEntityType())
	{
		case cEntity::etPlayer: break;  // TODO?
		case cEntity::etPickup:
		{
			WriteByte((5 << 5) | 10);  // Slot(5) + index 10
			WriteItem(((const cPickup &)a_Entity).GetItem());
			break;
		}
		case cEntity::etMinecart:
		{
			WriteByte(0x51);

			// The following expression makes Minecarts shake more with less health or higher damage taken
			// It gets half the maximum health, and takes it away from the current health minus the half health:
			/* Health: 5 | 3 - (5 - 3) = 1 (shake power)
			   Health: 3 | 3 - (3 - 3) = 3
			   Health: 1 | 3 - (1 - 3) = 5
			*/
			WriteInt((((a_Entity.GetMaxHealth() / 2) - (a_Entity.GetHealth() - (a_Entity.GetMaxHealth() / 2))) * ((const cMinecart &)a_Entity).LastDamage()) * 4);
			WriteByte(0x52);
			WriteInt(1); // Shaking direction, doesn't seem to affect anything
			WriteByte(0x73);
			WriteFloat((float)(((const cMinecart &)a_Entity).LastDamage() + 10)); // Damage taken / shake effect multiplyer

			if (((cMinecart &)a_Entity).GetPayload() == cMinecart::mpFurnace)
			{
				WriteByte(0x10);
				WriteByte(((const cMinecartWithFurnace &)a_Entity).IsFueled() ? 1 : 0);
			}
			break;
		}
		case cEntity::etProjectile:
		{
			if (((cProjectileEntity &)a_Entity).GetProjectileKind() == cProjectileEntity::pkArrow)
			{
				WriteByte(0x10);
				WriteByte(((const cArrowEntity &)a_Entity).IsCritical() ? 1 : 0);
			}
			break;
		}
		case cEntity::etMonster:
		{
			WriteMobMetadata((const cMonster &)a_Entity);
			break;
		}
	}
}





void cProtocol172::cPacketizer::WriteMobMetadata(const cMonster & a_Mob)
{
	switch (a_Mob.GetMobType())
	{
		case cMonster::mtCreeper:
		{
			WriteByte(0x10);
			WriteByte(((const cCreeper &)a_Mob).IsBlowing() ? 1 : -1);
			WriteByte(0x11);
			WriteByte(((const cCreeper &)a_Mob).IsCharged() ? 1 : 0);
			break;
		}
		
		case cMonster::mtBat:
		{
			WriteByte(0x10);
			WriteByte(((const cBat &)a_Mob).IsHanging() ? 1 : 0);
			break;
		}
		
		case cMonster::mtPig:
		{
			WriteByte(0x10);
			WriteByte(((const cPig &)a_Mob).IsSaddled() ? 1 : 0);
			break;
		}
		
		case cMonster::mtVillager:
		{
			WriteByte(0x50);
			WriteInt(((const cVillager &)a_Mob).GetVilType());
			break;
		}
		
		case cMonster::mtZombie:
		{
			WriteByte(0x0c);
			WriteByte(((const cZombie &)a_Mob).IsBaby() ? 1 : 0);
			WriteByte(0x0d);
			WriteByte(((const cZombie &)a_Mob).IsVillagerZombie() ? 1 : 0);
			WriteByte(0x0e);
			WriteByte(((const cZombie &)a_Mob).IsConverting() ? 1 : 0);
			break;
		}
		
		case cMonster::mtGhast:
		{
			WriteByte(0x10);
			WriteByte(((const cGhast &)a_Mob).IsCharging());
			break;
		}
		
		case cMonster::mtWolf:
		{
			const cWolf & Wolf = (const cWolf &)a_Mob;
			Byte WolfStatus = 0;
			if (Wolf.IsSitting())
			{
				WolfStatus |= 0x1;
			}
			if (Wolf.IsAngry())
			{
				WolfStatus |= 0x2;
			}
			if (Wolf.IsTame())
			{
				WolfStatus |= 0x4;
			}
			WriteByte(0x10);
			WriteByte(WolfStatus);

			WriteByte(0x72);
			WriteFloat((float)(a_Mob.GetHealth()));
			WriteByte(0x13);
			WriteByte(Wolf.IsBegging() ? 1 : 0);
			WriteByte(0x14);
			WriteByte(Wolf.GetCollarColor());
			break;
		}
		
		case cMonster::mtSheep:
		{
			WriteByte(0x10);
			Byte SheepMetadata = 0;
			SheepMetadata = ((const cSheep &)a_Mob).GetFurColor();
			if (((const cSheep &)a_Mob).IsSheared())
			{
				SheepMetadata |= 0x10;
			}
			WriteByte(SheepMetadata);
			break;
		}
		
		case cMonster::mtEnderman:
		{
			WriteByte(0x10);
			WriteByte((Byte)(((const cEnderman &)a_Mob).GetCarriedBlock()));
			WriteByte(0x11);
			WriteByte((Byte)(((const cEnderman &)a_Mob).GetCarriedMeta()));
			WriteByte(0x12);
			WriteByte(((const cEnderman &)a_Mob).IsScreaming() ? 1 : 0);
			break;
		}
		
		case cMonster::mtSkeleton:
		{
			WriteByte(0x0d);
			WriteByte(((const cSkeleton &)a_Mob).IsWither() ? 1 : 0);
			break;
		}
		
		case cMonster::mtWitch:
		{
			WriteByte(0x15);
			WriteByte(((const cWitch &)a_Mob).IsAngry() ? 1 : 0);
			break;
		}
		
		case cMonster::mtSlime:
		{
			WriteByte(0x10);
			WriteByte(((const cSlime &)a_Mob).GetSize());
			break;
		}
		
		case cMonster::mtMagmaCube:
		{
			WriteByte(0x10);
			WriteByte(((const cMagmaCube &)a_Mob).GetSize());
			break;
		}
		
		case cMonster::mtHorse:
		{
			const cHorse & Horse = (const cHorse &)a_Mob;
			int Flags = 0;
			if (Horse.IsTame())
			{
				Flags |= 0x02;
			}
			if (Horse.IsSaddled())
			{
				Flags |= 0x04;
			}
			if (Horse.IsChested())
			{
				Flags |= 0x08;
			}
			if (Horse.IsBaby())
			{
				Flags |= 0x10;
			}
			if (Horse.IsEating())
			{
				Flags |= 0x20;
			}
			if (Horse.IsRearing())
			{
				Flags |= 0x40;
			}
			if (Horse.IsMthOpen())
			{
				Flags |= 0x80;
			}
			WriteByte(0x50);  // Int at index 16
			WriteInt(Flags);
			WriteByte(0x13);  // Byte at index 19
			WriteByte(Horse.GetHorseType());
			WriteByte(0x54);  // Int at index 20
			int Appearance = 0;
			Appearance = Horse.GetHorseColor();
			Appearance |= Horse.GetHorseStyle() << 8;
			WriteInt(Appearance);
			WriteByte(0x56);  // Int at index 22
			WriteInt(Horse.GetHorseArmour());
			break;
		}
	}  // switch (a_Mob.GetType())
}





void cProtocol172::cPacketizer::WriteEntityProperties(const cEntity & a_Entity)
{
	if (!a_Entity.IsMob())
	{
		// No properties for anything else than mobs
		WriteInt(0);
		return;
	}
	const cMonster & Mob = (const cMonster &)a_Entity;

	// TODO: Send properties and modifiers based on the mob type
	
	WriteInt(0);  // NumProperties
}




