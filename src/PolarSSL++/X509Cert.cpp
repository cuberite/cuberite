
// X509Cert.cpp

// Implements the cX509Cert class representing a wrapper over X509 certs in PolarSSL

#include "Globals.h"
#include "X509Cert.h"





cX509Cert::cX509Cert(void)
{
	x509_crt_init(&m_Cert);
}





cX509Cert::~cX509Cert()
{
	x509_crt_free(&m_Cert);
}





int cX509Cert::Parse(const void * a_CertContents, size_t a_Size)
{
	return x509_crt_parse(&m_Cert, (const unsigned char *)a_CertContents, a_Size);
}




