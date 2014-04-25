
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Authenticator.h"
#include "../OSSupport/BlockingTCPLink.h"
#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"

#include "inifile/iniFile.h"
#include "json/json.h"

#include "polarssl/config.h"
#include "polarssl/net.h"
#include "polarssl/ssl.h"
#include "polarssl/entropy.h"
#include "polarssl/ctr_drbg.h"
#include "polarssl/error.h"
#include "polarssl/certs.h"

#include <sstream>
#include <iomanip>





#define DEFAULT_AUTH_SERVER "sessionserver.mojang.com"
#define DEFAULT_AUTH_ADDRESS "/session/minecraft/hasJoined?username=%USERNAME%&serverId=%SERVERID%"





cAuthenticator::cAuthenticator(void) :
	super("cAuthenticator"),
	m_Server(DEFAULT_AUTH_SERVER),
	m_Address(DEFAULT_AUTH_ADDRESS),
	m_ShouldAuthenticate(true)
{
}





cAuthenticator::~cAuthenticator()
{
	Stop();
}





void cAuthenticator::ReadINI(cIniFile & IniFile)
{
	m_Server = IniFile.GetValueSet("Authentication", "Server", DEFAULT_AUTH_SERVER);
	m_Address = IniFile.GetValueSet("Authentication", "Address", DEFAULT_AUTH_ADDRESS);
	m_ShouldAuthenticate = IniFile.GetValueSetB("Authentication", "Authenticate", true);
}





void cAuthenticator::Authenticate(int a_ClientID, const AString & a_UserName, const AString & a_ServerHash)
{
	if (!m_ShouldAuthenticate)
	{
		cRoot::Get()->AuthenticateUser(a_ClientID, a_UserName, cClientHandle::GenerateOfflineUUID(a_UserName));
		return;
	}

	cCSLock LOCK(m_CS);
	m_Queue.push_back(cUser(a_ClientID, a_UserName, a_ServerHash));
	m_QueueNonempty.Set();
}





void cAuthenticator::Start(cIniFile & IniFile)
{
	ReadINI(IniFile);
	m_ShouldTerminate = false;
	super::Start();
}





void cAuthenticator::Stop(void)
{
	m_ShouldTerminate = true;
	m_QueueNonempty.Set();
	Wait();
}





void cAuthenticator::Execute(void)
{
	for (;;)
	{
		cCSLock Lock(m_CS);
		while (!m_ShouldTerminate && (m_Queue.size() == 0))
		{
			cCSUnlock Unlock(Lock);
			m_QueueNonempty.Wait();
		}
		if (m_ShouldTerminate)
		{
			return;
		}
		ASSERT(!m_Queue.empty());

		cAuthenticator::cUser & User = m_Queue.front();
		int ClientID = User.m_ClientID;
		AString UserName = User.m_Name;
		AString ServerID = User.m_ServerID;
		m_Queue.pop_front();
		Lock.Unlock();

		AString NewUserName = UserName;
		AString UUID;
		if (AuthWithYggdrasil(NewUserName, ServerID, UUID))
		{
			LOGINFO("User %s authenticated with UUID '%s'", NewUserName.c_str(), UUID.c_str());
			cRoot::Get()->AuthenticateUser(ClientID, NewUserName, UUID);
		}
		else
		{
			cRoot::Get()->KickUser(ClientID, "Failed to authenticate account!");
		}
	}  // for (-ever)
}





bool cAuthenticator::AuthWithYggdrasil(AString & a_UserName, const AString & a_ServerId, AString & a_UUID)
{
	LOGD("Trying to auth user %s", a_UserName.c_str());
	
	int ret, server_fd = -1;
	unsigned char buf[1024];
	const char *pers = "cAuthenticator";

	entropy_context entropy;
	ctr_drbg_context ctr_drbg;
	ssl_context ssl;
	x509_crt cacert;

	/* Initialize the RNG and the session data */
	memset(&ssl, 0, sizeof(ssl_context));
	x509_crt_init(&cacert);

	entropy_init(&entropy);
	if ((ret = ctr_drbg_init(&ctr_drbg, entropy_func, &entropy, (const unsigned char *)pers, strlen(pers))) != 0)
	{
		LOGWARNING("cAuthenticator: ctr_drbg_init returned %d", ret);

		// Free all resources which have been initialized up to this line
		x509_crt_free(&cacert);
		entropy_free(&entropy);
		return false;
	}

	/* Initialize certificates */
	// TODO: Grab the sessionserver's root CA and any intermediates and hard-code them here, instead of test_ca_list
	ret = x509_crt_parse(&cacert, (const unsigned char *)test_ca_list, strlen(test_ca_list));

	if (ret < 0)
	{
		LOGWARNING("cAuthenticator: x509_crt_parse returned -0x%x", -ret);

		// Free all resources which have been initialized up to this line
		x509_crt_free(&cacert);
		entropy_free(&entropy);
		return false;
	}

	/* Connect */
	if ((ret = net_connect(&server_fd, m_Server.c_str(), 443)) != 0)
	{
		LOGWARNING("cAuthenticator: Can't connect to %s: %d", m_Server.c_str(), ret);

		// Free all resources which have been initialized up to this line
		x509_crt_free(&cacert);
		entropy_free(&entropy);
		return false;
	}

	/* Setup */
	if ((ret = ssl_init(&ssl)) != 0)
	{
		LOGWARNING("cAuthenticator: ssl_init returned %d", ret);

		// Free all resources which have been initialized up to this line
		x509_crt_free(&cacert);
		net_close(server_fd);
		ssl_free(&ssl);
		entropy_free(&entropy);
		memset(&ssl, 0, sizeof(ssl));
		return false;
	}
	ssl_set_endpoint(&ssl, SSL_IS_CLIENT);
	ssl_set_authmode(&ssl, SSL_VERIFY_OPTIONAL);
	ssl_set_ca_chain(&ssl, &cacert, NULL, "PolarSSL Server 1");
	ssl_set_rng(&ssl, ctr_drbg_random, &ctr_drbg);
	ssl_set_bio(&ssl, net_recv, &server_fd, net_send, &server_fd);

	/* Handshake */
	while ((ret = ssl_handshake(&ssl)) != 0)
	{
		if ((ret != POLARSSL_ERR_NET_WANT_READ) && (ret != POLARSSL_ERR_NET_WANT_WRITE))
		{
			LOGWARNING("cAuthenticator: ssl_handshake returned -0x%x", -ret);

			// Free all resources which have been initialized up to this line
			x509_crt_free(&cacert);
			net_close(server_fd);
			ssl_free(&ssl);
			entropy_free(&entropy);
			memset(&ssl, 0, sizeof(ssl));
			return false;
		}
	}

	/* Write the GET request */
	AString ActualAddress = m_Address;
	ReplaceString(ActualAddress, "%USERNAME%", a_UserName);
	ReplaceString(ActualAddress, "%SERVERID%", a_ServerId);

	AString Request;
	Request += "GET " + ActualAddress + " HTTP/1.1\r\n";
	Request += "Host: " + m_Server + "\r\n";
	Request += "User-Agent: MCServer\r\n";
	Request += "Connection: close\r\n";
	Request += "\r\n";

	ret = ssl_write(&ssl, (const unsigned char *)Request.c_str(), Request.size());
	if (ret <= 0)
	{
		LOGWARNING("cAuthenticator: ssl_write returned %d", ret);
		
		// Free all resources which have been initialized up to this line
		x509_crt_free(&cacert);
		net_close(server_fd);
		ssl_free(&ssl);
		entropy_free(&entropy);
		memset(&ssl, 0, sizeof(ssl));
		return false;
	}

	/* Read the HTTP response */
	std::string Response;
	for (;;)
	{
		memset(buf, 0, sizeof(buf));
		ret = ssl_read(&ssl, buf, sizeof(buf));

		if ((ret == POLARSSL_ERR_NET_WANT_READ) || (ret == POLARSSL_ERR_NET_WANT_WRITE))
		{
			continue;
		}
		if (ret == POLARSSL_ERR_SSL_PEER_CLOSE_NOTIFY)
		{
			break;
		}
		if (ret < 0)
		{
			LOGWARNING("cAuthenticator: ssl_read returned %d", ret);
			break;
		}
		if (ret == 0)
		{
			LOGWARNING("cAuthenticator: EOF");
			break;
		}

		Response.append((const char *)buf, ret);
	} 

	ssl_close_notify(&ssl);
	x509_crt_free(&cacert);
	net_close(server_fd);
	ssl_free(&ssl);
	entropy_free(&entropy);
	memset(&ssl, 0, sizeof(ssl));

	// Check the HTTP status line:
	AString prefix("HTTP/1.1 200 OK");
	AString HexDump;
	if (Response.compare(0, prefix.size(), prefix))
	{
		LOGINFO("User \"%s\" failed to auth, bad http status line received", a_UserName.c_str());
		LOG("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
		return false;
	}

	// Erase the HTTP headers from the response:
	size_t idxHeadersEnd = Response.find("\r\n\r\n");
	if (idxHeadersEnd == AString::npos)
	{
		LOGINFO("User \"%s\" failed to authenticate, bad http response header received", a_UserName.c_str());
		LOG("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
		return false;
	}
	Response.erase(0, idxHeadersEnd + 4);

	// Parse the Json response:
	if (Response.empty())
	{
		return false;
	}
	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(Response, root, false))
	{
		LOGWARNING("cAuthenticator: Cannot parse Received Data to json!");
		return false;
	}
	a_UserName = root.get("name", "Unknown").asString();
	a_UUID = root.get("id", "").asString();
	
	// If the UUID doesn't contain the hashes, insert them at the proper places:
	if (a_UUID.size() == 32)
	{
		a_UUID.insert(8, "-");
		a_UUID.insert(13, "-");
		a_UUID.insert(18, "-");
		a_UUID.insert(23, "-");
	}
	return true;
}





