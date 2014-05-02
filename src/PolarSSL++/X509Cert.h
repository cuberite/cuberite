
// X509Cert.h

// Declares the cX509Cert class representing a wrapper over X509 certs in PolarSSL





#pragma once

#include "polarssl/x509_crt.h"





class cX509Cert
{
	friend class cSslContext;
	
public:
	cX509Cert(void);
	~cX509Cert(void);
	
	/** Parses the certificate chain data into the context.
	Returns 0 on succes, or PolarSSL error code on failure. */
	int Parse(const void * a_CertContents, size_t a_Size);
	
protected:
	x509_crt m_Cert;

	/** Returns the internal cert ptr. Only use in PolarSSL API calls. */
	x509_crt * GetInternal(void) { return &m_Cert; }
} ;

typedef SharedPtr<cX509Cert> cX509CertPtr;




