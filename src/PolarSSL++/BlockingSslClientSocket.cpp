
// BlockingSslClientSocket.cpp

// Implements the cBlockingSslClientSocket class representing a blocking TCP socket with client SSL encryption over it

#include "Globals.h"
#include "BlockingSslClientSocket.h"





////////////////////////////////////////////////////////////////////////////////
// cBlockingSslClientSocketConnectCallbacks:

class cBlockingSslClientSocketConnectCallbacks:
	public cNetwork::cConnectCallbacks
{
	/** The socket object that is using this instance of the callbacks. */
	cBlockingSslClientSocket & m_Socket;

	virtual void OnConnected(cTCPLink & a_Link) override
	{
		m_Socket.OnConnected();
	}

	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override
	{
		m_Socket.OnConnectError(a_ErrorMsg);
	}

public:
	cBlockingSslClientSocketConnectCallbacks(cBlockingSslClientSocket & a_Socket):
		m_Socket(a_Socket)
	{
	}
};





////////////////////////////////////////////////////////////////////////////////
// cBlockingSslClientSocketLinkCallbacks:

class cBlockingSslClientSocketLinkCallbacks:
	public cTCPLink::cCallbacks
{
	cBlockingSslClientSocket & m_Socket;

	virtual void OnLinkCreated(cTCPLinkPtr a_Link) override
	{
		m_Socket.SetLink(a_Link);
	}


	virtual void OnReceivedData(const char * a_Data, size_t a_Length) override
	{
		m_Socket.OnReceivedData(a_Data, a_Length);
	}


	virtual void OnRemoteClosed(void) override
	{
		m_Socket.OnDisconnected();
	}


	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override
	{
		m_Socket.OnDisconnected();
	}
public:
	cBlockingSslClientSocketLinkCallbacks(cBlockingSslClientSocket & a_Socket):
		m_Socket(a_Socket)
	{
	}
};





////////////////////////////////////////////////////////////////////////////////
// cBlockingSslClientSocket:

cBlockingSslClientSocket::cBlockingSslClientSocket(void) :
	m_Ssl(*this),
	m_IsConnected(false)
{
	// Nothing needed yet
}





bool cBlockingSslClientSocket::Connect(const AString & a_ServerName, UInt16 a_Port)
{
	// If already connected, report an error:
	if (m_IsConnected)
	{
		// TODO: Handle this better - if connected to the same server and port, and the socket is alive, return success
		m_LastErrorText = "Already connected";
		return false;
	}
	
	// Connect the underlying socket:
	m_ServerName = a_ServerName;
	if (!cNetwork::Connect(a_ServerName, a_Port,
		std::make_shared<cBlockingSslClientSocketConnectCallbacks>(*this),
		std::make_shared<cBlockingSslClientSocketLinkCallbacks>(*this))
	)
	{
		return false;
	}

	// Wait for the connection to succeed or fail:
	m_Event.Wait();
	if (!m_IsConnected)
	{
		return false;
	}
	
	// Initialize the SSL:
	int ret = m_Ssl.Initialize(true);
	if (ret != 0)
	{
		Printf(m_LastErrorText, "SSL initialization failed: -0x%x", -ret);
		return false;
	}
	
	// If we have been assigned a trusted CA root cert store, push it into the SSL context:
	if (m_CACerts.get() != nullptr)
	{
		m_Ssl.SetCACerts(m_CACerts, m_ExpectedPeerName);
	}
	
	ret = m_Ssl.Handshake();
	if (ret != 0)
	{
		Printf(m_LastErrorText, "SSL handshake failed: -0x%x", -ret);
		return false;
	}
	
	m_IsConnected = true;
	return true;
}






bool cBlockingSslClientSocket::SetTrustedRootCertsFromString(const AString & a_CACerts, const AString & a_ExpectedPeerName)
{
	// Warn if used multiple times, but don't signal an error:
	if (m_CACerts.get() != nullptr)
	{
		LOGWARNING(
			"SSL: Trying to set multiple trusted CA root cert stores, only the last one will be used. Name: %s",
			a_ExpectedPeerName.c_str()
		);
	}
	
	// Parse the cert:
	m_CACerts.reset(new cX509Cert);
	int ret = m_CACerts->Parse(a_CACerts.data(), a_CACerts.size());
	if (ret < 0)
	{
		Printf(m_LastErrorText, "CA cert parsing failed: -0x%x", -ret);
		return false;
	}
	m_ExpectedPeerName = a_ExpectedPeerName;
	
	return true;
}





bool cBlockingSslClientSocket::Send(const void * a_Data, size_t a_NumBytes)
{
	ASSERT(m_IsConnected);
	
	// Keep sending the data until all of it is sent:
	const char * Data = reinterpret_cast<const char *>(a_Data);
	size_t NumBytes = a_NumBytes;
	for (;;)
	{
		int res = m_Ssl.WritePlain(Data, a_NumBytes);
		if (res < 0)
		{
			ASSERT(res != POLARSSL_ERR_NET_WANT_READ);   // This should never happen with callback-based SSL
			ASSERT(res != POLARSSL_ERR_NET_WANT_WRITE);  // This should never happen with callback-based SSL
			Printf(m_LastErrorText, "Data cannot be written to SSL context: -0x%x", -res);
			return false;
		}
		else
		{
			Data += res;
			NumBytes -= static_cast<size_t>(res);
			if (NumBytes == 0)
			{
				return true;
			}
		}
	}
}






int cBlockingSslClientSocket::Receive(void * a_Data, size_t a_MaxBytes)
{
	ASSERT(m_IsConnected);
	
	int res = m_Ssl.ReadPlain(a_Data, a_MaxBytes);
	if (res < 0)
	{
		Printf(m_LastErrorText, "Data cannot be read form SSL context: -0x%x", -res);
	}
	return res;
}





void cBlockingSslClientSocket::Disconnect(void)
{
	// Ignore if not connected
	if (!m_IsConnected)
	{
		return;
	}
	
	m_Ssl.NotifyClose();
	m_Socket->Close();
	m_Socket.reset();
	m_IsConnected = false;
}





int cBlockingSslClientSocket::ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes)
{
	// Wait for any incoming data, if there is none:
	cCSLock Lock(m_CSIncomingData);
	while (m_IsConnected && m_IncomingData.empty())
	{
		cCSUnlock Unlock(Lock);
		m_Event.Wait();
	}

	// If we got disconnected, report an error after processing all data:
	if (!m_IsConnected && m_IncomingData.empty())
	{
		return POLARSSL_ERR_NET_RECV_FAILED;
	}

	// Copy the data from the incoming buffer into the specified space:
	size_t NumToCopy = std::min(a_NumBytes, m_IncomingData.size());
	memcpy(a_Buffer, m_IncomingData.data(), NumToCopy);
	m_IncomingData.erase(0, NumToCopy);
	return static_cast<int>(NumToCopy);
}





int cBlockingSslClientSocket::SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes)
{
	cTCPLinkPtr Socket(m_Socket);  // Make a copy so that multiple threads don't race on deleting the socket.
	if (Socket == nullptr)
	{
		return POLARSSL_ERR_NET_SEND_FAILED;
	}
	if (!Socket->Send(a_Buffer, a_NumBytes))
	{
		// PolarSSL's net routines distinguish between connection reset and general failure, we don't need to
		return POLARSSL_ERR_NET_SEND_FAILED;
	}
	return static_cast<int>(a_NumBytes);
}




void cBlockingSslClientSocket::OnConnected(void)
{
	m_IsConnected = true;
	m_Event.Set();
}





void cBlockingSslClientSocket::OnConnectError(const AString & a_ErrorMsg)
{
	LOG("Cannot connect to %s: %s", m_ServerName.c_str(), a_ErrorMsg.c_str());
	m_Event.Set();
}





void cBlockingSslClientSocket::OnReceivedData(const char * a_Data, size_t a_Size)
{
	{
		cCSLock Lock(m_CSIncomingData);
		m_IncomingData.append(a_Data, a_Size);
	}
	m_Event.Set();
}





void cBlockingSslClientSocket::SetLink(cTCPLinkPtr a_Link)
{
	m_Socket = a_Link;
}





void cBlockingSslClientSocket::OnDisconnected(void)
{
	m_Socket.reset();
	m_IsConnected = false;
	m_Event.Set();
}




