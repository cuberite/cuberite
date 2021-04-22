
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
	int ret = 0;
	if (m_Config != nullptr)
	{
		ret = m_Ssl.Initialize(m_Config);
	}
	else
	{
		ret = m_Ssl.Initialize(true);
	}

	if (ret != 0)
	{
		Printf(m_LastErrorText, "SSL initialization failed: -0x%x", -ret);
		return false;
	}

	// If we have been assigned a trusted CA root cert store, push it into the SSL context:
	if (!m_ExpectedPeerName.empty())
	{
		m_Ssl.SetExpectedPeerName(m_ExpectedPeerName);
	}

	ret = m_Ssl.Handshake();
	if (ret != 0)
	{
		Printf(m_LastErrorText, "SSL handshake failed: -0x%x", -ret);
		return false;
	}

	return true;
}





void cBlockingSslClientSocket::SetExpectedPeerName(AString a_ExpectedPeerName)
{
	ASSERT(!m_IsConnected);  // Must be called before connect

	// Warn if used multiple times, but don't signal an error:
	if (!m_ExpectedPeerName.empty())
	{
		LOGWARNING(
			"SSL: Trying to set multiple expected peer names, only the last one will be used. Name: %s",
			a_ExpectedPeerName.c_str()
		);
	}

	m_ExpectedPeerName = std::move(a_ExpectedPeerName);
}





void cBlockingSslClientSocket::SetSslConfig(std::shared_ptr<const cSslConfig> a_Config)
{
	ASSERT(!m_IsConnected);  // Must be called before connect

	// Warn if used multiple times, but don't signal an error:
	if (m_Config != nullptr)
	{
		LOGWARNING("SSL: Trying to set multiple configurations, only the last one will be used.");
	}

	m_Config = std::move(a_Config);
}





bool cBlockingSslClientSocket::Send(const void * a_Data, size_t a_NumBytes)
{
	if (!m_IsConnected)
	{
		m_LastErrorText = "Socket is closed";
		return false;
	}

	// Keep sending the data until all of it is sent:
	const char * Data = static_cast<const char *>(a_Data);
	size_t NumBytes = a_NumBytes;
	for (;;)
	{
		int res = m_Ssl.WritePlain(Data, a_NumBytes);
		if (res < 0)
		{
			ASSERT(res != MBEDTLS_ERR_SSL_WANT_READ);   // This should never happen with callback-based SSL
			ASSERT(res != MBEDTLS_ERR_SSL_WANT_WRITE);  // This should never happen with callback-based SSL
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
	// Even if m_IsConnected is false (socket disconnected), the SSL context may have more data in the queue
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
	m_IsConnected = false;

	// Grab a copy of the socket so that we know it doesn't change under our hands:
	auto socket = m_Socket;
	if (socket != nullptr)
	{
		socket->Close();
	}

	m_Socket.reset();
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
		return MBEDTLS_ERR_NET_RECV_FAILED;
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
		return MBEDTLS_ERR_NET_SEND_FAILED;
	}
	if (!Socket->Send(a_Buffer, a_NumBytes))
	{
		// mbedTLS's net routines distinguish between connection reset and general failure, we don't need to
		return MBEDTLS_ERR_NET_SEND_FAILED;
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
	LOG("Cannot connect to %s: \"%s\"", m_ServerName.c_str(), a_ErrorMsg.c_str());
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
	m_Socket = std::move(a_Link);
}





void cBlockingSslClientSocket::OnDisconnected(void)
{
	m_IsConnected = false;
	m_Socket.reset();
	m_Event.Set();
}




