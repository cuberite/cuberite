
// RCONServer.cpp

// Implements the cRCONServer class representing the RCON server

#include "Globals.h"
#include "inifile/iniFile.h"
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
// cRCONCommandOutput:

class cRCONCommandOutput :
	public cCommandOutputCallback
{
public:
	cRCONCommandOutput(cRCONServer::cConnection & a_Connection, int a_RequestID) :
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
		m_Connection.SendResponse(m_RequestID, RCON_PACKET_RESPONSE, (int)m_Buffer.size(), m_Buffer.c_str());
		delete this;
	}
	
protected:
	cRCONServer::cConnection & m_Connection;
	int m_RequestID;
	AString m_Buffer;
} ;





////////////////////////////////////////////////////////////////////////////////
// cRCONServer:

cRCONServer::cRCONServer(cServer & a_Server) :
	m_Server(a_Server),
	m_ListenThread4(*this, cSocket::IPv4, "RCON"),
	m_ListenThread6(*this, cSocket::IPv6, "RCON")
{
}





cRCONServer::~cRCONServer()
{
	m_ListenThread4.Stop();
	m_ListenThread6.Stop();
}





void cRCONServer::Initialize(cIniFile & a_IniFile)
{
	if (!a_IniFile.GetValueSetB("RCON", "Enabled", false))
	{
		return;
	}

	// Read the password, don't allow an empty one:
	m_Password = a_IniFile.GetValueSet("RCON", "Password", "");
	if (m_Password.empty())
	{
		LOGWARNING("RCON is requested, but the password is not set. RCON is now disabled.");
		return;
	}
	
	// Read and initialize both IPv4 and IPv6 ports for RCON
	bool HasAnyPorts = false;
	AString Ports4 = a_IniFile.GetValueSet("RCON", "PortsIPv4", "25575");
	if (m_ListenThread4.Initialize(Ports4))
	{
		HasAnyPorts = true;
		m_ListenThread4.Start();
	}
	AString Ports6 = a_IniFile.GetValueSet("RCON", "PortsIPv6", "25575");
	if (m_ListenThread6.Initialize(Ports6))
	{
		HasAnyPorts = true;
		m_ListenThread6.Start();
	}
	if (!HasAnyPorts)
	{
		LOGWARNING("RCON is requested, but no ports are specified. Specify at least one port in PortsIPv4 or PortsIPv6. RCON is now disabled.");
		return;
	}
}





void cRCONServer::OnConnectionAccepted(cSocket & a_Socket)
{
	if (!a_Socket.IsValid())
	{
		return;
	}

	LOG("RCON Client \"%s\" connected!", a_Socket.GetIPString().c_str());
	
	// Create a new cConnection object, it will be deleted when the connection is closed
	m_SocketThreads.AddClient(a_Socket, new cConnection(*this, a_Socket));
}





////////////////////////////////////////////////////////////////////////////////
// cRCONServer::cConnection:

cRCONServer::cConnection::cConnection(cRCONServer & a_RCONServer, cSocket & a_Socket) :
	m_IsAuthenticated(false),
	m_RCONServer(a_RCONServer),
	m_Socket(a_Socket),
	m_IPAddress(a_Socket.GetIPString())
{
}





bool cRCONServer::cConnection::DataReceived(const char * a_Data, size_t a_Size)
{
	// Append data to the buffer:
	m_Buffer.append(a_Data, a_Size);
	
	// Process the packets in the buffer:
	while (m_Buffer.size() >= 14)
	{
		int Length = IntFromBuffer(m_Buffer.data());
		if (Length > 1500)
		{
			// Too long, drop the connection
			LOGWARNING("Received an invalid RCON packet length (%d), dropping RCON connection to %s.",
				Length, m_IPAddress.c_str()
			);
			m_RCONServer.m_SocketThreads.RemoveClient(this);
			m_Socket.CloseSocket();
			delete this;
			return false;
		}
		if (Length > (int)(m_Buffer.size() + 4))
		{
			// Incomplete packet yet, wait for more data to come
			return false;
		}
		
		int RequestID  = IntFromBuffer(m_Buffer.data() + 4);
		int PacketType = IntFromBuffer(m_Buffer.data() + 8);
		if (!ProcessPacket(RequestID, PacketType, Length - 10, m_Buffer.data() + 12))
		{
			m_RCONServer.m_SocketThreads.RemoveClient(this);
			m_Socket.CloseSocket();
			delete this;
			return false;
		}
		m_Buffer.erase(0, Length + 4);
	}  // while (m_Buffer.size() >= 14)
	return false;
}





void cRCONServer::cConnection::GetOutgoingData(AString & a_Data)
{
	a_Data.assign(m_Outgoing);
	m_Outgoing.clear();
}





void cRCONServer::cConnection::SocketClosed(void)
{
	m_RCONServer.m_SocketThreads.RemoveClient(this);
	delete this;
}





bool cRCONServer::cConnection::ProcessPacket(int a_RequestID, int a_PacketType, int a_PayloadLength, const char * a_Payload)
{
	switch (a_PacketType)
	{
		case RCON_PACKET_LOGIN:
		{
			if (strncmp(a_Payload, m_RCONServer.m_Password.c_str(), a_PayloadLength) != 0)
			{
				LOGINFO("RCON: Invalid password from client %s, dropping connection.", m_IPAddress.c_str());
				SendResponse(-1, RCON_PACKET_RESPONSE, 0, NULL);
				return false;
			}
			m_IsAuthenticated = true;
			
			LOGD("RCON: Client at %s has successfully authenticated", m_IPAddress.c_str());
			
			// Send OK response:
			SendResponse(a_RequestID, RCON_PACKET_RESPONSE, 0, NULL);
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
			cRoot::Get()->ExecuteConsoleCommand(cmd, *(new cRCONCommandOutput(*this, a_RequestID)));
			
			// Send an empty response:
			SendResponse(a_RequestID, RCON_PACKET_RESPONSE, 0, NULL);
			return true;
		}
	}
	
	// Unknown packet type, drop the connection:
	LOGWARNING("RCON: Client at %s has sent an unknown packet type %d, dropping connection.",
		m_IPAddress.c_str(), a_PacketType
	);
	return false;
}





/// Reads 4 bytes from a_Buffer and returns the int they represent
int cRCONServer::cConnection::IntFromBuffer(const char * a_Buffer)
{
	return ((unsigned char)a_Buffer[3] << 24) | ((unsigned char)a_Buffer[2] << 16) | ((unsigned char)a_Buffer[1] << 8) | (unsigned char)a_Buffer[0];
}





/// Puts 4 bytes representing the int into the buffer
void cRCONServer::cConnection::IntToBuffer(int a_Value, char * a_Buffer)
{
	a_Buffer[0] = a_Value & 0xff;
	a_Buffer[1] = (a_Value >> 8)  & 0xff;
	a_Buffer[2] = (a_Value >> 16) & 0xff;
	a_Buffer[3] = (a_Value >> 24) & 0xff;
}





/// Sends a RCON packet back to the client
void cRCONServer::cConnection::SendResponse(int a_RequestID, int a_PacketType, int a_PayloadLength, const char * a_Payload)
{
	ASSERT((a_PayloadLength == 0) || (a_Payload != NULL));  // Either zero data to send, or a valid payload ptr
	
	char Buffer[4];
	int Length = a_PayloadLength + 10;
	IntToBuffer(Length, Buffer);
	m_Outgoing.append(Buffer, 4);
	IntToBuffer(a_RequestID, Buffer);
	m_Outgoing.append(Buffer, 4);
	IntToBuffer(a_PacketType, Buffer);
	m_Outgoing.append(Buffer, 4);
	if (a_PayloadLength > 0)
	{
		m_Outgoing.append(a_Payload, a_PayloadLength);
	}
	m_Outgoing.push_back(0);
	m_Outgoing.push_back(0);
	m_RCONServer.m_SocketThreads.NotifyWrite(this);
}




