
// SslHTTPConnection.cpp

// Implements the cSslHTTPConnection class representing a HTTP connection made over a SSL link

#include "Globals.h"
#include "SslHTTPConnection.h"
#include "HTTPServer.h"





cSslHTTPConnection::cSslHTTPConnection(cHTTPServer & a_HTTPServer, const cX509CertPtr & a_Cert, const cCryptoKeyPtr & a_PrivateKey) :
	super(a_HTTPServer),
	m_Ssl(64000),
	m_Cert(a_Cert),
	m_PrivateKey(a_PrivateKey)
{
	m_Ssl.Initialize(false);
	m_Ssl.SetOwnCert(a_Cert, a_PrivateKey);
}





bool cSslHTTPConnection::DataReceived(const char * a_Data, size_t a_Size)
{
	// If there is outgoing data in the queue, notify the server that it should write it out:
	if (!m_OutgoingData.empty())
	{
		m_HTTPServer.NotifyConnectionWrite(*this);
	}
	
	// Process the received data:
	const char * Data = a_Data;
	size_t Size = a_Size;
	for (;;)
	{
		// Try to write as many bytes into Ssl's "incoming" buffer as possible:
		size_t BytesWritten = 0;
		if (Size > 0)
		{
			BytesWritten = m_Ssl.WriteIncoming(Data, Size);
			Data += BytesWritten;
			Size -= BytesWritten;
		}
		
		// Try to read as many bytes from SSL's decryption as possible:
		char Buffer[32000];
		int NumRead = m_Ssl.ReadPlain(Buffer, sizeof(Buffer));
		if (NumRead > 0)
		{
			if (super::DataReceived(Buffer, (size_t)NumRead))
			{
				// The socket has been closed, and the object is already deleted. Bail out.
				return true;
			}
		}
		
		// If both failed, bail out:
		if ((BytesWritten == 0) && (NumRead <= 0))
		{
			return false;
		}
	}
}





void cSslHTTPConnection::GetOutgoingData(AString & a_Data)
{
	for (;;)
	{
		// Write as many bytes from our buffer to SSL's encryption as possible:
		int NumWritten = 0;
		if (!m_OutgoingData.empty())
		{
			NumWritten = m_Ssl.WritePlain(m_OutgoingData.data(), m_OutgoingData.size());
			if (NumWritten > 0)
			{
				m_OutgoingData.erase(0, (size_t)NumWritten);
			}
		}
		
		// Read as many bytes from SSL's "outgoing" buffer as possible:
		char Buffer[32000];
		size_t NumBytes = m_Ssl.ReadOutgoing(Buffer, sizeof(Buffer));
		if (NumBytes > 0)
		{
			a_Data.append(Buffer, NumBytes);
		}
		
		// If both failed, bail out:
		if ((NumWritten <= 0) && (NumBytes == 0))
		{
			return;
		}
	}
}




