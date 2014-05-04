
// EntropyContext.cpp

// Implements the cEntropyContext class representing a wrapper over entropy contexts in PolarSSL

#include "Globals.h"
#include "EntropyContext.h"





cEntropyContext::cEntropyContext(void)
{
	entropy_init(&m_Entropy);
}





cEntropyContext::~cEntropyContext()
{
	entropy_free(&m_Entropy);
}




