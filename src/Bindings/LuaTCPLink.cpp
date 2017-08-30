
// LuaTCPLink.cpp

// Implements the cLuaTCPLink class representing a Lua wrapper for the cTCPLink class and the callbacks it needs

#include "Globals.h"
#include "LuaTCPLink.h"
#include "LuaServerHandle.h"
#include "../mbedTLS++/X509Cert.h"
#include "../mbedTLS++/CryptoKey.h"





cLuaTCPLink::cLuaTCPLink(cLuaState::cTableRefPtr && a_Callbacks):
	m_Callbacks(std::move(a_Callbacks))
{
}





cLuaTCPLink::cLuaTCPLink(cLuaState::cTableRefPtr && a_Callbacks, cLuaServerHandleWPtr a_ServerHandle):
	m_Callbacks(std::move(a_Callbacks)),
	m_Server(std::move(a_ServerHandle))
{
}





cLuaTCPLink::~cLuaTCPLink()
{
	// If the link is still open, close it:
	auto link = m_Link;
	if (link != nullptr)
	{
		link->Close();
	}

	Terminated();
}





bool cLuaTCPLink::Send(const AString & a_Data)
{
	// Safely grab a copy of the link:
	auto link = m_Link;
	if (link == nullptr)
	{
		return false;
	}

	// Send the data:
	return link->Send(a_Data);
}





AString cLuaTCPLink::GetLocalIP(void) const
{
	// Safely grab a copy of the link:
	auto link = m_Link;
	if (link == nullptr)
	{
		return "";
	}

	// Get the IP address:
	return link->GetLocalIP();
}





UInt16 cLuaTCPLink::GetLocalPort(void) const
{
	// Safely grab a copy of the link:
	auto link = m_Link;
	if (link == nullptr)
	{
		return 0;
	}

	// Get the port:
	return link->GetLocalPort();
}





AString cLuaTCPLink::GetRemoteIP(void) const
{
	// Safely grab a copy of the link:
	cTCPLinkPtr link = m_Link;
	if (link == nullptr)
	{
		return "";
	}

	// Get the IP address:
	return link->GetRemoteIP();
}





UInt16 cLuaTCPLink::GetRemotePort(void) const
{
	// Safely grab a copy of the link:
	cTCPLinkPtr link = m_Link;
	if (link == nullptr)
	{
		return 0;
	}

	// Get the port:
	return link->GetRemotePort();
}





void cLuaTCPLink::Shutdown(void)
{
	// Safely grab a copy of the link and shut it down:
	cTCPLinkPtr link = m_Link;
	if (link != nullptr)
	{
		link->Shutdown();
	}
}





void cLuaTCPLink::Close(void)
{
	// If the link is still open, close it:
	cTCPLinkPtr link = m_Link;
	if (link != nullptr)
	{
		link->Close();
	}

	Terminated();
}





AString cLuaTCPLink::StartTLSClient(
	const AString & a_OwnCertData,
	const AString & a_OwnPrivKeyData,
	const AString & a_OwnPrivKeyPassword
)
{
	auto link = m_Link;
	if (link != nullptr)
	{
		cX509CertPtr ownCert;
		if (!a_OwnCertData.empty())
		{
			ownCert = std::make_shared<cX509Cert>();
			auto res = ownCert->Parse(a_OwnCertData.data(), a_OwnCertData.size());
			if (res != 0)
			{
				return Printf("Cannot parse client certificate: -0x%x", res);
			}
		}
		cCryptoKeyPtr ownPrivKey;
		if (!a_OwnPrivKeyData.empty())
		{
			ownPrivKey = std::make_shared<cCryptoKey>();
			auto res = ownPrivKey->ParsePrivate(a_OwnPrivKeyData.data(), a_OwnPrivKeyData.size(), a_OwnPrivKeyPassword);
			if (res != 0)
			{
				return Printf("Cannot parse client private key: -0x%x", res);
			}
		}
		return link->StartTLSClient(ownCert, ownPrivKey);
	}
	return "";
}





AString cLuaTCPLink::StartTLSServer(
	const AString & a_OwnCertData,
	const AString & a_OwnPrivKeyData,
	const AString & a_OwnPrivKeyPassword,
	const AString & a_StartTLSData
)
{
	auto link = m_Link;
	if (link != nullptr)
	{
	// Create the peer cert:
	auto OwnCert = std::make_shared<cX509Cert>();
	int res = OwnCert->Parse(a_OwnCertData.data(), a_OwnCertData.size());
	if (res != 0)
	{
		return Printf("Cannot parse server certificate: -0x%x", res);
	}
	auto OwnPrivKey = std::make_shared<cCryptoKey>();
	res = OwnPrivKey->ParsePrivate(a_OwnPrivKeyData.data(), a_OwnPrivKeyData.size(), a_OwnPrivKeyPassword);
	if (res != 0)
	{
		return Printf("Cannot parse server private key: -0x%x", res);
	}

		return link->StartTLSServer(OwnCert, OwnPrivKey, a_StartTLSData);
	}
	return "";
}





void cLuaTCPLink::Terminated(void)
{
	// Disable the callbacks:
	if (m_Callbacks->IsValid())
	{
		m_Callbacks->Clear();
	}

	// If the managing server is still alive, let it know we're terminating:
	auto Server = m_Server.lock();
	if (Server != nullptr)
	{
		Server->RemoveLink(this);
	}

	// If the link is still open, close it:
	{
		auto link= m_Link;
		if (link != nullptr)
		{
			link->Close();
			m_Link.reset();
		}
	}
}





void cLuaTCPLink::ReceivedCleartextData(const char * a_Data, size_t a_NumBytes)
{
	// Call the callback:
	m_Callbacks->CallTableFn("OnReceivedData", this, AString(a_Data, a_NumBytes));
}





void cLuaTCPLink::OnConnected(cTCPLink & a_Link)
{
	// Call the callback:
	m_Callbacks->CallTableFn("OnConnected", this);
}





void cLuaTCPLink::OnError(int a_ErrorCode, const AString & a_ErrorMsg)
{
	// Call the callback:
	m_Callbacks->CallTableFn("OnError", this, a_ErrorCode, a_ErrorMsg);

	// Terminate all processing on the link:
	Terminated();
}





void cLuaTCPLink::OnLinkCreated(cTCPLinkPtr a_Link)
{
	// Store the cTCPLink for later use:
	m_Link = a_Link;
}





void cLuaTCPLink::OnReceivedData(const char * a_Data, size_t a_Length)
{
	// Call the callback:
	m_Callbacks->CallTableFn("OnReceivedData", this, AString(a_Data, a_Length));
}





void cLuaTCPLink::OnRemoteClosed(void)
{
	// Call the callback:
	m_Callbacks->CallTableFn("OnRemoteClosed", this);

	// Terminate all processing on the link:
	Terminated();
}





