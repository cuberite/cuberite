
// LuaTCPLink.cpp

// Implements the cLuaTCPLink class representing a Lua wrapper for the cTCPLink class and the callbacks it needs

#include "Globals.h"
#include "LuaTCPLink.h"
#include "LuaServerHandle.h"





cLuaTCPLink::cLuaTCPLink(cPluginLua & a_Plugin, int a_CallbacksTableStackPos):
	m_Plugin(a_Plugin),
	m_Callbacks(a_Plugin.GetLuaState(), a_CallbacksTableStackPos)
{
	// Warn if the callbacks aren't valid:
	if (!m_Callbacks.IsValid())
	{
		LOGWARNING("cTCPLink in plugin %s: callbacks could not be retrieved", m_Plugin.GetName().c_str());
		cPluginLua::cOperation Op(m_Plugin);
		Op().LogStackTrace();
	}
}





cLuaTCPLink::cLuaTCPLink(cPluginLua & a_Plugin, cLuaState::cRef && a_CallbacksTableRef, cLuaServerHandleWPtr a_ServerHandle):
	m_Plugin(a_Plugin),
	m_Callbacks(std::move(a_CallbacksTableRef)),
	m_Server(std::move(a_ServerHandle))
{
	// Warn if the callbacks aren't valid:
	if (!m_Callbacks.IsValid())
	{
		LOGWARNING("cTCPLink in plugin %s: callbacks could not be retrieved", m_Plugin.GetName().c_str());
		cPluginLua::cOperation Op(m_Plugin);
		Op().LogStackTrace();
	}
}





cLuaTCPLink::~cLuaTCPLink()
{
	// If the link is still open, close it:
	cTCPLinkPtr Link = m_Link;
	if (Link != nullptr)
	{
		Link->Close();
	}

	Terminated();
}





bool cLuaTCPLink::Send(const AString & a_Data)
{
	// If running in SSL mode, push the data into the SSL context instead:
	if (m_SslContext != nullptr)
	{
		m_SslContext->Send(a_Data);
		return true;
	}

	// Safely grab a copy of the link:
	cTCPLinkPtr Link = m_Link;
	if (Link == nullptr)
	{
		return false;
	}

	// Send the data:
	return Link->Send(a_Data);
}





AString cLuaTCPLink::GetLocalIP(void) const
{
	// Safely grab a copy of the link:
	cTCPLinkPtr Link = m_Link;
	if (Link == nullptr)
	{
		return "";
	}

	// Get the IP address:
	return Link->GetLocalIP();
}





UInt16 cLuaTCPLink::GetLocalPort(void) const
{
	// Safely grab a copy of the link:
	cTCPLinkPtr Link = m_Link;
	if (Link == nullptr)
	{
		return 0;
	}

	// Get the port:
	return Link->GetLocalPort();
}





AString cLuaTCPLink::GetRemoteIP(void) const
{
	// Safely grab a copy of the link:
	cTCPLinkPtr Link = m_Link;
	if (Link == nullptr)
	{
		return "";
	}

	// Get the IP address:
	return Link->GetRemoteIP();
}





UInt16 cLuaTCPLink::GetRemotePort(void) const
{
	// Safely grab a copy of the link:
	cTCPLinkPtr Link = m_Link;
	if (Link == nullptr)
	{
		return 0;
	}

	// Get the port:
	return Link->GetRemotePort();
}





void cLuaTCPLink::Shutdown(void)
{
	// Safely grab a copy of the link and shut it down:
	cTCPLinkPtr Link = m_Link;
	if (Link != nullptr)
	{
		if (m_SslContext != nullptr)
		{
			m_SslContext->NotifyClose();
			m_SslContext->ResetSelf();
			m_SslContext.reset();
		}
		Link->Shutdown();
	}
}





void cLuaTCPLink::Close(void)
{
	// If the link is still open, close it:
	cTCPLinkPtr Link = m_Link;
	if (Link != nullptr)
	{
		if (m_SslContext != nullptr)
		{
			m_SslContext->NotifyClose();
			m_SslContext->ResetSelf();
			m_SslContext.reset();
		}
		Link->Close();
	}

	Terminated();
}





AString cLuaTCPLink::StartTLSClient(
	const AString & a_OwnCertData,
	const AString & a_OwnPrivKeyData,
	const AString & a_OwnPrivKeyPassword
)
{
	// Check preconditions:
	if (m_SslContext != nullptr)
	{
		return "TLS is already active on this link";
	}
	if (
		(a_OwnCertData.empty()  && !a_OwnPrivKeyData.empty()) ||
		(!a_OwnCertData.empty() && a_OwnPrivKeyData.empty())
	)
	{
		return "Either provide both the certificate and private key, or neither";
	}

	// Create the SSL context:
	m_SslContext.reset(new cLinkSslContext(*this));
	m_SslContext->Initialize(true);

	// Create the peer cert, if required:
	if (!a_OwnCertData.empty() && !a_OwnPrivKeyData.empty())
	{
		auto OwnCert = std::make_shared<cX509Cert>();
		int res = OwnCert->Parse(a_OwnCertData.data(), a_OwnCertData.size());
		if (res != 0)
		{
			m_SslContext.reset();
			return Printf("Cannot parse peer certificate: -0x%x", res);
		}
		auto OwnPrivKey = std::make_shared<cCryptoKey>();
		res = OwnPrivKey->ParsePrivate(a_OwnPrivKeyData.data(), a_OwnPrivKeyData.size(), a_OwnPrivKeyPassword);
		if (res != 0)
		{
			m_SslContext.reset();
			return Printf("Cannot parse peer private key: -0x%x", res);
		}
		m_SslContext->SetOwnCert(OwnCert, OwnPrivKey);
	}
	m_SslContext->SetSelf(cLinkSslContextWPtr(m_SslContext));

	// Start the handshake:
	m_SslContext->Handshake();
	return "";
}





AString cLuaTCPLink::StartTLSServer(
	const AString & a_OwnCertData,
	const AString & a_OwnPrivKeyData,
	const AString & a_OwnPrivKeyPassword,
	const AString & a_StartTLSData
)
{
	// Check preconditions:
	if (m_SslContext != nullptr)
	{
		return "TLS is already active on this link";
	}
	if (a_OwnCertData.empty()  || a_OwnPrivKeyData.empty())
	{
		return "Provide the server certificate and private key";
	}

	// Create the SSL context:
	m_SslContext.reset(new cLinkSslContext(*this));
	m_SslContext->Initialize(false);

	// Create the peer cert:
	auto OwnCert = std::make_shared<cX509Cert>();
	int res = OwnCert->Parse(a_OwnCertData.data(), a_OwnCertData.size());
	if (res != 0)
	{
		m_SslContext.reset();
		return Printf("Cannot parse server certificate: -0x%x", res);
	}
	auto OwnPrivKey = std::make_shared<cCryptoKey>();
	res = OwnPrivKey->ParsePrivate(a_OwnPrivKeyData.data(), a_OwnPrivKeyData.size(), a_OwnPrivKeyPassword);
	if (res != 0)
	{
		m_SslContext.reset();
		return Printf("Cannot parse server private key: -0x%x", res);
	}
	m_SslContext->SetOwnCert(OwnCert, OwnPrivKey);
	m_SslContext->SetSelf(cLinkSslContextWPtr(m_SslContext));

	// Push the initial data:
	m_SslContext->StoreReceivedData(a_StartTLSData.data(), a_StartTLSData.size());

	// Start the handshake:
	m_SslContext->Handshake();
	return "";
}





void cLuaTCPLink::Terminated(void)
{
	// Disable the callbacks:
	if (m_Callbacks.IsValid())
	{
		m_Callbacks.UnRef();
	}

	// If the managing server is still alive, let it know we're terminating:
	auto Server = m_Server.lock();
	if (Server != nullptr)
	{
		Server->RemoveLink(this);
	}

	// If the link is still open, close it:
	{
		cTCPLinkPtr Link = m_Link;
		if (Link != nullptr)
		{
			Link->Close();
			m_Link.reset();
		}
	}

	// If the SSL context still exists, free it:
	m_SslContext.reset();
}





void cLuaTCPLink::ReceivedCleartextData(const char * a_Data, size_t a_NumBytes)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// Call the callback:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnReceivedData"), this, AString(a_Data, a_NumBytes)))
	{
		LOGINFO("cTCPLink OnReceivedData callback failed in plugin %s.", m_Plugin.GetName().c_str());
	}
}





void cLuaTCPLink::OnConnected(cTCPLink & a_Link)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// Call the callback:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnConnected"), this))
	{
		LOGINFO("cTCPLink OnConnected() callback failed in plugin %s.", m_Plugin.GetName().c_str());
	}
}





void cLuaTCPLink::OnError(int a_ErrorCode, const AString & a_ErrorMsg)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// Call the callback:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnError"), this, a_ErrorCode, a_ErrorMsg))
	{
		LOGINFO("cTCPLink OnError() callback failed in plugin %s; the link error is %d (%s).",
			m_Plugin.GetName().c_str(), a_ErrorCode, a_ErrorMsg.c_str()
		);
	}

	Terminated();
}





void cLuaTCPLink::OnLinkCreated(cTCPLinkPtr a_Link)
{
	// Store the cTCPLink for later use:
	m_Link = a_Link;
}





void cLuaTCPLink::OnReceivedData(const char * a_Data, size_t a_Length)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// If we're running in SSL mode, put the data into the SSL decryptor:
	auto sslContext = m_SslContext;
	if (sslContext != nullptr)
	{
		sslContext->StoreReceivedData(a_Data, a_Length);
		return;
	}

	// Call the callback:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnReceivedData"), this, AString(a_Data, a_Length)))
	{
		LOGINFO("cTCPLink OnReceivedData callback failed in plugin %s.", m_Plugin.GetName().c_str());
	}
}





void cLuaTCPLink::OnRemoteClosed(void)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// If running in SSL mode and there's data left in the SSL contect, report it:
	auto sslContext = m_SslContext;
	if (sslContext != nullptr)
	{
		sslContext->FlushBuffers();
	}

	// Call the callback:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnRemoteClosed"), this))
	{
		LOGINFO("cTCPLink OnRemoteClosed() callback failed in plugin %s.", m_Plugin.GetName().c_str());
	}

	Terminated();
}





////////////////////////////////////////////////////////////////////////////////
// cLuaTCPLink::cLinkSslContext:

cLuaTCPLink::cLinkSslContext::cLinkSslContext(cLuaTCPLink & a_Link):
	m_Link(a_Link)
{
}





void cLuaTCPLink::cLinkSslContext::SetSelf(cLinkSslContextWPtr a_Self)
{
	m_Self = a_Self;
}





void cLuaTCPLink::cLinkSslContext::ResetSelf(void)
{
	m_Self.reset();
}





void cLuaTCPLink::cLinkSslContext::StoreReceivedData(const char * a_Data, size_t a_NumBytes)
{
	// Hold self alive for the duration of this function
	cLinkSslContextPtr Self(m_Self);

	m_EncryptedData.append(a_Data, a_NumBytes);

	// Try to finish a pending handshake:
	TryFinishHandshaking();

	// Flush any cleartext data that can be "received":
	FlushBuffers();
}





void cLuaTCPLink::cLinkSslContext::FlushBuffers(void)
{
	// Hold self alive for the duration of this function
	cLinkSslContextPtr Self(m_Self);

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





void cLuaTCPLink::cLinkSslContext::TryFinishHandshaking(void)
{
	// Hold self alive for the duration of this function
	cLinkSslContextPtr Self(m_Self);

	// If the handshake hasn't finished yet, retry:
	if (!HasHandshaken())
	{
		Handshake();
	}

	// If the handshake succeeded, write all the queued plaintext data:
	if (HasHandshaken())
	{
		WritePlain(m_CleartextData.data(), m_CleartextData.size());
		m_CleartextData.clear();
	}
}





void cLuaTCPLink::cLinkSslContext::Send(const AString & a_Data)
{
	// Hold self alive for the duration of this function
	cLinkSslContextPtr Self(m_Self);

	// If the handshake hasn't completed yet, queue the data:
	if (!HasHandshaken())
	{
		m_CleartextData.append(a_Data);
		TryFinishHandshaking();
		return;
	}

	// The connection is all set up, write the cleartext data into the SSL context:
	WritePlain(a_Data.data(), a_Data.size());
	FlushBuffers();
}





int cLuaTCPLink::cLinkSslContext::ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes)
{
	// Hold self alive for the duration of this function
	cLinkSslContextPtr Self(m_Self);

	// If there's nothing queued in the buffer, report empty buffer:
	if (m_EncryptedData.empty())
	{
		return POLARSSL_ERR_NET_WANT_READ;
	}

	// Copy as much data as possible to the provided buffer:
	size_t BytesToCopy = std::min(a_NumBytes, m_EncryptedData.size());
	memcpy(a_Buffer, m_EncryptedData.data(), BytesToCopy);
	m_EncryptedData.erase(0, BytesToCopy);
	return static_cast<int>(BytesToCopy);
}





int cLuaTCPLink::cLinkSslContext::SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes)
{
	m_Link.m_Link->Send(a_Buffer, a_NumBytes);
	return static_cast<int>(a_NumBytes);
}




