
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
#include "../Entities/FallingBlock.h"
#include "../Entities/Pickup.h"
#include "../Entities/Player.h"
#include "../UI/Window.h"





#define HANDLE_READ(Proc, Type, Var) \
	Type Var; \
	m_ReceivedData.Proc(Var);





#define HANDLE_PACKET_READ(Proc, Type, Var) \
	Type Var; \
	{ \
		if (!m_ReceivedData.Proc(Var)) \
		{ \
			m_ReceivedData.CheckValid(); \
			return false; \
		} \
		m_ReceivedData.CheckValid(); \
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
	Pkt.WriteString(a_Reason);
}





void cProtocol172::SendEditSign(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cPacketizer Pkt(*this, 0x36);  // Sign Editor Open packet
	Pkt.WriteInt(a_BlockX);
	Pkt.WriteInt(a_BlockY);
	Pkt.WriteInt(a_BlockZ);
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
	/*
	cPacketizer Pkt(*this, 0x20);  // Entity Properties packet
	Pkt.WriteInt(a_Entity.GetUniqueID());
	// TODO
	*/
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
		Pkt.WriteByte((Byte)a_Player.GetEffectiveGameMode());
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
		Pkt.WriteInt(a_Pickup.GetUniqueID());
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
	}
	// TODO: Other flags (god mode, flying, can fly
	Pkt.WriteByte(Flags);
	// TODO: Pkt.WriteFloat(m_Client->GetPlayer()->GetMaxFlyingSpeed());
	Pkt.WriteFloat(0.05f);
	Pkt.WriteFloat((float)m_Client->GetPlayer()->GetMaxSpeed());
}





void cProtocol172::SendPlayerAnimation(const cPlayer & a_Player, char a_Animation)
{
	cPacketizer Pkt(*this, 0x0b);  // Animation packet
	Pkt.WriteInt(a_Player.GetUniqueID());
	Pkt.WriteChar(a_Animation);
}





void cProtocol172::SendPlayerListItem(const cPlayer & a_Player, bool a_IsOnline)
{
	cPacketizer Pkt(*this, 0x38);  // Playerlist Item packet
	Pkt.WriteString(a_Player.GetName());
	Pkt.WriteBool(a_IsOnline);
	Pkt.WriteShort(a_Player.GetClientHandle()->GetPing());
}





void cProtocol172::SendPlayerMaxSpeed(void)
{
	SendPlayerAbilities();
}





void cProtocol172::SendPlayerMoveLook(void)
{
	cPacketizer Pkt(*this, 0x08);  // Player Position And Look packet
	Pkt.WriteDouble(m_Client->GetPlayer()->GetPosX());
	Pkt.WriteDouble(m_Client->GetPlayer()->GetPosY());
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
	Pkt.WriteInt(a_Player.GetUniqueID());
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





void cProtocol172::SendRespawn(void)
{
	cPacketizer Pkt(*this, 0x07);  // Respawn packet
	Pkt.WriteInt(m_Client->GetPlayer()->GetWorld()->GetDimension());
	Pkt.WriteByte(2);  // TODO: Difficulty (set to Normal)
	Pkt.WriteByte((Byte)m_Client->GetPlayer()->GetEffectiveGameMode());
	Pkt.WriteString("default");
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
	// TODO: Check if this is really an int
	// wiki.vg says it's a byte, but that wouldn't cover the entire range needed (Y location * 8 = 0..2048)
	Pkt.WriteInt(a_SrcY);
	Pkt.WriteInt(a_SrcZ);
	Pkt.WriteInt(a_Data);
	Pkt.WriteBool(false);
}





void cProtocol172::SendSpawnFallingBlock(const cFallingBlock & a_FallingBlock)
{
	cPacketizer Pkt(*this, 0x0e);  // Spawn Object packet
	Pkt.WriteInt(a_FallingBlock.GetUniqueID());
	Pkt.WriteByte(70);  // Falling block
	Pkt.WriteFPInt(a_FallingBlock.GetPosX());
	Pkt.WriteFPInt(a_FallingBlock.GetPosY());
	Pkt.WriteFPInt(a_FallingBlock.GetPosZ());
	Pkt.WriteByteAngle(a_FallingBlock.GetYaw());
	Pkt.WriteByteAngle(a_FallingBlock.GetPitch());
	Pkt.WriteInt(((int)a_FallingBlock.GetBlockType()) | (((int)a_FallingBlock.GetBlockMeta()) << 12));
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
	Pkt.WriteInt(a_Vehicle.GetUniqueID());
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
	AString Results;
	Results.reserve(500);  // Make a moderate reservation to avoid excessive reallocations
	for (AStringVector::const_iterator itr = a_Results.begin(), end = a_Results.end(); itr != end; ++itr)
	{
		Results.append(*itr);
		Results.push_back(0);
	}
	
	cPacketizer Pkt(*this, 0x3a);  // Tab-Complete packet
	Pkt.WriteVarInt(a_Results.size());
	Pkt.WriteString(Results);
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
	cPacketizer Pkt(*this, 0x2b);  // Change Game State packet
	Pkt.WriteByte((a_Weather == wSunny) ? 2 : 1);  // begin rain / end rain
	Pkt.WriteFloat(0);  // unused
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





void cProtocol172::SendWindowOpen(char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots)
{
	cPacketizer Pkt(*this, 0x2d);
	Pkt.WriteChar(a_WindowID);
	Pkt.WriteChar(a_WindowType);
	Pkt.WriteString(a_WindowTitle);
	Pkt.WriteChar(a_NumSlots);
	Pkt.WriteBool(true);
	/*
	// TODO:
	if (a_WindowType == cWindow::wtHorse)
	{
		Pkt.WriteInt(HorseID);
	}
	*/
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
	while (true)
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
		UInt32 PacketType;
		UInt32 Mark1 = m_ReceivedData.GetReadableSpace();
		if (!m_ReceivedData.ReadVarInt(PacketType))
		{
			// Not enough data
			return;
		}

		UInt32 NumBytesRead = Mark1 - m_ReceivedData.GetReadableSpace();
		HandlePacket(PacketType, PacketLen - NumBytesRead);
		
		if (Mark1 - m_ReceivedData.GetReadableSpace() > PacketLen)
		{
			// Read more than packet length, report as error
			m_Client->PacketError(PacketType);
		}

		// Go to packet end in any case:
		m_ReceivedData.ResetRead();
		m_ReceivedData.ReadVarInt(PacketType);
		m_ReceivedData.SkipRead(PacketLen);
		m_ReceivedData.CommitRead();
	}  // while (true)
}




void cProtocol172::HandlePacket(UInt32 a_PacketType, UInt32 a_RemainingBytes)
{
	switch (m_State)
	{
		case 1:
		{
			// Status
			switch (a_PacketType)
			{
				case 0x00: HandlePacketStatusRequest(a_RemainingBytes); return;
				case 0x01: HandlePacketStatusPing   (a_RemainingBytes); return;
			}
			break;
		}
		
		case 2:
		{
			// Login
			switch (a_PacketType)
			{
				case 0x00: HandlePacketLoginStart(a_RemainingBytes); return;
				case 0x01: HandlePacketLoginEncryptionResponse(a_RemainingBytes); return;
			}
			break;
		}
		
		case 3:
		{
			// Game
			switch (a_PacketType)
			{
				case 0x00: HandlePacketKeepAlive              (a_RemainingBytes); return;
				case 0x01: HandlePacketChatMessage            (a_RemainingBytes); return;
				case 0x02: HandlePacketUseEntity              (a_RemainingBytes); return;
				case 0x03: HandlePacketPlayer                 (a_RemainingBytes); return;
				case 0x04: HandlePacketPlayerPos              (a_RemainingBytes); return;
				case 0x05: HandlePacketPlayerLook             (a_RemainingBytes); return;
				case 0x06: HandlePacketPlayerPosLook          (a_RemainingBytes); return;
				case 0x07: HandlePacketBlockDig               (a_RemainingBytes); return;
				case 0x08: HandlePacketBlockPlace             (a_RemainingBytes); return;
				case 0x09: HandlePacketSlotSelect             (a_RemainingBytes); return;
				case 0x0a: HandlePacketAnimation              (a_RemainingBytes); return;
				case 0x0b: HandlePacketEntityAction           (a_RemainingBytes); return;
				case 0x0c: HandlePacketSteerVehicle           (a_RemainingBytes); return;
				case 0x0d: HandlePacketWindowClose            (a_RemainingBytes); return;
				case 0x0e: HandlePacketWindowClick            (a_RemainingBytes); return;
				case 0x0f: // Confirm transaction - not used in MCS
				case 0x10: HandlePacketCreativeInventoryAction(a_RemainingBytes); return;
				case 0x12: HandlePacketUpdateSign             (a_RemainingBytes); return;
				case 0x13: HandlePacketPlayerAbilities        (a_RemainingBytes); return;
				case 0x14: HandlePacketTabComplete            (a_RemainingBytes); return;
				case 0x15: HandlePacketClientSettings         (a_RemainingBytes); return;
				case 0x16: HandlePacketClientStatus           (a_RemainingBytes); return;
				case 0x17: HandlePacketPluginMessage          (a_RemainingBytes); return;
			}
			break;
		}
	}  // switch (m_State)
	
	// Unknown packet type, report to the client:
	m_Client->PacketUnknown(a_PacketType);
	m_ReceivedData.SkipRead(a_RemainingBytes);
	m_ReceivedData.CommitRead();
}





void cProtocol172::HandlePacketStatusPing(UInt32 a_RemainingBytes)
{
	ASSERT(a_RemainingBytes == 8);
	if (a_RemainingBytes != 8)
	{
		m_Client->PacketError(0x01);
		m_ReceivedData.SkipRead(a_RemainingBytes);
		m_ReceivedData.CommitRead();
		return;
	}
	Int64 Timestamp;
	m_ReceivedData.ReadBEInt64(Timestamp);
	m_ReceivedData.CommitRead();
	
	cPacketizer Pkt(*this, 0x01);  // Ping packet
	Pkt.WriteInt64(Timestamp);
}





void cProtocol172::HandlePacketStatusRequest(UInt32 a_RemainingBytes)
{
	// No more bytes in this packet
	ASSERT(a_RemainingBytes == 0);
	m_ReceivedData.CommitRead();
	
	// Send the response:
	AString Response = "{\"version\":{\"name\":\"1.7.2\",\"protocol\":4},\"players\":{";
	AppendPrintf(Response, "\"max\":%u,\"online\":%u,\"sample\":[]},",
		cRoot::Get()->GetServer()->GetMaxPlayers(),
		cRoot::Get()->GetServer()->GetNumPlayers()
	);
	AppendPrintf(Response, "\"description\":{\"text\":\"%s\"}",
		cRoot::Get()->GetServer()->GetDescription().c_str()
	);
	Response.append("}");
	
	cPacketizer Pkt(*this, 0x00);  // Response packet
	Pkt.WriteString(Response);
}





void cProtocol172::HandlePacketLoginEncryptionResponse(UInt32 a_RemainingBytes)
{
	// TODO: Add protocol encryption
}





void cProtocol172::HandlePacketLoginStart(UInt32 a_RemainingBytes)
{
	AString Username;
	m_ReceivedData.ReadVarUTF8String(Username);
	
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





void cProtocol172::HandlePacketAnimation(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadBEInt, int,  EntityID);
	HANDLE_READ(ReadByte,  Byte, Animation);
	m_Client->HandleAnimation(Animation);
}





void cProtocol172::HandlePacketBlockDig(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadByte,  Byte, Status);
	HANDLE_READ(ReadBEInt, int,  BlockX);
	HANDLE_READ(ReadByte,  Byte, BlockY);
	HANDLE_READ(ReadBEInt, int,  BlockZ);
	HANDLE_READ(ReadByte,  Byte, Face);
	m_Client->HandleLeftClick(BlockX, BlockY, BlockZ, Face, Status);
}





void cProtocol172::HandlePacketBlockPlace(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadBEInt, int,  BlockX);
	HANDLE_READ(ReadByte,  Byte, BlockY);
	HANDLE_READ(ReadBEInt, int,  BlockZ);
	HANDLE_READ(ReadByte,  Byte, Face);
	HANDLE_READ(ReadByte,  Byte, CursorX);
	HANDLE_READ(ReadByte,  Byte, CursorY);
	HANDLE_READ(ReadByte,  Byte, CursorZ);
	m_Client->HandleRightClick(BlockX, BlockY, BlockZ, Face, CursorX, CursorY, CursorZ, m_Client->GetPlayer()->GetEquippedItem());
}





void cProtocol172::HandlePacketChatMessage(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadVarUTF8String, AString, Message);
	m_Client->HandleChat(Message);
}





void cProtocol172::HandlePacketClientSettings(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadVarUTF8String, AString, Locale);
	HANDLE_READ(ReadByte,          Byte,    ViewDistance);
	HANDLE_READ(ReadByte,          Byte,    ChatFlags);
	HANDLE_READ(ReadByte,          Byte,    Unused);
	HANDLE_READ(ReadByte,          Byte,    Difficulty);
	HANDLE_READ(ReadByte,          Byte,    ShowCape);
	// TODO: handle in m_Client
}





void cProtocol172::HandlePacketClientStatus(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadByte, Byte, ActionID);
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





void cProtocol172::HandlePacketCreativeInventoryAction(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadBEShort, short, SlotNum);
	cItem Item;
	if (!ReadItem(Item))
	{
		return;
	}
	m_Client->HandleCreativeInventory(SlotNum, Item);
}





void cProtocol172::HandlePacketEntityAction(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadBEInt, int,  PlayerID);
	HANDLE_READ(ReadByte,  Byte, Action);
	HANDLE_READ(ReadBEInt, int,  JumpBoost);
	m_Client->HandleEntityAction(PlayerID, Action);
}





void cProtocol172::HandlePacketKeepAlive(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadBEInt, int, KeepAliveID);
	m_Client->HandleKeepAlive(KeepAliveID);
}





void cProtocol172::HandlePacketPlayer(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadBool, bool, IsOnGround);
	// TODO: m_Client->HandlePlayerOnGround(IsOnGround);
}





void cProtocol172::HandlePacketPlayerAbilities(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadByte,    Byte,  Flags);
	HANDLE_READ(ReadBEFloat, float, FlyingSpeed);
	HANDLE_READ(ReadBEFloat, float, WalkingSpeed);
	// TODO: m_Client->HandlePlayerAbilities();
}





void cProtocol172::HandlePacketPlayerLook(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadBEFloat, float, Yaw);
	HANDLE_READ(ReadBEFloat, float, Pitch);
	HANDLE_READ(ReadBool,    bool,  IsOnGround);
	m_Client->HandlePlayerLook(Yaw, Pitch, IsOnGround);
}





void cProtocol172::HandlePacketPlayerPos(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadBEDouble, double, PosX);
	HANDLE_READ(ReadBEDouble, double, PosY);
	HANDLE_READ(ReadBEDouble, double, Stance);
	HANDLE_READ(ReadBEDouble, double, PosZ);
	HANDLE_READ(ReadBool,     bool,   IsOnGround);
	m_Client->HandlePlayerPos(PosX, PosY, PosZ, Stance, IsOnGround);
}





void cProtocol172::HandlePacketPlayerPosLook(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadBEDouble, double, PosX);
	HANDLE_READ(ReadBEDouble, double, PosY);
	HANDLE_READ(ReadBEDouble, double, Stance);
	HANDLE_READ(ReadBEDouble, double, PosZ);
	HANDLE_READ(ReadBEFloat,  float,  Yaw);
	HANDLE_READ(ReadBEFloat,  float,  Pitch);
	HANDLE_READ(ReadBool,     bool,   IsOnGround);
	m_Client->HandlePlayerMoveLook(PosX, PosY, PosZ, Stance, Yaw, Pitch, IsOnGround);
}





void cProtocol172::HandlePacketPluginMessage(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadVarUTF8String, AString, Channel);
	HANDLE_READ(ReadBEShort,       short,   Length);
	AString Data;
	m_ReceivedData.ReadString(Data, Length);
	// TODO: m_Client->HandlePluginMessage(Channel, Data);
}





void cProtocol172::HandlePacketSlotSelect(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadBEShort, short, SlotNum);
	m_Client->HandleSlotSelected(SlotNum);
}





void cProtocol172::HandlePacketSteerVehicle(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadBEFloat, float, Forward);
	HANDLE_READ(ReadBEFloat, float, Sideways);
	HANDLE_READ(ReadBool,    bool,  ShouldJump);
	HANDLE_READ(ReadBool,    bool,  ShouldUnmount);
	if (ShouldUnmount)
	{
		m_Client->HandleUnmount();
	}
	else
	{
		m_Client->HandleSteerVehicle(Forward, Sideways);
	}
}





void cProtocol172::HandlePacketTabComplete(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadVarUTF8String, AString, Text);
	m_Client->HandleTabCompletion(Text);
}





void cProtocol172::HandlePacketUpdateSign(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadBEInt,         int,     BlockX);
	HANDLE_READ(ReadBEShort,       short,   BlockY);
	HANDLE_READ(ReadBEInt,         int,     BlockZ);
	HANDLE_READ(ReadVarUTF8String, AString, Line1);
	HANDLE_READ(ReadVarUTF8String, AString, Line2);
	HANDLE_READ(ReadVarUTF8String, AString, Line3);
	HANDLE_READ(ReadVarUTF8String, AString, Line4);
	m_Client->HandleUpdateSign(BlockX, BlockY, BlockZ, Line1, Line2, Line3, Line4);
}





void cProtocol172::HandlePacketUseEntity(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadBEInt, int,  EntityID);
	HANDLE_READ(ReadByte,  Byte, MouseButton);
	// TODO: Verify that this works, wiki.vg has no info on the MouseButton values
	m_Client->HandleUseEntity(EntityID, (MouseButton == 0));
}





void cProtocol172::HandlePacketWindowClick(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadChar,    char,  WindowID);
	HANDLE_READ(ReadBEShort, short, SlotNum);
	HANDLE_READ(ReadByte,    Byte,  Button);
	HANDLE_READ(ReadBEShort, short, TransactionID);
	HANDLE_READ(ReadByte,    Byte,  Mode);
	cItem Item;
	ReadItem(Item);

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
	}

	m_Client->HandleWindowClick(WindowID, SlotNum, Action, Item);
}





void cProtocol172::HandlePacketWindowClose(UInt32 a_RemainingBytes)
{
	HANDLE_READ(ReadChar, char, WindowID);
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






bool cProtocol172::ReadItem(cItem & a_Item)
{
	HANDLE_PACKET_READ(ReadBEShort, short, ItemType);
	if (ItemType == -1)
	{
		// The item is empty, no more data follows
		a_Item.Empty();
		return true;
	}
	a_Item.m_ItemType = ItemType;
	
	HANDLE_PACKET_READ(ReadChar,    char,  ItemCount);
	HANDLE_PACKET_READ(ReadBEShort, short, ItemDamage);
	a_Item.m_ItemCount  = ItemCount;
	a_Item.m_ItemDamage = ItemDamage;
	if (ItemCount <= 0)
	{
		a_Item.Empty();
	}

	HANDLE_PACKET_READ(ReadBEShort, short, MetadataLength);
	if (MetadataLength <= 0)
	{
		return true;
	}
	
	// Read the metadata
	AString Metadata;
	if (!m_ReceivedData.ReadString(Metadata, MetadataLength))
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
		case cEntity::etMonster:
		{
			WriteMobMetadata((const cMonster &)a_Entity);
			break;
		}
		// TODO: Other types
	}
}





void cProtocol172::cPacketizer::WriteMobMetadata(const cMonster & a_Mob)
{
	// TODO
}




