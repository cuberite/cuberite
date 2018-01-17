
// X509Cert.h

// Declares the cX509Cert class representing a wrapper over X509 certs in mbedTLS





#pragma once

#include "mbedtls/x509_crt.h"





class cX509Cert
{
	friend class cSslConfig;

public:
	cX509Cert(void);
	~cX509Cert(void);

	/** Parses the certificate chain data into the context.
	The certificate can be DER- or PEM-encoded.
	Returns 0 on succes, or mbedTLS error code on failure. */
	int Parse(const void * a_CertContents, size_t a_Size);

protected:
	mbedtls_x509_crt m_Cert;

	/** Returns the internal cert ptr. Only use in mbedTLS API calls. */
	mbedtls_x509_crt * GetInternal(void) { return &m_Cert; }
} ;

typedef std::shared_ptr<cX509Cert> cX509CertPtr;




