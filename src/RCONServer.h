
// RCONServer.h

// Declares the cRCONServer class representing the RCON server





#pragma once

#include "OSSupport/Network.h"





// fwd:
class cServer;
class cSettingsRepositoryInterface;





class cRCONServer
{
public:
	cRCONServer(cServer & a_Server);
	virtual ~cRCONServer();

	void Initialize(cSettingsRepositoryInterface & a_Settings);

protected:
	friend class cRCONCommandOutput;
	friend class cRCONListenCallbacks;

	class cConnection :
		public cTCPLink::cCallbacks
	{
	public:
		cConnection(cRCONServer & a_RCONServer, const AString & a_IPAddress);

	protected:
		friend class cRCONCommandOutput;

		/** Set to true if the client has successfully authenticated */
		bool m_IsAuthenticated;

		/** Buffer for the incoming data */
		AString m_Buffer;

		/** Server that owns this connection and processes requests */
		cRCONServer & m_RCONServer;

		/** The TCP link to the client */
		cTCPLinkPtr m_Link;

		/** Address of the client */
		AString m_IPAddress;


		// cTCPLink::cCallbacks overrides:
		virtual void OnLinkCreated(cTCPLinkPtr a_Link) override;
		virtual void OnReceivedData(const char * a_Data, size_t a_Length) override;
		virtual void OnRemoteClosed(void) override;
		virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override;

		/** Processes the given packet and sends the response; returns true if successful, false if the connection is to be dropped */
		bool ProcessPacket(UInt32 a_RequestID, UInt32 a_PacketType, UInt32 a_PayloadLength, const char * a_Payload);

		/** Reads 4 bytes from a_Buffer and returns the LE UInt32 they represent */
		UInt32 UIntFromBuffer(const char * a_Buffer);

		/** Puts 4 bytes representing the int into the buffer */
		void UIntToBuffer(UInt32 a_Value, char * a_Buffer);

		/** Sends a RCON packet back to the client */
		void SendResponse(UInt32 a_RequestID, UInt32 a_PacketType, UInt32 a_PayloadLength, const char * a_Payload);
	} ;


	/** The server object that will process the commands received */
	cServer & m_Server;

	/** The sockets for accepting RCON connections (one socket per port). */
	cServerHandlePtrs m_ListenServers;

	/** Password for authentication */
	AString m_Password;
} ;





