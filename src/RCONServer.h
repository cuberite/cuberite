
// RCONServer.h

// Declares the cRCONServer class representing the RCON server





#pragma once

#include "OSSupport/SocketThreads.h"
#include "OSSupport/ListenThread.h"





// fwd:
class cServer;
class cIniFile;





class cRCONServer :
	public cListenThread::cCallback
{
public:
	cRCONServer(cServer & a_Server);
	virtual ~cRCONServer();
	
	void Initialize(cIniFile & a_IniFile);
	
protected:
	friend class cRCONCommandOutput;
	
	class cConnection :
		public cSocketThreads::cCallback
	{
	public:
		cConnection(cRCONServer & a_RCONServer, cSocket & a_Socket);
		
	protected:
		friend class cRCONCommandOutput;
	
		/// Set to true if the client has successfully authenticated
		bool m_IsAuthenticated;
		
		/// Buffer for the incoming data
		AString m_Buffer;
		
		/// Buffer for the outgoing data
		AString m_Outgoing;
		
		/// Server that owns this connection and processes requests
		cRCONServer & m_RCONServer;
		
		/// The socket belonging to the client
		cSocket & m_Socket;
		
		/// Address of the client
		AString m_IPAddress;
		

		// cSocketThreads::cCallback overrides:
		virtual bool DataReceived(const char * a_Data, size_t a_Size) override;
		virtual void GetOutgoingData(AString & a_Data) override;
		virtual void SocketClosed(void) override;
		
		/// Processes the given packet and sends the response; returns true if successful, false if the connection is to be dropped
		bool ProcessPacket(int a_RequestID, int a_PacketType, int a_PayloadLength, const char * a_Payload);
		
		/// Reads 4 bytes from a_Buffer and returns the int they represent
		int IntFromBuffer(const char * a_Buffer);
		
		/// Puts 4 bytes representing the int into the buffer
		void IntToBuffer(int a_Value, char * a_Buffer);
		
		/// Sends a RCON packet back to the client
		void SendResponse(int a_RequestID, int a_PacketType, int a_PayloadLength, const char * a_Payload);
	} ;
	
	
	/// The server object that will process the commands received
	cServer & m_Server;
	
	/// The thread(s) that take care of all the traffic on the RCON ports
	cSocketThreads m_SocketThreads;
	
	/// The thread for accepting IPv4 RCON connections
	cListenThread  m_ListenThread4;
	
	/// The thread for accepting IPv6 RCON connections
	cListenThread  m_ListenThread6;
	
	/// Password for authentication
	AString m_Password;


	// cListenThread::cCallback overrides:
	virtual void OnConnectionAccepted(cSocket & a_Socket) override;
} ;





