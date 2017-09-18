
// CtrDrbgContext.cpp

// Implements the cCtrDrbgContext class representing a wrapper over CTR-DRBG implementation in mbedTLS

#include "Globals.h"
#include "CtrDrbgContext.h"
#include "EntropyContext.h"





cCtrDrbgContext::cCtrDrbgContext(void) :
	m_EntropyContext(std::make_shared<cEntropyContext>()),
	m_IsValid(false)
{
	mbedtls_ctr_drbg_init(&m_CtrDrbg);
}





cCtrDrbgContext::cCtrDrbgContext(const std::shared_ptr<cEntropyContext> & a_EntropyContext) :
	m_EntropyContext(a_EntropyContext),
	m_IsValid(false)
{
	mbedtls_ctr_drbg_init(&m_CtrDrbg);
}





int cCtrDrbgContext::Initialize(const void * a_Custom, size_t a_CustomSize)
{
	if (m_IsValid)
	{
		// Already initialized
		return 0;
	}

	int res = mbedtls_ctr_drbg_seed(&m_CtrDrbg, mbedtls_entropy_func, &(m_EntropyContext->m_Entropy), reinterpret_cast<const unsigned char *>(a_Custom), a_CustomSize);
	m_IsValid = (res == 0);
	return res;
}




