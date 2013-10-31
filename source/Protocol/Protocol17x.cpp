
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





