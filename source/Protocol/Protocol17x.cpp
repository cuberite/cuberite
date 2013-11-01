
// Protocol17x.cpp

/*
Implements the 1.7.x protocol classes:
	- cProtocol172
		- release 1.7.2 protocol (#4)
(others may be added later in the future for the 1.7 release series)
*/

#include "Globals.h"
#include "Protocol17x.h"
#include "../ClientHandle.h"
#include "../Root.h"
#include "../Server.h"
#include "../Entities/Player.h"
#include "../World.h"
#include "ChunkDataSerializer.h"
#include "../Entities/Pickup.h"





#define HANDLE_PACKET_READ(Proc, Type, Var) \
	Type Var; \
	m_ReceivedData.Proc(Var);





cProtocol172::cProtocol172(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State) :
	super(a_Client),
	m_ServerAddress(a_ServerAddress),
	m_ServerPort(a_ServerPort),
	m_State(a_State),
	m_ReceivedData(32 KiB),
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
	cByteBuffer Packet(20);
	Packet.WriteVarInt(0x1b);  // Attach Entity packet
	Packet.WriteBEInt(a_Entity.GetUniqueID());
	Packet.WriteBEInt((a_Vehicle != NULL) ? a_Vehicle->GetUniqueID() : 0);
	Packet.WriteBool(false);
	WritePacket(Packet);
}





void cProtocol172::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType)
{
	cByteBuffer Packet(30);
	Packet.WriteVarInt(0x24);  // Block Action packet
	Packet.WriteBEInt(a_BlockX);
	Packet.WriteBEShort(a_BlockY);
	Packet.WriteBEInt(a_BlockZ);
	Packet.WriteChar(a_Byte1);
	Packet.WriteChar(a_Byte2);
	Packet.WriteVarInt(a_BlockType);
	WritePacket(Packet);
}





void cProtocol172::SendBlockBreakAnim	(int a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage)
{
	cByteBuffer Packet(20);
	Packet.WriteVarInt(0x24);  // Block Action packet
	Packet.WriteBEInt(a_EntityID);
	Packet.WriteBEInt(a_BlockX);
	Packet.WriteBEInt(a_BlockY);
	Packet.WriteBEInt(a_BlockZ);
	Packet.WriteChar(a_Stage);
	WritePacket(Packet);
}





void cProtocol172::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cByteBuffer Packet(20);
	Packet.WriteVarInt(0x23);  // Block Change packet
	Packet.WriteBEInt(a_BlockX);
	Packet.WriteByte(a_BlockY);
	Packet.WriteBEInt(a_BlockZ);
	Packet.WriteVarInt(a_BlockType);
	Packet.WriteByte(a_BlockMeta);
	WritePacket(Packet);
}





void cProtocol172::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	cByteBuffer Packet(20 + a_Changes.size() * 4);
	Packet.WriteVarInt(0x22);  // Multi Block Change packet
	Packet.WriteBEInt(a_ChunkX);
	Packet.WriteBEInt(a_ChunkZ);
	Packet.WriteBEShort((short)a_Changes.size());
	Packet.WriteBEInt(a_Changes.size() * 4);
	for (sSetBlockVector::const_iterator itr = a_Changes.begin(), end = a_Changes.end(); itr != end; ++itr)
	{
		unsigned int Coords = itr->y | (itr->z << 8) | (itr->x << 12);
		unsigned int Blocks = itr->BlockMeta | (itr->BlockType << 4);
		Packet.WriteBEInt((Coords << 16) | Blocks);
	}  // for itr - a_Changes[]
	WritePacket(Packet);
}





void cProtocol172::SendChat(const AString & a_Message)
{
	AString Message = Printf("{\"text\":\"%s\"}", EscapeString(a_Message).c_str());
	cByteBuffer Packet(20 + Message.size());
	Packet.WriteVarInt(0x02);  // Chat Message packet
	Packet.WriteVarUTF8String(Message);
	WritePacket(Packet);
}





void cProtocol172::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	const AString & ChunkData = a_Serializer.Serialize(cChunkDataSerializer::RELEASE_1_3_2);  // This contains the flags and bitmasks, too
	cByteBuffer Packet(40 + ChunkData.size());
	Packet.WriteVarInt(0x21);  // Chunk Data packet
	Packet.WriteBEInt(a_ChunkX);
	Packet.WriteBEInt(a_ChunkZ);
	Packet.Write(ChunkData.data(), ChunkData.size());
	WritePacket(Packet);
}





void cProtocol172::SendCollectPickup(const cPickup & a_Pickup, const cPlayer & a_Player)
{
	cByteBuffer Packet(9);
	Packet.WriteVarInt(0x0d);  // Collect Item packet
	Packet.WriteBEInt(a_Pickup.GetUniqueID());
	Packet.WriteBEInt(a_Player.GetUniqueID());
	WritePacket(Packet);
}





void cProtocol172::SendDestroyEntity(const cEntity & a_Entity)
{
	cByteBuffer Packet(6);
	Packet.WriteVarInt(0x13);  // Destroy Entities packet
	Packet.WriteByte(0);
	Packet.WriteBEInt(a_Entity.GetUniqueID());
	WritePacket(Packet);
}





void cProtocol172::SendDisconnect(const AString & a_Reason)
{
	cByteBuffer Packet(10 + a_Reason.size());
	Packet.WriteVarInt(0x40);  // Disconnect packet
	Packet.WriteVarUTF8String(a_Reason);
	WritePacket(Packet);
}





void cProtocol172::SendEditSign(int a_BlockX, int a_BlockY, int a_BlockZ)  ///< Request the client to open up the sign editor for the sign(1.6+)
{
	// TODO
}





void cProtocol172::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	// TODO
}





void cProtocol172::SendEntityHeadLook(const cEntity & a_Entity)
{
	// TODO
}





void cProtocol172::SendEntityLook(const cEntity & a_Entity)
{
	// TODO
}





void cProtocol172::SendEntityMetadata(const cEntity & a_Entity)
{
	// TODO
}





void cProtocol172::SendEntityProperties(const cEntity & a_Entity)
{
	// TODO
}





void cProtocol172::SendEntityRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	// TODO
}





void cProtocol172::SendEntityRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	// TODO
}





void cProtocol172::SendEntityStatus(const cEntity & a_Entity, char a_Status)
{
	// TODO
}





void cProtocol172::SendEntityVelocity(const cEntity & a_Entity)
{
	// TODO
}





void cProtocol172::SendExplosion(double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion)
{
	// TODO
}





void cProtocol172::SendGameMode(eGameMode a_GameMode)
{
	// TODO
}





void cProtocol172::SendHealth(void)
{
	// TODO
}





void cProtocol172::SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	// TODO
}





void cProtocol172::SendKeepAlive(int a_PingID)
{
	// TODO
}





void cProtocol172::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	// Send the spawn position:
	cByteBuffer Packet(20);
	Packet.WriteVarInt(0x05);  // Spawn Position packet
	Packet.WriteBEInt((int)a_World.GetSpawnX());
	Packet.WriteBEInt((int)a_World.GetSpawnY());
	Packet.WriteBEInt((int)a_World.GetSpawnZ());
	WritePacket(Packet);
	
	// Send player abilities:
	SendPlayerAbilities();
}





void cProtocol172::SendPickupSpawn(const cPickup & a_Pickup)
{
	// TODO
}





void cProtocol172::SendPlayerAbilities(void)
{
	cByteBuffer Packet(20);
	Packet.WriteVarInt(0x39);  // Player Abilities packet
	Byte Flags = 0;
	if (m_Client->GetPlayer()->IsGameModeCreative())
	{
		Flags |= 0x01;
	}
	// TODO: Other flags (god mode, flying, can fly
	Packet.WriteByte(Flags);
	// TODO: Packet.WriteBEFloat(m_Client->GetPlayer()->GetMaxFlyingSpeed());
	Packet.WriteBEFloat(0.05f);
	Packet.WriteBEFloat((float)m_Client->GetPlayer()->GetMaxSpeed());
	WritePacket(Packet);
}





void cProtocol172::SendPlayerAnimation(const cPlayer & a_Player, char a_Animation)
{
	// TODO
}





void cProtocol172::SendPlayerListItem(const cPlayer & a_Player, bool a_IsOnline)
{
	cByteBuffer Packet(10 + a_Player.GetName().size());
	Packet.WriteVarInt(0x38);  // Playerlist Item packet
	Packet.WriteVarUTF8String(a_Player.GetName());
	Packet.WriteBool(a_IsOnline);
	Packet.WriteBEShort(a_Player.GetClientHandle()->GetPing());
	WritePacket(Packet);
}





void cProtocol172::SendPlayerMaxSpeed(void)
{
	SendPlayerAbilities();
}





void cProtocol172::SendPlayerMoveLook(void)
{
	// TODO
}





void cProtocol172::SendPlayerPosition(void)
{
	// TODO
}





void cProtocol172::SendPlayerSpawn(const cPlayer & a_Player)
{
	// TODO
}





void cProtocol172::SendRespawn(void)
{
	// TODO
}





void cProtocol172::SendSoundEffect(const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch)  // a_Src coords are Block * 8
{
	// TODO
}





void cProtocol172::SendSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
	// TODO
}





void cProtocol172::SendSpawnFallingBlock(const cFallingBlock & a_FallingBlock)
{
	// TODO
}





void cProtocol172::SendSpawnMob(const cMonster & a_Mob)
{
	// TODO
}





void cProtocol172::SendSpawnObject(const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch)
{
	// TODO
}





void cProtocol172::SendSpawnVehicle(const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType)
{
	// TODO
}





void cProtocol172::SendTabCompletionResults(const AStringVector & a_Results)
{
	// TODO
}





void cProtocol172::SendTeleportEntity(const cEntity & a_Entity)
{
	// TODO
}





void cProtocol172::SendThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// TODO
}





void cProtocol172::SendTimeUpdate(Int64 a_WorldAge, Int64 a_TimeOfDay)
{
	// TODO
}





void cProtocol172::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	// TODO
}





void cProtocol172::SendUpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	// TODO
}





void cProtocol172::SendUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ )
{
	// TODO
}





void cProtocol172::SendWeather(eWeather a_Weather)
{
	// TODO
}





void cProtocol172::SendWholeInventory(const cInventory & a_Inventory)
{
	// TODO
}





void cProtocol172::SendWholeInventory(const cWindow    & a_Window)
{
	// TODO
}





void cProtocol172::SendWindowClose(const cWindow    & a_Window)
{
	// TODO
}





void cProtocol172::SendWindowOpen(char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots)
{
	// TODO
}





void cProtocol172::SendWindowProperty(const cWindow & a_Window, short a_Property, short a_Value)
{
	// TODO
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
				case 0x00: HandlePacketKeepAlive              (a_RemainingBytes); break;
				case 0x01: HandlePacketChatMessage            (a_RemainingBytes); break;
				case 0x02: HandlePacketUseEntity              (a_RemainingBytes); break;
				case 0x03: HandlePacketPlayer                 (a_RemainingBytes); break;
				case 0x04: HandlePacketPlayerPos              (a_RemainingBytes); break;
				case 0x05: HandlePacketPlayerLook             (a_RemainingBytes); break;
				case 0x06: HandlePacketPlayerPosLook          (a_RemainingBytes); break;
				case 0x07: HandlePacketBlockDig               (a_RemainingBytes); break;
				case 0x08: HandlePacketBlockPlace             (a_RemainingBytes); break;
				case 0x09: HandlePacketSlotSelect             (a_RemainingBytes); break;
				case 0x0a: HandlePacketAnimation              (a_RemainingBytes); break;
				case 0x0b: HandlePacketEntityAction           (a_RemainingBytes); break;
				case 0x0c: HandlePacketSteerVehicle           (a_RemainingBytes); break;
				case 0x0d: HandlePacketWindowClose            (a_RemainingBytes); break;
				case 0x0e: HandlePacketWindowClick            (a_RemainingBytes); break;
				case 0x0f: // Confirm transaction - not used in MCS
				case 0x10: HandlePacketCreativeInventoryAction(a_RemainingBytes); break;
				case 0x12: HandlePacketUpdateSign             (a_RemainingBytes); break;
				case 0x13: HandlePacketPlayerAbilities        (a_RemainingBytes); break;
				case 0x14: HandlePacketTabComplete            (a_RemainingBytes); break;
				case 0x15: HandlePacketClientSettings         (a_RemainingBytes); break;
				case 0x16: HandlePacketClientStatus           (a_RemainingBytes); break;
				case 0x17: HandlePacketPluginMessage          (a_RemainingBytes); break;
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
	
	cByteBuffer Packet(18);
	Packet.WriteVarInt(0x01);
	Packet.WriteBEInt64(Timestamp);
	WritePacket(Packet);
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
	
	cByteBuffer Packet(Response.size() + 10);
	Packet.WriteVarInt(0x00);  // Response packet
	Packet.WriteVarUTF8String(Response);
	WritePacket(Packet);
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
	cByteBuffer Packet(Username.size() + 30);
	Packet.WriteVarInt(0x02);  // Login success packet
	Packet.WriteVarUTF8String(Printf("%d", m_Client->GetUniqueID()));  // TODO: UUID
	Packet.WriteVarUTF8String(Username);
	WritePacket(Packet);

	m_Client->HandleLogin(4, Username);
}





void cProtocol172::HandlePacketAnimation(UInt32 a_RemainingBytes)
{
	HANDLE_PACKET_READ(ReadBEInt, int,  EntityID);
	HANDLE_PACKET_READ(ReadByte,  Byte, Animation);
	m_Client->HandleAnimation(Animation);
}





void cProtocol172::HandlePacketBlockDig(UInt32 a_RemainingBytes)
{
	HANDLE_PACKET_READ(ReadByte,  Byte, Status);
	HANDLE_PACKET_READ(ReadBEInt, int,  BlockX);
	HANDLE_PACKET_READ(ReadByte,  Byte, BlockY);
	HANDLE_PACKET_READ(ReadBEInt, int,  BlockZ);
	HANDLE_PACKET_READ(ReadByte,  Byte, Face);
	m_Client->HandleLeftClick(BlockX, BlockY, BlockZ, Face, Status);
}





void cProtocol172::HandlePacketBlockPlace(UInt32 a_RemainingBytes)
{
	HANDLE_PACKET_READ(ReadBEInt, int,  BlockX);
	HANDLE_PACKET_READ(ReadByte,  Byte, BlockY);
	HANDLE_PACKET_READ(ReadBEInt, int,  BlockZ);
	HANDLE_PACKET_READ(ReadByte,  Byte, Face);
	HANDLE_PACKET_READ(ReadByte,  Byte, CursorX);
	HANDLE_PACKET_READ(ReadByte,  Byte, CursorY);
	HANDLE_PACKET_READ(ReadByte,  Byte, CursorZ);
	m_Client->HandleRightClick(BlockX, BlockY, BlockZ, Face, CursorX, CursorY, CursorZ, m_Client->GetPlayer()->GetEquippedItem());
}





void cProtocol172::HandlePacketChatMessage(UInt32 a_RemainingBytes)
{
	HANDLE_PACKET_READ(ReadVarUTF8String, AString, Message);
	m_Client->HandleChat(Message);
}





void cProtocol172::HandlePacketClientSettings(UInt32 a_RemainingBytes)
{
	HANDLE_PACKET_READ(ReadVarUTF8String, AString, Locale);
	HANDLE_PACKET_READ(ReadByte,          Byte,    ViewDistance);
	HANDLE_PACKET_READ(ReadByte,          Byte,    ChatFlags);
	HANDLE_PACKET_READ(ReadByte,          Byte,    Unused);
	HANDLE_PACKET_READ(ReadByte,          Byte,    Difficulty);
	HANDLE_PACKET_READ(ReadByte,          Byte,    ShowCape);
	// TODO
}





void cProtocol172::HandlePacketClientStatus(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketCreativeInventoryAction(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketEntityAction(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketKeepAlive(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketPlayer(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketPlayerAbilities(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketPlayerLook(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketPlayerPos(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketPlayerPosLook(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketPluginMessage(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketSlotSelect(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketSteerVehicle(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketTabComplete(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketUpdateSign(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketUseEntity(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketWindowClick(UInt32 a_RemainingBytes)
{
	// TODO
}





void cProtocol172::HandlePacketWindowClose(UInt32 a_RemainingBytes)
{
	// TODO
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
	m_DataToSend.append(a_Data, a_Size);
}





void cProtocol172::Flush(void)
{
	ASSERT(m_CSPacket.IsLockedByCurrentThread());  // Did all packets lock the CS properly?
	
	if (m_DataToSend.empty())
	{
		LOGD("Flushing empty");
		return;
	}
	const char * a_Data = m_DataToSend.data();
	int a_Size = m_DataToSend.size();
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
	m_DataToSend.clear();
}






