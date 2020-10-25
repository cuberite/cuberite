#pragma once

#include "Protocol.h"





// Adjust these if a new protocol is added or an old one is removed:
#define MCS_CLIENT_VERSIONS "1.8.x-1.12.x"
#define MCS_PROTOCOL_VERSIONS "47, 107, 108, 109, 110, 210, 315, 316, 335, 338, 340"
#define MCS_LATEST_PROTOCOL_VERSION 340





/** Meta-protocol that recognizes multiple protocol versions, creates the specific
protocol version instance and redirects everything to it. */
class cMultiVersionProtocol
{
public:

	cMultiVersionProtocol();

	/** Translates protocol version number into protocol version text: 49 -> "1.4.4" */
	static AString GetVersionTextFromInt(cProtocol::Version a_ProtocolVersion);

	/** Returns if we contain a concrete protocol corresponding to the client's protocol version. */
	bool VersionRecognitionSuccessful()
	{
		return m_Protocol != nullptr;
	}

	/** Convenience overload to enable redirecting sends to the underlying implementation. */
	auto & operator->()
	{
		return m_Protocol;
	}

	/** The function that's responsible for processing incoming protocol data. */
	std::function<void(cClientHandle &, std::string_view)> HandleIncomingData;

	/** Sends a disconnect to the client as a result of a recognition error.
	This function can be used to disconnect before any protocol has been recognised. */
	void SendDisconnect(cClientHandle & a_Client, const AString & a_Reason);

private:

	/** Handles data reception in a newly-created client handle that doesn't yet have known protocol.
	a_Data contains a view of data that were just received.
	Calls TryRecognizeProtocol to populate m_Protocol, and transitions to another mode depending on success. */
	void HandleIncomingDataInRecognitionStage(cClientHandle & a_Client, std::string_view a_Data);

	/** Handles and responds to unsupported clients sending pings. */
	void HandleIncomingDataInOldPingResponseStage(cClientHandle & a_Client, const std::string_view a_Data);

	/* Tries to recognize a protocol based on a_Data and m_Buffer contents.
	a_Data is replaced with a sub-view, with handshake packet removed. */
	void TryRecognizeProtocol(cClientHandle & a_Client, std::string_view & a_Data);

	/** Tries to recognize a protocol in the lengthed family (1.7+), based on m_Buffer.
	The packet length and type have already been read, type is 0.
	Returns a cProtocol_XXX instance if recognized. */
	std::unique_ptr<cProtocol> TryRecognizeLengthedProtocol(cClientHandle & a_Client, std::string_view & a_Data);

	/** Sends one packet inside a cByteBuffer.
	This is used only when handling an outdated server ping. */
	static void SendPacket(cClientHandle & a_Client, cByteBuffer & a_OutPacketBuffer);

	/** Returns the protocol-specific packet ID given the protocol-agnostic packet enum. */
	static UInt32 GetPacketID(cProtocol::ePacketType a_PacketType);

	/* Status handler for unrecognised versions. */
	void HandlePacketStatusRequest(cClientHandle & a_Client, cByteBuffer & a_Out);

	/* Ping handler for unrecognised versions. */
	void HandlePacketStatusPing(cClientHandle & a_Client, cByteBuffer & a_Out);

	/** The actual protocol implementation.
	Created when recognition of the client version succeeds with a version we support. */
	std::unique_ptr<cProtocol> m_Protocol;

	/** Buffer for received protocol data. */
	cByteBuffer m_Buffer;

} ;
