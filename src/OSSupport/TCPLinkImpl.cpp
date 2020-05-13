
// TCPLinkImpl.cpp

// Implements the cTCPLinkImpl class implementing the TCP link functionality

#include "Globals.h"
#include "TCPLinkImpl.h"
#include "../mbedTLS++/SslConfig.h"
#include "NetworkSingleton.h"
#include "ServerHandleImpl.h"
#include "event2/buffer.h"





////////////////////////////////////////////////////////////////////////////////
// cTCPLinkImpl:

cTCPLinkImpl::cTCPLinkImpl(cTCPLink::cCallbacksPtr a_LinkCallbacks):
	Super(std::move(a_LinkCallbacks)),
	m_BufferEvent(bufferevent_socket_new(cNetworkSingleton::Get().GetEventBase(), -1, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE | BEV_OPT_DEFER_CALLBACKS | BEV_OPT_UNLOCK_CALLBACKS)),
	m_LocalPort(0),
	m_RemotePort(0),
	m_ShouldShutdown(false)
{
}





cTCPLinkImpl::cTCPLinkImpl(evutil_socket_t a_Socket, cTCPLink::cCallbacksPtr a_LinkCallbacks, cServerHandleImplPtr a_Server, const sockaddr * a_Address, socklen_t a_AddrLen):
	Super(std::move(a_LinkCallbacks)),
	m_BufferEvent(bufferevent_socket_new(cNetworkSingleton::Get().GetEventBase(), a_Socket, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE | BEV_OPT_DEFER_CALLBACKS | BEV_OPT_UNLOCK_CALLBACKS)),
	m_Server(std::move(a_Server)),
	m_LocalPort(0),
	m_RemotePort(0),
	m_ShouldShutdown(false)
{
	// Update the endpoint addresses:
	UpdateLocalAddress();
	UpdateAddress(a_Address, a_AddrLen, m_RemoteIP, m_RemotePort);
}





cTCPLinkImpl::~cTCPLinkImpl()
{
	// If the TLS context still exists, free it:
	m_TlsContext.reset();

	bufferevent_free(m_BufferEvent);
}





cTCPLinkImplPtr cTCPLinkImpl::Connect(const AString & a_Host, UInt16 a_Port, cTCPLink::cCallbacksPtr a_LinkCallbacks, cNetwork::cConnectCallbacksPtr a_ConnectCallbacks)
{
	ASSERT(a_LinkCallbacks != nullptr);
	ASSERT(a_ConnectCallbacks != nullptr);

	// Create a new link:
	cTCPLinkImplPtr res{new cTCPLinkImpl(std::move(a_LinkCallbacks))};  // Cannot use std::make_shared here, constructor is not accessible
	res->m_ConnectCallbacks = std::move(a_ConnectCallbacks);
	cNetworkSingleton::Get().AddLink(res);
	res->m_Callbacks->OnLinkCreated(res);
	res->Enable(res);

	// Callback to connect after performing lookup:
	class cHostnameCallback :
		public cNetwork::cResolveNameCallbacks
	{
		cTCPLinkImplPtr m_Link;
		UInt16 m_Port;
		bool m_IsConnecting;

	public:

		cHostnameCallback(cTCPLinkImplPtr a_Link, UInt16 a_ConnectPort):
			m_Link(std::move(a_Link)),
			m_Port(a_ConnectPort),
			m_IsConnecting(false)
		{
		}

		void DoConnect(const sockaddr * a_IP, int size)
		{
			// Make sure connect is only completed once
			if (!m_IsConnecting)
			{
				int ErrCode = bufferevent_socket_connect(m_Link->m_BufferEvent, a_IP, size);
				if (ErrCode == 0)
				{
					m_IsConnecting = true;
				}
				else
				{
					m_Link->GetCallbacks()->OnError(ErrCode, evutil_socket_error_to_string(ErrCode));
				}
			}
		}

		virtual bool OnNameResolvedV4(const AString & a_Name, const sockaddr_in * a_IP) override
		{
			sockaddr_in Addr = *a_IP;
			Addr.sin_port = htons(m_Port);
			DoConnect(reinterpret_cast<const sockaddr *>(&Addr), sizeof(Addr));
			return false;  // Don't care about recieving ip as string
		}

		virtual bool OnNameResolvedV6(const AString & a_Name, const sockaddr_in6 * a_IP) override
		{
			sockaddr_in6 Addr = *a_IP;
			Addr.sin6_port = htons(m_Port);
			DoConnect(reinterpret_cast<const sockaddr *>(&Addr), sizeof(Addr));
			return false;  // Don't care about recieving ip as string
		}

		virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override
		{
			m_Link->GetCallbacks()->OnError(a_ErrorCode, a_ErrorMsg);
			cNetworkSingleton::Get().RemoveLink(m_Link.get());
		}

		// Don't need to do anything for these
		virtual void OnFinished() override
		{
		}

		virtual void OnNameResolved(const AString & a_Name, const AString & a_IP) override
		{
		}
	};

	// Schedule the host query
	cNetwork::HostnameToIP(a_Host, std::make_shared<cHostnameCallback>(res, a_Port));
	return res;
}





void cTCPLinkImpl::Enable(cTCPLinkImplPtr a_Self)
{
	// Take hold of a shared copy of self, to keep as long as the callbacks are coming:
	m_Self = std::move(a_Self);

	// Set the LibEvent callbacks and enable processing:
	bufferevent_setcb(m_BufferEvent, ReadCallback, WriteCallback, EventCallback, this);
	bufferevent_enable(m_BufferEvent, EV_READ | EV_WRITE);
}





bool cTCPLinkImpl::Send(const void * a_Data, size_t a_Length)
{
	if (m_ShouldShutdown)
	{
		LOGD("%s: Cannot send data, the link is already shut down.", __FUNCTION__);
		return false;
	}

	// If running in TLS mode, push the data into the TLS context instead:
	if (m_TlsContext != nullptr)
	{
		m_TlsContext->Send(a_Data, a_Length);
		return true;
	}

	// Send the data:
	return SendRaw(a_Data, a_Length);
}





void cTCPLinkImpl::Shutdown(void)
{
	// If running in TLS mode, notify the TLS layer:
	if (m_TlsContext != nullptr)
	{
		m_TlsContext->NotifyClose();
		m_TlsContext->ResetSelf();
		m_TlsContext.reset();
	}

	// If there's no outgoing data, shutdown the socket directly:
	if (evbuffer_get_length(bufferevent_get_output(m_BufferEvent)) == 0)
	{
		DoActualShutdown();
		return;
	}

	// There's still outgoing data in the LibEvent buffer, schedule a shutdown when it's written to OS's TCP stack:
	m_ShouldShutdown = true;
}





void cTCPLinkImpl::Close(void)
{
	// If running in TLS mode, notify the TLS layer:
	if (m_TlsContext != nullptr)
	{
		m_TlsContext->NotifyClose();
		m_TlsContext->ResetSelf();
		m_TlsContext.reset();
	}

	// Disable all events on the socket, but keep it alive:
	bufferevent_disable(m_BufferEvent, EV_READ | EV_WRITE);
	if (m_Server == nullptr)
	{
		cNetworkSingleton::Get().RemoveLink(this);
	}
	else
	{
		m_Server->RemoveLink(this);
	}
	m_Self.reset();
}





AString cTCPLinkImpl::StartTLSClient(
	cX509CertPtr a_OwnCert,
	cCryptoKeyPtr a_OwnPrivKey
)
{
	// Check preconditions:
	if (m_TlsContext != nullptr)
	{
		return "TLS is already active on this link";
	}
	if ((a_OwnCert == nullptr) != (a_OwnPrivKey == nullptr))
	{
		return "Either provide both the certificate and private key, or neither";
	}

	// Create the TLS context:
	m_TlsContext = std::make_shared<cLinkTlsContext>(*this);
	if (a_OwnCert != nullptr)
	{
		auto Config = cSslConfig::MakeDefaultConfig(true);
		Config->SetOwnCert(std::move(a_OwnCert), std::move(a_OwnPrivKey));
		m_TlsContext->Initialize(Config);
	}
	else
	{
		m_TlsContext->Initialize(true);
	}

	m_TlsContext->SetSelf(cLinkTlsContextWPtr(m_TlsContext));

	// Start the handshake:
	m_TlsContext->Handshake();
	return {};
}





AString cTCPLinkImpl::StartTLSServer(
	cX509CertPtr a_OwnCert,
	cCryptoKeyPtr a_OwnPrivKey,
	const AString & a_StartTLSData
)
{
	// Check preconditions:
	if (m_TlsContext != nullptr)
	{
		return "TLS is already active on this link";
	}
	if ((a_OwnCert == nullptr) || (a_OwnPrivKey == nullptr))
	{
		return "Provide the server certificate and private key";
	}

	// Create the TLS context:
	m_TlsContext = std::make_shared<cLinkTlsContext>(*this);
	{
		auto Config = cSslConfig::MakeDefaultConfig(false);
		Config->SetOwnCert(a_OwnCert, a_OwnPrivKey);
		m_TlsContext->Initialize(std::move(Config));
	}
	m_TlsContext->SetSelf(cLinkTlsContextWPtr(m_TlsContext));

	// Push the initial data:
	m_TlsContext->StoreReceivedData(a_StartTLSData.data(), a_StartTLSData.size());

	// Start the handshake:
	m_TlsContext->Handshake();
	return {};
}





void cTCPLinkImpl::ReadCallback(bufferevent * a_BufferEvent, void * a_Self)
{
	ASSERT(a_Self != nullptr);
	cTCPLinkImpl * Self = static_cast<cTCPLinkImpl *>(a_Self);
	ASSERT(Self->m_BufferEvent == a_BufferEvent);
	ASSERT(Self->m_Callbacks != nullptr);

	// Read all the incoming data, in 1024-byte chunks:
	char data[1024];
	size_t length;
	auto tlsContext = Self->m_TlsContext;
	while ((length = bufferevent_read(a_BufferEvent, data, sizeof(data))) > 0)
	{
		if (tlsContext != nullptr)
		{
			ASSERT(tlsContext->IsLink(Self));
			tlsContext->StoreReceivedData(data, length);
		}
		else
		{
			Self->ReceivedCleartextData(data, length);
		}
	}
}





void cTCPLinkImpl::WriteCallback(bufferevent * a_BufferEvent, void * a_Self)
{
	ASSERT(a_Self != nullptr);
	auto Self = static_cast<cTCPLinkImpl *>(a_Self);
	ASSERT(Self->m_Callbacks != nullptr);

	// If there's no more data to write and the link has been scheduled for shutdown, do the shutdown:
	auto OutLen = evbuffer_get_length(bufferevent_get_output(Self->m_BufferEvent));
	if ((OutLen == 0) && (Self->m_ShouldShutdown))
	{
		Self->DoActualShutdown();
	}
}





void cTCPLinkImpl::EventCallback(bufferevent * a_BufferEvent, short a_What, void * a_Self)
{
	ASSERT(a_Self != nullptr);
	cTCPLinkImplPtr Self = static_cast<cTCPLinkImpl *>(a_Self)->m_Self;
	if (Self == nullptr)
	{
		// The link has already been freed
		return;
	}

	// If an error is reported, call the error callback:
	if (a_What & BEV_EVENT_ERROR)
	{
		// Choose the proper callback to call based on whether we were waiting for connection or not:
		int err = EVUTIL_SOCKET_ERROR();
		if (Self->m_ConnectCallbacks != nullptr)
		{
			if (err == 0)
			{
				// This could be a DNS failure
				err = bufferevent_socket_get_dns_error(a_BufferEvent);
			}
			Self->m_ConnectCallbacks->OnError(err, evutil_socket_error_to_string(err));
		}
		else
		{
			Self->m_Callbacks->OnError(err, evutil_socket_error_to_string(err));
			if (Self->m_Server == nullptr)
			{
				cNetworkSingleton::Get().RemoveLink(Self.get());
			}
			else
			{
				Self->m_Server->RemoveLink(Self.get());
			}
		}
		Self->m_Self.reset();
		return;
	}

	// Pending connection succeeded, call the connection callback:
	if (a_What & BEV_EVENT_CONNECTED)
	{
		Self->UpdateLocalAddress();
		Self->UpdateRemoteAddress();
		if (Self->m_ConnectCallbacks != nullptr)
		{
			Self->m_ConnectCallbacks->OnConnected(*Self);
			// Reset the connect callbacks so that later errors get reported through the link callbacks:
			Self->m_ConnectCallbacks.reset();
			return;
		}
	}

	// If the connection has been closed, call the link callback and remove the connection:
	if (a_What & BEV_EVENT_EOF)
	{
		// If running in TLS mode and there's data left in the TLS contect, report it:
		auto tlsContext = Self->m_TlsContext;
		if (tlsContext != nullptr)
		{
			tlsContext->FlushBuffers();
		}

		Self->m_Callbacks->OnRemoteClosed();
		if (Self->m_Server != nullptr)
		{
			Self->m_Server->RemoveLink(Self.get());
		}
		else
		{
			cNetworkSingleton::Get().RemoveLink(Self.get());
		}
		Self->m_Self.reset();
		return;
	}

	// Unknown event, report it:
	LOGWARNING("cTCPLinkImpl: Unhandled LibEvent event %d (0x%x)", a_What, a_What);
	ASSERT(!"cTCPLinkImpl: Unhandled LibEvent event");
}





void cTCPLinkImpl::UpdateAddress(const sockaddr * a_Address, socklen_t a_AddrLen, AString & a_IP, UInt16 & a_Port)
{
	// Based on the family specified in the address, use the correct datastructure to convert to IP string:
	char IP[128];
	switch (a_Address->sa_family)
	{
		case AF_INET:  // IPv4:
		{
			const sockaddr_in * sin = reinterpret_cast<const sockaddr_in *>(a_Address);
			evutil_inet_ntop(AF_INET, &(sin->sin_addr), IP, sizeof(IP));
			a_Port = ntohs(sin->sin_port);
			break;
		}
		case AF_INET6:  // IPv6
		{
			const sockaddr_in6 * sin = reinterpret_cast<const sockaddr_in6 *>(a_Address);
			evutil_inet_ntop(AF_INET6, &(sin->sin6_addr), IP, sizeof(IP));
			a_Port = ntohs(sin->sin6_port);
			break;
		}

		default:
		{
			LOGWARNING("%s: Unknown socket address family: %d", __FUNCTION__, a_Address->sa_family);
			ASSERT(!"Unknown socket address family");
			break;
		}
	}
	a_IP.assign(IP);
}





void cTCPLinkImpl::UpdateLocalAddress(void)
{
	sockaddr_storage sa;
	socklen_t salen = static_cast<socklen_t>(sizeof(sa));
	getsockname(bufferevent_getfd(m_BufferEvent), reinterpret_cast<sockaddr *>(&sa), &salen);
	UpdateAddress(reinterpret_cast<const sockaddr *>(&sa), salen, m_LocalIP, m_LocalPort);
}





void cTCPLinkImpl::UpdateRemoteAddress(void)
{
	sockaddr_storage sa;
	socklen_t salen = static_cast<socklen_t>(sizeof(sa));
	getpeername(bufferevent_getfd(m_BufferEvent), reinterpret_cast<sockaddr *>(&sa), &salen);
	UpdateAddress(reinterpret_cast<const sockaddr *>(&sa), salen, m_RemoteIP, m_RemotePort);
}





void cTCPLinkImpl::DoActualShutdown(void)
{
	#ifdef _WIN32
		shutdown(bufferevent_getfd(m_BufferEvent), SD_SEND);
	#else
		shutdown(bufferevent_getfd(m_BufferEvent), SHUT_WR);
	#endif
	bufferevent_disable(m_BufferEvent, EV_WRITE);
}





bool cTCPLinkImpl::SendRaw(const void * a_Data, size_t a_Length)
{
	return (bufferevent_write(m_BufferEvent, a_Data, a_Length) == 0);
}





void cTCPLinkImpl::ReceivedCleartextData(const char * a_Data, size_t a_Length)
{
	ASSERT(m_Callbacks != nullptr);
	m_Callbacks->OnReceivedData(a_Data, a_Length);
}





////////////////////////////////////////////////////////////////////////////////
// cTCPLinkImpl::cLinkTlsContext:

cTCPLinkImpl::cLinkTlsContext::cLinkTlsContext(cTCPLinkImpl & a_Link):
	m_Link(a_Link)
{
}





void cTCPLinkImpl::cLinkTlsContext::SetSelf(cLinkTlsContextWPtr a_Self)
{
	m_Self = std::move(a_Self);
}





void cTCPLinkImpl::cLinkTlsContext::ResetSelf(void)
{
	m_Self.reset();
}





void cTCPLinkImpl::cLinkTlsContext::StoreReceivedData(const char * a_Data, size_t a_NumBytes)
{
	// Hold self alive for the duration of this function
	cLinkTlsContextPtr Self(m_Self);

	m_EncryptedData.append(a_Data, a_NumBytes);

	// Try to finish a pending handshake:
	TryFinishHandshaking();

	// Flush any cleartext data that can be "received":
	FlushBuffers();
}





void cTCPLinkImpl::cLinkTlsContext::FlushBuffers(void)
{
	// Hold self alive for the duration of this function
	cLinkTlsContextPtr Self(m_Self);

	// If the handshake didn't complete yet, bail out:
	if (!HasHandshaken())
	{
		return;
	}

	char Buffer[1024];
	int NumBytes;
	while ((NumBytes = ReadPlain(Buffer, sizeof(Buffer))) > 0)
	{
		m_Link.ReceivedCleartextData(Buffer, static_cast<size_t>(NumBytes));
		if (m_Self.expired())
		{
			// The callback closed the SSL context, bail out
			return;
		}
	}
}





void cTCPLinkImpl::cLinkTlsContext::TryFinishHandshaking(void)
{
	// Hold self alive for the duration of this function
	cLinkTlsContextPtr Self(m_Self);

	// If the handshake hasn't finished yet, retry:
	if (!HasHandshaken())
	{
		Handshake();
		// If the handshake succeeded, write all the queued plaintext data:
		if (HasHandshaken())
		{
			m_Link.GetCallbacks()->OnTlsHandshakeCompleted();
			WritePlain(m_CleartextData.data(), m_CleartextData.size());
			m_CleartextData.clear();
		}
	}
}





void cTCPLinkImpl::cLinkTlsContext::Send(const void * a_Data, size_t a_Length)
{
	// Hold self alive for the duration of this function
	cLinkTlsContextPtr Self(m_Self);

	// If the handshake hasn't completed yet, queue the data:
	if (!HasHandshaken())
	{
		m_CleartextData.append(reinterpret_cast<const char *>(a_Data), a_Length);
		TryFinishHandshaking();
		return;
	}

	// The connection is all set up, write the cleartext data into the SSL context:
	WritePlain(a_Data, a_Length);
	FlushBuffers();
}





int cTCPLinkImpl::cLinkTlsContext::ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes)
{
	// Hold self alive for the duration of this function
	cLinkTlsContextPtr Self(m_Self);

	// If there's nothing queued in the buffer, report empty buffer:
	if (m_EncryptedData.empty())
	{
		return MBEDTLS_ERR_SSL_WANT_READ;
	}

	// Copy as much data as possible to the provided buffer:
	size_t BytesToCopy = std::min(a_NumBytes, m_EncryptedData.size());
	memcpy(a_Buffer, m_EncryptedData.data(), BytesToCopy);
	m_EncryptedData.erase(0, BytesToCopy);
	return static_cast<int>(BytesToCopy);
}





int cTCPLinkImpl::cLinkTlsContext::SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes)
{
	m_Link.SendRaw(a_Buffer, a_NumBytes);
	return static_cast<int>(a_NumBytes);
}





////////////////////////////////////////////////////////////////////////////////
// cNetwork API:

bool cNetwork::Connect(
	const AString & a_Host,
	UInt16 a_Port,
	cNetwork::cConnectCallbacksPtr a_ConnectCallbacks,
	cTCPLink::cCallbacksPtr a_LinkCallbacks
)
{
	// Add a connection request to the queue:
	cTCPLinkImplPtr Conn = cTCPLinkImpl::Connect(a_Host, a_Port, std::move(a_LinkCallbacks), std::move(a_ConnectCallbacks));
	return (Conn != nullptr);
}





