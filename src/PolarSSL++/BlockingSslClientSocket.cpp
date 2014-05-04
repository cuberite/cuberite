
// BlockingSslClientSocket.cpp

// Implements the cBlockingSslClientSocket class representing a blocking TCP socket with client SSL encryption over it

#include "Globals.h"
#include "BlockingSslClientSocket.h"





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
	m_Socket.CreateSocket(cSocket::IPv4);
	if (!m_Socket.ConnectIPv4(a_ServerName.c_str(), a_Port))
	{
		Printf(m_LastErrorText, "Socket connect failed: %s", m_Socket.GetLastErrorString().c_str());
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
	if (m_CACerts.get() != NULL)
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
	if (m_CACerts.get() != NULL)
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
	const char * Data = (const char *)a_Data;
	size_t NumBytes = a_NumBytes;
	for (;;)
	{
		int res = m_Ssl.WritePlain(a_Data, a_NumBytes);
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
			NumBytes -= res;
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
	m_Socket.CloseSocket();
	m_IsConnected = false;
}





int cBlockingSslClientSocket::ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes)
{
	int res = m_Socket.Receive((char *)a_Buffer, a_NumBytes, 0);
	if (res < 0)
	{
		// PolarSSL's net routines distinguish between connection reset and general failure, we don't need to
		return POLARSSL_ERR_NET_RECV_FAILED;
	}
	return res;
}





int cBlockingSslClientSocket::SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes)
{
	int res = m_Socket.Send((const char *)a_Buffer, a_NumBytes);
	if (res < 0)
	{
		// PolarSSL's net routines distinguish between connection reset and general failure, we don't need to
		return POLARSSL_ERR_NET_SEND_FAILED;
	}
	return res;
}




