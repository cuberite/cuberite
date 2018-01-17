
#pragma once

#include "mbedtls/ssl.h"

// fwd:
class cCryptoKey;
class cCtrDrbgContext;
class cX509Cert;

using cCryptoKeyPtr = std::shared_ptr<cCryptoKey>;
using cCtrDrbgContextPtr = std::shared_ptr<cCtrDrbgContext>;
using cX509CertPtr = std::shared_ptr<cX509Cert>;

enum class eSslAuthMode
{
	None = 0,      // MBEDTLS_SSL_VERIFY_NONE
	Optional = 1,  // MBEDTLS_SSL_VERIFY_OPTIONAL
	Required = 2,  // MBEDTLS_SSL_VERIFY_REQUIRED
	Unset = 3,     // MBEDTLS_SSL_VERIFY_UNSET
};



class cSslConfig
{
	friend class cSslContext;
public:
	/** Type of the SSL debug callback.
	Parameters are:
		void *       Opaque context for the callback
		int          Debug level
		const char * File name
		int          Line number
		const char * Message */
	using cDebugCallback = void(*)(void *, int, const char *, int, const char *);

	/** Type of the SSL certificate verify callback.
	Parameters are:
		void *             Opaque context for the callback
		mbedtls_x509_crt * Current cert
		int                Cert chain depth
		uint32_t *         Verification flags */
	using cVerifyCallback = int(*)(void *, mbedtls_x509_crt *, int, uint32_t *);

	cSslConfig();
	~cSslConfig();

	/** Initialize with mbedTLS default settings. */
	int InitDefaults(bool a_IsClient);

	/** Set the authorization mode. */
	void SetAuthMode(eSslAuthMode a_AuthMode);

	/** Set the random number generator. */
	void SetRng(cCtrDrbgContextPtr a_CtrDrbg);

	/** Set the debug callback. */
	void SetDebugCallback(cDebugCallback a_CallbackFun, void * a_CallbackData);

	/** Set the certificate verify callback. */
	void SetVerifyCallback(cVerifyCallback a_CallbackFun, void * a_CallbackData);

	/** Set the enabled cipher suites. */
	void SetCipherSuites(std::vector<int> a_CipherSuites);

	/** Set the certificate to use for connections. */
	void SetOwnCert(cX509CertPtr a_OwnCert, cCryptoKeyPtr a_OwnCertPrivKey);

	/** Set the trusted certificate authority chain. */
	void SetCACerts(cX509CertPtr a_CACert);

	/** Creates a new config with some sensible defaults on top of mbedTLS basic settings. */
	static std::shared_ptr<cSslConfig> MakeDefaultConfig(bool a_IsClient);

	/** Returns the default config for client connections. */
	static std::shared_ptr<const cSslConfig> GetDefaultClientConfig();

	/** Returns the default config for server connections. */
	static std::shared_ptr<const cSslConfig> GetDefaultServerConfig();

private:

	/** Returns a pointer to the wrapped mbedtls representation. */
	const mbedtls_ssl_config * GetInternal() const { return &m_Config; }

	mbedtls_ssl_config m_Config;
	cCtrDrbgContextPtr m_CtrDrbg;
	cX509CertPtr m_OwnCert;
	cCryptoKeyPtr m_OwnCertPrivKey;
	cX509CertPtr m_CACerts;
	std::vector<int> m_CipherSuites;
};
