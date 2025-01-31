#pragma once

#include "Protocol.h"





// Adjust these if a new protocol is added or an old one is removed:
#define MCS_CLIENT_VERSIONS "1.8.x-1.21.4"
#define MCS_PROTOCOL_VERSIONS "47, 107, 108, 109, 110, 210, 315, 316, 335, 338, 340, 393, 401, 477, 480, 485, 490, 498, 573, 575, 578, 735, 736, 751, 753, 754, 755, 756, 757, 758, 759, 760, 761, 762, 763, 764, 765, 766, 767, 768, 769"
#define MCS_LATEST_PROTOCOL_VERSION 769





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

	auto & operator->() const
	{
		return m_Protocol;
	}

	/** Directs incoming protocol data along the correct pathway, depending on the state of the version recognition process.
	The protocol modifies the provided buffer in-place. */
	void HandleIncomingData(cClientHandle & a_Client, ContiguousByteBuffer & a_Data);

	/** Allows the protocol (if any) to do a final pass on outgiong data, possibly modifying the provided buffer in-place. */
	void HandleOutgoingData(ContiguousByteBuffer & a_Data);

	/** Sends a disconnect to the client as a result of a recognition error.
	This function can be used to disconnect before any protocol has been recognised. */
	void SendDisconnect(cClientHandle & a_Client, const AString & a_Reason);

private:

	/** Handles data reception in a newly-created client handle that doesn't yet have a known protocol.
	a_Data contains a view of data that were just received.
	Tries to recognize a protocol, populate m_Protocol, and transitions to another mode depending on success. */
	void HandleIncomingDataInRecognitionStage(cClientHandle & a_Client, ContiguousByteBuffer & a_Data);

	/** Handles and responds to unsupported clients sending pings. */
	void HandleIncomingDataInOldPingResponseStage(cClientHandle & a_Client, ContiguousByteBufferView a_Data);

	/* Checks if incoming data is an HTTP request and handles it if it is. */
	bool TryHandleHTTPRequest(cClientHandle & a_Client, ContiguousByteBuffer & a_Data);

	/** Tries to recognize a protocol in the lengthed family (1.7+), based on m_Buffer.
	Returns a cProtocol_XXX instance if recognized. */
	std::unique_ptr<cProtocol> TryRecognizeLengthedProtocol(cClientHandle & a_Client);

	/** Sends one packet inside a cByteBuffer.
	This is used only when handling an outdated server ping. */
	static void SendPacket(cClientHandle & a_Client, cByteBuffer & a_OutPacketBuffer);

	/** Returns the protocol-specific packet ID given the protocol-agnostic packet enum. */
	static UInt32 GetPacketID(cProtocol::ePacketType a_PacketType);

	/* Status handler for unrecognised versions. */
	void HandlePacketStatusRequest(cClientHandle & a_Client);

	/* Ping handler for unrecognised versions. */
	void HandlePacketStatusPing(cClientHandle & a_Client);

	/** Buffer for received protocol data. */
	cByteBuffer m_Buffer;

	/** The actual protocol implementation.
	Created when recognition of the client version succeeds with a version we support. */
	std::unique_ptr<cProtocol> m_Protocol;

	/** If we're still waiting for data required for version recognition to arrive. */
	bool m_WaitingForData;
} ;
