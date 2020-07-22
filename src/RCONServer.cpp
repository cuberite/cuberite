
// RCONServer.cpp

// Implements the cRCONServer class representing the RCON server

#include "Globals.h"
#include "IniFile.h"
#include "RCONServer.h"
#include "Server.h"
#include "Root.h"
#include "CommandOutput.h"





// Disable MSVC warnings:
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:4355)  // 'this' : used in base member initializer list
#endif





enum
{
	// Client -> Server:
	RCON_PACKET_COMMAND = 2,
	RCON_PACKET_LOGIN   = 3,

	// Server -> Client:
	RCON_PACKET_RESPONSE = 2,
} ;





////////////////////////////////////////////////////////////////////////////////
// cRCONListenCallbacks:

class cRCONListenCallbacks:
	public cNetwork::cListenCallbacks
{
public:
	cRCONListenCallbacks(cRCONServer & a_RCONServer, UInt16 a_Port):
		m_RCONServer(a_RCONServer),
		m_Port(a_Port)
	{
	}

protected:
	/** The RCON server instance that we're attached to. */
	cRCONServer & m_RCONServer;

	/** The port for which this instance is responsible. */
	UInt16 m_Port;

	// cNetwork::cListenCallbacks overrides:
	virtual cTCPLink::cCallbacksPtr OnIncomingConnection(const AString & a_RemoteIPAddress, UInt16 a_RemotePort) override
	{
		LOG("RCON Client \"%s\" connected!", a_RemoteIPAddress.c_str());
		return std::make_shared<cRCONServer::cConnection>(m_RCONServer, a_RemoteIPAddress);
	}
	virtual void OnAccepted(cTCPLink & a_Link) override {}
	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override
	{
		LOGWARNING("RCON server error on port %d: %d (%s)", m_Port, a_ErrorCode, a_ErrorMsg.c_str());
	}
};





////////////////////////////////////////////////////////////////////////////////
// cRCONCommandOutput:

class cRCONCommandOutput :
	public cCommandOutputCallback
{
public:
	cRCONCommandOutput(cRCONServer::cConnection & a_Connection, UInt32 a_RequestID) :
		m_Connection(a_Connection),
		m_RequestID(a_RequestID)
	{
	}

	// cCommandOutputCallback overrides:
	virtual void Out(const AString & a_Text) override
	{
		m_Buffer.append(a_Text);
	}

	virtual void Finished(void) override
	{
		m_Connection.SendResponse(m_RequestID, RCON_PACKET_RESPONSE, static_cast<UInt32>(m_Buffer.size()), m_Buffer.c_str());
		delete this;
	}

protected:
	cRCONServer::cConnection & m_Connection;
	UInt32 m_RequestID;
	AString m_Buffer;
} ;





////////////////////////////////////////////////////////////////////////////////
// cRCONServer:

cRCONServer::cRCONServer(cServer & a_Server) :
	m_Server(a_Server)
{
}





cRCONServer::~cRCONServer()
{
	for (const auto & srv: m_ListenServers)
	{
		srv->Close();
	}
}





void cRCONServer::Initialize(cSettingsRepositoryInterface & a_Settings)
{
	if (!a_Settings.GetValueSetB("RCON", "Enabled", false))
	{
		return;
	}

	// Read the password, don't allow an empty one:
	m_Password = a_Settings.GetValueSet("RCON", "Password", "");
	if (m_Password.empty())
	{
		LOGWARNING("RCON is requested, but the password is not set. RCON is now disabled.");
		return;
	}

	// Read the listening ports for RCON from config:
	AStringVector Ports = ReadUpgradeIniPorts(a_Settings, "RCON", "Ports", "PortsIPv4", "PortsIPv6", "25575");

	// Start listening on each specified port:
	for (const auto & port: Ports)
	{
		UInt16 PortNum;
		if (!StringToInteger(port, PortNum))
		{
			LOGINFO("Invalid RCON port value: \"%s\". Ignoring.", port.c_str());
			continue;
		}
		auto Handle = cNetwork::Listen(PortNum, std::make_shared<cRCONListenCallbacks>(*this, PortNum));
		if (Handle->IsListening())
		{
			m_ListenServers.push_back(Handle);
		}
	}

	if (m_ListenServers.empty())
	{
		LOGWARNING("RCON is enabled but no valid ports were found. RCON is not accessible.");
	}
}





////////////////////////////////////////////////////////////////////////////////
// cRCONServer::cConnection:

cRCONServer::cConnection::cConnection(cRCONServer & a_RCONServer, const AString & a_IPAddress) :
	m_IsAuthenticated(false),
	m_RCONServer(a_RCONServer),
	m_IPAddress(a_IPAddress)
{
}





void cRCONServer::cConnection::OnLinkCreated(cTCPLinkPtr a_Link)
{
	m_Link = a_Link;
}





void cRCONServer::cConnection::OnReceivedData(const char * a_Data, size_t a_Size)
{
	ASSERT(m_Link != nullptr);

	// Append data to the buffer:
	m_Buffer.append(a_Data, a_Size);

	// Process the packets in the buffer:
	while (m_Buffer.size() >= 14)
	{
		UInt32 Length = UIntFromBuffer(m_Buffer.data());
		if (Length > 1500)
		{
			// Too long, drop the connection
			LOGWARNING("Received an invalid RCON packet length (%d), dropping RCON connection to %s.",
				Length, m_IPAddress.c_str()
			);
			m_Link->Close();
			m_Link.reset();
			return;
		}
		if (Length > static_cast<UInt32>(m_Buffer.size() + 4))
		{
			// Incomplete packet yet, wait for more data to come
			return;
		}

		UInt32 RequestID  = UIntFromBuffer(m_Buffer.data() + 4);
		UInt32 PacketType = UIntFromBuffer(m_Buffer.data() + 8);
		if (!ProcessPacket(RequestID, PacketType, Length - 10, m_Buffer.data() + 12))
		{
			m_Link->Close();
			m_Link.reset();
			return;
		}
		m_Buffer.erase(0, Length + 4);
	}  // while (m_Buffer.size() >= 14)
}





void cRCONServer::cConnection::OnRemoteClosed(void)
{
	m_Link.reset();
}





void cRCONServer::cConnection::OnError(int a_ErrorCode, const AString & a_ErrorMsg)
{
	LOGD("Error in RCON connection %s: %d (%s)", m_IPAddress.c_str(), a_ErrorCode, a_ErrorMsg.c_str());
	m_Link.reset();
}





bool cRCONServer::cConnection::ProcessPacket(UInt32 a_RequestID, UInt32 a_PacketType, UInt32 a_PayloadLength, const char * a_Payload)
{
	switch (a_PacketType)
	{
		case RCON_PACKET_LOGIN:
		{
			if (strncmp(a_Payload, m_RCONServer.m_Password.c_str(), a_PayloadLength) != 0)
			{
				LOGINFO("RCON: Invalid password from client %s, dropping connection.", m_IPAddress.c_str());
				SendResponse(0xffffffffU, RCON_PACKET_RESPONSE, 0, nullptr);
				return false;
			}
			m_IsAuthenticated = true;

			LOGD("RCON: Client at %s has successfully authenticated", m_IPAddress.c_str());

			// Send OK response:
			SendResponse(a_RequestID, RCON_PACKET_RESPONSE, 0, nullptr);
			return true;
		}

		case RCON_PACKET_COMMAND:
		{
			if (!m_IsAuthenticated)
			{
				char AuthNeeded[] = "You need to authenticate first!";
				SendResponse(a_RequestID, RCON_PACKET_RESPONSE, sizeof(AuthNeeded), AuthNeeded);
				return false;
			}

			AString cmd(a_Payload, a_PayloadLength);
			LOGD("RCON command from %s: \"%s\"", m_IPAddress.c_str(), cmd.c_str());
			cRoot::Get()->QueueExecuteConsoleCommand(cmd, *(new cRCONCommandOutput(*this, a_RequestID)));

			// Send an empty response:
			SendResponse(a_RequestID, RCON_PACKET_RESPONSE, 0, nullptr);
			return true;
		}
	}

	// Unknown packet type, drop the connection:
	LOGWARNING("RCON: Client at %s has sent an unknown packet type %d, dropping connection.",
		m_IPAddress.c_str(), a_PacketType
	);
	return false;
}





UInt32 cRCONServer::cConnection::UIntFromBuffer(const char * a_Buffer)
{
	const Byte * Buffer = reinterpret_cast<const Byte *>(a_Buffer);
	return static_cast<UInt32>((Buffer[3] << 24) | (Buffer[2] << 16) | (Buffer[1] << 8) | Buffer[0]);
}





void cRCONServer::cConnection::UIntToBuffer(UInt32 a_Value, char * a_Buffer)
{
	a_Buffer[0] = static_cast<char>(a_Value & 0xff);
	a_Buffer[1] = static_cast<char>((a_Value >> 8)  & 0xff);
	a_Buffer[2] = static_cast<char>((a_Value >> 16) & 0xff);
	a_Buffer[3] = static_cast<char>((a_Value >> 24) & 0xff);
}





void cRCONServer::cConnection::SendResponse(UInt32 a_RequestID, UInt32 a_PacketType, UInt32 a_PayloadLength, const char * a_Payload)
{
	ASSERT((a_PayloadLength == 0) || (a_Payload != nullptr));  // Either zero data to send, or a valid payload ptr
	ASSERT(m_Link != nullptr);

	char Buffer[12];
	UInt32 Length = a_PayloadLength + 10;
	UIntToBuffer(Length, Buffer);
	UIntToBuffer(a_RequestID, Buffer + 4);
	UIntToBuffer(a_PacketType, Buffer + 8);
	m_Link->Send(Buffer, 12);
	if (a_PayloadLength > 0)
	{
		m_Link->Send(a_Payload, a_PayloadLength);
	}
	m_Link->Send("\0", 2);  // Send two zero chars as the padding
}




