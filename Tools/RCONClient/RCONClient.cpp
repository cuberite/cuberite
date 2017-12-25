
// RCONClient.cpp

// Implements the main app entrypoint

#include "Globals.h"
#include "OSSupport/Socket.h"
#include "ByteBuffer.h"





// If set to true, verbose messages are output to stderr. Use the "-v" or "--verbose" param to turn on
bool g_IsVerbose = false;





/** This class can read and write RCON packets to / from a connected socket */
class cRCONPacketizer
{
public:
	enum
	{
		ptCommand = 2,
		ptLogin = 3,
	} ;

	cRCONPacketizer(cSocket & a_Socket);

	/** Sends the packet to the socket and waits until the response is received.
	Returns true if response successfully received, false if the client disconnected or protocol error.
	Dumps the reply payload to stdout. */
	bool SendPacket(int a_PacketType, const AString & a_PacketPayload);

protected:
	/** The socket to use for reading incoming data and writing outgoing data: */
	cSocket & m_Socket;

	/** The RequestID of the packet that is being sent. Incremented when the reply is received */
	int m_RequestID;

	/** Receives the full response and dumps its payload to stdout.
	Returns true if successful, false if the client disconnected or protocol error. */
	bool ReceiveResponse(void);

	/** Parses the received response packet and dumps its payload to stdout.
	Returns true if successful, false on protocol error
	Assumes that the packet length has already been read from the packet
	If the packet is successfully parsed, increments m_RequestID */
	bool ParsePacket(cByteBuffer & a_Buffer, int a_PacketLength);
} ;





cRCONPacketizer::cRCONPacketizer(cSocket & a_Socket) :
	m_Socket(a_Socket),
	m_RequestID(0)
{
}





bool cRCONPacketizer::SendPacket(int a_PacketType, const AString & a_PacketPayload)
{
	// Send the packet:
	cByteBuffer bb(a_PacketPayload.size() + 30);
	bb.WriteLEInt(m_RequestID);
	bb.WriteLEInt(a_PacketType);
	bb.WriteBuf(a_PacketPayload.data(), a_PacketPayload.size());
	bb.WriteBEShort(0);  // Padding
	AString Packet;
	bb.ReadAll(Packet);
	size_t Length = Packet.size();
	if (!m_Socket.Send((const char *)&Length, 4))
	{
		fprintf(stderr, "Network error while sending packet: %d (%s). Aborting.\n",
			cSocket::GetLastError(), cSocket::GetLastErrorString().c_str()
		);
		return false;
	}
	if (!m_Socket.Send(Packet.data(), Packet.size()))
	{
		fprintf(stderr, "Network error while sending packet: %d (%s). Aborting.\n",
			cSocket::GetLastError(), cSocket::GetLastErrorString().c_str()
		);
		return false;
	}

	return ReceiveResponse();
}





bool cRCONPacketizer::ReceiveResponse(void)
{
	// Receive the response:
	cByteBuffer Buffer(64 KiB);
	while (true)
	{
		char buf[1024];
		int NumReceived = m_Socket.Receive(buf, sizeof(buf), 0);
		if (NumReceived == 0)
		{
			fprintf(stderr, "The remote end closed the connection. Aborting.\n");
			return false;
		}
		if (NumReceived < 0)
		{
			fprintf(stderr, "Network error while receiving response: %d, %d (%s). Aborting.\n",
				NumReceived, cSocket::GetLastError(), cSocket::GetLastErrorString().c_str()
			);
			return false;
		}
		Buffer.Write(buf, NumReceived);
		Buffer.ResetRead();

		// Check if the buffer contains the full packet:
		if (!Buffer.CanReadBytes(14))
		{
			// 14 is the minimum packet size for RCON
			continue;
		}
		int PacketSize;
		VERIFY(Buffer.ReadLEInt(PacketSize));
		if (!Buffer.CanReadBytes(PacketSize))
		{
			// The packet is not complete yet
			continue;
		}

		// Parse the packet
		return ParsePacket(Buffer, PacketSize);
	}
}





bool cRCONPacketizer::ParsePacket(cByteBuffer & a_Buffer, int a_PacketLength)
{
	// Check that the request ID is equal
	bool IsValid = true;
	int RequestID = 0;
	VERIFY(a_Buffer.ReadLEInt(RequestID));
	if (RequestID != m_RequestID)
	{
		if ((RequestID == -1) && (m_RequestID == 0))
		{
			fprintf(stderr, "Login failed. Aborting.\n");
			IsValid = false;
			// Continue, so that the payload is printed before the program aborts.
		}
		else
		{
			fprintf(stderr, "The server returned an invalid request ID, got %d, exp. %d. Aborting.\n", RequestID, m_RequestID);
			return false;
		}
	}

	// Check the packet type:
	int PacketType = 0;
	VERIFY(a_Buffer.ReadLEInt(PacketType));
	if (PacketType != ptCommand)
	{
		fprintf(stderr, "The server returned an unknown packet type: %d. Aborting.\n", PacketType);
		IsValid = false;
		// Continue, so that the payload is printed before the program aborts.
	}

	AString Payload;
	VERIFY(a_Buffer.ReadString(Payload, a_PacketLength - 10));

	// Dump the payload to stdout, in a binary mode
	fwrite(Payload.data(), Payload.size(), 1, stdout);

	if (IsValid)
	{
		m_RequestID++;
		return true;
	}
	return false;
}





////////////////////////////////////////////////////////////////////////////////
// main:

int RealMain(int argc, char * argv[])
{
	cLogger::InitiateMultithreading();

	// Parse the cmdline params for server IP, port, password and the commands to send:
	AString ServerAddress, Password;
	int ServerPort = -1;
	AStringVector Commands;
	for (int i = 1; i < argc; i++)
	{
		if (((NoCaseCompare(argv[i], "-s") == 0) || (NoCaseCompare(argv[i], "--server") == 0)) && (i < argc - 1))
		{
			ServerAddress = argv[i + 1];
			i++;
			continue;
		}
		if (((NoCaseCompare(argv[i], "-p") == 0) || (NoCaseCompare(argv[i], "--port") == 0)) && (i < argc - 1))
		{
			ServerPort = atoi(argv[i + 1]);
			i++;
			continue;
		}
		if (((NoCaseCompare(argv[i], "-w") == 0) || (NoCaseCompare(argv[i], "--password") == 0)) && (i < argc - 1))
		{
			Password = argv[i + 1];
			i++;
			continue;
		}
		if (((NoCaseCompare(argv[i], "-c") == 0) || (NoCaseCompare(argv[i], "--cmd") == 0) || (NoCaseCompare(argv[i], "--command") == 0)) && (i < argc - 1))
		{
			Commands.push_back(argv[i + 1]);
			i++;
			continue;
		}
		if (((NoCaseCompare(argv[i], "-f") == 0) || (NoCaseCompare(argv[i], "--file") == 0)) && (i < argc - 1))
		{
			i++;
			cFile f(argv[i], cFile::fmRead);
			if (!f.IsOpen())
			{
				fprintf(stderr, "Cannot read commands from file \"%s\", aborting.", argv[i]);
				return 2;
			}
			AString cmd;
			f.ReadRestOfFile(cmd);
			Commands.push_back(cmd);
			continue;
		}
		if ((NoCaseCompare(argv[i], "-v") == 0) || (NoCaseCompare(argv[i], "--verbose") == 0))
		{
			fprintf(stderr, "Verbose output enabled\n");
			g_IsVerbose = true;
			continue;
		}
		fprintf(stderr, "Unknown parameter: \"%s\". Aborting.", argv[i]);
		return 1;
	}  // for i - argv[]

	if (ServerAddress.empty() || (ServerPort < 0))
	{
		fprintf(stderr, "Server address or port not set. Use the --server and --port parameters to set them. Aborting.");
		return 1;
	}

	// Connect:
	if (cSocket::WSAStartup() != 0)
	{
		fprintf(stderr, "Cannot initialize network stack. Aborting\n");
		return 6;
	}
	if (g_IsVerbose)
	{
		fprintf(stderr, "Connecting to \"%s:%d\"...\n", ServerAddress.c_str(), ServerPort);
	}
	cSocket s = cSocket::CreateSocket(cSocket::IPv4);
	if (!s.ConnectIPv4(ServerAddress, (unsigned short)ServerPort))
	{
		fprintf(stderr, "Cannot connect to \"%s:%d\": %s\n", ServerAddress.c_str(), ServerPort, cSocket::GetLastErrorString().c_str());
		return 3;
	}
	cRCONPacketizer Packetizer(s);

	// Authenticate using the provided password:
	if (!Password.empty())
	{
		if (g_IsVerbose)
		{
			fprintf(stderr, "Sending the login packet...\n");
		}
		if (!Packetizer.SendPacket(cRCONPacketizer::ptLogin, Password))
		{
			// Error message has already been printed, bail out
			return 4;
		}
	}
	else
	{
		if (g_IsVerbose)
		{
			fprintf(stderr, "No password provided, not sending a login packet.\n");
		}
	}

	// Send each command:
	for (AStringVector::const_iterator itr = Commands.begin(), end = Commands.end(); itr != end; ++itr)
	{
		if (g_IsVerbose)
		{
			fprintf(stderr, "Sending command \"%s\"...\n", itr->c_str());
		}
		if (!Packetizer.SendPacket(cRCONPacketizer::ptCommand, *itr))
		{
			return 5;
		}
	}

	return 0;
}





int main(int argc, char * argv[])
{
	// This redirection function is only so that debugging the program is easier in MSVC - when RealMain exits, it's still possible to place a breakpoint
	int res = RealMain(argc, argv);
	return res;
}
