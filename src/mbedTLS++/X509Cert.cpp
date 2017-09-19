
// X509Cert.cpp

// Implements the cX509Cert class representing a wrapper over X509 certs in mbedTLS

#include "Globals.h"
#include "X509Cert.h"





cX509Cert::cX509Cert(void)
{
	mbedtls_x509_crt_init(&m_Cert);
}





cX509Cert::~cX509Cert()
{
	mbedtls_x509_crt_free(&m_Cert);
}





int cX509Cert::Parse(const void * a_CertContents, size_t a_Size)
{
	return mbedtls_x509_crt_parse(&m_Cert, reinterpret_cast<const unsigned char *>(a_CertContents), a_Size);
}




