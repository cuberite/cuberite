
// ProtocolRecognizer.cpp

// Implements the cProtocolRecognizer class representing the meta-protocol that recognizes possibly multiple
// protocol versions and redirects everything to them

#include "Globals.h"

#include "ProtocolRecognizer.h"
#include "Protocol_1_8.h"
#include "Protocol_1_9.h"
#include "Protocol_1_10.h"
#include "Protocol_1_11.h"
#include "Protocol_1_12.h"
#include "Protocol_1_13.h"
#include "Protocol_1_14.h"
#include "../ClientHandle.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"
#include "../JsonUtils.h"
#include "../Bindings/PluginManager.h"





struct TriedToJoinWithUnsupportedProtocolException : public std::runtime_error
{
	explicit TriedToJoinWithUnsupportedProtocolException(const std::string & a_Message) :
		std::runtime_error(a_Message)
	{
	}
};





cMultiVersionProtocol::cMultiVersionProtocol() :
	m_Buffer(32 KiB),
	m_WaitingForData(true)
{
}





AString cMultiVersionProtocol::GetVersionTextFromInt(cProtocol::Version a_ProtocolVersion)
{
	switch (a_ProtocolVersion)
	{
		case cProtocol::Version::v1_8_0:   return "1.8";
		case cProtocol::Version::v1_9_0:   return "1.9";
		case cProtocol::Version::v1_9_1:   return "1.9.1";
		case cProtocol::Version::v1_9_2:   return "1.9.2";
		case cProtocol::Version::v1_9_4:   return "1.9.4";
		case cProtocol::Version::v1_10_0:  return "1.10";
		case cProtocol::Version::v1_11_0:  return "1.11";
		case cProtocol::Version::v1_11_1:  return "1.11.1";
		case cProtocol::Version::v1_12:    return "1.12";
		case cProtocol::Version::v1_12_1:  return "1.12.1";
		case cProtocol::Version::v1_12_2:  return "1.12.2";
		case cProtocol::Version::v1_13:    return "1.13";
		case cProtocol::Version::v1_13_1:  return "1.13.1";
		case cProtocol::Version::v1_13_2:  return "1.13.2";
		case cProtocol::Version::v1_14:    return "1.14";
	}

	ASSERT(!"Unknown protocol version");
	return Printf("Unknown protocol (%d)", a_ProtocolVersion);
}





void cMultiVersionProtocol::HandleIncomingDataInRecognitionStage(cClientHandle & a_Client, ContiguousByteBuffer & a_Data)
{
	// NOTE: If a new protocol is added or an old one is removed, adjust MCS_CLIENT_VERSIONS and MCS_PROTOCOL_VERSIONS macros in the header file

	/* Write all incoming data unmodified into m_Buffer.
	Writing everything is always okay to do:
	1. We can be sure protocol encryption hasn't started yet since m_Protocol hasn't been called, hence no decryption needs to take place
	2. The extra data are processed at the end of this function */
	if (!m_Buffer.Write(a_Data.data(), a_Data.size()))
	{
		a_Client.PacketBufferFull();
		return;
	}

	// TODO: recover from git history
	// Unlengthed protocol, ...

	// Lengthed protocol, try if it has the entire initial handshake packet:
	if (
		UInt32 PacketLen;

		// If not enough bytes for the packet length, keep waiting
		!m_Buffer.ReadVarInt(PacketLen) ||

		// If not enough bytes for the packet, keep waiting
		// (More of a sanity check to make sure no one tries anything funny, since ReadXXX can wait for data themselves)
		!m_Buffer.CanReadBytes(PacketLen)
	)
	{
		m_Buffer.ResetRead();
		return;
	}

	/* Figure out the client's version.
	1. m_Protocol != nullptr: the protocol is supported and we have a handler
	2. m_Protocol == nullptr: the protocol is unsupported, handling is a special case done by ourselves
	3. Exception: the data sent were garbage, the client handle deals with it by disconnecting */
	m_Protocol = TryRecognizeLengthedProtocol(a_Client);

	// Version recognised. Cause HandleIncomingData to stop calling us to handle data:
	m_WaitingForData = false;

	// Explicitly process any remaining data (already written to m_Buffer) with the new handler:
	{
		ContiguousByteBuffer Empty;
		HandleIncomingData(a_Client, Empty);
	}
}





void cMultiVersionProtocol::HandleIncomingDataInOldPingResponseStage(cClientHandle & a_Client, const ContiguousByteBufferView a_Data)
{
	if (!m_Buffer.Write(a_Data.data(), a_Data.size()))
	{
		a_Client.PacketBufferFull();
		return;
	}

	cByteBuffer OutPacketBuffer(6 KiB);

	// Handle server list ping packets
	for (;;)
	{
		UInt32 PacketLen;
		UInt32 PacketID;
		if (
			!m_Buffer.ReadVarInt32(PacketLen) ||
			!m_Buffer.CanReadBytes(PacketLen) ||
			!m_Buffer.ReadVarInt32(PacketID)
		)
		{
			// Not enough data
			m_Buffer.ResetRead();
			break;
		}

		if ((PacketID == 0x00) && (PacketLen == 1))  // Request packet
		{
			HandlePacketStatusRequest(a_Client, OutPacketBuffer);
		}
		else if ((PacketID == 0x01) && (PacketLen == 9))  // Ping packet
		{
			HandlePacketStatusPing(a_Client, OutPacketBuffer);
		}
		else
		{
			a_Client.PacketUnknown(PacketID);
			return;
		}

		m_Buffer.CommitRead();
	}
}





void cMultiVersionProtocol::HandleIncomingData(cClientHandle & a_Client, ContiguousByteBuffer & a_Data)
{
	if (m_WaitingForData)
	{
		HandleIncomingDataInRecognitionStage(a_Client, a_Data);
	}
	else if (m_Protocol == nullptr)
	{
		// Got a Handshake for an unrecognised version, process future data accordingly:
		HandleIncomingDataInOldPingResponseStage(a_Client, a_Data);
	}
	else
	{
		// The protocol recogniser succesfully identified a supported version, direct data to that protocol:
		m_Protocol->DataReceived(m_Buffer, a_Data);
	}
}





void cMultiVersionProtocol::HandleOutgoingData(ContiguousByteBuffer & a_Data)
{
	// Normally only the protocol sends data, so outgoing data are only present when m_Protocol != nullptr.
	// However, for unrecognised protocols we send data too, and that's when m_Protocol == nullptr. Check to avoid crashing (GH #5260).

	if (m_Protocol != nullptr)
	{
		m_Protocol->DataPrepared(a_Data);
	}
}





void cMultiVersionProtocol::SendDisconnect(cClientHandle & a_Client, const AString & a_Reason)
{
	if (m_Protocol != nullptr)
	{
		m_Protocol->SendDisconnect(a_Reason);
		return;
	}

	const AString Message = Printf("{\"text\":\"%s\"}", EscapeString(a_Reason).c_str());
	const auto PacketID = GetPacketID(cProtocol::ePacketType::pktDisconnectDuringLogin);
	cByteBuffer Out(
		cByteBuffer::GetVarIntSize(PacketID) +
		cByteBuffer::GetVarIntSize(static_cast<UInt32>(Message.size())) + Message.size()
	);

	VERIFY(Out.WriteVarInt32(PacketID));
	VERIFY(Out.WriteVarUTF8String(Message));
	SendPacket(a_Client, Out);
}





std::unique_ptr<cProtocol> cMultiVersionProtocol::TryRecognizeLengthedProtocol(cClientHandle & a_Client)
{
	UInt32 PacketType;
	UInt32 ProtocolVersion;
	AString ServerAddress;
	UInt16 ServerPort;
	UInt32 NextStateValue;

	if (!m_Buffer.ReadVarInt(PacketType) || (PacketType != 0x00))
	{
		// Not an initial handshake packet, we don't know how to talk to them:
		LOGD("Client \"%s\" uses an unsupported protocol (lengthed, initial packet %u)",
			a_Client.GetIPString().c_str(), PacketType
		);

		throw TriedToJoinWithUnsupportedProtocolException("Your client isn't supported.\nTry connecting with Minecraft " MCS_CLIENT_VERSIONS);
	}

	if (
		!m_Buffer.ReadVarInt(ProtocolVersion) ||
		!m_Buffer.ReadVarUTF8String(ServerAddress) ||
		!m_Buffer.ReadBEUInt16(ServerPort) ||
		!m_Buffer.ReadVarInt(NextStateValue)
	)
	{
		// TryRecognizeProtocol guarantees that we will have as much
		// data to read as the client claims in the protocol length field:
		throw TriedToJoinWithUnsupportedProtocolException("Incorrect amount of data received - hacked client?");
	}

	const auto NextState = [NextStateValue]
	{
		switch (NextStateValue)
		{
			case 1:  return cProtocol::State::Status;
			case 2:  return cProtocol::State::Login;
			case 3:  return cProtocol::State::Game;
			default: throw TriedToJoinWithUnsupportedProtocolException("Your client isn't supported.\nTry connecting with Minecraft " MCS_CLIENT_VERSIONS);
		}
	}();

	// TODO: this should be a protocol property, not ClientHandle:
	a_Client.SetProtocolVersion(ProtocolVersion);

	// All good, eat up the data:
	m_Buffer.CommitRead();

	switch (ProtocolVersion)
	{
		case static_cast<UInt32>(cProtocol::Version::v1_8_0):  return std::make_unique<cProtocol_1_8_0> (&a_Client, ServerAddress, NextState);
		case static_cast<UInt32>(cProtocol::Version::v1_9_0):  return std::make_unique<cProtocol_1_9_0> (&a_Client, ServerAddress, NextState);
		case static_cast<UInt32>(cProtocol::Version::v1_9_1):  return std::make_unique<cProtocol_1_9_1> (&a_Client, ServerAddress, NextState);
		case static_cast<UInt32>(cProtocol::Version::v1_9_2):  return std::make_unique<cProtocol_1_9_2> (&a_Client, ServerAddress, NextState);
		case static_cast<UInt32>(cProtocol::Version::v1_9_4):  return std::make_unique<cProtocol_1_9_4> (&a_Client, ServerAddress, NextState);
		case static_cast<UInt32>(cProtocol::Version::v1_10_0): return std::make_unique<cProtocol_1_10_0>(&a_Client, ServerAddress, NextState);
		case static_cast<UInt32>(cProtocol::Version::v1_11_0): return std::make_unique<cProtocol_1_11_0>(&a_Client, ServerAddress, NextState);
		case static_cast<UInt32>(cProtocol::Version::v1_11_1): return std::make_unique<cProtocol_1_11_1>(&a_Client, ServerAddress, NextState);
		case static_cast<UInt32>(cProtocol::Version::v1_12):   return std::make_unique<cProtocol_1_12>  (&a_Client, ServerAddress, NextState);
		case static_cast<UInt32>(cProtocol::Version::v1_12_1): return std::make_unique<cProtocol_1_12_1>(&a_Client, ServerAddress, NextState);
		case static_cast<UInt32>(cProtocol::Version::v1_12_2): return std::make_unique<cProtocol_1_12_2>(&a_Client, ServerAddress, NextState);
		case static_cast<UInt32>(cProtocol::Version::v1_13):   return std::make_unique<cProtocol_1_13>  (&a_Client, ServerAddress, NextState);
		case static_cast<UInt32>(cProtocol::Version::v1_13_1): return std::make_unique<cProtocol_1_13_1>(&a_Client, ServerAddress, NextState);
		case static_cast<UInt32>(cProtocol::Version::v1_13_2): return std::make_unique<cProtocol_1_13_2>(&a_Client, ServerAddress, NextState);
		case static_cast<UInt32>(cProtocol::Version::v1_14):   return std::make_unique<cProtocol_1_14>  (&a_Client, ServerAddress, NextState);
		default:
		{
			LOGD("Client \"%s\" uses an unsupported protocol (lengthed, version %u (0x%x))",
				a_Client.GetIPString().c_str(), ProtocolVersion, ProtocolVersion
			);

			if (NextState != cProtocol::State::Status)
			{
				throw TriedToJoinWithUnsupportedProtocolException(
					Printf("Unsupported protocol version %u.\nTry connecting with Minecraft " MCS_CLIENT_VERSIONS, ProtocolVersion)
				);
			}

			// No cProtocol can handle the client:
			return nullptr;
		}
	}
}





void cMultiVersionProtocol::SendPacket(cClientHandle & a_Client, cByteBuffer & a_OutPacketBuffer)
{
	// Writes out the packet normally.
	UInt32 PacketLen = static_cast<UInt32>(a_OutPacketBuffer.GetUsedSpace());
	cByteBuffer OutPacketLenBuffer(cByteBuffer::GetVarIntSize(PacketLen));

	// Compression doesn't apply to this state, send raw data:
	VERIFY(OutPacketLenBuffer.WriteVarInt32(PacketLen));
	ContiguousByteBuffer LengthData;
	OutPacketLenBuffer.ReadAll(LengthData);
	a_Client.SendData(LengthData);

	// Send the packet's payload:
	ContiguousByteBuffer PacketData;
	a_OutPacketBuffer.ReadAll(PacketData);
	a_OutPacketBuffer.CommitRead();
	a_Client.SendData(PacketData);
}





UInt32 cMultiVersionProtocol::GetPacketID(cProtocol::ePacketType a_PacketType)
{
	switch (a_PacketType)
	{
		case cProtocol::ePacketType::pktDisconnectDuringLogin: return 0x00;
		case cProtocol::ePacketType::pktStatusResponse:        return 0x00;
		case cProtocol::ePacketType::pktPingResponse:          return 0x01;
		default:
		{
			ASSERT(!"GetPacketID() called for an unhandled packet");
			return 0;
		}
	}
}





void cMultiVersionProtocol::HandlePacketStatusRequest(cClientHandle & a_Client, cByteBuffer & a_Out)
{
	cServer * Server = cRoot::Get()->GetServer();
	AString ServerDescription = Server->GetDescription();
	auto NumPlayers = static_cast<signed>(Server->GetNumPlayers());
	auto MaxPlayers = static_cast<signed>(Server->GetMaxPlayers());
	AString Favicon = Server->GetFaviconData();
	cRoot::Get()->GetPluginManager()->CallHookServerPing(a_Client, ServerDescription, NumPlayers, MaxPlayers, Favicon);

	// Version:
	Json::Value Version;
	Version["name"] = "Cuberite " MCS_CLIENT_VERSIONS;
	Version["protocol"] = MCS_LATEST_PROTOCOL_VERSION;

	// Players:
	Json::Value Players;
	Players["online"] = NumPlayers;
	Players["max"] = MaxPlayers;
	// TODO: Add "sample"

	// Description:
	Json::Value Description;
	Description["text"] = ServerDescription.c_str();

	// Create the response:
	Json::Value ResponseValue;
	ResponseValue["version"] = Version;
	ResponseValue["players"] = Players;
	ResponseValue["description"] = Description;
	if (!Favicon.empty())
	{
		ResponseValue["favicon"] = Printf("data:image/png;base64,%s", Favicon.c_str());
	}

	AString Response = JsonUtils::WriteFastString(ResponseValue);

	VERIFY(a_Out.WriteVarInt32(GetPacketID(cProtocol::ePacketType::pktStatusResponse)));
	VERIFY(a_Out.WriteVarUTF8String(Response));

	SendPacket(a_Client, a_Out);
}





void cMultiVersionProtocol::HandlePacketStatusPing(cClientHandle & a_Client, cByteBuffer & a_Out)
{
	Int64 Timestamp;
	if (!m_Buffer.ReadBEInt64(Timestamp))
	{
		return;
	}

	VERIFY(a_Out.WriteVarInt32(GetPacketID(cProtocol::ePacketType::pktPingResponse)));
	VERIFY(a_Out.WriteBEInt64(Timestamp));

	SendPacket(a_Client, a_Out);
}
