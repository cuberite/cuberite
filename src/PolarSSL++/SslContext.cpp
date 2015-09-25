
// SslContext.cpp

// Implements the cSslContext class that holds everything a single SSL context needs to function

#include "Globals.h"
#include "SslContext.h"
#include "EntropyContext.h"
#include "CtrDrbgContext.h"
#include "polarssl/debug.h"





cSslContext::cSslContext(void) :
	m_IsValid(false),
	m_HasHandshaken(false)
{
	memset(&m_Ssl, 0, sizeof(m_Ssl));
}





cSslContext::~cSslContext()
{
	if (m_IsValid)
	{
		ssl_free(&m_Ssl);
	}
}





int cSslContext::Initialize(bool a_IsClient, const SharedPtr<cCtrDrbgContext> & a_CtrDrbg)
{
	// Check double-initialization:
	if (m_IsValid)
	{
		LOGWARNING("SSL: Double initialization is not supported.");
		return POLARSSL_ERR_SSL_BAD_INPUT_DATA;  // There is no return value well-suited for this, reuse this one.
	}
	
	// Set the CtrDrbg context, create a new one if needed:
	m_CtrDrbg = a_CtrDrbg;
	if (m_CtrDrbg.get() == nullptr)
	{
		m_CtrDrbg.reset(new cCtrDrbgContext);
		m_CtrDrbg->Initialize("Cuberite", 8);
	}
	
	// Initialize PolarSSL's structures:
	memset(&m_Ssl, 0, sizeof(m_Ssl));
	int res = ssl_init(&m_Ssl);
	if (res != 0)
	{
		return res;
	}
	ssl_set_endpoint(&m_Ssl, a_IsClient ? SSL_IS_CLIENT : SSL_IS_SERVER);
	ssl_set_authmode(&m_Ssl, a_IsClient ? SSL_VERIFY_OPTIONAL : SSL_VERIFY_NONE);  // Clients ask for server's cert but don't verify strictly; servers don't ask clients for certs by default
	ssl_set_rng(&m_Ssl, ctr_drbg_random, &m_CtrDrbg->m_CtrDrbg);
	ssl_set_bio(&m_Ssl, ReceiveEncrypted, this, SendEncrypted, this);
	
	#ifdef _DEBUG
		/*
		// These functions allow us to debug SSL and certificate problems, but produce way too much output,
		// so they're disabled until someone needs them
		ssl_set_dbg(&m_Ssl, &SSLDebugMessage, this);
		debug_set_threshold(2);

		ssl_set_verify(&m_Ssl, &SSLVerifyCert, this);
		//*/
		
		/*
		// Set ciphersuite to the easiest one to decode, so that the connection can be wireshark-decoded:
		static const int CipherSuites[] =
		{
			TLS_RSA_WITH_RC4_128_MD5,
			TLS_RSA_WITH_RC4_128_SHA,
			TLS_RSA_WITH_AES_128_CBC_SHA,
			0,  // Must be 0-terminated!
		};
		ssl_set_ciphersuites(&m_Ssl, CipherSuites);
		*/
	#endif
	
	m_IsValid = true;
	return 0;
}





void cSslContext::SetOwnCert(const cX509CertPtr & a_OwnCert, const cRsaPrivateKeyPtr & a_OwnCertPrivKey)
{
	ASSERT(m_IsValid);  // Call Initialize() first
	
	// Check that both the cert and the key is valid:
	if ((a_OwnCert.get() == nullptr) || (a_OwnCertPrivKey.get() == nullptr))
	{
		LOGWARNING("SSL: Own certificate is not valid, skipping the set.");
		return;
	}
	
	// Make sure we have the cert stored for later, PolarSSL only uses the cert later on
	m_OwnCert = a_OwnCert;
	m_OwnCertPrivKey = a_OwnCertPrivKey;
	
	// Set into the context:
	ssl_set_own_cert_rsa(&m_Ssl, m_OwnCert->GetInternal(), m_OwnCertPrivKey->GetInternal());
}





void cSslContext::SetOwnCert(const cX509CertPtr & a_OwnCert, const cCryptoKeyPtr & a_OwnCertPrivKey)
{
	ASSERT(m_IsValid);  // Call Initialize() first
	
	// Check that both the cert and the key is valid:
	if ((a_OwnCert.get() == nullptr) || (a_OwnCertPrivKey.get() == nullptr))
	{
		LOGWARNING("SSL: Own certificate is not valid, skipping the set.");
		return;
	}
	
	// Make sure we have the cert stored for later, PolarSSL only uses the cert later on
	m_OwnCert = a_OwnCert;
	m_OwnCertPrivKey2 = a_OwnCertPrivKey;
	
	// Set into the context:
	ssl_set_own_cert(&m_Ssl, m_OwnCert->GetInternal(), m_OwnCertPrivKey2->GetInternal());
}





void cSslContext::SetCACerts(const cX509CertPtr & a_CACert, const AString & a_ExpectedPeerName)
{
	ASSERT(m_IsValid);  // Call Initialize() first
	
	// Store the data in our internal buffers, to avoid losing the pointers later on
	// PolarSSL will need these after this call returns, and the caller may move / delete the data before that:
	m_ExpectedPeerName = a_ExpectedPeerName;
	m_CACerts = a_CACert;
	
	// Set the trusted CA root cert store:
	ssl_set_authmode(&m_Ssl, SSL_VERIFY_REQUIRED);
	ssl_set_ca_chain(&m_Ssl, m_CACerts->GetInternal(), nullptr, m_ExpectedPeerName.empty() ? nullptr : m_ExpectedPeerName.c_str());
}





int cSslContext::WritePlain(const void * a_Data, size_t a_NumBytes)
{
	ASSERT(m_IsValid);  // Need to call Initialize() first
	if (!m_HasHandshaken)
	{
		int res = Handshake();
		if (res != 0)
		{
			return res;
		}
	}
	
	return ssl_write(&m_Ssl, reinterpret_cast<const unsigned char *>(a_Data), a_NumBytes);
}





int cSslContext::ReadPlain(void * a_Data, size_t a_MaxBytes)
{
	ASSERT(m_IsValid);  // Need to call Initialize() first
	if (!m_HasHandshaken)
	{
		int res = Handshake();
		if (res != 0)
		{
			return res;
		}
	}
	
	return ssl_read(&m_Ssl, reinterpret_cast<unsigned char *>(a_Data), a_MaxBytes);
}





int cSslContext::Handshake(void)
{
	ASSERT(m_IsValid);  // Need to call Initialize() first
	ASSERT(!m_HasHandshaken);  // Must not call twice
	
	int res = ssl_handshake(&m_Ssl);
	if (res == 0)
	{
		m_HasHandshaken = true;
	}
	return res;
}





int cSslContext::NotifyClose(void)
{
	return ssl_close_notify(&m_Ssl);
}





#ifdef _DEBUG
	void cSslContext::SSLDebugMessage(void * a_UserParam, int a_Level, const char * a_Text)
	{
		if (a_Level > 3)
		{
			// Don't want the trace messages
			return;
		}
		
		// Remove the terminating LF:
		size_t len = strlen(a_Text) - 1;
		while ((len > 0) && (a_Text[len] <= 32))
		{
			len--;
		}
		AString Text(a_Text, len + 1);
		
		LOGD("SSL (%d): %s", a_Level, Text.c_str());
	}





	int cSslContext::SSLVerifyCert(void * a_This, x509_crt * a_Crt, int a_Depth, int * a_Flags)
	{
		char buf[1024];
		UNUSED(a_This);

		LOG("Verify requested for (Depth %d):", a_Depth);
		x509_crt_info(buf, sizeof(buf) - 1, "", a_Crt);
		LOG("%s", buf);

		int Flags = *a_Flags;
		if ((Flags & BADCERT_EXPIRED) != 0)
		{
			LOG(" ! server certificate has expired");
		}

		if ((Flags & BADCERT_REVOKED) != 0)
		{
			LOG(" ! server certificate has been revoked");
		}

		if ((Flags & BADCERT_CN_MISMATCH) != 0)
		{
			LOG(" ! CN mismatch");
		}

		if ((Flags & BADCERT_NOT_TRUSTED) != 0)
		{
			LOG(" ! self-signed or not signed by a trusted CA");
		}

		if ((Flags & BADCRL_NOT_TRUSTED) != 0)
		{
			LOG(" ! CRL not trusted");
		}

		if ((Flags & BADCRL_EXPIRED) != 0)
		{
			LOG(" ! CRL expired");
		}

		if ((Flags & BADCERT_OTHER) != 0)
		{
			LOG(" ! other (unknown) flag");
		}

		if (Flags == 0)
		{
			LOG(" This certificate has no flags");
		}

		return 0;
	}
#endif  // _DEBUG




