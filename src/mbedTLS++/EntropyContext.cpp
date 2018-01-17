
// EntropyContext.cpp

// Implements the cEntropyContext class representing a wrapper over entropy contexts in mbedTLS

#include "Globals.h"
#include "EntropyContext.h"





cEntropyContext::cEntropyContext(void)
{
	mbedtls_entropy_init(&m_Entropy);
}





cEntropyContext::~cEntropyContext()
{
	mbedtls_entropy_free(&m_Entropy);
}




