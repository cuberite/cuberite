
#include "Globals.h"

#include "SslConfig.h"
#include "EntropyContext.h"
#include "CtrDrbgContext.h"
#include "CryptoKey.h"
#include "X509Cert.h"


// This allows us to debug SSL and certificate problems, but produce way too much output,
// so it's disabled until someone needs it
// #define ENABLE_SSL_DEBUG_MSG


#if defined(_DEBUG) && defined(ENABLE_SSL_DEBUG_MSG)
	#include "mbedtls/debug.h"


	namespace
	{
		void SSLDebugMessage(void * a_UserParam, int a_Level, const char * a_Filename, int a_LineNo, const char * a_Text)
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





		int SSLVerifyCert(void * a_This, mbedtls_x509_crt * a_Crt, int a_Depth, uint32_t * a_Flags)
		{
			char buf[1024];
			UNUSED(a_This);

			LOG("Verify requested for (Depth %d):", a_Depth);
			mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", a_Crt);
			LOG("%s", buf);

			uint32_t Flags = *a_Flags;
			if ((Flags & MBEDTLS_X509_BADCERT_EXPIRED) != 0)
			{
				LOG(" ! server certificate has expired");
			}

			if ((Flags & MBEDTLS_X509_BADCERT_REVOKED) != 0)
			{
				LOG(" ! server certificate has been revoked");
			}

			if ((Flags & MBEDTLS_X509_BADCERT_CN_MISMATCH) != 0)
			{
				LOG(" ! CN mismatch");
			}

			if ((Flags & MBEDTLS_X509_BADCERT_NOT_TRUSTED) != 0)
			{
				LOG(" ! self-signed or not signed by a trusted CA");
			}

			if ((Flags & MBEDTLS_X509_BADCRL_NOT_TRUSTED) != 0)
			{
				LOG(" ! CRL not trusted");
			}

			if ((Flags & MBEDTLS_X509_BADCRL_EXPIRED) != 0)
			{
				LOG(" ! CRL expired");
			}

			if ((Flags & MBEDTLS_X509_BADCERT_OTHER) != 0)
			{
				LOG(" ! other (unknown) flag");
			}

			if (Flags == 0)
			{
				LOG(" This certificate has no flags");
			}

			return 0;
		}
	}
#endif  // defined(_DEBUG) && defined(ENABLE_SSL_DEBUG_MSG)




////////////////////////////////////////////////////////////////////////////////
// cSslConfig:

cSslConfig::cSslConfig()
{
	mbedtls_ssl_config_init(&m_Config);
}





cSslConfig::~cSslConfig()
{
	mbedtls_ssl_config_free(&m_Config);
}





int cSslConfig::InitDefaults(const bool a_IsClient)
{
	return mbedtls_ssl_config_defaults(
		&m_Config,
		a_IsClient ? MBEDTLS_SSL_IS_CLIENT : MBEDTLS_SSL_IS_SERVER,
		MBEDTLS_SSL_TRANSPORT_STREAM,
		MBEDTLS_SSL_PRESET_DEFAULT
	);
}





void cSslConfig::SetAuthMode(const eSslAuthMode a_AuthMode)
{
	const int Mode = [=]()
	{
		switch (a_AuthMode)
		{
			case eSslAuthMode::None:     return MBEDTLS_SSL_VERIFY_NONE;
			case eSslAuthMode::Optional: return MBEDTLS_SSL_VERIFY_OPTIONAL;
			case eSslAuthMode::Required: return MBEDTLS_SSL_VERIFY_REQUIRED;
			case eSslAuthMode::Unset:    return MBEDTLS_SSL_VERIFY_UNSET;
		}
		UNREACHABLE("Unsupported SSL auth mode");
	}();

	mbedtls_ssl_conf_authmode(&m_Config, Mode);
}





void cSslConfig::SetRng(cCtrDrbgContextPtr a_CtrDrbg)
{
	ASSERT(a_CtrDrbg != nullptr);
	m_CtrDrbg = std::move(a_CtrDrbg);
	mbedtls_ssl_conf_rng(&m_Config, mbedtls_ctr_drbg_random, &m_CtrDrbg->m_CtrDrbg);
}





void cSslConfig::SetDebugCallback(cDebugCallback a_CallbackFun, void * a_CallbackData)
{
	mbedtls_ssl_conf_dbg(&m_Config, a_CallbackFun, a_CallbackData);
}





void cSslConfig::SetOwnCert(cX509CertPtr a_OwnCert, cCryptoKeyPtr a_OwnCertPrivKey)
{
	ASSERT(a_OwnCert != nullptr);
	ASSERT(a_OwnCertPrivKey != nullptr);

	// Make sure we have the cert stored for later, mbedTLS only uses the cert later on
	m_OwnCert = std::move(a_OwnCert);
	m_OwnCertPrivKey = std::move(a_OwnCertPrivKey);

	// Set into the config:
	mbedtls_ssl_conf_own_cert(&m_Config, m_OwnCert->GetInternal(), m_OwnCertPrivKey->GetInternal());
}





void cSslConfig::SetVerifyCallback(cVerifyCallback a_CallbackFun, void * a_CallbackData)
{
	mbedtls_ssl_conf_verify(&m_Config, a_CallbackFun, a_CallbackData);
}





void cSslConfig::SetCipherSuites(std::vector<int> a_CipherSuites)
{
	m_CipherSuites = std::move(a_CipherSuites);
	m_CipherSuites.push_back(0);  // Must be null terminated
	mbedtls_ssl_conf_ciphersuites(&m_Config, m_CipherSuites.data());
}





void cSslConfig::SetCACerts(cX509CertPtr a_CACert)
{
	m_CACerts = std::move(a_CACert);
	mbedtls_ssl_conf_ca_chain(&m_Config, m_CACerts->GetInternal(), nullptr);
}





std::shared_ptr<cSslConfig> cSslConfig::MakeDefaultConfig(bool a_IsClient)
{
	// TODO: Default CA chain and SetAuthMode(eSslAuthMode::Required)
	auto Ret = std::make_shared<cSslConfig>();

	Ret->InitDefaults(a_IsClient);

	{
		auto CtrDrbg = std::make_shared<cCtrDrbgContext>();
		CtrDrbg->Initialize("Cuberite", 8);
		Ret->SetRng(std::move(CtrDrbg));
	}

	Ret->SetAuthMode(eSslAuthMode::None);  // We cannot verify because we don't have a CA chain

	#ifdef _DEBUG
		#ifdef ENABLE_SSL_DEBUG_MSG
			Ret->SetDebugCallback(&SSLDebugMessage, nullptr);
			Ret->SetVerifyCallback(SSLVerifyCert, nullptr);
			mbedtls_debug_set_threshold(2);
		#endif

		/*
		// Set ciphersuite to the easiest one to decode, so that the connection can be wireshark-decoded:
		Ret->SetCipherSuites(
			{
				MBEDTLS_TLS_RSA_WITH_RC4_128_MD5,
				MBEDTLS_TLS_RSA_WITH_RC4_128_SHA,
				MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA
			}
		);
		*/
	#endif

	return Ret;
}





std::shared_ptr<const cSslConfig> cSslConfig::GetDefaultClientConfig()
{
	static const std::shared_ptr<const cSslConfig> ClientConfig = MakeDefaultConfig(true);
	return ClientConfig;
}





std::shared_ptr<const cSslConfig> cSslConfig::GetDefaultServerConfig()
{
	static const std::shared_ptr<const cSslConfig> ServerConfig = MakeDefaultConfig(false);
	return ServerConfig;
}




