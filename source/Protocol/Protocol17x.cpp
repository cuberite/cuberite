
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
		UInt32 NumBytesRead = m_ReceivedData.GetReadableSpace();
		if (!m_ReceivedData.ReadVarInt(PacketType))
		{
			// Not enough data
			return;
		}
		NumBytesRead -= m_ReceivedData.GetReadableSpace();
		HandlePacket(PacketType, PacketLen - NumBytesRead);
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
			}
			break;
		}
		
		case 2:
		{
			// Login
			break;
		}
		
		case 3:
		{
			// Game
			break;
		}
	}  // switch (m_State)
	
	// Unknown packet type, report to the client:
	m_Client->PacketUnknown(a_PacketType);
	m_ReceivedData.SkipRead(a_RemainingBytes);
	m_ReceivedData.CommitRead();
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





