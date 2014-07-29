
// CtrDrbgContext.cpp

// Implements the cCtrDrbgContext class representing a wrapper over CTR-DRBG implementation in PolarSSL

#include "Globals.h"
#include "CtrDrbgContext.h"
#include "EntropyContext.h"





cCtrDrbgContext::cCtrDrbgContext(void) :
	m_EntropyContext(new cEntropyContext),
	m_IsValid(false)
{
}





cCtrDrbgContext::cCtrDrbgContext(const SharedPtr<cEntropyContext> & a_EntropyContext) :
	m_EntropyContext(a_EntropyContext),
	m_IsValid(false)
{
}





int cCtrDrbgContext::Initialize(const void * a_Custom, size_t a_CustomSize)
{
	if (m_IsValid)
	{
		// Already initialized
		return 0;
	}
	
	int res = ctr_drbg_init(&m_CtrDrbg, entropy_func, &(m_EntropyContext->m_Entropy), (const unsigned char *)a_Custom, a_CustomSize);
	m_IsValid = (res == 0);
	return res;
}




