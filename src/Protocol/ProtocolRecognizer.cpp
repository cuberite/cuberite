
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





struct UnsupportedButPingableProtocolException : public std::runtime_error
{
	explicit UnsupportedButPingableProtocolException() :
		std::runtime_error("")
	{
	}
};





struct TriedToJoinWithUnsupportedProtocolException : public std::runtime_error
{
	explicit TriedToJoinWithUnsupportedProtocolException(const std::string & a_Message) :
		std::runtime_error(a_Message)
	{
	}
};





cMultiVersionProtocol::cMultiVersionProtocol() :
	HandleIncomingData(std::bind(&cMultiVersionProtocol::HandleIncomingDataInRecognitionStage, this, std::placeholders::_1, std::placeholders::_2)),
	m_Buffer(32 KiB)
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





void cMultiVersionProtocol::HandleIncomingDataInRecognitionStage(cClientHandle & a_Client, std::string_view a_Data)
{
	// We read more than the handshake packet here, oh well.
	if (!m_Buffer.Write(a_Data.data(), a_Data.size()))
	{
		a_Client.Kick("Your client sent too much data; please try again later.");
		return;
	}

	try
	{
		// Note that a_Data is assigned to a subview containing the data to pass to m_Protocol or UnsupportedPing

		TryRecognizeProtocol(a_Client, a_Data);
		if (m_Protocol == nullptr)
		{
			m_Buffer.ResetRead();
			return;
		}

		// The protocol recogniser succesfully identified, switch mode:
		HandleIncomingData = [this](cClientHandle &, const std::string_view a_In)
		{
			m_Protocol->DataReceived(m_Buffer, a_In.data(), a_In.size());
		};
	}
	catch (const UnsupportedButPingableProtocolException &)
	{
		// Got a server list ping for an unrecognised version,
		// switch into responding to unknown protocols mode:
		HandleIncomingData = [this](cClientHandle & a_Clyent, const std::string_view a_In)
		{
			HandleIncomingDataInOldPingResponseStage(a_Clyent, a_In);
		};
	}
	catch (const std::exception & Oops)
	{
		a_Client.Kick(Oops.what());
		return;
	}

	// Explicitly process any remaining data with the new handler:
	HandleIncomingData(a_Client, a_Data);
}





void cMultiVersionProtocol::HandleIncomingDataInOldPingResponseStage(cClientHandle & a_Client, const std::string_view a_Data)
{
	if (!m_Buffer.Write(a_Data.data(), a_Data.size()))
	{
		a_Client.Kick("Server list ping failed, too much data.");
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
			SendPacket(a_Client, OutPacketBuffer);
		}
		else if ((PacketID == 0x01) && (PacketLen == 9))  // Ping packet
		{
			HandlePacketStatusPing(a_Client, OutPacketBuffer);
			SendPacket(a_Client, OutPacketBuffer);
		}
		else
		{
			a_Client.Kick("Server list ping failed, unrecognized packet.");
			return;
		}

		m_Buffer.CommitRead();
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





void cMultiVersionProtocol::TryRecognizeProtocol(cClientHandle & a_Client, std::string_view & a_Data)
{
	// NOTE: If a new protocol is added or an old one is removed, adjust MCS_CLIENT_VERSIONS and MCS_PROTOCOL_VERSIONS macros in the header file

	// Lengthed protocol, try if it has the entire initial handshake packet:
	UInt32 PacketLen;
	if (!m_Buffer.ReadVarInt(PacketLen))
	{
		// Not enough bytes for the packet length, keep waiting
		return;
	}

	if (!m_Buffer.CanReadBytes(PacketLen))
	{
		// Not enough bytes for the packet, keep waiting
		// More of a sanity check to make sure no one tries anything funny (since ReadXXX can wait for data themselves):
		return;
	}

	m_Protocol = TryRecognizeLengthedProtocol(a_Client, a_Data);
	ASSERT(m_Protocol != nullptr);
}





std::unique_ptr<cProtocol> cMultiVersionProtocol::TryRecognizeLengthedProtocol(cClientHandle & a_Client, std::string_view & a_Data)
{
	UInt32 PacketType;
	UInt32 ProtocolVersion;
	AString ServerAddress;
	UInt16 ServerPort;
	UInt32 NextStateValue;

	if (!m_Buffer.ReadVarInt(PacketType) || (PacketType != 0x00))
	{
		// Not an initial handshake packet, we don't know how to talk to them
		LOGINFO("Client \"%s\" uses an unsupported protocol (lengthed, initial packet %u)",
			a_Client.GetIPString().c_str(), PacketType
		);

		throw TriedToJoinWithUnsupportedProtocolException(
			Printf("Your client isn't supported.\nTry connecting with Minecraft " MCS_CLIENT_VERSIONS, ProtocolVersion)
		);
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

	cProtocol::State NextState = [&]
		{
			switch (NextStateValue)
			{
				case cProtocol::State::Status: return cProtocol::State::Status;
				case cProtocol::State::Login: return cProtocol::State::Login;
				case cProtocol::State::Game: return cProtocol::State::Game;
				default:
				{
					throw TriedToJoinWithUnsupportedProtocolException(
						fmt::format("Invalid next game state: {}", NextStateValue)
					);
				}
			}
		}();

	// TODO: this should be a protocol property, not ClientHandle:
	a_Client.SetProtocolVersion(ProtocolVersion);

	// The protocol has just been recognized, advance data start
	// to after the handshake and leave the rest to the protocol:
	a_Data = a_Data.substr(m_Buffer.GetUsedSpace() - m_Buffer.GetReadableSpace());

	// We read more than we can handle, purge the rest:
	[[maybe_unused]] const bool Success =
		m_Buffer.SkipRead(m_Buffer.GetReadableSpace());
	ASSERT(Success);

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

			throw UnsupportedButPingableProtocolException();
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
	AString LengthData;
	OutPacketLenBuffer.ReadAll(LengthData);
	a_Client.SendData(LengthData.data(), LengthData.size());

	// Send the packet's payload:
	AString PacketData;
	a_OutPacketBuffer.ReadAll(PacketData);
	a_OutPacketBuffer.CommitRead();
	a_Client.SendData(PacketData.data(), PacketData.size());
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
}
